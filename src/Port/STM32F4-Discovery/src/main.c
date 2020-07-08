/*
 * main.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include "Interface/charon_uds.h"

int main (void)
{
    while (1)
    {
        charon_task();
    }
    return 0;
}
