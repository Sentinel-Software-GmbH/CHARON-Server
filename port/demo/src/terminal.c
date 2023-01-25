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
 * @addtogroup CharonUDS_Server
 * @{
 * @addtogroup ShowCase Show Case demo
 * @{
 * @file terminal.c
 * Implementation of the Terminal output used for debug without communication between server and client. 
 * Used for server only debugging.
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

/** @brief The function ignores alls received data. 
 * 
 * @param buffer buffer with received data.
 * @param length Number of received bytes in bytes.
 * @return Returns immediately to  ignore all data.
 */
static int32_t terminal_receive (uint8_t *buffer, uint32_t length);

/** @brief Function is used to get number of available bytes. 
 * 
 * @return Number of bytes.
 */
static int32_t terminal_numAvailableBytes (void);

/** @brief Function is used to print data into terminal output for debug purposes.
 * 
 * @param data Buffer containing data to send to terminal output.
 * @param length Size of buffer in Bytes.
 * @return Number Bytes send to terminal in bytes.
 */
static int32_t terminal_send(const uint8_t *data, uint32_t length);

/* Interfaces  ***************************************************************/

/** @brief Abstract hardware socket of target system. */
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

    return length;
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
