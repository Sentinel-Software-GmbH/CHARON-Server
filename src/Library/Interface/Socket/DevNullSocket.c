/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2020 Andreas Hofmann
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
 * @addtogroup HSDI
 * @{
 * @file DevNullSocket.c
 * Implementation of NULL Socket.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "Interface/Socket/DevNullSocket.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

static uint32_t DevNull_numOfData(void);
static uint32_t DevNull_receive(uint8_t *buf, uint32_t len);
static uint32_t DevNull_transmit(const uint8_t *buf, uint32_t len);

/* Interfaces  ***************************************************************/

extern const ISocket_t DevNullSocket =
{
        DevNull_numOfData,
        DevNull_receive,
        DevNull_transmit
};

/* Private Function **********************************************************/

static uint32_t DevNull_numOfData(void)
{
    return 0;
}

static uint32_t DevNull_receive(uint8_t *buf, uint32_t len)
{
    return 0;
}

static uint32_t DevNull_transmit(const uint8_t *buf, uint32_t len)
{
    return len;
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
