/*
 * uart.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include "HSDI/charon_interface_canisotp.h"
#include <windows.h>

HANDLE pipeHandle;
LPCSTR pipeName = "\\\\.\\pipe\\charonserver";

void pipe_init (void)
{
    pipeHandle = CreateNamedPipe(
            pipeName,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT,
            PIPE_UNLIMITED_INSTANCES,
            4100,
            4100,
            0,
            NULL);
}


uint16_t charon_interface_isotp_receive (uint8_t* data, uint32_t maxSize)
{
    DWORD numBytes = 0;
    ReadFile(pipeHandle, data, maxSize, &numBytes, NULL);
    return numBytes;
}

uint16_t charon_interface_isotp_transmit (uint8_t* data, uint32_t size)
{
    DWORD numBytes;
    WriteFile(pipeHandle, data, size, &numBytes, NULL);
    return numBytes;
}




