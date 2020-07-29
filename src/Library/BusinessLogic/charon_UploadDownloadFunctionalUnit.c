/*
 * charon_UploadDownloadFunctionalUnit.c
 *
 *  Created on: 16.01.2020
 *      Author: Florian Kaup
 */

#include <HSDI/charon_interface_NvmDriver.h>
#include "HSDI/charon_interface_debug.h"
#include "BusinessLogic/charon_UploadDownloadFunctionalUnit.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "Common/charon_negativeResponse.h"

#define     UDS_MAX_INPUT_FRAME_SIZE    4095u
#define     UDS_MAX_OUTPUT_FRAME_SIZE   4095u

typedef enum {
    transfer_idle,
    transfer_download,
    transfer_upload
} transferDirection_t;
static transferDirection_t s_transferDirection = transfer_idle;
static uint32_t s_currentMemoryAddress = 0;
static uint32_t s_remainingMemoryLength = 0;
static uint8_t s_nextSequenceCounter = 0;


static uds_responseCode_t requestTransfer(transferDirection_t direction, const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    const struct __attribute__((packed)) {
        uint8_t sid;
        uint8_t dataFormatIdentifier;
        uint8_t addressAndLengthFormatIdentifier;
        uint8_t AddressInformation[8];
    } * receivedMessage = (const void*)receiveBuffer;

    uds_responseCode_t result = uds_responseCode_PositiveResponse;

    uint8_t lengthOfMemoryLength = receivedMessage->addressAndLengthFormatIdentifier >> 4;
    uint8_t lengthOfMemoryAddress = receivedMessage->addressAndLengthFormatIdentifier & 0xFu;
    if (
            (lengthOfMemoryAddress == 0u) ||
            (lengthOfMemoryLength == 0u) ||
            (lengthOfMemoryAddress > 4u) ||
            (lengthOfMemoryLength > 4u) ||
            (receivedMessage->dataFormatIdentifier != 0x00u)
        )
    {
        CHARON_ERROR("Format Identifier invalid: size of address = %i, size of length = %i.", lengthOfMemoryAddress, lengthOfMemoryLength);
        result = uds_responseCode_RequestOutOfRange;
    }

    else if ( (((uint32_t)lengthOfMemoryAddress + lengthOfMemoryLength) + 3u) != receiveBufferSize )
    {
        CHARON_ERROR("Unexpected message length.");
        result = uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    else
    {
        uint32_t memoryAddress = 0;
        uint32_t memoryLength = 0;
        for (uint8_t i = 0; i<lengthOfMemoryAddress; i++)
        {
            memoryAddress |= (uint32_t)(receivedMessage->AddressInformation[lengthOfMemoryAddress - (i+1u)]) << (i*8u);
        }

        for (uint8_t i = 0; i<lengthOfMemoryLength; i++)
        {
            memoryLength |= (uint32_t)receivedMessage->AddressInformation[(lengthOfMemoryAddress + lengthOfMemoryLength) - (i+1u)] << (i*8u);
        }
        CHARON_INFO("Transfer Requested, address 0x%x, length 0x%x, direction %s.", memoryAddress, memoryLength, direction == transfer_download ? "download" : "upload");

        if ( false == charon_NvmDriver_checkAddressRange(memoryAddress, memoryLength) )
        {
            CHARON_ERROR("Reuqested memory is out of range.");
            result = uds_responseCode_RequestOutOfRange;
        }
        else if (s_transferDirection != transfer_idle)
        {
            CHARON_ERROR("A transfer is already ongoing.");
            result = uds_responseCode_ConditionsNotCorrect;
        }
        else
        {
            s_currentMemoryAddress = memoryAddress;
            s_remainingMemoryLength = memoryLength;
            s_transferDirection = direction;
            s_nextSequenceCounter = 1;
            uint8_t transmitBuffer[4] = {
                    receivedMessage->sid | (uint8_t)uds_sid_PositiveResponseMask,
                    0x20,
                    (uint8_t)((UDS_MAX_INPUT_FRAME_SIZE >> 8u) & 0xFFu),
                    (uint8_t)(UDS_MAX_INPUT_FRAME_SIZE & 0xFFu)
            };
            charon_sscTxMessage(transmitBuffer, sizeof(transmitBuffer));
        }

    }
    if (result != uds_responseCode_PositiveResponse)
    {
        uds_sid_t receivedSid;
        if (direction == transfer_download)
        {
            receivedSid = uds_sid_RequestDownload;
        }
        else
        {
            receivedSid = uds_sid_RequestUpload;
        }
        charon_sendNegativeResponse(result, receivedSid);
    }
    return result;
}

void charon_UploadDownloadFunctionalUnit_reset (void)
{
    s_transferDirection = transfer_idle;
    s_currentMemoryAddress = 0;
    s_remainingMemoryLength = 0;
    s_nextSequenceCounter = 0;
}

uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestDownload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return requestTransfer(transfer_download, receiveBuffer, receiveBufferSize);
}

uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestUpload (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    return requestTransfer(transfer_upload, receiveBuffer, receiveBufferSize);
}



