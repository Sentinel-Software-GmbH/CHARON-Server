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
 * @addtogroup BusinessLogic
 * @{
 * @file charon_SessionAndServiceControl.c
 * Implementation of the Service and Session Control Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <stdint.h>
#include "charon_types.h"
#include "charon_DataLookupTable.h"
#include "charon_ServiceLookupTable.h"
#include "charon_SessionAndServiceControl.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

static charon_dataIdentifierObject_t charonDIDLookupTable[] =
{
    /* Data Identifier */     /* length */      /* Allowed Sessions */                                                              /* Start Address of Data */         /* Has Scaling Data */      /* Scaling Data Length */      /* Scaling Data Address */
    {uds_data_test_did,          10u,            (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),       0x00000000,                        true,                       10,                             0x00000000},
    {uds_data_test_did2,         10u,            (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),       0x00000000,                        false,                      10,                             0x00000000},

};

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

/* Private Function **********************************************************/

int init (void)
{
    for(int counter = 0; counter < ARRAY_SIZE(charonDIDLookupTable); counter ++)
    {
        charon_dataIdentifierObject_t * pTableEntryBase = &charonDIDLookupTable[counter];
        charon_dataIdentifierObject_t * pTableEntry = &charonDIDLookupTable[counter + 1];
        pTableEntry->AddressOfData = pTableEntryBase->AddressOfData + pTableEntry->lengthOfData;
    }  
    return 0;
}


charon_dataIdentifierObject_t* charon_getDataLookupTableByDID (uint16_t DID )
{
    charon_dataIdentifierObject_t* pDidEntry = NULL;

    for(uint32_t i=0; i < ARRAY_SIZE(charonDIDLookupTable); i++)
    {
        if(DID == charonDIDLookupTable[i].DID)
        {
            pDidEntry = &charonDIDLookupTable[i];
        }
    }
    return pDidEntry;        
}

charon_dataIdentifierObject_t* charon_getDataLookupTableByAddress (uint32_t DataAddress)
{
    charon_dataIdentifierObject_t* pAddressEntry = NULL;

    for(uint32_t i=0; i < ARRAY_SIZE(charonDIDLookupTable); i++)
    {
        if(DataAddress == charonDIDLookupTable[i].AddressOfData)
        {
            pAddressEntry = &charonDIDLookupTable[i];
        }
    }
    return pAddressEntry;
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
