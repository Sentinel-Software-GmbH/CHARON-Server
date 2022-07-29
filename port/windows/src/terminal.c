/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 René Mutscheller
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
 * @file terminal.c
 * Implementation of the Terminal output
 * 
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "ISocket.h"
#include "charon_interface_debug.h"


/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

static int32_t terminal_receive (uint8_t *buffer, uint32_t length);
static int32_t terminal_numAvailableBytes (void);
static int32_t terminal_send(const uint8_t *data, uint32_t length);

/* Interfaces  ***************************************************************/

ISocket_t terminal_socket = {
    .numAvailableBytes = terminal_numAvailableBytes,
    .receive = terminal_receive,
    .transmit = terminal_send
};

/* Private Function **********************************************************/

static int32_t terminal_numAvailableBytes (void)
{
    return UINT32_MAX;
} 

static int32_t terminal_receive (uint8_t *buffer, uint32_t length)
{
    return 0;
}

static int32_t terminal_send(const uint8_t *data, uint32_t length)
{
    uint8_t buff[1024];
    uint32_t i, pos = 0u;

    for (i = 0; i < length; i++)
    {
        sprintf((char *)&buff[pos], "0x");
        pos += 2u; 
        sprintf((char *)&buff[pos], "%02x", data[i]); 
        pos += 2u;
        buff[pos] = ' ';
        pos++;
    }

    buff[pos] = '\0';

    CHARON_INFO("Debug Console Output for Terminal Socket: %s", buff);
    fflush(stdout);

    return length;
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
