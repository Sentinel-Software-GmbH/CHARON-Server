/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Kaup
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
 * @addtogroup SystemTest
 * @{
 * @file main.c
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <stdint.h>
#include <windows.h>
#include <stdio.h>

//static const uint8_t [] = {};

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

HANDLE pipeHandle;
LPCSTR pipeName = "\\\\.\\pipe\\charonserver";

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

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
    uint8_t transferDataCommand1[0x100] = "\066\001This is a very big ... lamp C================8";
    transferData(sizeof(transferDataCommand1), transferDataCommand1);
    uint8_t transferDataCommand2[0x100] = "\066\002More lamp C================8";
    transferData(sizeof(transferDataCommand2), transferDataCommand2);
    uint8_t transferDataCommand3[0x6] = "\066\003oO";
    transferData(sizeof(transferDataCommand3), transferDataCommand3);

    printf("transfer exit...\n");
    uint8_t transferExitCommand[0x1] = {0x37};
    transferData(sizeof(transferExitCommand), transferExitCommand);

    return 0;
}

/* Private Function **********************************************************/

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
