/*
 * charon_negativeResponse.c
 *
 *  Created on: 03.02.2020
 *      Author: Florian Kaup
 */

#include "charon_negativeResponse.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"

void charon_sendNegativeResponse (uds_responseCode_t ResponseCode, uds_sid_t RequestedSid)
{
    uint8_t transmitBuffer[3] = {(uint8_t)uds_sid_NegativeResponse, (uint8_t)RequestedSid, (uint8_t)ResponseCode};
    charon_sscTxMessage(transmitBuffer, sizeof(transmitBuffer));
}

