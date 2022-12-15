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
 * @defgroup ComLogic Communication Logic Modules
 * @{
 * @file charon_ServiceLookupTable.h
 * This Module holds the lookup Table to connect all relevant data of
 * allowance and target FU for every service.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_SERVICELOOKUPTABLE_H_
#define CHARON_SERVICELOOKUPTABLE_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include <charon_types.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/
/** @brief Cast and shift charon_sscType_default for later usage in serviceLookupTable. */
#define SESSION_DEFAULT			((uint32_t) 1u << (uint8_t)charon_sscType_default)
/** @brief Cast and shift charon_sscType_programming for later usage in serviceLookupTable. */
#define SESSION_PROGRAMMING  	((uint32_t) 1u << (uint8_t)charon_sscType_programming)
/** @brief Cast and shift charon_sscType_extended for later usage in serviceLookupTable. */
#define SESSION_EXTENDED		((uint32_t) 1u << (uint8_t)charon_sscType_extended)
/** @brief Cast and shift charon_sscType_secured for later usage in serviceLookupTable. */
#define SESSION_SECURED			((uint32_t) 1u << (uint8_t)charon_sscType_secured)

/* Types *********************************************************************/

/** @brief Service Function Signature */
typedef uds_responseCode_t (*charonUdsFunctionSignature)(const uint8_t * pData, uint32_t length);

/** @brief Service Object Type. This describes all Attributes needed to check execution rights of a UDS Service.*/
typedef struct
{
    uds_sid_t                   sid;                /**< Service ID @ref uds_sid_t. */
    uint32_t                    sessionMask;        /**< Mask for current session. */
    charonUdsFunctionSignature  serviceRunable;     /**< Function called to run Service. */ 
    uint32_t                    emcryptionMask;     /**< Type of used encryption. */
} charon_serviceObject_t;

/* Interfaces ****************************************************************/

/** @brief Function is used to get a Service id object out of ServiceLookupTable. 
 * 
 * @param sid Service ID to get from LookupTable.
 * @return @ref charon_serviceObject_t* 
 */
charon_serviceObject_t* charon_ServiceLookupTable_getServiceObject (uint8_t sid);

/** @brief Function is used to get the name of Service ID as string.
 * 
 * @param sid The SID to get the name from.
 * @return Name of SID as a string.
 */
const char * charon_ServiceLookupTable_getNameForServiceSid (uint8_t sid);

/** @brief Function is used to get the name of uds_responseCode_t as a string.
 * 
 * @param returnCode Return code to get the name from.
 * @return Name of ReturnCode as string.
 */
const char * charon_ServiceLookupTable_getNameForReturnCode (uds_responseCode_t returnCode);

#endif /* CHARON_SERVICELOOKUPTABLE_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
