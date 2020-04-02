/*
 * charon_RoutineFunctionalUnit.c
 *
 *  Created on: 29.01.2020
 *      Author: Florian Kaup
 */

#include <HSDI/charon_interface_NvmDriver.h>
#include "charon_RoutineFunctionalUnit.h"
#include "Common/charon_negativeResponse.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"


uds_responseCode_t charon_RoutineFunctionalUnit_RoutineControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uds_responseCode_t result = uds_responseCode_PositiveResponse;

    if (receiveBufferSize < 4u)
    {
        result = uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else if ( (receiveBuffer[1] == 0u) || (receiveBuffer[1] > 3u) )
    {
        result = uds_responseCode_SubfunctionNotSupported;
    }
    else
    {
        uint16_t routineIdentifier = ((uint16_t)receiveBuffer[2] << 8) | receiveBuffer[3];
        if ( (routineIdentifier == 0xFF00u) && (receiveBuffer[1] == 1u) )
        {
            charon_NvmDriver_erase();
        }
        else
        {
            result = uds_responseCode_RequestOutOfRange;
        }
    }

    if (result != uds_responseCode_PositiveResponse)
    {
        charon_sendNegativeResponse(result, receiveBuffer[0]);
    }
    else
    {
        uint8_t transmitBuffer[4] = {
                receiveBuffer[0] | (uint8_t)uds_sid_PositiveResponseMask,
                receiveBuffer[1],
                receiveBuffer[2],
                receiveBuffer[3]
        };
        charon_sscTxProcessMessage(transmitBuffer, sizeof(transmitBuffer));
    }
    return result;
}

