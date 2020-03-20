/*
 * charon_negativeResponse.c
 *
 *  Created on: 03.02.2020
 *      Author: Florian Kaup
 */

#include "charon_negativeResponse.h"

uint32_t charon_generateNegativeResponse (charon_responseCode_t ResponseCode, charon_sid_t RequestedSid, uint8_t * transmitBuffer)
{
    transmitBuffer[0] = charon_sid_NegativeResponse;
    transmitBuffer[1] = RequestedSid;
    transmitBuffer[2] = ResponseCode;
    return 3;
}

