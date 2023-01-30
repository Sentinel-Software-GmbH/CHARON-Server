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
 * @defgroup showcaseDemoPort showcase demo
 * @{
 * @file charon_config.h
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

/** @brief Macro to switch comfortably on or off charon  welcome message. */ 
#define CHARON_CONFIG_DO_NOT_PRINT_WELCOME 1
/** @brief Macro to switch comfortably between endianess. */
#define CHARON_CONFIG_IS_BIG_ENDIAN 0
/** @brief Macro to switch comfortably logging messages with function.*/
#define CHARON_CONFIG_LOG_MSG_OUT_AS_FUNCTION 1
/** @brief Macro to switch comfortably logging messages with Macro.*/
#define CHARON_CONFIG_LOG_MSG_OUT_AS_MACRO 0
/** @brief Macro to switch comfortably on or off debug mode for server. */
#define DEBUGGING 0
/** @brief Macro to switch comfortably on or off debug option for nvm. */
#define CHARON_CONFIG_OBD_SUPPORT 0
/** @brief Macro to switch comfortably on or off DTC debug mode for server. */
#define CHARON_CONFIG_DEBUGGING 0

// Used by different files and very important.
/** @brief For DTC Nvm size, NVm config may be changed depending on targets memory.*/
#define AMOUNT_OF_DTC                                   ((uint16_t)111u)
/** @brief For snapshot Nvm size, NVm config may be changed depending on targets memory. AMOUNT_OF_SNAPSHOT and AMOUNT... are only uint8_t so only 255 can be saved max. */
#define AMOUNT_OF_SNAPSHOT                              ((uint8_t) 100u)
/** @brief For storedData Nvm size, NVm config may be changed depending on targets memory. AMOUNT_OF_SNAPSHOT and AMOUNT... are only uint8_t so only 255 can be saved max. */
#define AMOUNT_OF_STOREDDATA                            ((uint8_t) 100u)
/** @brief For extData Nvm size, NVm config may be changed depending on targets memory. AMOUNT_OF_SNAPSHOT and AMOUNT... are only uint8_t so only 255 can be saved max. */
#define AMOUNT_OF_EXTENDEDDATA                          ((uint8_t) 100u)


/** @brief Used to define how many addresses the USER wants to save per DTC and datatype. */
#define NVM_AMOUNT_OF_SNAPSHOTS                         ((uint8_t)3u)
/** @brief Used to define how many addresses the USER wants to save per DTC and datatype. */
#define NVM_AMOUNT_OF_DATARECORDS                       ((uint8_t)3u)
/** @brief Used to define how many addresses the USER wants to save per DTC and datatype. */
#define NVM_AMOUNT_OF_EXTENDED                          ((uint8_t)3u)


/** @brief Used to define how much memory space the USER wants to reserve, for the payload. */
#define NVM_RESERVED_SIZE_SNAPSHOT                      ((uint16_t)100u)
/** @brief Used to define how much memory space the USER wants to reserve, for the payload. */
#define NVM_RESERVED_SIZE_DATARECORD                    ((uint16_t)100u)
/** @brief Used to define how much memory space the USER wants to reserve, for the payload. */
#define NVM_RESERVED_SIZE_EXTENDED                      ((uint16_t)100u)

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

#endif /* CHARON_CONFIG_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/