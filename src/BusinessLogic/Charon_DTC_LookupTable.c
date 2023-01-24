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
 * @file charon_DTC_LookupTable.c
 * Implementation of the DTC Lookup Table Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/
#include <stdint.h>
#include <string.h>
#include "charon_types.h"
#include "charon_config.h"
#include "charon_DTC_LookupTable.h"
#include "charon_negativeResponse.h"
#include "charon_interface_NvmDriver.h"
#include "charon_SessionAndServiceControl.h"
#include "charon_StoredDataTransmissionFunctionalUnit.h"

#include "charon_interface_debug.h"


/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/** @brief Max Transmission Massage buffer size */
#define MAX_TX_BUFFER_SIZE          ((uint32_t) 1024u)

/** @brief Macro: split input in 8byte index and number to shift */
#define LOOKUP_CALC(i, index, number) \
index = i / 8u; \
number = i % 8u

/** @brief Special user input defined in ISO 14229-1:2013: 0x33 = Emission. */
#define EMISSION                    ((uint8_t) 0x33)
/** @brief Special user input defined in ISO 14229-1:2013: 0xD0 = Safety. */
#define SAFETY                      ((uint8_t) 0xD0)
/** @brief Special user input defined in ISO 14229-1:2013: 0xFE = VOBD. */
#define VOBD                        ((uint8_t) 0xFE)

/** @brief For for loops to check up to 8x the same index. */
#define BYTE_SIZE                   ((uint8_t) 8u)


/* Variables *****************************************************************/

/** @brief Builds the StatusMask out of in charon_DTC_LookupTable.h defined bits. */
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

/**
 * @brief Function to delete single DTC.
 * 
 * @param found             DTC index in Nvm.
 * @param aDTC              Address to DTC Nvm.
 * @param aDTC_header       Address to DTC_header Nvm.
 * @param aSnap_t           Address to snapshot space in Nvm.
 * @param aStored_t         Address to storedData space in Nvm.
 * @param aExt_t            Address to extData space in Nvm.
 */
static void charon_deleteDTC_inNvm (uint16_t found, uint32_t aDTC, uint32_t aDTC_header, uint32_t aSnap_t, uint32_t aStored_t, uint32_t aExt_t);

/**
 * @brief Small function to verify found input.
 * 
 * @param input             Found index number.
 * @param address           Header address.
 * @return true             Found DTC is valid.
 * @return false            Found DTC is invalid.
 */
static bool charon_DTC_LookupTable_DTClookupTableValidCheck(uint32_t input, uint32_t address);


/* Interfaces  ***************************************************************/

uint8_t charon_getDTCStatusAvailabilityMask(void)
{
    return DTCStatusAvailabilityMask;
}


DTC_t* charon_getDTCLookupByDTCNumber (uint8_t DTCHighByte, uint8_t DTCMiddleByte, uint8_t DTCLowByte, bool mirror, bool userDefMemory, uint8_t MemorySelection)
{
    DTC_t* pDTCEntry = NULL;
    uint32_t totalSavedDTC;
    DTC_t *DTC = NULL;
    bool header = false;
    DTC_header_t *DTC_header = NULL;

    // Depending on the requested function different storage areas are needed. 
    if (userDefMemory)
    {
        DTC = (DTC_t*)charon_NvmDriver_getNvmAddress(); /** @todo USER: Change where you want to safe this information and use MemorySelection! */
        totalSavedDTC = charon_StoredDataTransmissionFunctionalUnit_get_total_userDefDTCCounter(MemorySelection);
    }
    else if (mirror)
    {
        DTC = (DTC_t*)charon_NvmDriver_getMirrorNvmAddress(0,header);
        totalSavedDTC = charon_StoredDataTransmissionFunctionalUnit_get_total_mirrorDTCCounter();
    }
    else
    {
        DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,header);
        DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
        totalSavedDTC = DTC_header->totalDTCCounter;
    }
    
    // Compares all saved DTCs to match the requested DTC, a found DTC is not = valid: does a verification check.
    for(uint32_t i=0; i < (totalSavedDTC); i++)
    {
        if((DTCHighByte == DTC[i].DTCHighByte) && (DTCMiddleByte == DTC[i].DTCMiddleByte) && (DTCLowByte == DTC[i].DTCLowByte))
        {
            if ((mirror) || (userDefMemory) || (charon_DTC_LookupTable_DTClookupTableValidCheck(i,(uint32_t)DTC_header)))
            {
                pDTCEntry = &DTC[i];
                return pDTCEntry;
            }
        }
    }
    return pDTCEntry;
}


