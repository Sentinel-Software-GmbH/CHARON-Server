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
 * @addtogroup ComLogic
 * @{
 * @file charon_SessionAndServiceControl.c
 * Implementation of the Service and Session Control Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 * main.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "Interface/charon_uds.h"
#include "Interface/Socket/ISocket.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

extern void pipe_init (void);
extern ISocket_t uart_socket;

int main (void)
{
    pipe_init();
    charon_init(uart_socket);
    while (1)
    {
        charon_task();
    }
    return 0;
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
