/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2023 Rene Mutscheller
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
 * @file main.c
 *  Charon Server Demo port implementation. 
 *  Can be used as example code.
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
#include "demo.h"


/* Imports *******************************************************************/

/** @brief Function to initialize pipe for communication between Client and Server when both entities launched on windows. */ 
extern void pipe_init (void);
/** @brief abstract windows hardware Interface for communication. */
extern ISocket_t pipe_socket;
/** @brief Function to initialize pipe for communication between Client and Server when communication via uart. */ 
extern void uart_init (void);
/** @brief abstract uart hardware Interface for communication. */
extern ISocket_t uart_socket;


/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

/** @brief main function for charon demo port */
int main (void)
{
    pipe_init();
    charon_init(pipe_socket);

    // fills dummy data into emulated NVM.
    DTC_demo();

    CHARON_INFO("CharonUDS Initialized...");
    while (1)
    {
        charon_task();
    }
    return 0;
}


/* Private Function **********************************************************/


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