DTC_t* charon_getDTCLookupByStatusMask (uint8_t StatusMask, uint16_t offset, bool mirror)
{
    DTC_t* pDTCEntry = NULL;
    uint8_t isStatusMask;
    uint16_t offsetCounter = offset;
    uint32_t countOfSavedDTC;
    uint16_t counter = 0u;
    DTC_t *DTC = NULL;
    DTC_header_t *DTC_header = NULL;
    bool header = false;


    // Depending on the requested function different storage areas are needed. 
    if (mirror)
    {
        DTC = (DTC_t*)charon_NvmDriver_getMirrorNvmAddress(0,header);
        countOfSavedDTC = charon_StoredDataTransmissionFunctionalUnit_get_total_mirrorDTCCounter();
    }
    else
    {
        DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,header);
        DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
        countOfSavedDTC = DTC_header->totalDTCCounter;
    }


    // Depending on requested StatusMask returns DTC address, the offset input prevents the same address to be returned again, a found DTC is not = valid: does a verification check.
    do
    {
        // Only one bit in mask must be a hit.
        isStatusMask = (DTC[counter].DTCStatusMask & StatusMask);
        if(isStatusMask != 0u)
        {
            if ((mirror) || (charon_DTC_LookupTable_DTClookupTableValidCheck(counter,(uint32_t)DTC_header)))
            {
                pDTCEntry = &DTC[counter];
                offsetCounter --;
            }
        }

        counter ++;
        
        if(countOfSavedDTC < counter)
        {
            pDTCEntry = NULL;
            break;
        }

    } while (offsetCounter > 0u );
    
    return pDTCEntry;
}


uint32_t charon_getDTCCountByStatusMask (uint8_t StatusMask, bool mirror, bool userDefMemory, uint8_t MemorySelection)
{
    uint16_t retval = 0u;
    uint8_t isStatusMask;
    uint32_t countOfSavedDTC;
    uint32_t currentSavedDTC;
    DTC_t *DTC = NULL;
    DTC_header_t *DTC_header = NULL;
    bool header = false;


    // Depending on the requested function different storage areas are needed. 
    if (userDefMemory)
    {
        DTC = (DTC_t*)charon_NvmDriver_getNvmAddress(); /** @todo USER: Change where you want to safe this information and use MemorySelection! */
        countOfSavedDTC = charon_StoredDataTransmissionFunctionalUnit_get_total_userDefDTCCounter(MemorySelection);
        currentSavedDTC = countOfSavedDTC;
    }
    else if (mirror)
    {
        DTC = (DTC_t*)charon_NvmDriver_getMirrorNvmAddress(0,header);
        countOfSavedDTC = charon_StoredDataTransmissionFunctionalUnit_get_total_mirrorDTCCounter();
        currentSavedDTC = countOfSavedDTC;
    }
    else
    {
        DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,header);
        DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
        countOfSavedDTC = DTC_header->totalDTCCounter;
        currentSavedDTC = DTC_header->currentDTCCounter;
    }
    

    // Depending on requested StatusMask returns Count of matching DTC, the offset input prevents the same address to be returned again, a found DTC is not = valid: does a verification check.
    for(uint16_t i = 0u; i < countOfSavedDTC; i++)
    {
        // Only one bit in mask must be a hit.
        isStatusMask = (DTC[i].DTCStatusMask & StatusMask);
        if(isStatusMask != 0)
        {
            if ((userDefMemory) || (mirror) || (charon_DTC_LookupTable_DTClookupTableValidCheck(i,(uint32_t)DTC_header)))
            {
                retval ++;
            }

            // Current count must not be the same a total count, to save some cycles an early break.
            if (retval == currentSavedDTC)
            {
                break;
            }
        }
    }
    return retval;
}


