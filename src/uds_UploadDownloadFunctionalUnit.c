/*
 * uds_UploadDownloadFunctionalUnit.c
 *
 *  Created on: 16.01.2020
 *      Author: Florian Kaup
 */

#include "uds_UploadDownloadFunctionalUnit.h"
#include "uds_negativeResponse.h"
#include "uds_types.h"
#include "uds_interface_NvmDriver.h"

#define     UDS_MAX_INPUT_FRAME_SIZE    4095

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
        uint8_t AddressInformation[];
    } * receivedMessage = (void*)receiveBuffer;

    uint32_t transmitLength = 0;

    uint8_t lengthOfMemoryLength = receivedMessage->addressAndLengthFormatIdentifier >> 4;
    uint8_t lengthOfMemoryAddress = receivedMessage->addressAndLengthFormatIdentifier & 0xF;
    if (
            (lengthOfMemoryAddress == 0) ||
            (lengthOfMemoryLength == 0) ||
            (lengthOfMemoryAddress > 4) ||
            (lengthOfMemoryLength > 4) ||
            (receivedMessage->dataFormatIdentifier != 0x00)
        )
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_RequestOutOfRange, receivedMessage->sid, transmitBuffer);
    }

    else if ( lengthOfMemoryAddress + lengthOfMemoryLength + 3 != receiveBufferSize )
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receivedMessage->sid, transmitBuffer);
    }

    else
    {
        uint32_t memoryAddress = 0;
        uint32_t memoryLength = 0;
        switch (lengthOfMemoryAddress)
        {
        case 4:
            memoryAddress |= receivedMessage->AddressInformation[lengthOfMemoryAddress - 4] << 24;
            /* no break */
        case 3:
            memoryAddress |= receivedMessage->AddressInformation[lengthOfMemoryAddress - 3] << 16;
            /* no break */
        case 2:
            memoryAddress |= receivedMessage->AddressInformation[lengthOfMemoryAddress - 2] << 8;
            /* no break */
        case 1:
            memoryAddress |= receivedMessage->AddressInformation[lengthOfMemoryAddress - 1];
            /* no break */
        }
        switch (lengthOfMemoryLength)
        {
        case 4:
            memoryLength |= receivedMessage->AddressInformation[lengthOfMemoryAddress + lengthOfMemoryLength - 4] << 24;
            /* no break */
        case 3:
            memoryLength |= receivedMessage->AddressInformation[lengthOfMemoryAddress + lengthOfMemoryLength - 3] << 16;
            /* no break */
        case 2:
            memoryLength |= receivedMessage->AddressInformation[lengthOfMemoryAddress + lengthOfMemoryLength - 2] << 8;
            /* no break */
        case 1:
            memoryLength |= receivedMessage->AddressInformation[lengthOfMemoryAddress + lengthOfMemoryLength - 1];
            /* no break */
        }

        if ( false == uds_NvmDriver_checkAddressRange(memoryAddress, memoryLength) )
        {
            transmitLength = uds_generateNegativeResponse(uds_responseCode_RequestOutOfRange, receivedMessage->sid, transmitBuffer);
        }
        else if (s_transferDirection != transfer_idle)
        {
            transmitLength = uds_generateNegativeResponse(uds_responseCode_ConditionsNotCorrect, receivedMessage->sid, transmitBuffer);
        }
        else
        {
            s_currentMemoryAddress = memoryAddress;
            s_remainingMemoryLength = memoryLength;
            s_transferDirection = direction;
            s_nextSequenceCounter = 1;
            transmitBuffer[0] = receivedMessage->sid | uds_sid_PositiveResponseMask;
            transmitBuffer[1] = 0x20;
            transmitBuffer[2] = (UDS_MAX_INPUT_FRAME_SIZE >> 8) & 0xFF;
            transmitBuffer[3] = (UDS_MAX_INPUT_FRAME_SIZE >> 0) & 0xFF;
            transmitLength = 4;
        }

    }

    return transmitLength;
}

uint32_t uds_UploadDownloadFunctionalUnit_RequestDownload (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    return requestTransfer(transfer_download, receiveBuffer, receiveBufferSize, transmitBuffer, transmitBufferSize);
}

uint32_t uds_UploadDownloadFunctionalUnit_RequestUpload (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    return requestTransfer(transfer_upload, receiveBuffer, receiveBufferSize, transmitBuffer, transmitBufferSize);
}



