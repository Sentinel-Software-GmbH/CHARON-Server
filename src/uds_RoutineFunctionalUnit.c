/*
 * uds_RoutineFunctionalUnit.c
 *
 *  Created on: 29.01.2020
 *      Author: Florian Kaup
 */

#include "uds_RoutineFunctionalUnit.h"
#include "uds_types.h"
#include "uds_interface_NvmDriver.h"

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



uint32_t uds_RoutineFunctionalUnit_RoutineControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    uint32_t transmitLength = 0;
    if (receiveBufferSize < 4)
    {
        transmitLength = generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }
    else if (receiveBuffer[1] == 0 || receiveBuffer[1] > 3)
    {
        transmitLength = generateNegativeResponse(uds_responseCode_SubfunctionNotSupported, receiveBuffer[0], transmitBuffer, transmitBufferSize);
    }
    else
    {
        uint16_t routineIdentifier = ((uint16_t)receiveBuffer[3] << 8) | receiveBuffer[4];
        if (routineIdentifier == 0xFF00)
        {
            uds_NvmDriver_erase();
        }
        else
        {
            transmitLength = generateNegativeResponse(uds_responseCode_RequestOutOfRange, receiveBuffer[0], transmitBuffer, transmitBufferSize);
        }
    }

    return transmitLength;
}