DTC_t* charon_getDTCSnapshotAddress (uint16_t offset)
{
    DTC_t* pDTCEntry = NULL;
    DTC_t* DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint16_t offsetCounter = (offset+1u);
    uint16_t counter = 0u;
    uint32_t isSnapshot;
    uint32_t countOfSavedSnapshot = DTC_header->totalSnapshotRecordCounter;


    // Retuns all DTC with a saved snapshot, the offset input prevents the same address to be returned again, a found DTC is not = valid: does a verification check.
    do
    {
        isSnapshot = (DTC[counter].NumberOfSavedSnapshots);
        // The amount of Snapshots can be changed in "charon_StoredDataTransmissionFunctionalUnit.h".
        if((isSnapshot <= sizeof(DTC->DTCSnapshotRecordNumber)) && (isSnapshot >= 1))
        {
            if (charon_DTC_LookupTable_DTClookupTableValidCheck(counter,(uint32_t)DTC_header))
            {
                pDTCEntry = &DTC[counter];

                if (offsetCounter != 0u)
                {
                    offsetCounter --;
                }
            }
        }

        counter ++;
        
        if(countOfSavedSnapshot < counter)
        {
            pDTCEntry = NULL;
            return pDTCEntry;
        }

    } while (offsetCounter > 0u );

    return pDTCEntry;
}


DTC_t* charon_getDTCLookupByRecordNumber (uint8_t RecordNumber)
{
    DTC_t* pDTCEntry = NULL;
    DTC_t *DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t countOfSavedDTC = DTC_header->totalDTCCounter;

    // Array in struct, search for every entry to get the requested RecordNumber, a found DTC is not = valid: does a verification check. 
    for(uint32_t i=0; i < countOfSavedDTC; i++)
    {   
        // The amount of records can be changed in "charon_StoredDataTransmissionFunctionalUnit.h".
        for (uint32_t u = 0; u < sizeof(DTC->DTCStoredDataRecordNumber); u++)
        {
            if(RecordNumber == ((uint8_t) DTC[i].DTCStoredDataRecordNumber[u]))
            {
                if (charon_DTC_LookupTable_DTClookupTableValidCheck(i,(uint32_t)DTC_header))
                {
                    return pDTCEntry = &DTC[i];
                }
            }
        }
    }
    return pDTCEntry;
}


