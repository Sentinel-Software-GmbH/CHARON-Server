/*
 * charon_UploadDownloadFunctionalUnit.c
 *
 *  Created on: 16.01.2020
 *      Author: Florian Kaup
 */

#include <HSDI/charon_interface_NvmDriver.h>
#include "charon_UploadDownloadFunctionalUnit.h"
#include "Common/charon_negativeResponse.h"
#include "Common/charon_types.h"

#define     UDS_MAX_INPUT_FRAME_SIZE    4095u

typedef enum {
    transfer_idle,
    transfer_download,
    transfer_upload
} transferDirection_t;
static transferDirection_t s_transferDirection = transfer_idle;
static uint32_t s_currentMemoryAddress = 0;
static uint32_t s_remainingMemoryLength = 0;
static uint8_t s_nextSequenceCounter = 0;


static uint32_t requestTransfer(transferDirection_t direction, uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    struct __attribute__((packed)) {
        uint8_t sid;
        uint8_t dataFormatIdentifier;
        uint8_t addressAndLengthFormatIdentifier;
        uint8_t AddressInformation[8];
    } * receivedMessage = (void*)receiveBuffer;

    uint32_t transmitLength = 0;

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
        transmitLength = charon_generateNegativeResponse(uds_responseCode_RequestOutOfRange, receivedMessage->sid, transmitBuffer);
    }

    else if ( ((lengthOfMemoryAddress + lengthOfMemoryLength) + 3u) != receiveBufferSize )
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receivedMessage->sid, transmitBuffer);
    }

    else
    {
        uint32_t memoryAddress = 0;
        uint32_t memoryLength = 0;
        for (uint8_t i = 0; i<lengthOfMemoryAddress; i++)
        {
            memoryAddress |= receivedMessage->AddressInformation[lengthOfMemoryAddress - (i+1u)] << (i*8u);
        }

        for (uint8_t i = 0; i<lengthOfMemoryLength; i++)
        {
            memoryLength |= (uint32_t)receivedMessage->AddressInformation[(lengthOfMemoryAddress + lengthOfMemoryLength) - (i+1u)] << (i*8u);
        }

        if ( false == charon_NvmDriver_checkAddressRange(memoryAddress, memoryLength) )
        {
            transmitLength = charon_generateNegativeResponse(uds_responseCode_RequestOutOfRange, receivedMessage->sid, transmitBuffer);
        }
        else if (s_transferDirection != transfer_idle)
        {
            transmitLength = charon_generateNegativeResponse(uds_responseCode_ConditionsNotCorrect, receivedMessage->sid, transmitBuffer);
        }
        else
        {
            s_currentMemoryAddress = memoryAddress;
            s_remainingMemoryLength = memoryLength;
            s_transferDirection = direction;
            s_nextSequenceCounter = 1;
            transmitBuffer[0] = receivedMessage->sid | (uint8_t)uds_sid_PositiveResponseMask;
            transmitBuffer[1] = 0x20;
            transmitBuffer[2] = (UDS_MAX_INPUT_FRAME_SIZE >> 8u) & 0xFFu;
            transmitBuffer[3] = (UDS_MAX_INPUT_FRAME_SIZE >> 0u) & 0xFFu;
            transmitLength = 4;
        }

    }

    return transmitLength;
}

uint32_t charon_UploadDownloadFunctionalUnit_RequestDownload (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    return requestTransfer(transfer_download, receiveBuffer, receiveBufferSize, transmitBuffer, transmitBufferSize);
}

uint32_t charon_UploadDownloadFunctionalUnit_RequestUpload (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    return requestTransfer(transfer_upload, receiveBuffer, receiveBufferSize, transmitBuffer, transmitBufferSize);
}



uint32_t charon_UploadDownloadFunctionalUnit_TransferData (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    struct __attribute__((packed)) {
        uint8_t sid;
        uint8_t blockSequenceCounter;
        uint8_t data[UDS_MAX_INPUT_FRAME_SIZE];
    } * receivedMessage = (void*)receiveBuffer;

    uint32_t transmitLength = 0;
    if (receiveBufferSize > UDS_MAX_INPUT_FRAME_SIZE)
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receivedMessage->sid, transmitBuffer);
    }
    else if (s_transferDirection == transfer_idle)
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_RequestSequenceError, receivedMessage->sid, transmitBuffer);
    }
    else if (s_remainingMemoryLength < (receiveBufferSize - 2u) )
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_TransferDataSuspended, receivedMessage->sid, transmitBuffer);
    }
    else if (receivedMessage->blockSequenceCounter != s_nextSequenceCounter)
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_WrongBlockSequenceCounter, receivedMessage->sid, transmitBuffer);
    }
    else
    {
        if (s_transferDirection == transfer_download)
        {
            uds_responseCode_t result = charon_NvmDriver_write(s_currentMemoryAddress, receivedMessage->data, receiveBufferSize - 2u);
            if (result == uds_responseCode_PositiveResponse)
            {
                s_currentMemoryAddress += receiveBufferSize - 2u;
                s_remainingMemoryLength -= receiveBufferSize - 2u;
                s_nextSequenceCounter++;

                transmitBuffer[0] = receivedMessage->sid | (uint8_t)uds_sid_PositiveResponseMask;
                transmitBuffer[1] = receivedMessage->blockSequenceCounter;
                transmitLength = 2;
            }
            else
            {
                transmitLength = charon_generateNegativeResponse(result, receivedMessage->sid, transmitBuffer);
            }
        }
        else
        {
            if (s_remainingMemoryLength < (transmitBufferSize - 2u) )
            {
                transmitBufferSize = s_remainingMemoryLength + 2u;
            }
            charon_NvmDriver_read(s_currentMemoryAddress, &(transmitBuffer[2]), transmitBufferSize - 2u);
            s_currentMemoryAddress += transmitBufferSize - 2u;
            s_remainingMemoryLength -= transmitBufferSize - 2u;
            s_nextSequenceCounter++;

            transmitBuffer[0] = receivedMessage->sid | (uint8_t)uds_sid_PositiveResponseMask;
            transmitBuffer[1] = receivedMessage->blockSequenceCounter;
            transmitLength = transmitBufferSize;
        }
    }
    return transmitLength;
}

uint32_t charon_UploadDownloadFunctionalUnit_RequestTransferExit (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    uint32_t transmitLength = 0;
    if (receiveBufferSize > 1u)
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0], transmitBuffer);
    }
    else if (s_remainingMemoryLength != 0u)
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_RequestSequenceError, receiveBuffer[0], transmitBuffer);
    }
    else if (s_transferDirection == transfer_idle)
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_RequestSequenceError, receiveBuffer[0], transmitBuffer);
    }
    else
    {
        s_currentMemoryAddress = 0;
        s_remainingMemoryLength = 0;
        s_nextSequenceCounter = 0;
        s_transferDirection = transfer_idle;

        transmitBuffer[0] = receiveBuffer[0] | (uint8_t)uds_sid_PositiveResponseMask;
        transmitLength = 1;
    }
    return transmitLength;
}


uint32_t charon_UploadDownloadFunctionalUnit_RequestFileTransfer (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    return charon_generateNegativeResponse(uds_responseCode_ServiceNotSupported, receiveBuffer[0], transmitBuffer);
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


