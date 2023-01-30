/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Andreas Hofmann
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
 * @defgroup Interface Interface
 * @{
 * @file charon_uds.h
 * This Module Contains All Interfaces provided to the Application to Handle the
 * CharonUDS Stack.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_H_
#define CHARON_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include "ISocket.h"
#include "charon_config.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/** @brief Function resets all dependent charon server processes */
void charon_reset (void);

/** @brief The function initializes charon sever.
 * 
 * @param systemCommunicationSocket Socket to be initialized @ref ISocket_t.
 */
void charon_init (ISocket_t systemCommunicationSocket);

/** @brief Task can be cyclic called to fullfil its duty to respond to client requests. */
void charon_task (void);

#endif /* CHARON_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