uds_responseCode_t charon_getDTCLookup_reportAllDataRecords (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    DTC_t* DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t countOfSavedDTC = DTC_header->totalDTCCounter;
    uint16_t countOfSavedRecords = DTC_header->totalDataRecordCounter;
    uint8_t buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 2u;
    uint8_t lengthOfDTC = 3u;
    uint8_t counter = 0u;

    // Building response buffer.
    buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    buffer[1] = reportDTCStoredDataByRecordNumber;


    if (countOfSavedRecords == 0)
    {
        memcpy(&buffer[length], &receiveBuffer[2], 1u);
        length++;

        // Empty positive response needs to be displayed even if no DataRecord is saved.
        charon_sscTxMessage(buffer,length);
        //charon info einfügen für debugging!
        return uds_responseCode_PositiveResponse;
    }
    

    // Search for all StoredData and builds the Record buffer.
    for (uint32_t i = 0; i < countOfSavedDTC; i++)
    {
        // The amount of Records can be changed in "charon_StoredDataTransmissionFunctionalUnit.h".
        for (size_t o = 0; o < sizeof(DTC->DTCStoredDataRecordNumber); o++)
        {
            // Check if the new input will fit into the buffer. 
            if((length + 1u + lengthOfDTC + 2u + (uint16_t)DTC[i].DTCStoredDataLength[o]) > MAX_TX_BUFFER_SIZE)
            {
                charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);
                
                return uds_responseCode_ResponseTooLong;
            }

            if (DTC[i].DTCStoredDataLength[o] != 0)
            {   
                // Found DTC is not = valid: does a verification check.
                if (charon_DTC_LookupTable_DTClookupTableValidCheck(i,(uint32_t)DTC_header))
                {
                    // Building response buffer.
                    memcpy(&buffer[length], &DTC[i].DTCStoredDataRecordNumber[o], 1u);
                    length++;
                    memcpy(&buffer[length], &DTC[i].DTCHighByte, lengthOfDTC);
                    length += lengthOfDTC;
                    memcpy(&buffer[length], &DTC[i].statusOfDTC, 1u);
                    length++;
                    memcpy(&buffer[length], &DTC[i].DTCStoredDataAddress[o]->DTCStoredDataRecordNumberOfIdentifiers, 1u); 
                    length++;
                    memcpy(&buffer[length], &DTC[i].DTCStoredDataAddress[o]->DTCStoredDataPayload, (uint16_t)DTC[i].DTCStoredDataLength[o]);
                    length += (uint16_t)DTC[i].DTCStoredDataLength[o];
                }

                counter++;
                
                // Current count must not be the same a total count, to save some cycles an early break.
                if (counter == countOfSavedRecords)
                {
                    break;
                }
            }
        }
        if (counter == countOfSavedRecords)
        {
            // Double break.
            break;
        }
    }
    charon_sscTxMessage(buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


uint32_t charon_getDTCCountBySeverityMask (uint8_t DTCSeverityMask, uint8_t DTCStatusMask)
{
    uint32_t retval = 0u;
    DTC_t *DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0, false);
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t countOfSavedDTC = DTC_header->totalDTCCounter;
    uint32_t currentSavedDTC = DTC_header->currentDTCCounter;


    // Depending on requested DTCStatusMask and DTCSeverityMask returns count of matched DTC, a found DTC is not = valid: does a verification check.
    for (uint32_t i = 0; i < countOfSavedDTC; i++)
    {
        // Only one bit per mask must be a hit.
        if (((DTC[i].DTCSeverityMaskRecordLow & DTCStatusMask) != 0) && ((DTC[i].DTCSeverityMaskRecordHigh & DTCSeverityMask) != 0))
        {
            if (charon_DTC_LookupTable_DTClookupTableValidCheck(i,(uint32_t)DTC_header))
            {
                retval++;

                // Current count must not be the same a total count, to save some cycles an early break.
                if (retval == currentSavedDTC)
                {
                    break;
                }
            }
        }
    }
    return retval;
}


DTC_t* charon_getDTCSeverityMaskAddress (uint8_t DTCSeverityMask, uint8_t DTCStatusMask, uint16_t offset)
{
    DTC_t* pDTCEntry = NULL;
    uint16_t offsetCounter = (offset+1u);
    uint16_t counter = 0u;
    DTC_t *DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t countOfSavedDTC = DTC_header->totalDTCCounter;


    // Depending on requested DTCStatusMask and DTCSeverityMask returns address of matched DTC, the offset input prevents the same address to be returned again, a found DTC is not = valid: does a verification check.
    do
    {
        // Only one bit per mask must be a hit.
        if (((DTC[counter].DTCSeverityMaskRecordLow & DTCStatusMask) != 0) && ((DTC[counter].DTCSeverityMaskRecordHigh & DTCSeverityMask) != 0))
        {
            if (charon_DTC_LookupTable_DTClookupTableValidCheck(counter,(uint32_t)DTC_header))
            {
                pDTCEntry = &DTC[counter];

                if (offsetCounter != 0u)
                {
                    offsetCounter --;
                }
            }
        }

        counter ++;
        
        if(countOfSavedDTC < counter)
        {
            return pDTCEntry = NULL;
        }

    } while (offsetCounter > 0u );
    
    return pDTCEntry;
}


DTC_t* charon_getDTCExtDataAddressByRecordNumber (uint8_t RecordNumber, uint16_t offset , uint32_t* pIndexFoundAt)
{
    uint32_t toBeIgnored = (offset + 0x01);
    DTC_t* pDTCEntry = NULL;
    DTC_t *DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t totalCountOfSavedDTC = DTC_header->totalDTCCounter;


    // Array in struct search for every entry. 
    for(uint32_t i = 0; i < totalCountOfSavedDTC; i++)
    {   
        // The amount of Records can be changed in "charon_StoredDataTransmissionFunctionalUnit.h".
        for (uint32_t u = 0 ; u < sizeof(DTC->DTCExtDataRecordNumber); u++)
        {
            if(RecordNumber == ((uint8_t) DTC[i].DTCExtDataRecordNumber[u]))
            {
                // For the offset to not give the same entry back.
                toBeIgnored--;
                if (toBeIgnored == 0x00)
                {   
                    // Found DTC is not = valid: does a verification check.
                    if (charon_DTC_LookupTable_DTClookupTableValidCheck(i,(uint32_t)DTC_header))
                    {
                        // Index is also needed here.
                        if(pIndexFoundAt != NULL)
                        {
                            *pIndexFoundAt = u;
                        }
                        return pDTCEntry = &DTC[i];
                    }
                }
            }
        }
    }
    return pDTCEntry;
}


