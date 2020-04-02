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

void charon_task (void)
{
    /* Process Received Data */
    charon_sscRcvMessage();

    return;
}

