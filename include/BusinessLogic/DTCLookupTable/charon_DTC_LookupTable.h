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
 * @defgroup BusinessLogic Business Logic Modules
 * @{
 * @file charon_DTC_LookupTable.h
 * This Module handles the DTC masks and provides the DTC Funktions
 * 
 * 
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_DTC_LookupTable_H_
#define CHARON_DTC_LookupTable_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "charon_StoredDataTransmissionFunctionalUnit.h"


/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** @brief This Bit defines if the "Test failed" Bit of the DTC status Mask is available for requests. */
#define DTC_Enable_testFailed_Status_Bit                            ((uint8_t)0x01) 
/** @brief This Bit defines if the "Test failed this operation Cycle" Bit of the DTC status Mask is available for requests. */
#define DTC_Enable_testFailedThisOperationCycle_Status_Bit          ((uint8_t)0x01) 
/** @brief This Bit defines if the "Pending DTC" Bit of the DTC status Mask is available for requests. */
#define DTC_Enable_pendingDTC_Status_Bit                            ((uint8_t)0x01) 
/** @brief This Bit defines if the "confirmed DTC" Bit of the DTC status Mask is available for requests. */
#define DTC_Enable_confirmedDTC_Status_Bit                          ((uint8_t)0x01) 
/** @brief This Bit defines if the "test Not Completed Since Last Clear" Bit of the DTC status Mask is available for requests. */
#define DTC_Enable_testNotCompletedSinceLastClear_Status_Bit        ((uint8_t)0x01) 
/** @brief This Bit defines if the "Test Failed Since Last Clear" Bit of the DTC status Mask is available for requests. */
#define DTC_Enable_testFailedSinceLastClear_Status_Bit              ((uint8_t)0x01) 
/** @brief This Bit defines if the "test Not Completed This Operation Cycle" Bit of the DTC status Mask is available for requests. */
#define DTC_Enable_testNotCompletedThisOperationCycle_Status_Bit    ((uint8_t)0x01) 
/** @brief This Bit defines if the "Warning Indicator Requested" Bit of the DTC status Mask is available for requests. */
#define DTC_Enable_warningIndicatorRequested_Status_Bit             ((uint8_t)0x01) 

/** @brief Defines the Format Identifier for the ISO 14229-1 DTC_Format */
#define DTC_Format_Identifier                                       ((uint8_t)0x01)


/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/**
 * @brief Returns DTC status availability mask.
 * 
 * @return uint8_t          Contains the DTC status availability Mask.
 */
uint8_t charon_getDTCStatusAvailabilityMask(void);

/**
 * @brief Searches the NVM for a matching DTCnumber und will return this DTC address.
 * 
 * @param DTCHighByte       Part of the 24 bit DTCnumber.
 * @param DTCMiddleByte     Part of the 24 bit DTCnumber.
 * @param DTCLowByte        Part of the 24 bit DTCnumber.
 * @param mirror            Only used if a mirror subfunction was requested by the user.
 * @param userDefMemory     Only used if a userDefMemory subfunction was requested by the user.
 * @param MemorySelection   Only needed by userDefMemory.
 * @return DTC_t*           Pointer to the DTC requested address.
 */
DTC_t* charon_getDTCLookupByDTCNumber (uint8_t DTCHighByte, uint8_t DTCMiddleByte, uint8_t DTCLowByte, bool mirror, bool userDefMemory, uint8_t MemorySelection);

/**
 * @brief Searches the NVM for a matching StatusMask and will return this DTC address.
 * 
 * @param StatusMask        The statusmask put in by the user.
 * @param offset            When the first DTC is given back the next is picked using this value.
 * @param mirror            Only used if a mirror subfunction was requested by the user.
 * @return DTC_t*           Pointer to the DTC requested address.
 */
DTC_t* charon_getDTCLookupByStatusMask (uint8_t StatusMask, uint16_t offset, bool mirror);

/**
 * @brief Searches the NVM for all matching DTC and will return the amount.
 * 
 * @param StatusMask        The statusmask put in by the user.
 * @param mirror            Only used if a mirror subfunction was requested by the user.
 * @param userDefMemory     Only used if a userDefMemory subfunction was requested by the user.
 * @param MemorySelection   Only needed by userDefMemory
 * @return uint32_t         Amount of DTC.
 */
uint32_t charon_getDTCCountByStatusMask (uint8_t StatusMask, bool mirror, bool userDefMemory, uint8_t MemorySelection);

/**
 * @brief Searches the NVM for saved Snapshot and will return found DTC address.
 * 
 * @param offset            When the first DTC is given back the next is picked using this value.
 * @return DTC_t*           Pointer to the DTC requested address.
 */
DTC_t* charon_getDTCSnapshotAddress (uint16_t offset);

/**
 * @brief Searches the NVM for DTCStoredDataRecordNumber and will return found DTC address.
 * 
 * @param RecordNumber      The User inputted requested number.
 * @return DTC_t*           Pointer to the DTC requested address.
 */
DTC_t* charon_getDTCLookupByRecordNumber (uint8_t RecordNumber);

/**
 * @brief Prints all DataRecords, only used when 0xFF was input by user.
 * 
 * @param receiveBuffer     Payload.
 * @param receiveBufferSize Payload size.
 */
uds_responseCode_t charon_getDTCLookup_reportAllDataRecords (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Searches the NVM for all matching DTC and will return the amount.
 * 
 * @param DTCSeverityMask   The severity mask put in by the user.
 * @param DTCStatusMask     The status mask put in by the user.
 * @return uint32_t         Amount of DTCAmount of DTC.
 */
uint32_t charon_getDTCCountBySeverityMask (uint8_t DTCSeverityMask, uint8_t DTCStatusMask);

/**
 * @brief Searches the NVM for a matching StatusMask AND SeverityMask, will return this DTC address.
 * 
 * @param DTCSeverityMask   The severity mask put in by the user.
 * @param DTCStatusMask     The status mask put in by the user.
 * @param offset            When the first DTC is given back the next is picked using this value.
 * @return DTC_t*           Pointer to the DTC requested address.
 */
DTC_t* charon_getDTCSeverityMaskAddress (uint8_t DTCSeverityMask, uint8_t DTCStatusMask, uint16_t offset);

/**
 * @brief Searches the NVM for ExtData with a matching RecordNumber and returns found DTC address.
 * 
 * @param RecordNumber      The User inputted requested number.
 * @param offset            When the first DTC is given back the next is picked using this value.
 * @param p_index_found_at  Index is needed for response building.
 * @return DTC_t*           Pointer to the DTC requested address.
 */
DTC_t* charon_getDTCExtDataAddressByRecordNumber (uint8_t RecordNumber, uint16_t offset, uint32_t * p_index_found_at);

/**
 * @brief Searches the NVM for all ExtData with a matching RecordNumber and returns the amount.
 * 
 * @param RecordNumber      The User inputted requested number.
 * @return uint8_t          Amount of found DTC.
 */
uint8_t charon_getDTCExtDataCountByRecordNumber (uint8_t RecordNumber);

/**
 * @brief Use by subfunction 0x14, will delete DTC based on input and save the changes.
 * 
 * @param DTCHighByte       Part of the 24 bit DTCnumber.
 * @param DTCMiddleByte     Part of the 24 bit DTCnumber.
 * @param DTCLowByte        Part of the 24 bit DTCnumber.
 * @param start             of defined range.
 * @param end               of defined range.
 * @param vobd              Flag for processing.
 * @param safety            Flag for processing.
 * @param emission          Flag for processing.
 * @param delAll           Flag for processing.
 */
void charon_deleteDTC (uint8_t DTCHighByte, uint8_t DTCMiddleByte, uint8_t DTCLowByte, uint32_t start, uint32_t end, bool vobd, bool safety, bool emission, bool delAll);

/** @brief Safety function for Nvm to not read non valid data. USER: NEEDS TO BE CALLED ONCE WHEN EVER THE NVM IS RESET! */
void charon_DTC_LookupTable_header_SET (void);


#endif /* CHARON_DTC_LookupTable_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