uint8_t charon_getDTCExtDataCountByRecordNumber (uint8_t RecordNumber)
{
    DTC_t *DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t totalCountOfSavedDTC = DTC_header->totalDTCCounter;
    uint8_t counter = 0u;

    // Array in struct, search for every entry. 
    for(uint32_t i=0; i < totalCountOfSavedDTC; i++)
    {   
        // The amount of Records can be changed in "charon_StoredDataTransmissionFunctionalUnit.h".
        for (uint32_t u = 0; u < sizeof(DTC->DTCExtDataRecordNumber); u++)
        {
            if(RecordNumber == ((uint8_t) DTC[i].DTCExtDataRecordNumber[u]))
            {   
                // Found DTC is not = valid: does a verification check.
                if (charon_DTC_LookupTable_DTClookupTableValidCheck(i,(uint32_t)DTC_header))
                {
                    counter++;
                }
            }
        }
    }
    return counter;
}


void charon_deleteDTC (uint8_t DTCHighByte, uint8_t DTCMiddleByte, uint8_t DTCLowByte, uint32_t start, uint32_t end, bool vobd, bool safety, bool emission, bool delAll)
{
    uint32_t number;
    uint32_t index;
    uint8_t bitfield;
    uint8_t mask;

    // Prepare address to save cycles.
    DTC_t *DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t totalCountOfSavedDTC = DTC_header->totalDTCCounter;

    DTC_SnapshotData_t *snapshot = (DTC_SnapshotData_t*)charon_NvmDriver_getNvmAddress_for_Snapshot(0);
    DTC_StoredData_t *storedData = (DTC_StoredData_t*)charon_NvmDriver_getNvmAddress_for_StoredData(0);
    DTC_ExtendedData_t *extData = (DTC_ExtendedData_t*)charon_NvmDriver_getNvmAddress_for_ExtendedData(0);


    // For non group delete request.
    /** @todo USER: New groups need to be put in too. */
    if ((start == 0) && (vobd == false) && (safety == false) && (emission == false) && (delAll == false)) 
    {
        for(uint32_t i=0; i < totalCountOfSavedDTC; i++)
        {
            if((DTCHighByte == DTC[i].DTCHighByte) && (DTCMiddleByte == DTC[i].DTCMiddleByte) && (DTCLowByte == DTC[i].DTCLowByte))
            {
                // Is found DTC valid?
                LOOKUP_CALC(i, index, number);
                bitfield = (uint8_t)DTC_header->nvmDTCLookupTable[index];
                mask = 0x01 << number;

                if ((bitfield & mask) == mask)
                {
                    charon_deleteDTC_inNvm (i, (uint32_t)DTC, (uint32_t)DTC_header, (uint32_t)snapshot, (uint32_t)storedData, (uint32_t)extData);
                }
            }
        }
        charon_StoredDataTransmissionFunctionalUnit_CRC16_update();
    }
    

    if (start > 0) // Start has to be at least 0x000100, below is reserved.
    {
        uint32_t DTC_number;
        for(uint32_t i=0; i < totalCountOfSavedDTC; i++)
        {
            DTC_number = ((uint8_t)DTC[i].DTCHighByte << 16u | (uint8_t)DTC[i].DTCMiddleByte << 8u | (uint8_t)DTC[i].DTCLowByte << 0u);
            if ((DTC_number >= start) && (DTC_number <= end))
            {
                // Is found DTC valid?
                LOOKUP_CALC(i, index, number);
                bitfield = (uint8_t)DTC_header->nvmDTCLookupTable[index];
                mask = 0x01 << number;

                if ((bitfield & mask) == mask)
                {
                    charon_deleteDTC_inNvm (i, (uint32_t)DTC, (uint32_t)DTC_header, (uint32_t)snapshot, (uint32_t)storedData, (uint32_t)extData);
                } 
            }
        }
        charon_StoredDataTransmissionFunctionalUnit_CRC16_update();
    }


    /** @todo USER: New groups need to be put in too. */
    if ((vobd) || (safety) || (emission))
    {
        uint8_t group;
        if (vobd)
        {
            group = VOBD;
        } 
        else if (safety)
        {
            group = SAFETY;
        }
        else if (emission)
        {
            group = EMISSION;
        }

        for(uint32_t i=0; i < totalCountOfSavedDTC; i++)
        {
            if (DTC[i].FunctionalGroupIdentifier == group)
            {
                // Is found DTC valid?
                LOOKUP_CALC(i, index, number);
                bitfield = (uint8_t)DTC_header->nvmDTCLookupTable[index];
                mask = 0x01 << number;

                if ((bitfield & mask) == mask)
                {
                    charon_deleteDTC_inNvm (i, (uint32_t)DTC, (uint32_t)DTC_header, (uint32_t)snapshot, (uint32_t)storedData, (uint32_t)extData);
                } 
            }
        }
        charon_StoredDataTransmissionFunctionalUnit_CRC16_update();
    }


    if (delAll)
    {
        memset(DTC_header,0x00,sizeof(DTC_header_t));
        DTC_header->iniDone = 0xDEADBEEF;
        charon_StoredDataTransmissionFunctionalUnit_CRC16_update();
    }
}


