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
 * @addtogroup CharonUDS
 * @{
 * @defgroup Interface
 * @{
 * @file charon_uds
 * This Interface Describes the Functions needed by CHARON UDS for
 * its debug output Requirements.
 * For production these functions can be stubbed, so no debug overhead
 * is generated by debug messages.
 * For development these functions can be routed to any kind of
 * serial interface or console output. CHARON expects a printf-like
 * behavior.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_CONFIG_H_
#define CHARON_CONFIG_H_

/* Includes ******************************************************************/

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

#define CHARON_CONFIG_DO_NOT_PRINT_WELCOME 0
#define CHARON_CONFIG_IS_BIG_ENDIAN 1
#define CHARON_CONFIG_LOG_MSG_OUT_AS_FUNCTION 1

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

#endif /* CHARON_CONFIG_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
