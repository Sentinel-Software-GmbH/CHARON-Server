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

#include "Interface/charon_uds.h"
#include "Interface/Socket/ISocket.h"
#include "HSDI/charon_interface_debug.h"

/* Imports *******************************************************************/

extern void pipe_init (void);
extern ISocket_t pipe_socket;
extern void uart_init (void);
extern ISocket_t uart_socket;

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

int main (void)
{
    pipe_init();
    charon_init(pipe_socket);
    CHARON_INFO("CharonUDS Initialized...");
    while (1)
    {
        charon_task();
    }
    return 0;
}

/* Private Function **********************************************************/