uds_responseCode_t charon_UploadDownloadFunctionalUnit_TransferData (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    const struct __attribute__((packed)) {
        uint8_t sid;
        uint8_t blockSequenceCounter;
        uint8_t data[UDS_MAX_INPUT_FRAME_SIZE];
    } * receivedMessage = (const void*)receiveBuffer;

    uds_responseCode_t result = uds_responseCode_PositiveResponse;

    if (receiveBufferSize > UDS_MAX_INPUT_FRAME_SIZE)
    {
        CHARON_ERROR("Frame is too long! Maximum size is %i.", UDS_MAX_INPUT_FRAME_SIZE);
        result = uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if (s_transferDirection == transfer_idle)
    {
        CHARON_ERROR("Transfer Data was not expected. Forgot to request upload/downlaod?");
        result = uds_responseCode_RequestSequenceError;
    }
    else if (s_remainingMemoryLength < (receiveBufferSize - 2u) )
    {
        CHARON_ERROR("Too much data received!");
        result = uds_responseCode_TransferDataSuspended;
    }
    else if (receivedMessage->blockSequenceCounter != s_nextSequenceCounter)
    {
        CHARON_ERROR("Sequence number not expected! Received: %i, Expected: %i.", receivedMessage->blockSequenceCounter, s_nextSequenceCounter);
        result = uds_responseCode_WrongBlockSequenceCounter;
    }
    else
    {
        if (s_transferDirection == transfer_download)
        {
            result = charon_NvmDriver_write(s_currentMemoryAddress, receivedMessage->data, receiveBufferSize - 2u);
            if (result == uds_responseCode_PositiveResponse)
            {
                s_currentMemoryAddress += receiveBufferSize - 2u;
                s_remainingMemoryLength -= receiveBufferSize - 2u;
                s_nextSequenceCounter++;

                uint8_t transmitBuffer[2] = {
                        receivedMessage->sid | (uint8_t)uds_sid_PositiveResponseMask,
                        receivedMessage->blockSequenceCounter
                };
                charon_sscTxMessage(transmitBuffer, sizeof(transmitBuffer));
            }
            else
            {
                CHARON_ERROR("NVM driver reported error while writing to flash.");
            }
        }
        else
        {
            uint8_t transmitBuffer[UDS_MAX_OUTPUT_FRAME_SIZE];
            uint32_t transmitBufferSize = sizeof(transmitBuffer);
            if (s_remainingMemoryLength < (transmitBufferSize - 2u) )
            {
                transmitBufferSize = s_remainingMemoryLength + 2u;
            }
            charon_NvmDriver_read(s_currentMemoryAddress, &(transmitBuffer[2]), transmitBufferSize - 2u);
            s_currentMemoryAddress += transmitBufferSize - 2u;
            s_remainingMemoryLength -= transmitBufferSize - 2u;
            s_nextSequenceCounter++;

            transmitBuffer[0] = (uint8_t)uds_sid_TransferData | (uint8_t)uds_sid_PositiveResponseMask;
            transmitBuffer[1] = receivedMessage->blockSequenceCounter;
            charon_sscTxMessage(transmitBuffer, transmitBufferSize);
        }
    }
    if (result != uds_responseCode_PositiveResponse)
    {
        charon_sendNegativeResponse(result, uds_sid_TransferData);
    }
    return result;
}

uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestTransferExit (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uds_responseCode_t result = uds_responseCode_PositiveResponse;
    (void) receiveBuffer;

    if (receiveBufferSize > 1u)
    {
        CHARON_ERROR("Unexpected message length.");
        result = uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if (s_remainingMemoryLength != 0u)
    {
        CHARON_ERROR("Transfer Exit received, but not all data was transfered.");
        result = uds_responseCode_RequestSequenceError;
    }
    else if (s_transferDirection == transfer_idle)
    {
        CHARON_ERROR("No transfer ongoing, cannot exit transfer.");
        result = uds_responseCode_RequestSequenceError;
    }
    else
    {
        s_currentMemoryAddress = 0;
        s_remainingMemoryLength = 0;
        s_nextSequenceCounter = 0;
        s_transferDirection = transfer_idle;

        CHARON_INFO("Exiting transfer mode.");
        uint8_t transmitBuffer[1] = {(uint8_t)uds_sid_RequestTransferExit | (uint8_t)uds_sid_PositiveResponseMask};
        charon_sscTxMessage(transmitBuffer, sizeof(transmitBuffer));
    }
    if (result != uds_responseCode_PositiveResponse)
    {
        charon_sendNegativeResponse(result, uds_sid_RequestTransferExit);
    }
    return result;
}


uds_responseCode_t charon_UploadDownloadFunctionalUnit_RequestFileTransfer (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    (void)receiveBuffer;
    (void)receiveBufferSize;
    charon_sendNegativeResponse(uds_responseCode_ServiceNotSupported, uds_sid_RequestFileTransfer);
    return uds_responseCode_ServiceNotSupported;
}


#ifdef TEST
void charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress (uint32_t newAddress)
{
    s_currentMemoryAddress = newAddress;
}
void charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength (uint32_t newLength)
{
    s_remainingMemoryLength = newLength;
}
void charon_UploadDownloadFunctionalUnit_setTransferDirection (uint8_t newDirection)
{
    s_transferDirection = newDirection;
}
void charon_UploadDownloadFunctionalUnit_setNextSequenceCounter (uint8_t newCounter)
{
    s_nextSequenceCounter = newCounter;
}
uint32_t charon_UploadDownloadFunctionalUnit_getCurrentMemoryAddress (void)
{
    return s_currentMemoryAddress;
}
uint32_t charon_UploadDownloadFunctionalUnit_getRemainingMemoryLength (void)
{
    return s_remainingMemoryLength;
}
uint8_t charon_UploadDownloadFunctionalUnit_getTransferDirection (void)
{
    return s_transferDirection;
}
uint8_t charon_UploadDownloadFunctionalUnit_getNextSequenceCounter (void)
{
    return s_nextSequenceCounter;
}

#endif


