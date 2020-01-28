/*
 * uds_UploadDownloadFunctionalUnit.c
 *
 *  Created on: 16.01.2020
 *      Author: Florian Kaup
 */

#include "uds_UploadDownloadFunctionalUnit.h"
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


static uint32_t generateNegativeResponse (uds_responseCode_t ResponseCode, uds_sid_t RequestedSid, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    uint32_t transmitLength = 0;
    if (transmitBufferSize >= 3)
    {
        transmitBuffer[0] = uds_sid_NegativeResponse;
        transmitBuffer[1] = RequestedSid;
        transmitBuffer[2] = ResponseCode;
        transmitLength = 3;
    }
    return transmitLength;
}

static uint32_t requestTransfer(transferDirection_t direction, uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    uint32_t transmitLength = 0;

    uint8_t lengthOfMemoryAddress = receiveBuffer[2] >> 4;
    uint8_t lengthOfMemoryLength = receiveBuffer[2] & 0xF;
    if (
            (lengthOfMemoryAddress == 0) ||
            (lengthOfMemoryLength == 0) ||
            (lengthOfMemoryAddress > 4) ||
            (lengthOfMemoryLength > 4) ||
            (receiveBuffer[1] != 0x00)
        )
    {
        transmitLength = generateNegativeResponse(uds_responseCode_RequestOutOfRange, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }

    else if ( lengthOfMemoryAddress + lengthOfMemoryLength + 3 != receiveBufferSize )
    {
        transmitLength = generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }

    else
    {
        uint32_t memoryAddress = 0;
        uint32_t memoryLength = 0;
        switch (lengthOfMemoryAddress)
        {
        case 4:
            memoryAddress |= receiveBuffer[3 + lengthOfMemoryAddress - 4] << 24;
            /* no break */
        case 3:
            memoryAddress |= receiveBuffer[3 + lengthOfMemoryAddress - 3] << 16;
            /* no break */
        case 2:
            memoryAddress |= receiveBuffer[3 + lengthOfMemoryAddress - 2] << 8;
            /* no break */
        case 1:
            memoryAddress |= receiveBuffer[3 + lengthOfMemoryAddress - 1];
            /* no break */
        }
        switch (lengthOfMemoryLength)
        {
        case 4:
            memoryLength |= receiveBuffer[3 + lengthOfMemoryAddress + lengthOfMemoryLength - 4] << 24;
            /* no break */
        case 3:
            memoryLength |= receiveBuffer[3 + lengthOfMemoryAddress + lengthOfMemoryLength - 3] << 16;
            /* no break */
        case 2:
            memoryLength |= receiveBuffer[3 + lengthOfMemoryAddress + lengthOfMemoryLength - 2] << 8;
            /* no break */
        case 1:
            memoryLength |= receiveBuffer[3 + lengthOfMemoryAddress + lengthOfMemoryLength - 1];
            /* no break */
        }

        if ( false == uds_NvmDriver_checkAddressRange(memoryAddress, memoryLength) )
        {
            transmitLength = generateNegativeResponse(uds_responseCode_RequestOutOfRange, receiveBuffer[0], transmitBuffer, transmitBufferSize);
        }
        else if (s_transferDirection != transfer_idle)
        {
            transmitLength = generateNegativeResponse(uds_responseCode_ConditionsNotCorrect, receiveBuffer[0], transmitBuffer, transmitBufferSize);
        }
        else
        {
            s_currentMemoryAddress = memoryAddress;
            s_remainingMemoryLength = memoryLength;
            s_transferDirection = direction;
            s_nextSequenceCounter = 1;
            transmitBuffer[0] = receiveBuffer[0] | uds_sid_PositiveResponseMask;
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
    uint32_t transmitLength = 0;
    if (receiveBufferSize > UDS_MAX_INPUT_FRAME_SIZE)
    {
        transmitLength = generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }
    else if (s_transferDirection == transfer_idle)
    {
        transmitLength = generateNegativeResponse(uds_responseCode_RequestSequenceError, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }
    else if (s_remainingMemoryLength < (receiveBufferSize - 2) )
    {
        transmitLength = generateNegativeResponse(uds_responseCode_TransferDataSuspended, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }
    else if (receiveBuffer[1] != s_nextSequenceCounter)
    {
        transmitLength = generateNegativeResponse(uds_responseCode_WrongBlockSequenceCounter, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }
    else
    {
        if (s_transferDirection == transfer_download)
        {
            uds_NvmDriver_write(s_currentMemoryAddress, &(receiveBuffer[2]), receiveBufferSize - 2);
            s_currentMemoryAddress += receiveBufferSize - 2;
            s_remainingMemoryLength -= receiveBufferSize - 2;
            s_nextSequenceCounter++;

            transmitBuffer[0] = receiveBuffer[0] | uds_sid_PositiveResponseMask;
            transmitBuffer[1] = receiveBuffer[1];
            transmitLength = 2;
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

            transmitBuffer[0] = receiveBuffer[0] | uds_sid_PositiveResponseMask;
            transmitBuffer[1] = receiveBuffer[1];
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
        transmitLength = generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }
    else if (s_remainingMemoryLength != 0)
    {
        transmitLength = generateNegativeResponse(uds_responseCode_RequestSequenceError, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }
    else if (s_transferDirection == transfer_idle)
    {
        transmitLength = generateNegativeResponse(uds_responseCode_RequestSequenceError, receiveBuffer[0], transmitBuffer, transmitBufferSize);
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
    return generateNegativeResponse(uds_responseCode_ServiceNotSupported, receiveBuffer[0], transmitBuffer, transmitBufferSize);
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


