/*
 * main.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include "Interface/charon_uds.h"
#include "Interface/Socket/ISocket.h"

extern void pipe_init (void);
extern ISocket_t uart_socket;

int main (void)
{
    pipe_init();
    charon_init(uart_socket);
    while (1)
    {
        charon_task();
    }
    return 0;
}
