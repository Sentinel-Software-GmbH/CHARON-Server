/*
 * uds_negativeResponse.c
 *
 *  Created on: 03.02.2020
 *      Author: Florian Kaup
 */

#include "uds_negativeResponse.h"

uint32_t uds_generateNegativeResponse (uds_responseCode_t ResponseCode, uds_sid_t RequestedSid, uint8_t * transmitBuffer)
{
    transmitBuffer[0] = uds_sid_NegativeResponse;
    transmitBuffer[1] = RequestedSid;
    transmitBuffer[2] = ResponseCode;
    return 3;
}

