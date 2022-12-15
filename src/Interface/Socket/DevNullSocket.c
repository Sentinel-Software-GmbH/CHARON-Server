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
 * @addtogroup CharonUDS_Server
 * @{
 * @addtogroup Socket Hardware abstraction Socket
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

#include "DevNullSocket.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/** @brief Dummy function to ignore data like "dev/null" on unix systems.
 * 
 * @return returns 0 immediately to ignore data.
 */
static int32_t DevNull_numOfData(void);

/** @brief Dummy function to ignore data like "dev/null" on unix systems.
 * 
 * @param buf Date buffer pointer can contain data but will be ignored.  
 * @param len Size of data buffer in bytes.
 * @return returns 0 immediately to ignore data.
 */
static int32_t DevNull_receive(uint8_t *buf, uint32_t len);

/** @brief Dummy function to ignore data like "dev/null" on unix systems.
 * 
 * @param buf Date buffer pointer can contain data but will be ignored.  
 * @param len Size of data buffer in bytes.
 * @return Number of bytes in bytes that are ignored.
 */
static int32_t DevNull_transmit(const uint8_t *buf, uint32_t len);

/* Interfaces  ***************************************************************/

const ISocket_t DevNullSocket =
{
        DevNull_numOfData,
        DevNull_receive,
        DevNull_transmit
};

/* Private Function **********************************************************/

static int32_t DevNull_numOfData(void)
{
    return 0;
}

static int32_t DevNull_receive(uint8_t *buf, uint32_t len)
{
    return 0;
}

static int32_t DevNull_transmit(const uint8_t *buf, uint32_t len)
{
    return len;
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/

