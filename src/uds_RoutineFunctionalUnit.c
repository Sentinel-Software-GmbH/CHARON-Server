/*
 * uds_RoutineFunctionalUnit.c
 *
 *  Created on: 29.01.2020
 *      Author: Florian Kaup
 */

#include "uds_RoutineFunctionalUnit.h"
#include "uds_negativeResponse.h"
#include "uds_types.h"
#include "uds_interface_NvmDriver.h"


uint32_t uds_RoutineFunctionalUnit_RoutineControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    uint32_t transmitLength = 0;
    if (receiveBufferSize < 4)
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0], transmitBuffer);
    }
    else if (receiveBuffer[1] == 0 || receiveBuffer[1] > 3)
    {
        transmitLength = uds_generateNegativeResponse(uds_responseCode_SubfunctionNotSupported, receiveBuffer[0], transmitBuffer);
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
            transmitLength = uds_generateNegativeResponse(uds_responseCode_RequestOutOfRange, receiveBuffer[0], transmitBuffer);
        }
    }

    return transmitLength;
}

