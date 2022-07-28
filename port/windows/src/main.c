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
 * @addtogroup Port
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

#include "charon_uds.h"
#include "ISocket.h"
#include "charon_interface_debug.h"


#define DEBUGGING 1
#define FILL_NVM_AUTOMATIC 1


#if DEBUGGING
#include "BusinessLogic/charon_StoredDataTransmissionFunctionalUnit.h"
#include "charon_interface_NvmDriver.h"
#endif

/* Imports *******************************************************************/

extern void pipe_init (void);
extern ISocket_t pipe_socket;
extern void uart_init (void);
extern ISocket_t uart_socket;

#if DEBUGGING
extern ISocket_t terminal_socket;
#endif

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/
void debug (void);

/* Interfaces  ***************************************************************/

int main (void)
{
    pipe_init();

#if !DEBUGGING
    charon_init(pipe_socket);
#endif 

#if DEBUGGING
    charon_init(terminal_socket);
    debug ();
#endif    


    CHARON_INFO("CharonUDS Initialized...");
    while (1)
    {
        charon_task();
    }
    return 0;
}





#if DEBUGGING
void debug (void)
{
#if !FILL_NVM_AUTOMATIC
    //write to first
    charon_StoredDataTransmissionFunctionalUnit_writeDTCto_Nvm(0,111,1,2,3,222,222,222,222,222);
    //write to second
    charon_StoredDataTransmissionFunctionalUnit_writeDTCto_Nvm(0,222,222,222,222,222,222,222,222,222);
    //write to third
    charon_StoredDataTransmissionFunctionalUnit_writeDTCto_Nvm(0,111,7,8,9,222,222,222,222,222);
    //change second
    charon_StoredDataTransmissionFunctionalUnit_writeDTCto_Nvm(2,155,4,5,6,111,111,111,111,111);
#endif

#if FILL_NVM_AUTOMATIC
    for (uint16_t i = 0; i < (255-3); i++)
    {
        //write into NVM
        charon_StoredDataTransmissionFunctionalUnit_writeDTCto_Nvm(0,0b11111111,i+1,i+2,i+3,111,111,111,111,111);
    }
#endif


    // Input your SID and other masks here           high middle low
    //                          SID  SUB     MASK    H B   M B   L B
    uint8_t receiveBuffer[] = {0x09,0x01,0b11111111, 0xCA, 0xFF, 0xEE ,0x01};
    uint32_t receiveBufferSize = sizeof(receiveBuffer);


    //choose your function
    charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation (receiveBuffer,receiveBufferSize);
}
#endif

/* Private Function **********************************************************/
