/*
 * timer.c
 *
 *  Created on: 13.07.2020
 *      Author: Florian Kaup
 */

#include "HSDI/charon_interface_clock.h"
#include <time.h>


uint32_t charon_interface_clock_getTime(void)
{
    return clock()*1000/CLOCKS_PER_SEC;
}

uint32_t charon_interface_clock_getTimeElapsed(uint32_t timestamp)
{
    return clock()*1000/CLOCKS_PER_SEC - timestamp;
}

