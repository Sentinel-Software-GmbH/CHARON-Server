/*
 * charon_RoutineFunctionalUnit.c
 *
 *  Created on: 29.01.2020
 *      Author: Florian Kaup
 */

#include <HSDI/charon_interface_NvmDriver.h>
#include "charon_RoutineFunctionalUnit.h"
#include "Common/charon_negativeResponse.h"
#include "Common/charon_types.h"


uint32_t charon_RoutineFunctionalUnit_RoutineControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    uint32_t transmitLength = 0;
    if (receiveBufferSize < 4u)
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0], transmitBuffer);
    }
    else if ( (receiveBuffer[1] == 0u) || (receiveBuffer[1] > 3u) )
    {
        transmitLength = charon_generateNegativeResponse(uds_responseCode_SubfunctionNotSupported, receiveBuffer[0], transmitBuffer);
    }
    else
    {
        uint16_t routineIdentifier = ((uint16_t)receiveBuffer[2] << 8) | receiveBuffer[3];
        if ( (routineIdentifier == 0xFF00u) && (receiveBuffer[1] == 1u) )
        {
            charon_NvmDriver_erase();

            transmitBuffer[0] = receiveBuffer[0] | (uint8_t)uds_sid_PositiveResponseMask;
            transmitBuffer[1] = receiveBuffer[1];
            transmitBuffer[2] = receiveBuffer[2];
            transmitBuffer[3] = receiveBuffer[3];
            transmitLength = 4;
        }
        else
        {
            transmitLength = charon_generateNegativeResponse(uds_responseCode_RequestOutOfRange, receiveBuffer[0], transmitBuffer);
        }
    }

    return transmitLength;
}

