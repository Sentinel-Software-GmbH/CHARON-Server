/*
 * main.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include "Interface/charon_uds.h"

extern void pipe_init (void);

int main (void)
{
    pipe_init();
    while (1)
    {
        charon_task();
    }
    return 0;
}
