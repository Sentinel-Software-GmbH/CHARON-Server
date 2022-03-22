/*
 * uart.c
 * 
 * Created on: 14.09.2020
 *     Author: Steven Inácio
 */

#include "ISocket.h"
#include <windows.h>
#include "charon_interface_debug.h"


HANDLE hComm;
DWORD dwRead;
BOOL fWaitingOnRead = FALSE;
OVERLAPPED osReader = {0};
DCB UartConfig;
COMMTIMEOUTS UartTimeouts;
LPTSTR COM_PORT_NAME  = "\\\\.\\COM12";

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
    return UINT32_MAX;
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

static int32_t uart_send(const uint8_t *data, uint32_t length) {
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

ISocket_t uart_socket = {
    .numAvailableBytes = uart_numAvailableBytes,
    .receive = uart_receive,
    .transmit = uart_send
};