void charon_DTC_LookupTable_header_SET (void)
{
    // Re/Setting the DTC header.
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0, true);

    for (uint32_t i = 0; i < (uint16_t)DTC_header->sizeOfDTCLookupTable; i++)
    {
        DTC_header->nvmDTCLookupTable[i] = 0x00;
    }
    for (uint32_t i = 0; i < (uint16_t)DTC_header->sizeOfSnapshotLookupTable; i++)
    {
        DTC_header->nvmSnapshotLookupTable[i] = 0x00;
    }
    for (uint32_t i = 0; i < (uint16_t)DTC_header->sizeOfStoredDataLookupTable; i++)
    {
        DTC_header->nvmStoredDataLookupTable[i] = 0x00;
    }
    for (uint32_t i = 0; i < (uint16_t)DTC_header->sizeOfExtDataLookupTable; i++)
    {
        DTC_header->nvmExtDataLookupTable[i] = 0x00;
    }

    DTC_header->deletedDTCCounter = 0x00;
    DTC_header->deletedSnapshotRecordCounter = 0x00;
    DTC_header->deletedDataRecordCounter = 0x00;
    DTC_header->deletedExtDataRecordCounter = 0x00;
    DTC_header->FirstConfirmedDTC = 0x00;
    DTC_header->FirstFailedDTC = 0x00;
    DTC_header->MostRecentConfirmedDTC = 0x00;
    DTC_header->MostRecentTestFailed = 0x00;

    DTC_header->sizeOfDTCLookupTable = sizeof(DTC_header->nvmDTCLookupTable);
    DTC_header->sizeOfSnapshotLookupTable = sizeof(DTC_header->nvmSnapshotLookupTable);
    DTC_header->sizeOfStoredDataLookupTable = sizeof(DTC_header->nvmStoredDataLookupTable);
    DTC_header->sizeOfExtDataLookupTable = sizeof(DTC_header->nvmExtDataLookupTable);

    DTC_header->iniDone = 0xDEADBEEF;
    charon_StoredDataTransmissionFunctionalUnit_CRC16_update();
    CHARON_INFO("DTC header is now set.");
}


/* Private Function **********************************************************/

