/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Kaup
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/**
 * @addtogroup CharonUDS
 * @{
 * @addtogroup Windows Port
 * @{
 * @file uart.c
 * Implementation of the Service and Session Control Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/
#include "Interface/Socket/ISocket.h"
#include <windows.h>

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

HANDLE pipeHandle;
LPCSTR pipeName = "\\\\.\\pipe\\charonserver";

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

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

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
