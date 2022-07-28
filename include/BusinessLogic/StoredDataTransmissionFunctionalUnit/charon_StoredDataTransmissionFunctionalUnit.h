/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Kaup
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
 * @file charon_StoredDataTransmissionFunctionalUnit.h
 * Module handles Service Group the Stored Data Transmission Unit
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_
#define CHARON_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_

/* Includes ******************************************************************/
#include <stdint.h>
#include "charon_types.h"

/* Imports *******************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

typedef enum DTCSubfunction_t_public
{
    ISOSAEReserved                                      = 0x00,
    reportNumberOfDTCByStatusMask                       = 0x01,
    reportDTCByStatusMask                               = 0x02,
    reportDTCSnapshotIdentification                     = 0x03,
    reportDTCSnapshotRecordByDTCNumber                  = 0x04,
    reportDTCStoredDataByRecordNumber                   = 0x05,
    reportDTCExtDataRecordByDTCNumber                   = 0x06,
    reportNumberOfDTCBySeverityMaskRecord               = 0x07,
    reportDTCBySeverityMaskRecord                       = 0x08,
    reportSeverityInformationOfDTC                      = 0x09,
    reportSupportedDTC                                  = 0x0A,
    reportFirstTestFailedDTC                            = 0x0B,
    reportFirstConfirmedDTC                             = 0x0C,
    reportMostRecentTestFailedDTC                       = 0x0D,
    reportMostRecentConfirmedDTC                        = 0x0E,
    reportMirrorMemoryDTCByStatusMask                   = 0x0F,
    reportMirrorMemoryDTCExtDataRecordByDTCNumber       = 0x10,
    reportNumberOfMirrorMemoryDTCByStatusMask           = 0x11,
    reportNumberOfEmissionsOBDDTCByStatusMask           = 0x12,
    reportEmissionsOBDDTCByStatusMask                   = 0x13,
    reportDTCFaultDetectionCounter                      = 0x14,
    reportDTCWithPermanentStatus                        = 0x15,
    reportDTCExtDataRecordByRecordNumber                = 0x16,
    reportUserDefMemoryDTCByStatusMask                  = 0x17,
    reportUserDefMemoryDTCSnapshotRecordByDTCNumber     = 0x18,
    reportUserDefMemoryDTCExtDataRecordByDTCNumber      = 0x19,

    amountOFSubfunctions
}DTCSubfunction_t;


typedef enum DTCtypeidentifier_t_private
{
    DTCStatusMask                                       = 1u,
    DTCMaskRecord                                       = 2u,
    DTCSnapshotRecorNumber                              = 3u,
    DTCStoredDataRecordNumber                           = 4u,
    DTCExtDataRecordNumber                              = 5u,
    DTCSeverityMaskRecord                               = 6u,
    DTCSeverityMask                                     = 7u,
    FunctionalGroupidentifier                           = 8u,
    MemorySelection                                     = 9u,
}DTCtypeidentifier_t;


typedef PACKED_STRUCT(DTC_t_public)            //                                                                                                      source:
{
    uint8_t DTCStatusMask;                      // Bit 0-8 def:    0 =testfailed, 1 =testFailedThisOperationCycle, 2 =pendingDTC, 3=confirmedDTC,       [ISO D.2.3 S=356]
                                                // 4 =testNotCompletedSinceLastClear, 5 =TestFailedSinceLastClear, 6 =testNotCompletedThisOperationCycle
                                                // 7 =warningIndicatorRequested
    uint8_t DTCHighByte;                        //    H + M + L = DTCMaskRecord                                                                                                  [ISO 11.3.2.3 S=198]
    uint8_t DTCMiddleByte;                      // 3-byte value, together represent a unique identification number for a specific diagnostic trouble code
    uint8_t DTCLowByte;                         // 
    uint8_t DTCSnapshotRecorNumber;             // 0x01-0xFE manufacturer specific usage, 0xFF = report all stored records at once.                     [ISO 11.3.2.3 S=198]
    uint8_t DTCStoredDataRecordNumber;          // 0x01-0xFE manufacturer specific usage, 0xFF = report all stored records at once.                     [ISO 11.3.2.3 S=198]
    uint8_t DTCExtDataRecordNumber;             // 0x01-0x8F manufacturer specific records, 0x90-0xEF report legislated OBD records,                    [ISO 11.3.2.3 S=199]
                                                // 0xFE report all legislated OBD in a single message, 0xFF report all records.
    uint8_t DTCSeverityMaskRecordHigh;          // = DTCSeverityMask
    uint8_t DTCSeverityMaskRecordLow;           // = DTCStatusMask 
    uint8_t DTCSeverityMask;                    // Bit 0-4 def: DTC class information, Bit 5-7 def: DTC severity information                            [ISO D.3 S=366]
    uint8_t FunctionalGroupidentifier;          // 0x33 = Emission, 0xD0 = Safety, 0xFE = VOBD, 0xFF = ALL                                              [ISO D.5 S=369]
    uint32_t MemorySelection;                   // 
}DTC_t;


/* Interfaces  ***************************************************************/

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x14
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @param transmitBuffer Payload
 * @param transmitBufferSize Payload Size
 * @return uds_responseCode_t
 */
uds_responseCode_t charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation (const uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

/**
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x19
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return uds_responseCode_t
 */
uds_responseCode_t charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * used to input or change a DTC in the Nvm 
 * default for DTCnumber is 0 and will add a new DTC, if you want to change a specific DTC you will need to input the DTCnumber 
 * DTCnumber: 1 = the first saved DTC, 2 = the second saved DTC, etc...
 * 
 * DTCnumber CAN'T be used to generate new DTC!
 * 
 * @param DTCnumber                     0 for defult, Number to edit DTC
 * @param StatusMask                    Bitstatus mask 
 * @param HighByte                      Part of the 24 bit DTC identification number
 * @param MiddleByte                    Part of ...
 * @param LowByte                       Part of ...
 * @param SnapshotRecorNumber           0x01 - 0xFE to store the DTC snapshot record number
 * @param StoredDataRecordNumber        0x01 - 0xFE to store the DTC data record number
 * @param ExtDataRecordNumber           Storage for extern data
 * @param SeverityMask                  Bit 7-5 for severity, Bit 4-0 for DTC class information
 * @param FunctionalGroupidentifier     ISO 14229-1 S369  (or D.5)
 * @return uds_responseCode_t               
 */
uds_responseCode_t charon_StoredDataTransmissionFunctionalUnit_writeDTCto_Nvm (uint32_t DTCnumber, uint8_t StatusMask, uint8_t HighByte, uint8_t MiddleByte, uint8_t LowByte,
    uint8_t SnapshotRecorNumber, uint8_t StoredDataRecordNumber, uint8_t ExtDataRecordNumber, uint8_t SeverityMask, uint8_t FunctionalGroupidentifier);

/**
 * Gets the amount of stored DTC.
 * 
 * @return uint32_t     The amount of stored DTC
 */
uint32_t charon_StoredDataTransmissionFunctionalUnit_getDTCcount (void);

#endif /* CHARON_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/