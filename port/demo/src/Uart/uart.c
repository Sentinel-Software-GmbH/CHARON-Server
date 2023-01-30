/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Steven Inacio
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
 * @addtogroup showcaseDemoPort showcase demo
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

#include "ISocket.h"
#include <windows.h>
#include "charon_interface_debug.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/
/** @brief Declaration of target hardware handle: */
HANDLE hComm;
/** @brief Access to target serial communications device. */
DCB UartConfig;
/** @brief Acces to target time-out parameters for a communications device. */
COMMTIMEOUTS UartTimeouts;
/** @brief pointer to a constant null-terminated string of target */
LPTSTR COM_PORT_NAME  = "\\\\.\\COM12";

/* Function Definition *******************************************************/

/** @brief Initialize target uart hardware*/
void uart_init();

/* Private Function Definitions **********************************************/

/** @brief Function  to  get number of available bytes. 
 * 
 * @return Number of bytes available.
 */
static int32_t uart_numAvailableBytes (void);

/** @brief Receiving data from Client via target serial communication device.
 * 
 * @param buffer Buffer containing received data.
 * @param length Number of bytes to read in bytes.
 * @return Number of Bytes received. 
 */
static int32_t uart_receive(uint8_t *buffer, uint32_t length);

/** @brief Sending data to Client via target serial communication device.
 * 
 * @param data Data to send to Client.
 * @param length Number of Bytes to send in bytes.
 * @return Number of send bytes in bytes.
 */
static int32_t uart_send(const uint8_t *data, uint32_t length);

/* Interfaces  ***************************************************************/

ISocket_t uart_socket = {
    .numAvailableBytes = uart_numAvailableBytes,
    .receive = uart_receive,
    .transmit = uart_send
};

void uart_init()
{
    hComm = CreateFile(COM_PORT_NAME,
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       0,
                       OPEN_EXISTING,
                       0,
                       NULL);
    if (hComm == INVALID_HANDLE_VALUE)
    {
        CHARON_ERROR("Invalid File Handle!");
        CloseHandle(hComm);
        return;
    }
    UartConfig.DCBlength = sizeof(DCB);
    GetCommState(hComm, &UartConfig);
    UartConfig.BaudRate = 115200;
    UartConfig.fBinary = TRUE;
    UartConfig.fParity = TRUE;
    UartConfig.fOutxCtsFlow = FALSE;
    UartConfig.fOutxDsrFlow = FALSE;
    UartConfig.fDtrControl = DTR_CONTROL_DISABLE;
    UartConfig.fDsrSensitivity = FALSE;
    UartConfig.fTXContinueOnXoff = TRUE;
    UartConfig.fOutX = FALSE;
    UartConfig.fInX = FALSE;
    UartConfig.fErrorChar = FALSE;
    UartConfig.fNull = FALSE;
    UartConfig.fRtsControl = RTS_CONTROL_DISABLE;
    UartConfig.fAbortOnError = FALSE;
    UartConfig.ByteSize = 8;
    UartConfig.Parity = NOPARITY;
    UartConfig.StopBits = ONESTOPBIT;
    if(!SetCommState(hComm, &UartConfig)) {
        CHARON_ERROR("Could not set Config");
        return;
    }
    GetCommTimeouts(hComm, &UartTimeouts);
    UartTimeouts.ReadIntervalTimeout = MAXDWORD;
    UartTimeouts.ReadTotalTimeoutMultiplier = 0;
    UartTimeouts.ReadTotalTimeoutConstant = 0;
    UartTimeouts.WriteTotalTimeoutConstant = 1000;
    UartTimeouts.WriteTotalTimeoutMultiplier = 10;
    if(!SetCommTimeouts(hComm, &UartTimeouts)) {
        CHARON_ERROR("Could not set timeouts.");
        return;
    }
    return;
}

static int32_t uart_numAvailableBytes (void)
{
    return INT32_MAX;
} 

static int32_t uart_receive(uint8_t *buffer, uint32_t length)
{
    DWORD numBytes = 0;
    BOOL result = ReadFile(hComm, buffer, length, &numBytes, NULL);
    INT error = GetLastError();
    if (!result && error == 109) uart_init();
    if (numBytes > 10) {
        uint32_t recv_bytes = buffer[6] << 24 | buffer[7] << 16 | buffer[8] << 8 | buffer[9];
        for (int i = 0; i < recv_bytes; i++) {
            buffer[i] = buffer[i+10];
        }
        return (int32_t)recv_bytes;
    }
    return -1;
}

static int32_t uart_send(const uint8_t *data, uint32_t length) 
{
    DWORD sentBytes = 0, numBytes = 0;
    uint8_t message[10 + length];
    message[6] = length >> 24;
    message[7] = length >> 16;
    message[8] = length >> 8;
    message[9] = length;
    memcpy(&message[10], data, length);
    do {
        if(!WriteFile(hComm, &message[sentBytes], 10 + length - sentBytes, &numBytes, NULL)) {
            CHARON_ERROR("Unable to write to the Port.");
            return -1;
        }
        sentBytes += numBytes;
    } while (numBytes < 10 + length);
    return length;
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
