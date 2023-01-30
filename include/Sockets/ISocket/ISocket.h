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
 * @file ISocket.h
 * An abstract interface to all kind of hardware interfaces, like uart, usb, spi,
 * even tcp/ip and file access are possible. This way the upper layer software is
 * free from all implementation details and is more versatile.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef ISOCKET_H
#define ISOCKET_H

/* Includes ******************************************************************/

#include <stdint.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/** @brief abstract interface to all kind of hardware interfaces. */
typedef struct ISocket_t_public
{
    int32_t (*numAvailableBytes)(void);                     /**< User should implement this function for hardware abstraction. */
    int32_t (*receive)(uint8_t *buf, uint32_t len);         /**< User should implement this function for hardware abstraction. */
    int32_t (*transmit)(const uint8_t *buf, uint32_t len);  /**< The User should implement this function if it is used for hardware abstraction. */
} ISocket_t;

/* Interfaces ****************************************************************/

#endif /* ISOCKET_H */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
