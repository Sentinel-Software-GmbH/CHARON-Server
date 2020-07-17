/*
 * main.c
 *
 *  Created on: 15.07.2020
 *      Author: Florian Kaup
 */


#include <stdint.h>
#include <windows.h>
#include <stdio.h>

//static const uint8_t [] = {};


HANDLE pipeHandle;
LPCSTR pipeName = "\\\\.\\pipe\\charonserver";

void initPipe (void)
{
    pipeHandle = CreateFile(
            pipeName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_ALWAYS,
            0,
            NULL
            );
    DWORD mode = PIPE_READMODE_MESSAGE;
    SetNamedPipeHandleState(pipeHandle, &mode, NULL, NULL);
}

void transferData (uint16_t count, uint8_t* data)
{
    printf("sending:\t");
    for (uint16_t i = 0; i<count ; i++)
    {
        printf("0x%x ", data[i]);
    }
    printf("\n");

    DWORD numBytes = 0;
    uint8_t receiveData[4100];
    TransactNamedPipe(pipeHandle, data, count, receiveData, sizeof(receiveData), &numBytes, NULL);

    printf("receiving:\t");
    for (uint16_t i = 0; i<numBytes ; i++)
    {
        printf("0x%x ", receiveData[i]);
    }
    printf("\n");
}



int main (int argc, char ** argv)
{
    initPipe();

    printf("try to reset...\n");
    uint8_t resetCommand[] = {0x11, 0x01};
    transferData(sizeof(resetCommand), resetCommand);

    printf("change session to programming...\n");
    uint8_t changeSessionToProgrammingCommand[] = {0x10, 0x02};
    transferData(sizeof(changeSessionToProgrammingCommand), changeSessionToProgrammingCommand);

    printf("erase memory...\n");
    uint8_t eraseFlashCommand[] = {0x31, 0x01, 0xff, 0x00};
    transferData(sizeof(eraseFlashCommand), eraseFlashCommand);

    printf("request download...\n");
    uint8_t requestDownloadCommand[] = {0x34, 0x00, 0x22, 0x00, 0x00, 0x02, 0x00};
    transferData(sizeof(requestDownloadCommand), requestDownloadCommand);

    printf("transfer data...\n");
    uint8_t transferDataCommand[0x200] = "\066This is a very big penis C================8";
    transferData(sizeof(transferDataCommand), transferDataCommand);


    return 0;
}


