/*
 * main.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include "Interface/charon_uds.h"
#include "Interface/Socket/ISocket.h"
#include "HSDI/charon_interface_debug.h"

extern void pipe_init (void);
extern ISocket_t pipe_socket;
extern void uart_init (void);
extern ISocket_t uart_socket;

int main (void)
{
    pipe_init();
    charon_init(pipe_socket);
    CHARON_INFO("CharonUDS Initialized...");
    while (1)
    {
        charon_task();
    }
    return 0;
}

