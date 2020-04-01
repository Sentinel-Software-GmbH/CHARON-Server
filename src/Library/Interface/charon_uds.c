/*
 * uds.c
 *
 *  Created on: 15.01.2020
 *      Author: Florian Kaup
 */


#include <stdint.h>

#include "charon_uds.h"
#include "Common/charon_types.h"
#include "HSDI/charon_interface_canisotp.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"

static uint8_t s_receiveBuffer[4096];

void charon_task (void)
{
    uint32_t recevieByteCount = charon_isotp_receive(s_receiveBuffer, sizeof(s_receiveBuffer));

    /* Check for received Data and forward to session control */
    if (recevieByteCount != 0u)
    {
        charon_sscRcvProcessMessage(s_receiveBuffer, recevieByteCount);
    }
    return;
}