uint32_t uds_UploadDownloadFunctionalUnit_TransferData (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    struct __attribute__((packed)) {
        uint8_t sid;
        uint8_t blockSequenceCounter;
        uint8_t data[];
    } * receivedMessage = (void*)receiveBuffer;

    uint32_t transmitLength = 0;
    if (receiveBufferSize > UDS_MAX_INPUT_FRAME_SIZE)
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receivedMessage->sid, transmitBuffer);
    }
    else if (s_transferDirection == transfer_idle)
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_RequestSequenceError, receivedMessage->sid, transmitBuffer);
    }
    else if (s_remainingMemoryLength < (receiveBufferSize - 2) )
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_TransferDataSuspended, receivedMessage->sid, transmitBuffer);
    }
    else if (receivedMessage->blockSequenceCounter != s_nextSequenceCounter)
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_WrongBlockSequenceCounter, receivedMessage->sid, transmitBuffer);
    }
    else
    {
        if (s_transferDirection == transfer_download)
        {
            uds_responseCode_t result = uds_NvmDriver_write(s_currentMemoryAddress, receivedMessage->data, receiveBufferSize - 2);
            if (result == uds_responseCode_PositiveResponse)
            {
                s_currentMemoryAddress += receiveBufferSize - 2;
                s_remainingMemoryLength -= receiveBufferSize - 2;
                s_nextSequenceCounter++;

                transmitBuffer[0] = receivedMessage->sid | uds_sid_PositiveResponseMask;
                transmitBuffer[1] = receivedMessage->blockSequenceCounter;
                transmitLength = 2;
            }
            else
            {
                transmitLength = uds_generateNegativeResponse(result, receivedMessage->sid, transmitBuffer);
            }
        }
        else
        {
            if (s_remainingMemoryLength < (transmitBufferSize - 2) )
            {
                transmitBufferSize = s_remainingMemoryLength + 2;
            }
            uds_NvmDriver_read(s_currentMemoryAddress, &(transmitBuffer[2]), transmitBufferSize - 2);
            s_currentMemoryAddress += transmitBufferSize - 2;
            s_remainingMemoryLength -= transmitBufferSize - 2;
            s_nextSequenceCounter++;

            transmitBuffer[0] = receivedMessage->sid | uds_sid_PositiveResponseMask;
            transmitBuffer[1] = receivedMessage->blockSequenceCounter;
            transmitLength = transmitBufferSize;
        }
    }
    return transmitLength;
}

uint32_t uds_UploadDownloadFunctionalUnit_RequestTransferExit (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    uint32_t transmitLength = 0;
    if (receiveBufferSize > 1)
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0], transmitBuffer);
    }
    else if (s_remainingMemoryLength != 0)
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_RequestSequenceError, receiveBuffer[0], transmitBuffer);
    }
    else if (s_transferDirection == transfer_idle)
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_RequestSequenceError, receiveBuffer[0], transmitBuffer);
    }
    else
    {
        s_currentMemoryAddress = 0;
        s_remainingMemoryLength = 0;
        s_nextSequenceCounter = 0;
        s_transferDirection = transfer_idle;

        transmitBuffer[0] = receiveBuffer[0] | uds_sid_PositiveResponseMask;
        transmitLength = 1;
    }
    return transmitLength;
}


uint32_t uds_UploadDownloadFunctionalUnit_RequestFileTransfer (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    return uds_generateNegativeResponse(uds_responseCode_ServiceNotSupported, receiveBuffer[0], transmitBuffer);
}


#ifdef TEST
void uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress (uint32_t newAddress)
{
    s_currentMemoryAddress = newAddress;
}
void uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength (uint32_t newLength)
{
    s_remainingMemoryLength = newLength;
}
void uds_UploadDownloadFunctionalUnit_setTransferDirection (uint8_t newDirection)
{
    s_transferDirection = newDirection;
}
void uds_UploadDownloadFunctionalUnit_setNextSequenceCounter (uint8_t newCounter)
{
    s_nextSequenceCounter = newCounter;
}
uint32_t uds_UploadDownloadFunctionalUnit_getCurrentMemoryAddress (void)
{
    return s_currentMemoryAddress;
}
uint32_t uds_UploadDownloadFunctionalUnit_getRemainingMemoryLength (void)
{
    return s_remainingMemoryLength;
}
uint8_t uds_UploadDownloadFunctionalUnit_getTransferDirection (void)
{
    return s_transferDirection;
}
uint8_t uds_UploadDownloadFunctionalUnit_getNextSequenceCounter (void)
{
    return s_nextSequenceCounter;
}

#endif


