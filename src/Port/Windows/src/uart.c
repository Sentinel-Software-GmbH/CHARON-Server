/*
 * uart.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include "Interface/Socket/ISocket.h"
#include <windows.h>

HANDLE pipeHandle;
LPCSTR pipeName = "\\\\.\\pipe\\charonserver";

void pipe_init (void)
{
    pipeHandle = CreateNamedPipe(
            pipeName,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
            PIPE_UNLIMITED_INSTANCES,
            4100,
            4100,
            0,
            NULL);
}

static int32_t uart_numAvailableBytes (void)
{
    return UINT32_MAX;
}

static int32_t uart_receive (uint8_t* data, uint32_t maxSize)
{
	DWORD numBytes = 0;
    BOOL result = ReadFile(pipeHandle, data, maxSize, &numBytes, NULL);
    INT error = GetLastError();
    if (!result && error == 109) pipe_init();
    return (int32_t)numBytes;
}

static int32_t uart_transmit (const uint8_t* data, uint32_t size)
{
	DWORD numBytes;
    WriteFile(pipeHandle, data, size, &numBytes, NULL);
    return (int32_t)numBytes;
}


ISocket_t uart_socket = {
        .numAvailableBytes = uart_numAvailableBytes,
        .receive = uart_receive,
        .transmit = uart_transmit,
};




