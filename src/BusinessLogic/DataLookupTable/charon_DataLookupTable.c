/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Schöner
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
 * @addtogroup BusinessLogic Business Logic Modules
 * @{
 * @file charon_DataLookupTable.c
 * Implementation of the Data Lookup Table
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

/**
 * @brief the lookup table for the Data Identifiers to store and handle all the needed Data
 * @ref charon_dataIdentifierObject_t
 */
static charon_dataIdentifierObject_t charonDIDLookupTable[] =
{
    /* Data Identifier */           /* Allowed Sessions */                                                           /* length */   /* Start Address of Data */      /* Has Scaling Data */      /* Scaling Data Length */       /* Scaling Data Address */  /* DID is Read only*/      /* DynamicallyDefineDID */
    {uds_data_test_did,             (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),      10u,         0x100000,                        true,                       10,                             0x00000000,                 false,                     false},
    {uds_data_test_did2,            (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),      10u,         0x200000,                        false,                      10,                             0x00000000,                 true,                      false},
    {uds_data_test_periodic,        (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),      10u,         0x150000,                        false,                      10,                             0x00000000,                 false,                     false},
    {uds_data_test_periodic1,       (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),      10u,         0x160000,                        false,                      10,                             0x00000000,                 false,                     false},
    {uds_data_test_dynamic,         (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),      10u,         0x140000,                        false,                      10,                             0x00000000,                 false,                     true},
};


/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void charonDIDLookUpInit (void)
{
    for(uint32_t counter = 0; counter < ARRAY_SIZE(charonDIDLookupTable); counter ++)
    {
        charon_dataIdentifierObject_t * pTableEntryBase = &charonDIDLookupTable[counter];
        charon_dataIdentifierObject_t * pTableEntry = &charonDIDLookupTable[counter + 1];
        if(pTableEntryBase->lengthOfData == 0u)
        {
            pTableEntry->addressOfData = pTableEntryBase->addressOfData + MAX_VARIABLE_DATA_LENGTH;
        }
        else
        {
            pTableEntry->addressOfData = pTableEntryBase->addressOfData + pTableEntryBase->lengthOfData;
        }
    }  
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

charon_dataIdentifierObject_t* charon_getDataLookupTableByAddress (uint32_t dataAddress)
{
    charon_dataIdentifierObject_t* pAddressEntry = NULL;

    for(uint32_t i=0; i < ARRAY_SIZE(charonDIDLookupTable); i++)
    {
        if(dataAddress == charonDIDLookupTable[i].addressOfData)
        {
            pAddressEntry = &charonDIDLookupTable[i];
        }
    }
    return pAddressEntry;
}


/* Private Function **********************************************************/

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
