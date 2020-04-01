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
 * @file charon_ServiceLookupTable
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
#include <Common/charon_types.h>

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

/**
 * UDS Service Function Signature
 */
typedef int32_t (*charonUdsFunctionSignature)(uint8_t *const pData, uint32_t length);

/**
 * Service Object Type.
 * This describes all Attributes needed to check execution rights of
 * a UDS Service
 */
typedef struct charon_serviceAttributes_t_public
{
    uds_sid_t                   sid;
    uint32_t                    sessionMask;
    charonUdsFunctionSignature  serviceRunable;
} charon_serviceAttributes_t;

typedef struct charon_serviceObject_t_public
{
    charon_serviceAttributes_t const * const    attributes;
    uint32_t                                    emcryptionMask;
}charon_serviceObject_t;

/* Interfaces ****************************************************************/

charon_serviceObject_t* charon_ServiceLookupTable_getServiceObject( uds_sid_t sid);



#endif /* CHARON_SERVICELOOKUPTABLE_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
