/*
 * uds.c
 *
 *  Created on: 15.01.2020
 *      Author: Florian Kaup
 */


#include "charon_uds.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"

void charon_task (void)
{
    /* Process Received Data */
    charon_sscRcvMessage();

    return;
}

