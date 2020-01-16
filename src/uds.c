/*
 * uds.c
 *
 *  Created on: 15.01.2020
 *      Author: Florian Kaup
 */


#include "uds.h"
#include "uds_interfaces.h"


#include <stdint.h>

static uint8_t s_receiveBuffer[4096];
static uint8_t s_transmitBuffer[4096];




void uds_task (void)
{
    uint32_t recevieByteCount = uds_isotp_receive(s_receiveBuffer, sizeof(s_receiveBuffer));

    if (recevieByteCount != 0)
    {
        uds_isotp_transmit(0,0);
    }

    return;
}

