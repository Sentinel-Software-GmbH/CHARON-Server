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
 * @defgroup BusinessLogic Business Logic
 * @{
 * @file charon_DataLookupTable.h
 * This Module handles the Receive and Transfer of the charon uds stack data.
 * It Controls encryption and checks for all demanded Timeouts.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_DATALOOKUPTABLE_H_
#define CHARON_DATALOOKUPTABLE_H_

/* Includes ******************************************************************/
#include <stdint.h>

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

typedef struct charon_dataIdentifierObject_t_public
{
    uds_data_t      DID; /* dataIdentifier */
    uint32_t        sessionMask;
    uint32_t        lengthOfData;
    uint32_t        AddressOfData;
} charon_dataIdentifierObject_t;

typedef enum
{
    uds_data_test_did  = 0x4269,
    uds_data_test_did2 = 0x6942,

    uds_data_amount
} uds_data_t;


/* Interfaces ****************************************************************/

charon_dataIdentifierObject_t* charon_getDataLookupTable (uint16_t DID, uint32_t DataAddress);

#endif /* CHARON_DATALOOKUPTABLE_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