static void charon_deleteDTC_inNvm (uint16_t found, uint32_t aDTC, uint32_t aDTC_header, uint32_t aSnap_t, uint32_t aStored_t, uint32_t aExt_t)
{
    uint32_t number = 0u;
    uint32_t index = 0u;

    DTC_t *DTC = (DTC_t*)aDTC;
    DTC_header_t *DTC_header = (DTC_header_t*)aDTC_header;

    DTC_SnapshotData_t *snapshot = (DTC_SnapshotData_t*)aSnap_t;
    DTC_StoredData_t *storedData = (DTC_StoredData_t*)aStored_t;
    DTC_ExtendedData_t *extData = (DTC_ExtendedData_t*)aExt_t;


    // Handling lower storage. looks for saved address in DTC and in the NVM for the same address, index gets split and so the lookupTable gets updated to invalidate the address.
    for (uint32_t o = 0; o < DTC[found].NumberOfSavedSnapshots; o++)
    {
        for (uint32_t j = 0; j < AMOUNT_OF_SNAPSHOT; j++)
        {
            if ((uint32_t)DTC[found].DTCSnapshotAddress[o] == (uint32_t)&snapshot[j])
            {
                LOOKUP_CALC(j, index, number);
                DTC_header->nvmSnapshotLookupTable[index] &= ~(0x01 << number);

                DTC_header->currentSnapshotRecordCounter--;
                DTC_header->deletedSnapshotRecordCounter++;
                break;
            }
        }
    }
    // Handling lower storage. looks for saved address in DTC and in the NVM for the same address, index gets split and so the lookupTable gets updated to invalidate the address.
    for (uint32_t o = 0; o < DTC[found].NumberOfSavedDataRecords; o++)
    {
        for (uint32_t j = 0; j < AMOUNT_OF_STOREDDATA; j++)
        {
            if ((uint32_t)DTC[found].DTCStoredDataAddress[o] == (uint32_t)&storedData[j])
            {
                LOOKUP_CALC(j, index, number);
                DTC_header->nvmStoredDataLookupTable[index] &= ~(0x01 << number);

                DTC_header->currentDataRecordCounter--;
                DTC_header->deletedDataRecordCounter++;
                break;
            }
        }
    }
    // Handling lower storage. looks for saved address in DTC and in the NVM for the same address, index gets split and so the lookupTable gets updated to invalidate the address.
    for (uint32_t o = 0; o < DTC[found].NumberOfSavedExtendedData; o++)
    {
        for (uint32_t j = 0; j < AMOUNT_OF_EXTENDEDDATA; j++)
        {
            if ((uint32_t)DTC[found].DTCExtendedDataAddress[o] == (uint32_t)&extData[j])
            {
                LOOKUP_CALC(j, index, number);
                DTC_header->nvmExtDataLookupTable[index] &= ~(0x01 << number);

                DTC_header->currentExtDataRecordCounter--;
                DTC_header->deletedExtDataRecordCounter++;
                break;
            }
        }
    }

    // Resetting most recent and First 
    if ((uint32_t)DTC_header->MostRecentConfirmedDTC == (uint32_t)&DTC[found])
    {
        DTC_header->MostRecentConfirmedDTC = 0u;
    }
    if ((uint32_t)DTC_header->MostRecentTestFailed == (uint32_t)&DTC[found])
    {
        DTC_header->MostRecentTestFailed = 0u;
    }

    if ((uint32_t)DTC_header->FirstConfirmedDTC == (uint32_t)&DTC[found])
    {
        DTC_header->FirstConfirmedDTC = 0x01;
    }
    if ((uint32_t)DTC_header->FirstFailedDTC == (uint32_t)&DTC[found])
    {
        DTC_header->FirstFailedDTC = 0x01;
    }


    // Handling DTC itself.
    LOOKUP_CALC(found, index, number);
    DTC_header->nvmDTCLookupTable[index] &= ~(0x01 << number);

    DTC_header->currentDTCCounter--;
    DTC_header->deletedDTCCounter++;
}


static bool charon_DTC_LookupTable_DTClookupTableValidCheck(uint32_t input, uint32_t address)
{
    /** @todo Maybe upgrade to inline function / macro in future. */
    uint8_t index = 0u;
    uint8_t number = 0u;
    bool itsFine = false;
    DTC_header_t *DTC_header = (DTC_header_t*)address;

    LOOKUP_CALC(input, index, number);
    index = (uint8_t)DTC_header->nvmDTCLookupTable[index];
    number = (0x01 << number);

    if ((index & number) > 0x00)
    {
        itsFine = true;
    }
    return itsFine;
}


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
