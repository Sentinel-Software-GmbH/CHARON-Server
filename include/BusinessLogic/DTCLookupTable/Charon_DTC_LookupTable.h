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
 * @defgroup BusinessLogic Business Logic
 * @{
 * @file charon_SessionAndServiceControl.h
 * This Module handles the Receive and Transfer of the charon uds stack data.
 * It Controls encryption and checks for all demanded Timeouts.
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
#include "charon_StoredDataTransmissionFunctionalUnit.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** This Bit defines if the "Test failed" Bit of the DTC status Mask is available for requests */
#define DTC_Enable_testFailed_Status_Bit                            ((uint8_t) 1) 
/** This Bit defines if the "Test failed this operation Cycle" Bit of the DTC status Mask is available for requests */
#define DTC_Enable_testFailedThisOperationCycle_Status_Bit          ((uint8_t) 1) 
/** This Bit defines if the "Pending DTC" Bit of the DTC status Mask is available for requests */
#define DTC_Enable_pendingDTC_Status_Bit                            ((uint8_t) 1) 
/** This Bit defines if the "confirmed DTC" Bit of the DTC status Mask is available for requests */
#define DTC_Enable_confirmedDTC_Status_Bit                          ((uint8_t) 1) 
/** This Bit defines if the "test Not Completed Since Last Clear" Bit of the DTC status Mask is available for requests */
#define DTC_Enable_testNotCompletedSinceLastClear_Status_Bit        ((uint8_t) 1) 
/** This Bit defines if the "Test Failed Since Last Clear" Bit of the DTC status Mask is available for requests */
#define DTC_Enable_testFailedSinceLastClear_Status_Bit              ((uint8_t) 1) 
/** This Bit defines if the "test Not Completed This Operation Cycle" Bit of the DTC status Mask is available for requests */
#define DTC_Enable_testNotCompletedThisOperationCycle_Status_Bit    ((uint8_t) 1) 
/** This Bit defines if the "Warning Indicator Requested" Bit of the DTC status Mask is available for requests */
#define DTC_Enable_warningIndicatorRequested_Status_Bit             ((uint8_t) 1) 

/** Defines the Format Identifier for the ISO 14229-1 DTC_Format */
#define DTC_Format_Identifier ((uint8_t)0x01)

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/**
 * Returns DTC status availability mask.
 * 
 * @return uint8_t 
 */
uint8_t charon_getDTCStatusAvailabilityMask(void);

/**
 * Searches the NVM for a matching DTCnumber und will return this DTC address.
 * 
 * @param DTCHighByte       Part of the 24 bit DTCnumber
 * @param DTCMiddleByte     Part of the 24 bit DTCnumber
 * @param DTCLowByte        Part of the 24 bit DTCnumber
 * @return DTC_t*           Pointer to the DTC address
 */
DTC_t* charon_getDTCLookupByDTCNumber (uint8_t DTCHighByte, uint8_t DTCMiddleByte, uint8_t DTCLowByte);

/**
 *  Searches the NVM for a matching StatusMask und will return this DTC address.
 * 
 * @param StatusMask        The Status mask put in by the user
 * @param offset            When the first DTC is given back the second is picked using this value
 * @return DTC_t*           Pointer to the DTC address
 */
DTC_t* charon_getDTCLookupByStatusMask (uint8_t StatusMask, uint16_t offset);

/**
 *  Searches the NVM for all matching DTC and will return the amount.
 * 
 * @param StatusMask        The Status mask put in by the user
 * @return uint32_t         Amount of DTC
 */
uint32_t charon_getDTCCountByStatusMask (uint8_t StatusMask);

#endif /* CHARON_DTC_LookupTable_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
