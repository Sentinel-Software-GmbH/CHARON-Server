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
 * @defgroup ComLogic Communication Logic Modules
 * @{
 * @file charon_SessionAndServiceControl.h
 * This Module handles the Receive and Transfer of the charon uds stack data.
 * It Controls encryption and checks for all demanded Timeouts.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_SERVICEANDSESSIONCONTROL_H_
#define CHARON_SERVICEANDSESSIONCONTROL_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include "ISocket.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/** @brief Session Types */
typedef enum
{
    charon_sscType_invalid,
    charon_sscType_default,         /**< charon_sscType_default */
    charon_sscType_programming,     /**< charon_sscType_programming */
    charon_sscType_extended,        /**< charon_sscType_extended */
    charon_sscType_secured,         /**< charon_sscType_secured */

    charon_sscType_amount,          /**< The amount of valid session IDs */
    charon_sscType_notStarted,      /**< There is no active session with a client */
    charon_sscType_timedOut         /**< A session has timed out, a library wide reset is necessary */
} charon_sessionTypes_t;

/* Interfaces ****************************************************************/

/** @brief The function resets all Session and Service Control involved Parameters to default. */
void charon_sscReset (void);

/** @brief The function initializes the System given Communication Socket.
 * 
 * @param sscComSocket Abstract Hardware socked that should be used to initializes Communication Socket.
 */
void charon_sscInit (ISocket_t sscComSocket);

/** @brief This function checks if current session is timed out and checks if the current session needs to be exceeded. */
void charon_sscCyclic(void);

/** @brief The function handles incoming messages and process them. */
void charon_sscRcvMessage (void);

/** @brief Function is used to send data to client. 
 * 
 * @param pBuffer Contains data to send from Server to Client.
 * @param length Size of pBuffer in bytes
 */
void charon_sscTxMessage (uint8_t const * const pBuffer, uint32_t length);

/** @brief This function is used to change the current Session.  
 * 
 * @param sessionType Session to set to @ref charon_sessionTypes_t.
 * @param timeoutP2 Timeout for the initial answer @ref ComTimeoutLimits_t.
 * @param timeoutP2extended Timeout for the extended answer @ref ComTimeoutLimits_t.
 */
void charon_sscSetSession (charon_sessionTypes_t sessionType, uint32_t timeoutP2, uint32_t timeoutP2extended);

/** @brief Function to get current Session. 
 * 
 * @return @ref charon_sessionTypes_t. 
 */
charon_sessionTypes_t charon_sscGetSession (void);

/** @brief Function checks timestamp for tester present heartbeat. */
void charon_sscTesterPresentHeartbeat(void);

#endif /* CHARON_SERVICEANDSESSIONCONTROL_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
