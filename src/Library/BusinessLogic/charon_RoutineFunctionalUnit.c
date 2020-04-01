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


int32_t charon_RoutineFunctionalUnit_RoutineControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    if (receiveBufferSize < 4u)
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, receiveBuffer[0]);
    }
    else if ( (receiveBuffer[1] == 0u) || (receiveBuffer[1] > 3u) )
    {
        charon_sendNegativeResponse(uds_responseCode_SubfunctionNotSupported, receiveBuffer[0]);
    }
    else
    {
        uint16_t routineIdentifier = ((uint16_t)receiveBuffer[3] << 8) | receiveBuffer[4];
        if (routineIdentifier == 0xFF00u)
        {
            charon_NvmDriver_erase();
        }
        else
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, receiveBuffer[0]);
        }
    }

    return 0;
}

