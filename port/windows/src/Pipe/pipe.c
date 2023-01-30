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
 * @addtogroup CharonUDS_Server
 * @{
 * @addtogroup WindowsPort Windows porting 
 * @{
 * @file pipe.c
 * Implementation of windows pipe for server client communication.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "ISocket.h"
#include <windows.h>

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/** @brief Handle for windows pipe usage. */
HANDLE pipeHandle;

/** @brief 32-bit pointer to a constant null-terminated string of 8-bit Windows. */ 
LPCSTR pipeName = "\\\\.\\pipe\\charonserver";

/* Function Definition *******************************************************/

/** @brief Function to initialize pipe for communication between Client and Server when boot entities launched on windows. */
void pipe_Init(void);

/* Private Function Definitions **********************************************/

/** @brief Function to get number of available bytes.
 * 
 * @return Number of Bytes.
 */
static int32_t pipe_numAvailableBytes (void);
/**
 * @brief Function to receive data via pipe.
 * 
 * @param data Data buffer with received message.
 * @param maxSize maximum size of data to receive in bytes.
 * @return Number of received bytes.
 */
static int32_t pipe_receive (uint8_t* data, uint32_t maxSize);

/** @brief Function sends data using pipe and windows std. libraries.
 * 
 * @param data Data buffer to send. 
 * @param size Size of data buffer in bytes.
 * @return Number of bytes transmitted.
 */
static int32_t pipe_transmit (const uint8_t* data, uint32_t size);

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

static int32_t pipe_numAvailableBytes (void)
{
    return UINT32_MAX;
}

static int32_t pipe_receive (uint8_t* data, uint32_t maxSize)
{
	DWORD numBytes = 0;
    BOOL result = ReadFile(pipeHandle, data, maxSize, &numBytes, NULL);
    INT error = GetLastError();
    if (!result && error == 109) pipe_init();
    return (int32_t)numBytes;
}

static int32_t pipe_transmit (const uint8_t* data, uint32_t size)
{
	DWORD numBytes;
    WriteFile(pipeHandle, data, size, &numBytes, NULL);
    return (int32_t)numBytes;
}


ISocket_t pipe_socket = {
        .numAvailableBytes = pipe_numAvailableBytes,
        .receive = pipe_receive,
        .transmit = pipe_transmit,
};

/* Private Function **********************************************************/

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
