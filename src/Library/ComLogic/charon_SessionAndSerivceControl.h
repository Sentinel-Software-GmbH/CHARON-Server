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
 * @defgroup ComLogic
 * @{
 * @file charon_SessionAndSerivceControl.h
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

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/**
 * UDS Session Types
 */
typedef enum charon_sessionTypes_t_public
{
    charon_sscType_default,         /**< charon_sscType_default */
    charon_sscType_programming,     /**< charon_sscType_programming */
    charon_sscType_extended,        /**< charon_sscType_extended */
    charon_sscType_secured,         /**< charon_sscType_secured */

    charon_sscType_amount /**< charon_sscType_amount */
} charon_sessionTypes_t;

/* Interfaces ****************************************************************/

int32_t charon_sscRcvProcessMessage (uint8_t * const pBuffer, uint32_t length);

void charon_sscTxProcessMessage (uint8_t * const pBuffer, uint32_t length);

int32_t charon_sscSetSession (charon_sessionTypes_t sessionType, uint32_t timeout);

charon_sessionTypes_t charon_sscGetSession (void);

#endif /* CHARON_SERVICEANDSESSIONCONTROL_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
