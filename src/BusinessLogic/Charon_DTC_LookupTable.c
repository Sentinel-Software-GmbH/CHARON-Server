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
 * @addtogroup CharonUDS
 * @{
 * @addtogroup BusinessLogic Business Logic
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
#include "Charon_DTC_LookupTable.h"
#include "charon_StoredDataTransmissionFunctionalUnit.h"
#include "charon_interface_NvmDriver.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/
#define HEADER_OFFSET ((uint8_t) 1u)

/* Variables *****************************************************************/

static const uint8_t DTCStatusAvailabilityMask =
(
    DTC_Enable_testFailed_Status_Bit                            << 0u |
    DTC_Enable_testFailedThisOperationCycle_Status_Bit          << 1u |
    DTC_Enable_pendingDTC_Status_Bit                            << 2u |
    DTC_Enable_confirmedDTC_Status_Bit                          << 3u |
    DTC_Enable_testNotCompletedSinceLastClear_Status_Bit        << 4u |
    DTC_Enable_testFailedSinceLastClear_Status_Bit              << 5u |
    DTC_Enable_testNotCompletedThisOperationCycle_Status_Bit    << 6u |
    DTC_Enable_warningIndicatorRequested_Status_Bit             << 7u 
);

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

/* Private Function **********************************************************/

uint8_t charon_getDTCStatusAvailabilityMask(void)
{
    return DTCStatusAvailabilityMask;
}


DTC_t* charon_getDTCLookupByDTCNumber (uint8_t DTCHighByte, uint8_t DTCMiddleByte, uint8_t DTCLowByte)
{
    DTC_t* pDTCEntry = NULL;
    uint32_t countOfSavedDTC = charon_StoredDataTransmissionFunctionalUnit_getDTCcount();
    DTC_t *DTC = (DTC_t*)charon_NvmDriver_getNvmAddress();

    for(uint32_t i=0; i < countOfSavedDTC; i++)
    {
        if((DTCHighByte == DTC[i].DTCHighByte) && (DTCMiddleByte == DTC[i].DTCMiddleByte) && (DTCLowByte == DTC[i].DTCLowByte))
        {
            pDTCEntry = &DTC[i];
        }
    }
    return pDTCEntry;
}


DTC_t* charon_getDTCLookupByStatusMask (uint8_t StatusMask, uint16_t offset)
{
    DTC_t* pDTCEntry = NULL;
    uint8_t isStatusMask = 0u;
    uint16_t offsetCounter = offset;
    uint16_t counter = 0u;
    DTC_t *DTC = (DTC_t*)charon_NvmDriver_getNvmAddress();
    uint32_t countOfSavedDTC = charon_StoredDataTransmissionFunctionalUnit_getDTCcount();

    do
    {
        isStatusMask = (DTC[(counter + HEADER_OFFSET)].DTCStatusMask & StatusMask);
        if(isStatusMask != 0u)
        {
            pDTCEntry = &DTC[counter + HEADER_OFFSET];
            offsetCounter --;
        }

        counter ++;
        
        if(countOfSavedDTC + HEADER_OFFSET == counter)
        {
            pDTCEntry = NULL;
            break;
        }

    } while (offsetCounter > 0u );
    
    return pDTCEntry;
}


uint32_t charon_getDTCCountByStatusMask (uint8_t StatusMask)
{
    uint16_t retval = 0u;
    uint8_t isStatusMask = 0u;
    uint32_t countOfSavedDTC = charon_StoredDataTransmissionFunctionalUnit_getDTCcount();
    DTC_t *DTC = (DTC_t*)charon_NvmDriver_getNvmAddress();


    for(uint16_t i = 0u; i < countOfSavedDTC; i++)
    {
        isStatusMask = (DTC[(i+HEADER_OFFSET)].DTCStatusMask & StatusMask);
        if( isStatusMask != 0)
        {
            retval ++;
        }
    }
    return retval;
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
