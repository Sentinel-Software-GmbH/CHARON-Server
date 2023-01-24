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
 * @file charon_StoredDataTransmissionFunctionalUnit.c
 * Implementation of the Stored Data Transmission Unit Module
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
#include "charon_crc16.h"
#include "charon_types.h"
#include "charon_DTC_LookupTable.h"
#include "charon_interface_debug.h"
#include "charon_negativeResponse.h"
#include "charon_interface_NvmDriver.h"
#include "charon_SessionAndServiceControl.h"
#include "charon_StoredDataTransmissionFunctionalUnit.h"


/* Imports *******************************************************************/

/* Macros ********************************************************************/

/** @brief Max Transmission Massage buffer size. */
#define MAX_TX_BUFFER_SIZE          ((uint32_t) 1024u)

/** @brief Safe area in Nvm. */
#define STORAGE_HEADER              ((uint8_t) sizeof(DTC_header_t))

/** @brief Special user input case. */
#define PRINT_ALL                   ((uint8_t) 0xFF)

/** @brief Special user input case. */
#define PRINT_ALL_OBD               ((uint8_t) 0xFE)

/** @brief For easy StatusMask check. */
#define TEST_FAILED_CHECK           ((uint8_t) 0x01 << 0)
/** @brief For easy StatusMask check. */
#define CONFIRMED_DTC_CHECK         ((uint8_t) 0x01 << 3)

/** @brief Clear DTC, FunctionalGroupIdentifiers */
#define EMISSION_SYSTEM_GROUP       ((uint32_t) 0xFFFF33)
/** @brief Clear DTC, FunctionalGroupIdentifiers */
#define SAFETY_SYSTEM_GROUP         ((uint32_t) 0xFFFFD0)
/** @brief Clear DTC, FunctionalGroupIdentifiers */
#define VOBD_SYSTEM_GROUP           ((uint32_t) 0xFFFFFE)
/** @brief Clear DTC, FunctionalGroupIdentifiers */
#define DEL_ALL_DTC                 ((uint32_t) 0xFFFFFF)

/** @brief No magic values */
#define BYTE_SIZE                   ((uint8_t) 8u)


/** @brief Macro: split input in 8bit index and number */
#define LOOKUP_CALC(i, index, number) \
index = i / 8u; \
number = i % 8u


/* Types *********************************************************************/

/* Variables *****************************************************************/

// reserved DTC like 0xFFFFFE are already filtered
/** @brief Array to filter by USER: blocked DTC. */
static const uint32_t NOT_SUPPORTED_DTC[] = {0x040506}; /** @todo USER: put your unwanted codes in here. Just a dummy atm. */

/** @brief Array to filter by USER: blocked snapshots. */
static const uint32_t NOT_SUPPORTED_SNAPSHOT[] = {0x10, 0x20}; /** @todo USER: put your unwanted codes in here. Just a dummy atm. */

/** @brief Array to filter by USER: blocked dataRecords. */
static const uint8_t NOT_SUPPORTED_DATARECORD[] = {0xFA, 0xFB, 0xFC, 0xFD, 0xFE};    /** @todo USER: put your unwanted codes in here. Just a dummy atm. */

/** @brief Array to filter by USER: blocked extData. */
static const uint8_t NOT_SUPPORTED_EXTDATA[] = {0x10, 0x11, 0x12, 0x13, 0x14};    /** @todo USER: put your un wanted codes in here. Just a dummy atm. */


/* Private Function Definitions **********************************************/

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x01 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @param mirror                    Only used if a mirror subfunction was requested by the user.
 * @param userDefMemory             Only used if a userDefMemory subfunction was requested by the user.
 * 
 * @return uds_responseCode_t 
 */
static uds_responseCode_t NumberOfDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize, bool mirror, bool userDefMemory);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x02 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @param mirror                    Only used if a mirror subfunction was requested by the user.
 * 
 * @return uds_responseCode_t       
 */
static uds_responseCode_t DTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize, bool mirror);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x03 from ISO: 14229-1.
 *
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t DTCSnapshotIdentification (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x04 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @param userDefMemory             Only used if a userDefMemory subfunction was requested by the user.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t DTCSnapshotRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize, bool userDefMemory);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x05 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t DTCStoredDataByRecordNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x06 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @param mirror                    Only used if a mirror subfunction was requested by the user.
 * @param userDefMemory             Only used if a userDefMemory subfunction was requested by the user.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t DTCExtDataRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize, bool mirror, bool userDefMemory);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x07 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t NumberOfDTCBySeverityMaskRecord (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x08 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t DTCBySeverityMaskRecord (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x08 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t SeverityInformationOfDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x0A from ISO: 14229-1.
 * 
 * @return uds_responseCode_t 
 */
static uds_responseCode_t SupportedDTC (void);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x0B from ISO: 14229-1.
 * 
 * @return uds_responseCode_t 
 */
static uds_responseCode_t FirstTestFailedDTC (void);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x0C from ISO: 14229-1.
 * 
 * @return uds_responseCode_t 
 */
static uds_responseCode_t FirstConfirmedDTC (void);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x0D from ISO: 14229-1.
 * 
 * @return uds_responseCode_t 
 */
static uds_responseCode_t MostRecentTestFailedDTC (void);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x0E from ISO: 14229-1.
 * 
 * @return uds_responseCode_t 
 */
static uds_responseCode_t MostRecentConfirmedDTC (void);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x12 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t NumberOfEmissionsOBDDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x13 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t EmissionsOBDDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x14 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t DTCFaultDetectionCounter (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x15 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t DTCWithPermanentStatus (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Read DTC Information (SID 0x019) Subfunction 0x16 from ISO: 14229-1.
 * 
 * @param receiveBuffer             Payload.
 * @param receiveBufferSize         Payload size.
 * @return uds_responseCode_t 
 */
static uds_responseCode_t DTCExtDataRecordByRecordNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Searches for the first empty Bit in found Byte cell. Splits the found Byte in low and half parts until found.
 * 
 * @param input                     In table Found Byte.
 * @return uint8_t                  First Empty Bit.
 */
static uint8_t charon_DTC_LookupTable_getBitNumber (uint8_t input);

/**
 * @brief Saves new DTC into NVM.
 * 
 * @param DTCup                      New DTC data to be saved.
 */
static DTC_t* charon_StoredDataTransmissionFunctionalUnit_writeNewDTCToNvm (DTC_t DTCup);

/**
 * @brief Saves updates to status bits.
 * 
 * @param DTCup                     New DTC data to be saved.
 * @param address                   Already saved DTC.
 */
static void charon_StoredDataTransmissionFunctionalUnit_writeUpdateDTCToNvm (DTC_t DTCup, DTC_t* address);

/**
 * @brief Saves snapshot data into the NVM and searches for an empty spot.
 * 
 * @param payload                   New snapshot data to be saved.
 * @param DTCup                     New DTC data to be saved.
 * @param address                   Already saved DTC.
 */
static void charon_StoredDataTransmissionFunctionalUnit_writeSnapshotToNvm (DTC_SnapshotData_t payload, DTC_t DTCup, DTC_t* address);

/**
 * @brief Saves storedData data into the NVM and searches for an empty spot.
 * 
 * @param payload                   New storedData data to be saved.
 * @param DTCup                     New DTC data to be saved.
 * @param address                   Already saved DTC.
 */
static void charon_StoredDataTransmissionFunctionalUnit_writeStoredDataToNvm (DTC_StoredData_t payload, DTC_t DTCup, DTC_t* address);

/**
 * @brief Saves extendedData data into the NVM and searches for an empty spot.
 * 
 * @param payload                   New extendedData data to be saved.
 * @param DTCup                     New DTC data to be saved.
 * @param address                   Already saved DTC.
 */
static void charon_StoredDataTransmissionFunctionalUnit_writeExtendedDataToNvm (DTC_ExtendedData_t payload, DTC_t DTCup, DTC_t* address);


/* Interfaces  ***************************************************************/

void charon_StoredDataTransmissionFunctionalUnit_DTCSettingTypeUpdater (DTC_t DTCInput)
{
    DTC_t *DTC = NULL;
    DTC_t DTCupdate = DTCInput;

    // Depending on input will search for wanted DTC, when found and valid will update SettingType to new value.
    DTC = charon_getDTCLookupByDTCNumber((uint8_t)DTCupdate.DTCHighByte, (uint8_t)DTCupdate.DTCMiddleByte, (uint8_t)DTCupdate.DTCLowByte, false, false, 0x00);
    if (DTC != NULL)
    {
        if (DTCupdate.DTCSettingType != 0x00)
        {
            DTC->DTCSettingType = DTCupdate.DTCSettingType;
        }
    }
}


uds_responseCode_t charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint32_t input_nr = ((receiveBuffer[1] << 16u) | (receiveBuffer[2] << 8u) | (receiveBuffer[3] << 0u));
    uint8_t DTCHighByte = receiveBuffer[1];
    uint8_t DTCMiddleByte = receiveBuffer[2];
    uint8_t DTCLowByte = receiveBuffer[3];
    uint32_t start = 0u;
    uint32_t end = 0u;
    bool vobd = false;
    bool safety = false;
    bool emission = false;
    bool del_all = false;

    CHARON_INFO("Clear Diagnostic Information Service SID:0x14 Triggered");

    // Check for correct message length.
    if (receiveBufferSize != 4u)
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_ClearDiagnosticInformation);
        CHARON_ERROR("Unexpected message length.");
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    
    // DTC must be in the valid range, some are reserved by ISO 14229-1.
    if ((input_nr <= 0x0000FF) || (input_nr >= 0xFFFF00 && input_nr <= 0xFFFF32) || (input_nr >= 0xFFFF34 && input_nr <= 0xFFFFCF) || (input_nr >= 0xFFFFD1 && input_nr <= 0xFFFFFD))
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ClearDiagnosticInformation);
        CHARON_ERROR("Request not in valid range, reserved by ISO 14229-1.");
        return uds_responseCode_RequestOutOfRange;
    }
    // DTC may not be supported by user.
    for (uint32_t i = 0; i < (sizeof(NOT_SUPPORTED_DTC) / sizeof(NOT_SUPPORTED_DTC[0])); i++)
    {
        if (NOT_SUPPORTED_DTC[i] == input_nr)
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ClearDiagnosticInformation);
            CHARON_ERROR("Request not valid, DTC not supported.");
            return uds_responseCode_RequestOutOfRange;
        }
    }
    
    
    /** @todo USER: Add your self determined case here or input will always end in delete single DTC. */
    switch (input_nr)
    {
    case VOBD_SYSTEM_GROUP:
        //######################################
        vobd = true;
        CHARON_INFO("Start deleting VOBD group.");
        charon_deleteDTC (DTCHighByte,DTCMiddleByte,DTCLowByte,start,end,vobd,safety,emission,del_all);
        //######################################
        break;

    case SAFETY_SYSTEM_GROUP:
        //######################################
        safety = true;
        CHARON_INFO("Start deleting SAFETY_SYSTEM group.");
        charon_deleteDTC (DTCHighByte,DTCMiddleByte,DTCLowByte,start,end,vobd,safety,emission,del_all);
        //######################################
        break;

    case EMISSION_SYSTEM_GROUP:
        //######################################
        emission = true;
        CHARON_INFO("Start deleting EMISSION_SYSTEM group.");
        charon_deleteDTC (DTCHighByte,DTCMiddleByte,DTCLowByte,start,end,vobd,safety,emission,del_all);
        //######################################
        break;

    case DEL_ALL_DTC:
        //######################################
        del_all = true;
        CHARON_INFO("Start deleting ALL DTC.");
        charon_deleteDTC (DTCHighByte,DTCMiddleByte,DTCLowByte,start,end,vobd,safety,emission,del_all);
        //######################################
        break;


    case powertrainGroup:
        //######################################
        start = powertrainGroupSTART;
        end = powertrainGroupEND;
        CHARON_INFO("Start deleting POWERTRAIN group.");
        charon_deleteDTC (DTCHighByte,DTCMiddleByte,DTCLowByte,start,end,vobd,safety,emission,del_all);
        //######################################
        break;

    case chassisGroup:
        //######################################
        start = chassisGroupSTART;
        end = chassisGroupEND;
        CHARON_INFO("Start deleting CHASSIS group.");
        charon_deleteDTC (DTCHighByte,DTCMiddleByte,DTCLowByte,start,end,vobd,safety,emission,del_all);
        //######################################
        break;

    case bodyGroup:
        //######################################
        start = bodyGroupSTART;
        end = bodyGroupEND;
        CHARON_INFO("Start deleting BODY group.");
        charon_deleteDTC (DTCHighByte,DTCMiddleByte,DTCLowByte,start,end,vobd,safety,emission,del_all);
        //######################################
        break;

    case networkCommunicationGroup:
        //######################################
        start = networkCommunicationGroupSTART;
        end = networkCommunicationGroupEND;
        CHARON_INFO("Start deleting NETWORK_COMMUNICATION group.");
        charon_deleteDTC (DTCHighByte,DTCMiddleByte,DTCLowByte,start,end,vobd,safety,emission,del_all);
        //######################################
        break;


    default:
        CHARON_INFO("Start deleting SINGLE DTC.");
        charon_deleteDTC (DTCHighByte,DTCMiddleByte,DTCLowByte,start,end,vobd,safety,emission,del_all);
        break;
    }

    static uint8_t s_buffer[1];
    s_buffer[0] = (uds_sid_ClearDiagnosticInformation | (uint8_t)uds_sid_PositiveResponseMask);
    uint32_t length = 1u;
    charon_sscTxMessage(s_buffer,length);
    CHARON_INFO("DTC are now cleared.");
    return uds_responseCode_PositiveResponse;
}


uds_responseCode_t charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint16_t crc_check = 0x00;
    uint8_t data[STORAGE_HEADER - 2u];

    CHARON_INFO("Read DTC Information Service SID:0x19 Triggered");

    // NVM needs to be setted on installation or CRC will pick up random values and may fail without real reason.
    if (DTC_header->iniDone != 0xDEADBEEF)
    {
        charon_DTC_LookupTable_header_SET();
    }

    // CRC check before any search in Nvm.
    memcpy(data,DTC_header, (STORAGE_HEADER - 2u));

    crc_check = (uint16_t)charon_crc16_update(0x00,&data,(STORAGE_HEADER - 2u));
    crc_check = (uint16_t)charon_crc16_finalize(crc_check);
    if ((crc_check - (uint16_t)DTC_header->CRC16Checksum) != 0)
    {
        // CRC16 failed, something went wrong in the Nvm.
        charon_sendNegativeResponse(uds_responseCode_FailurePreventsExecutionOfRequestedAction, uds_sid_ReadDtcInformation);
        CHARON_ERROR("NVM CRC16 check failed! Something went wrong and data could be corrupt.");
        return uds_responseCode_FailurePreventsExecutionOfRequestedAction;
    }


    // Build Data Identifier
    DTCSubfunction_t subfunction = receiveBuffer [1];

    switch (subfunction)
    {
    case reportNumberOfDTCByStatusMask:                     //0x01
    {
        CHARON_INFO("Sub 0x01: reportNumberOfDTCByStatusMask start.");
        bool userDefMemory = false;
        bool mirror = false;
        return NumberOfDTCByStatusMask(receiveBuffer, receiveBufferSize, mirror, userDefMemory);
    }

    case reportDTCByStatusMask:                             //0x02  
    {
        CHARON_INFO("Sub 0x02: reportDTCByStatusMask start.");
        bool mirror = false;
        return DTCByStatusMask(receiveBuffer, receiveBufferSize, mirror);
    }

    case reportDTCSnapshotIdentification:                   //0x03   
    {
        CHARON_INFO("Sub 0x03: reportDTCSnapshotIdentification start.");
        return DTCSnapshotIdentification(receiveBuffer, receiveBufferSize);
    }

    case reportDTCSnapshotRecordByDTCNumber:                //0x04
    {
        CHARON_INFO("Sub 0x04: reportDTCSnapshotRecordByDTCNumber start.");
        bool userDefMemory = false;
        return DTCSnapshotRecordByDTCNumber(receiveBuffer, receiveBufferSize, userDefMemory);
    }

    case reportDTCStoredDataByRecordNumber:                 //0x05 
    {
        CHARON_INFO("Sub 0x05: reportDTCStoredDataByRecordNumber start.");
        return DTCStoredDataByRecordNumber(receiveBuffer, receiveBufferSize);
    }

    case reportDTCExtDataRecordByDTCNumber:                 //0x06
    {
        CHARON_INFO("Sub 0x06: reportDTCExtDataRecordByDTCNumber start.");
        bool mirror = false;
        bool userDefMemory = false;
        return DTCExtDataRecordByDTCNumber(receiveBuffer, receiveBufferSize, mirror, userDefMemory);
    }

    case reportNumberOfDTCBySeverityMaskRecord:             //0x07 
    {
        CHARON_INFO("Sub 0x07: reportNumberOfDTCBySeverityMaskRecord start.");
        return NumberOfDTCBySeverityMaskRecord(receiveBuffer, receiveBufferSize);
    }

    case reportDTCBySeverityMaskRecord:                     //0x08    
    {
        CHARON_INFO("Sub 0x08: reportDTCBySeverityMaskRecord start.");
        return DTCBySeverityMaskRecord(receiveBuffer, receiveBufferSize);
    }

    case reportSeverityInformationOfDTC:                    //0x09 
    {
        CHARON_INFO("Sub 0x09: reportSeverityInformationOfDTC start.");
        return SeverityInformationOfDTC(receiveBuffer, receiveBufferSize);
    }

    case reportSupportedDTC:                                //0x0A      
    {
        CHARON_INFO("Sub 0x0A: reportSupportedDTC start.");
        return SupportedDTC();
    }

    case reportFirstTestFailedDTC:                          //0x0B  
    {
        CHARON_INFO("Sub 0x0B: reportFirstTestFailedDTC start.");
        return FirstTestFailedDTC();
    }

    case reportFirstConfirmedDTC:                           //0x0C
    {
        CHARON_INFO("Sub 0x0C: reportFirstConfirmedDTC start.");
        return FirstConfirmedDTC();
    }

    case reportMostRecentTestFailedDTC:                     //0x0D
    {
        CHARON_INFO("Sub 0x0D: reportMostRecentTestFailedDTC start.");
        return MostRecentTestFailedDTC();
    }

    case reportMostRecentConfirmedDTC:                      //0x0E      
    {
        CHARON_INFO("Sub 0x0E: reportMostRecentConfirmedDTC start.");
        return MostRecentConfirmedDTC();
    }

    case reportMirrorMemoryDTCByStatusMask:                 //0x0F   
    {
        CHARON_INFO("Sub 0x0F: reportMirrorMemoryDTCByStatusMask start.");
        bool mirror = true;
        return DTCByStatusMask(receiveBuffer, receiveBufferSize, mirror);
    }

    case reportMirrorMemoryDTCExtDataRecordByDTCNumber:     //0x10    
    {
        CHARON_INFO("Sub 0x10: reportMirrorMemoryDTCExtDataRecordByDTCNumber start.");
        bool mirror = true;
        bool userDefMemory = false;
        return DTCExtDataRecordByDTCNumber(receiveBuffer, receiveBufferSize, mirror, userDefMemory);
    }

    case reportNumberOfMirrorMemoryDTCByStatusMask:         //0x11   
    {
        CHARON_INFO("Sub 0x11: reportNumberOfMirrorMemoryDTCByStatusMask start.");
        bool mirror = true;
        bool userDefMemory = false;
        return NumberOfDTCByStatusMask(receiveBuffer, receiveBufferSize, mirror, userDefMemory);
    }

    case reportNumberOfEmissionsOBDDTCByStatusMask:         //0x12  
    {
        CHARON_INFO("Sub 0x12: reportNumberOfEmissionsOBDDTCByStatusMask start.");
        return NumberOfEmissionsOBDDTCByStatusMask(receiveBuffer, receiveBufferSize);
    }

    case reportEmissionsOBDDTCByStatusMask:                 //0x13 
    {
        CHARON_INFO("Sub 0x13: reportEmissionsOBDDTCByStatusMask start.");
        return EmissionsOBDDTCByStatusMask(receiveBuffer, receiveBufferSize);
    }

    case reportDTCFaultDetectionCounter:                    //0x14  
    {
        CHARON_INFO("Sub 0x14: reportDTCFaultDetectionCounter start.");
        return DTCFaultDetectionCounter(receiveBuffer, receiveBufferSize);
    }

    case reportDTCWithPermanentStatus:                      //0x15 
    {
        CHARON_INFO("Sub 0x15: reportDTCWithPermanentStatus start.");
        return DTCWithPermanentStatus(receiveBuffer, receiveBufferSize);
    }

    case reportDTCExtDataRecordByRecordNumber:              //0x16  
    {
        CHARON_INFO("Sub 0x16: reportDTCExtDataRecordByRecordNumber start.");
        return DTCExtDataRecordByRecordNumber(receiveBuffer, receiveBufferSize);
    }
         
    case reportUserDefMemoryDTCByStatusMask:                //0x17
    {
        CHARON_INFO("Sub 0x17: reportUserDefMemoryDTCByStatusMask start.");
        bool userDefMemory = true;
        bool mirror = false;
        return NumberOfDTCByStatusMask(receiveBuffer, receiveBufferSize, mirror, userDefMemory);
    }

    case reportUserDefMemoryDTCSnapshotRecordByDTCNumber:   //0x18
    {
        CHARON_INFO("Sub 0x18: reportUserDefMemoryDTCSnapshotRecordByDTCNumber start.");
        bool userDefMemory = true;
        return DTCSnapshotRecordByDTCNumber(receiveBuffer, receiveBufferSize, userDefMemory);
    }

    case reportUserDefMemoryDTCExtDataRecordByDTCNumber:    //0x19
    {
        CHARON_INFO("Sub 0x19: reportUserDefMemoryDTCExtDataRecordByDTCNumber start.");
        bool userDefMemory = true;
        bool mirror =  false;
        return DTCExtDataRecordByDTCNumber(receiveBuffer, receiveBufferSize, mirror, userDefMemory);
    }

    default:
        // Subfunction not supported.
        charon_sendNegativeResponse(uds_responseCode_SubfunctionNotSupported, uds_sid_ReadDtcInformation);
        CHARON_ERROR("No valid subfunction requested.");
        return uds_responseCode_SubfunctionNotSupported;
    }
}


uds_responseCode_t charon_StoredDataTransmissionFunctionalUnit_writeDTCToNvm (DTC_t DTCInput, DTC_SnapshotData_t snapPayload, DTC_StoredData_t storedPayload, DTC_ExtendedData_t extPayload)
{ 
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    DTC_SnapshotData_t snapInput = snapPayload;
    DTC_StoredData_t storedInput = storedPayload;
    DTC_ExtendedData_t extInput = extPayload;
    DTC_t DTCinput = DTCInput;
    DTC_t* DTCAddress = NULL;
    bool headerIsUpdated = false;
    

    uint32_t input_nr = (((uint8_t)DTCinput.DTCHighByte << 16u) | ((uint8_t)DTCinput.DTCMiddleByte << 8u) | ((uint8_t)DTCinput.DTCLowByte << 0u));
    // DTC must be in the valid range, some are reserved by ISO 14229-1.
    if ((input_nr <= 0x0000FF) || (input_nr >= 0xFFFF00))
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ClearDiagnosticInformation);
        CHARON_ERROR("Request not in valid range, reserved by ISO 14229-1.");
        return uds_responseCode_RequestOutOfRange;
    }


    // New or update?
    DTCAddress = charon_getDTCLookupByDTCNumber((uint8_t)DTCinput.DTCHighByte, (uint8_t)DTCinput.DTCMiddleByte, (uint8_t)DTCinput.DTCLowByte, false, false, 0x00); /** @todo To add user defined memory change the second bool to true and the value next to it to the requested one in the DTC_t input. */
    if (DTCAddress != NULL)
    {
        // DTC is already present.
        charon_StoredDataTransmissionFunctionalUnit_writeUpdateDTCToNvm(DTCinput, DTCAddress);
    }
    else if (DTC_header->currentDTCCounter != AMOUNT_OF_DTC)
    {
        // DTC is new.
        DTCAddress = charon_StoredDataTransmissionFunctionalUnit_writeNewDTCToNvm(DTCinput);
        headerIsUpdated = true;
    }

    // DTC memory space is full, could not write new DTC.
    if (DTCAddress == NULL)
    {
        return uds_responseCode_GeneralProgrammingFailure;
    }
    

    // Check input, there are no magic numbers so an input without identifier is not valid. When the reserved space in NVM is fully blocked no new input will be saved.
    if (snapInput.DTCSnapshotDataRecordNumberOfIdentifiers != 0)
    {
        if (DTC_header->currentSnapshotRecordCounter != AMOUNT_OF_SNAPSHOT)
        {
            charon_StoredDataTransmissionFunctionalUnit_writeSnapshotToNvm (snapInput, DTCinput, DTCAddress);
            headerIsUpdated = true;
        }
    }
    if (storedInput.DTCStoredDataRecordNumberOfIdentifiers != 0)
    {
        if (DTC_header->currentDataRecordCounter != AMOUNT_OF_STOREDDATA)
        {
            charon_StoredDataTransmissionFunctionalUnit_writeStoredDataToNvm(storedInput, DTCinput, DTCAddress);
            headerIsUpdated = true;
        }
    }
    if (extInput.DTCExtendedDataRecordNumberOfIdentifiers != 0)
    {
        if (DTC_header->currentExtDataRecordCounter != AMOUNT_OF_EXTENDEDDATA)
        {
            charon_StoredDataTransmissionFunctionalUnit_writeExtendedDataToNvm(extInput, DTCinput, DTCAddress);
            headerIsUpdated = true;
        }
    }

    // When changes to the header are made (does not happen when updating) crc16 needs to be updated.
    if (headerIsUpdated)
    {
        charon_StoredDataTransmissionFunctionalUnit_CRC16_update();
    }


    return uds_responseCode_PositiveResponse;
}


// Getter for Mirror location.
DTC_header_t* charon_StoredDataTransmissionFunctionalUnit_getMirrorHeaderAddress(void)
{
    return ((DTC_header_t*)charon_NvmDriver_getMirrorNvmAddress(0, true)); /** @todo USER: check for changed path! */
}

uint16_t charon_StoredDataTransmissionFunctionalUnit_get_total_mirrorDTCCounter (void)
{
    DTC_header_t* DTC_header = charon_StoredDataTransmissionFunctionalUnit_getMirrorHeaderAddress();
    return ((uint16_t)DTC_header->totalDTCCounter);
}

uint8_t charon_StoredDataTransmissionFunctionalUnit_get_total_mirrorSnapshotCounter (void)
{
    DTC_header_t* DTC_header = charon_StoredDataTransmissionFunctionalUnit_getMirrorHeaderAddress();
    return ((uint8_t)DTC_header->totalSnapshotRecordCounter);
}

uint8_t charon_StoredDataTransmissionFunctionalUnit_get_total_mirrorDataRecordCounter (void)
{
    DTC_header_t* DTC_header = charon_StoredDataTransmissionFunctionalUnit_getMirrorHeaderAddress();
    return ((uint8_t)DTC_header->totalDataRecordCounter);
}

uint8_t charon_StoredDataTransmissionFunctionalUnit_get_total_mirrorExtDataRecordCounter (void)
{
    DTC_header_t* DTC_header = charon_StoredDataTransmissionFunctionalUnit_getMirrorHeaderAddress();
    return ((uint8_t)DTC_header->totalExtDataRecordCounter);
}


// Getter for User defined location.
DTC_header_t* charon_StoredDataTransmissionFunctionalUnit_getUserDefHeaderAddress(uint8_t memorySelection)
{
    return ((DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true)); /** @todo future implementation  */
}

uint16_t charon_StoredDataTransmissionFunctionalUnit_get_total_userDefDTCCounter (uint8_t memorySelection)
{
    DTC_header_t* DTC_header = charon_StoredDataTransmissionFunctionalUnit_getUserDefHeaderAddress(memorySelection);
    return ((uint16_t)DTC_header->totalDTCCounter);
}

uint8_t charon_StoredDataTransmissionFunctionalUnit_get_total_userDefSnapshotCounter (uint8_t memorySelection)
{
    DTC_header_t* DTC_header = charon_StoredDataTransmissionFunctionalUnit_getUserDefHeaderAddress(memorySelection);
    return ((uint8_t)DTC_header->totalSnapshotRecordCounter);
}

uint8_t charon_StoredDataTransmissionFunctionalUnit_get_total_userDefDataRecordCounter (uint8_t memorySelection)
{
    DTC_header_t* DTC_header = charon_StoredDataTransmissionFunctionalUnit_getUserDefHeaderAddress(memorySelection);
    return ((uint8_t)DTC_header->totalDataRecordCounter);
}

uint8_t charon_StoredDataTransmissionFunctionalUnit_get_total_userDefExtDataRecordCounter (uint8_t memorySelection)
{
    DTC_header_t* DTC_header = charon_StoredDataTransmissionFunctionalUnit_getUserDefHeaderAddress(memorySelection);
    return ((uint8_t)DTC_header->totalExtDataRecordCounter);
}


void charon_StoredDataTransmissionFunctionalUnit_CRC16_update (void)
{
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint16_t length = (sizeof(DTC_header_t)-2u);
    uint8_t data[length];

    memcpy(data,DTC_header, length);
    // Resetting the checksum.
    DTC_header->CRC16Checksum = charon_crc16_init();
    DTC_header->CRC16Checksum = (uint16_t)charon_crc16_update(0x00,&data,length);
    // Complete CRC16.
    DTC_header->CRC16Checksum = (uint16_t)charon_crc16_finalize(DTC_header->CRC16Checksum);
}


/* Private Function **********************************************************/

static uds_responseCode_t NumberOfDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize, bool mirror, bool userDefMemory)
{
    uint8_t StatusMask = receiveBuffer[2];
    uint8_t memorySelection = 0u;
    if (userDefMemory)
    {
        memorySelection = receiveBuffer[3];
    }
    uint16_t countOfMatchedDTC = charon_getDTCCountByStatusMask(StatusMask, mirror, userDefMemory, memorySelection);
    // length of the buffer is alway 6 bytes. due to consistent response. 
    static uint8_t s_buffer[6];
    uint32_t length = 6u; 


    // Depending on calling request, different data storage is used and so different response are send back.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    if (userDefMemory)
    {
        s_buffer[1] = reportUserDefMemoryDTCByStatusMask;
    }
    else if (mirror)
    {
        s_buffer[1] = reportNumberOfMirrorMemoryDTCByStatusMask;
    }
    else
    {
        s_buffer[1] = reportNumberOfDTCByStatusMask;
    }
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();
    s_buffer[3] = DTC_Format_Identifier;


    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    countOfMatchedDTC = REV16(countOfMatchedDTC);
    #endif
    // Fill uint16_t in uint8_t buffer: cast at start address a new int type and fill this type with wanted value. 
    *(uint16_t*)&s_buffer[4] = countOfMatchedDTC;

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t DTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize, bool mirror)
{
    uint8_t StatusMask = receiveBuffer[2];
    DTC_t *matchedDTC = NULL;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint16_t countOfMatchedDTC = charon_getDTCCountByStatusMask(StatusMask, mirror, false, 0x00);
    uint8_t lengthOfDTC = 3u;


    // Depending on calling request, different data storage is used and so different response are send back.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    if (mirror)
    {
        s_buffer[1] = reportMirrorMemoryDTCByStatusMask;
        matchedDTC = (DTC_t*)charon_NvmDriver_getMirrorNvmAddress(0,false);
    }
    else
    {
        s_buffer[1] = reportDTCByStatusMask;
        matchedDTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    }
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();


    // Builds the response buffer out of found valid DTCs.
    for(uint32_t i = 0u; ((i < countOfMatchedDTC) && (matchedDTC != NULL)); i++)
    {
        // Check if the new input will fit into the buffer.
        if((length + 4u) > MAX_TX_BUFFER_SIZE)
        {
            charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);
            CHARON_ERROR("Response is too long! Maximum size is %i.", MAX_TX_BUFFER_SIZE);
            return uds_responseCode_ResponseTooLong;
        }

        matchedDTC = charon_getDTCLookupByStatusMask(StatusMask,(i+1), mirror);
        if(matchedDTC != NULL)
        {
            memcpy(&s_buffer[length], &matchedDTC->DTCHighByte , lengthOfDTC);
            length += lengthOfDTC;
            memcpy(&s_buffer[length], &matchedDTC->DTCStatusMask, 1u);
            length ++;
        }
    }


    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t DTCSnapshotIdentification (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    DTC_t *DTCwithSnapshot;
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t totalCountOfSavedDTC = DTC_header->totalDTCCounter;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint8_t amountOfSnapshots;
    uint32_t length = 2u;
    uint8_t lengthOfDTC = 3u;
    uint32_t offset = 0u;
    uint16_t currentSnapshotCounter = DTC_header->currentSnapshotRecordCounter;
    uint16_t foundSnapshotCounter = 0u;

    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportDTCSnapshotIdentification;

    // Searches the complete NVM for all valid saved snapshots and will return these DTC.
    for (uint32_t i = 0u; i < totalCountOfSavedDTC; i++)
    {
        DTCwithSnapshot = charon_getDTCSnapshotAddress(offset);

        if (DTCwithSnapshot != NULL)
        {
            amountOfSnapshots = DTCwithSnapshot->NumberOfSavedSnapshots;

            // Check if the new input will fit into the buffer.
            if((length + (amountOfSnapshots*4)) > MAX_TX_BUFFER_SIZE)
            {
                // Too many DTC found. 
                charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);
                CHARON_ERROR("Response is too long! Maximum size is %i.", MAX_TX_BUFFER_SIZE);
                return uds_responseCode_ResponseTooLong;
            }

            // Building buffer.
            for (uint8_t o = 0u; o < amountOfSnapshots; o++)
            {
                memcpy(&s_buffer[length], &DTCwithSnapshot[0].DTCHighByte , lengthOfDTC);
                length += lengthOfDTC;
                memcpy(&s_buffer[length], &DTCwithSnapshot[0].DTCSnapshotRecordNumber[o], 1u);
                length ++;
                foundSnapshotCounter++;
            }

            // Current count must not be the same as total, will break when all snapshots are found.
            if (currentSnapshotCounter == foundSnapshotCounter)
            {
                break;
            }
            
            offset++;
        }
    }
    

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t DTCSnapshotRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize, bool userDefMemory)
{
    DTC_t *matchedDTC = NULL;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 2u;
    uint8_t lengthOfDTC = 3u;
    uint8_t counter = 0u;
    uint32_t input_nr = ((receiveBuffer[2] << 16u) | (receiveBuffer[3] << 8u) | (receiveBuffer[4] << 0u));
    uint8_t DTCMaskRecordhigh = receiveBuffer[2];
    uint8_t DTCMaskRecordmid  = receiveBuffer[3];
    uint8_t DTCMaskRecordlow  = receiveBuffer[4];
    uint8_t DTCSnapshotRecordNumber = receiveBuffer[5];
    uint8_t memorySelection = 0u;
    if (userDefMemory)
    {
        memorySelection = receiveBuffer[6];
    }

    /** @todo New test cases for these 2 conditions. */
    // DTC may not be supported by user.
    for (uint32_t i = 0; i < (sizeof(NOT_SUPPORTED_DTC) / sizeof(NOT_SUPPORTED_DTC[0])); i++)
    {
        if (NOT_SUPPORTED_DTC[i] == input_nr)
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ClearDiagnosticInformation);
            CHARON_ERROR("Request not valid, DTC not supported.");
            return uds_responseCode_RequestOutOfRange;
        }
    }
    // And Snapshot may not be supported by user.
    for (uint32_t i = 0; i < (sizeof(NOT_SUPPORTED_SNAPSHOT) / sizeof(NOT_SUPPORTED_SNAPSHOT[0])); i++)
    {
        if (NOT_SUPPORTED_SNAPSHOT[i] == DTCSnapshotRecordNumber)
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ClearDiagnosticInformation);
            CHARON_ERROR("Request not valid, SnapShot not supported.");
            return uds_responseCode_RequestOutOfRange;
        }
    }


    // Build positive response, always needed even if no snapshot is saved.
    // Depending on calling request, different data storage is used and so different response are send back.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    if (userDefMemory)
    {
        s_buffer[1] = reportUserDefMemoryDTCSnapshotRecordByDTCNumber;
    }
    else
    {
        s_buffer[1] = reportDTCSnapshotRecordByDTCNumber;
    }


    matchedDTC = charon_getDTCLookupByDTCNumber(DTCMaskRecordhigh, DTCMaskRecordmid, DTCMaskRecordlow, false, userDefMemory, memorySelection);

    if (matchedDTC != NULL)
    {
        if (matchedDTC->NumberOfSavedSnapshots > 0)
        {
            // If 0xFF was input by user, all Snapshots shall be printed.
            if (DTCSnapshotRecordNumber == PRINT_ALL)
            {

                for (uint8_t i = 0; i < sizeof(matchedDTC->DTCSnapshotRecordNumber); i++)
                {
                    if (matchedDTC->DTCSnapshotLength[i] != 0x00)
                    {
                        counter++;
                    }  
                }
            }
            else
            {
                counter = 1;
            }
            
            for (uint8_t i = 0 ; i < counter; i++)
            {
                // Check if the new input will fit into the buffer.
                if((length + lengthOfDTC + 1u + 1u + 1u + (uint16_t)matchedDTC->DTCSnapshotLength[i]) > MAX_TX_BUFFER_SIZE)
                {
                    // Too many DTC found, 
                    charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);
                    CHARON_ERROR("Response is too long! Maximum size is %i.", MAX_TX_BUFFER_SIZE);
                    return uds_responseCode_ResponseTooLong;
                }
                // Building response buffer.
                memcpy(&s_buffer[length], &matchedDTC->DTCHighByte, lengthOfDTC);
                length += lengthOfDTC;
                memcpy(&s_buffer[length], &matchedDTC->statusOfDTC, 1u);
                length++;
                memcpy(&s_buffer[length], &matchedDTC->DTCSnapshotRecordNumber[i], 1u);
                length++;
                memcpy(&s_buffer[length], &matchedDTC->DTCSnapshotAddress[i]->DTCSnapshotDataRecordNumberOfIdentifiers, 1u);
                length++;
                memcpy(&s_buffer[length], &matchedDTC->DTCSnapshotAddress[i]->DTCSnapshotDataPayload[0], (uint16_t)matchedDTC->DTCSnapshotLength[i]);
                length += (uint16_t)matchedDTC->DTCSnapshotLength[i];
            }
        }
    } else
    {
        // When no snapshot was found an echo shall be send back.
        memcpy(&s_buffer[length], &receiveBuffer[2], lengthOfDTC);
        length += lengthOfDTC;
        memcpy(&s_buffer[length], &receiveBuffer[5], 1u);
        length++;
    }

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t DTCStoredDataByRecordNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    bool header = false;
    DTC_t *matchedDTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,header);
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 2u;
    uint8_t lengthOfDTC = 3u;
    uint8_t DTCStoredRecordNumber = receiveBuffer[2];

    // If user put in 0xFF all Records need to be printed.
    if (DTCStoredRecordNumber ==  PRINT_ALL)
    {
        // Depending on outcome only two possible cases are valid.
        if ((charon_getDTCLookup_reportAllDataRecords (receiveBuffer, receiveBufferSize)) == uds_responseCode_PositiveResponse)
        {
            return uds_responseCode_PositiveResponse;
        }
        else
        {
            return uds_responseCode_ResponseTooLong;
        }
    }
    // StoredRecord may not be supported by user.
    for (uint32_t i = 0; i < sizeof(NOT_SUPPORTED_DATARECORD); i++)
    {
        if ((DTCStoredRecordNumber == 0x00) || (NOT_SUPPORTED_DATARECORD[i] == DTCStoredRecordNumber))
        {
            // Not supported or invalid input.
            charon_sendNegativeResponse(uds_responseCode_ServiceNotSupported, uds_sid_ReadDtcInformation);
            CHARON_ERROR("Request not valid, DataRecord not supported.");
            return uds_responseCode_ServiceNotSupported;
        }
    }

    matchedDTC = charon_getDTCLookupByRecordNumber(DTCStoredRecordNumber);

    // Build positive response, always needed even if no DataRecord is saved.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportDTCStoredDataByRecordNumber;

    if (matchedDTC != NULL)
    {
        if (matchedDTC->NumberOfSavedDataRecords > 0)
        {
            for (uint8_t i = 0; i < sizeof(matchedDTC->DTCStoredDataRecordNumber); i++)
            {
                if (DTCStoredRecordNumber == (uint8_t)matchedDTC->DTCStoredDataRecordNumber[i])
                {
                    // Building response buffer.
                    memcpy(&s_buffer[length], &matchedDTC->DTCStoredDataRecordNumber[i], 1u);
                    length++;
                    memcpy(&s_buffer[length], &matchedDTC->DTCHighByte, lengthOfDTC);
                    length += lengthOfDTC;
                    memcpy(&s_buffer[length], &matchedDTC->statusOfDTC, 1u);
                    length++;
                    memcpy(&s_buffer[length], &matchedDTC->DTCStoredDataAddress[i]->DTCStoredDataRecordNumberOfIdentifiers, 1u);
                    length++;
                    memcpy(&s_buffer[length], &matchedDTC->DTCStoredDataAddress[i]->DTCStoredDataPayload[0], (uint16_t)matchedDTC->DTCStoredDataLength[i]);
                    length += (uint16_t)matchedDTC->DTCStoredDataLength[i];
                }
            }
        }
    } else
    {
        // When no DataRecord was found an echo shall be send back.
        memcpy(&s_buffer[length], &receiveBuffer[2], 1u);
        length++;
    }

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t DTCExtDataRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize, bool mirror, bool userDefMemory)
{
    DTC_t *matchedDTC = NULL;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 2u;
    uint8_t lengthOfDTC = 3u;
    uint8_t recordNumberLoops = 0u;
    uint8_t DTCMaskRecordhigh = receiveBuffer[2];
    uint8_t DTCMaskRecordmid  = receiveBuffer[3];
    uint8_t DTCMaskRecordlow  = receiveBuffer[4];
    uint8_t DTCExtDataRecordNumber = receiveBuffer[5];
    uint8_t memorySelection = 0u;
    bool printAll = false;
    if (userDefMemory)
    {
        memorySelection = receiveBuffer[6];
    }

#if CHARON_CONFIG_OBD_SUPPORT
    // Only true if ExtData is not supported.
    for (uint32_t i = 0; i < sizeof(NOT_SUPPORTED_EXTDATA); i++)
    {
        if (NOT_SUPPORTED_EXTDATA[i] == DTCExtDataRecordNumber)
        {
            // No matching DTCRecord found.
            charon_sendNegativeResponse(uds_responseCode_ServiceNotSupported, uds_sid_ReadDtcInformation);
            CHARON_ERROR("Request not valid, ExtendedDataRecord not supported.");
            return uds_responseCode_ServiceNotSupported;
        }
    }
#endif
#if !CHARON_CONFIG_OBD_SUPPORT
    // Only true if ExtData is not supported OR PRINT_ALL_OBD (0xFE) was input by user (when not supported).
    for (uint32_t i = 0; i < sizeof(NOT_SUPPORTED_EXTDATA); i++)
    {                                                                                               // 0x90 - 0xFE are reserved for OBD 
        if (((DTCExtDataRecordNumber == NOT_SUPPORTED_EXTDATA[i]) || (DTCExtDataRecordNumber >= 0x90)) && (DTCExtDataRecordNumber != PRINT_ALL))
        {
            // No matching DTCRecord found.
            charon_sendNegativeResponse(uds_responseCode_ServiceNotSupported, uds_sid_ReadDtcInformation);
            CHARON_ERROR("Request not valid, ExtendedDataRecord not supported.");
            return uds_responseCode_ServiceNotSupported;
        }
    }
#endif    

    // Build positive response, always needed even if no ExtData is saved.
    // Depending on calling request, different data storage is used and so different response are send back.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    if (userDefMemory)
    {
        s_buffer[1] = reportUserDefMemoryDTCExtDataRecordByDTCNumber;
    }
    else if (mirror)
    {
        s_buffer[1] = reportMirrorMemoryDTCExtDataRecordByDTCNumber;
    }
    else
    {
        s_buffer[1] = reportDTCExtDataRecordByDTCNumber;
    }

    matchedDTC = charon_getDTCLookupByDTCNumber(DTCMaskRecordhigh, DTCMaskRecordmid, DTCMaskRecordlow, mirror, userDefMemory, memorySelection);

    if (matchedDTC != NULL)
    {
        if (matchedDTC->NumberOfSavedExtendedData > 0)
        {
#if CHARON_CONFIG_OBD_SUPPORT        
            // OBD case, if 0xFE was input by user, all OCB shall be printed.
            if (DTCExtDataRecordNumber == PRINT_ALL_OBD)
            {
                for (uint8_t i = 0; i < sizeof(matchedDTC->DTCExtDataRecordNumber); i++)
                {
                    if ((matchedDTC->DTCExtDataRecordNumber >= 0x90) && (matchedDTC->DTCExtDataRecordNumber <= 0xEF))
                    {
                        // Check if the new input will fit into the buffer.
                        if((length + lengthOfDTC + 1u + 1u + (uint16_t)matchedDTC->DTCExtendedDataLength[i]) > MAX_TX_BUFFER_SIZE)
                        {
                            // Too many DTC found, 
                            charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);
                            CHARON_ERROR("Response is too long! Maximum size is %i.", MAX_TX_BUFFER_SIZE);
                            return uds_responseCode_ResponseTooLong;
                        }

                        memcpy(&s_buffer[length], &matchedDTC->DTCHighByte, lengthOfDTC);
                        length += lengthOfDTC;
                        memcpy(&s_buffer[length], &matchedDTC->statusOfDTC, 1u);
                        length++;
                        memcpy(&s_buffer[length], &matchedDTC->DTCExtDataRecordNumber[i], 1u);
                        length++;
                        memcpy(&s_buffer[length], &matchedDTC->DTCExtendedDataAddress[i]->DTCExtendedDataPayload, (uint16_t)matchedDTC->DTCExtendedDataLength[i]);
                        length += (uint16_t)matchedDTC->DTCExtendedDataLength[i];
                    }
                }
            }          
#endif     
            if (DTCExtDataRecordNumber != PRINT_ALL_OBD)
            {
                // If 0xFF was input by user, all ExtData shall be printed.
                if (DTCExtDataRecordNumber == PRINT_ALL)
                {
                    recordNumberLoops = matchedDTC->NumberOfSavedExtendedData;
                    printAll = true;
                }
                else
                {
                    recordNumberLoops = 0x01; 
                }

                for (uint8_t i = 0; i < sizeof(matchedDTC->DTCExtDataRecordNumber); i++)
                {
                    if (matchedDTC->DTCExtendedDataLength[i] != 0)
                    {
                        if ((matchedDTC->DTCExtDataRecordNumber[i] == DTCExtDataRecordNumber) || (printAll))
                        {
                            // Check if the new input will fit into the buffer.
                            if((length + lengthOfDTC + 1u + 1u + (uint16_t)matchedDTC->DTCExtendedDataLength[i]) > MAX_TX_BUFFER_SIZE)
                            {
                                // Too many DTC found, 
                                charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);
                                CHARON_ERROR("Response is too long! Maximum size is %i.", MAX_TX_BUFFER_SIZE);
                                return uds_responseCode_ResponseTooLong;
                            }

                            // Building response buffer.
                            memcpy(&s_buffer[length], &matchedDTC->DTCHighByte, lengthOfDTC);
                            length += lengthOfDTC;
                            memcpy(&s_buffer[length], &matchedDTC->statusOfDTC, 1u);
                            length++;
                            memcpy(&s_buffer[length], &matchedDTC->DTCExtDataRecordNumber[i], 1u);
                            length++;
                            memcpy(&s_buffer[length], &matchedDTC->DTCExtendedDataAddress[i]->DTCExtendedDataPayload, (uint16_t)matchedDTC->DTCExtendedDataLength[i]);
                            length += (uint16_t)matchedDTC->DTCExtendedDataLength[i];

                            recordNumberLoops--;
                            if (recordNumberLoops == 0)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    
    if ((matchedDTC == NULL) || (recordNumberLoops != 0))
    {
        // When no ExtData was found an echo shall be send back.
        memcpy(&s_buffer[length], &receiveBuffer[2], lengthOfDTC);
        length += lengthOfDTC;
        s_buffer[length] = charon_getDTCStatusAvailabilityMask();
        length++; 
    }

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t NumberOfDTCBySeverityMaskRecord (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint8_t DTCSeverityMask = receiveBuffer[2];
    uint8_t DTCStatusMask = receiveBuffer[3];
    uint16_t countOfMatchedDTC = charon_getDTCCountBySeverityMask(DTCSeverityMask,DTCStatusMask);
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    /* length of the buffer is alway 6 bytes. due to consistent response*/
    uint32_t length = 6u; 

    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportNumberOfDTCBySeverityMaskRecord;
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();
    s_buffer[3] = DTC_Format_Identifier;

    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    countOfMatchedDTC = REV16(countOfMatchedDTC);
    #endif
    // Fill uint16_t in uint8_t buffer: cast at start address a new int type and fill this type with wanted value. 
    *(uint16_t*)&s_buffer[4] = countOfMatchedDTC;
    s_buffer[4] = countOfMatchedDTC;
    
    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t DTCBySeverityMaskRecord (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    DTC_t *DTCwithMatch;
    uint8_t DTCSeverityMask = receiveBuffer[2];
    uint8_t DTCStatusMask = receiveBuffer[3];
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint8_t lengthOfDTC = 3u;
    uint16_t offset = 0;
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t totalCountOfSavedDTC = DTC_header->totalDTCCounter;
    uint32_t currentCountOfSavedDTC = DTC_header->currentDTCCounter;
    uint32_t foundDTCCounter = 0;


    // Building response buffer.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportDTCBySeverityMaskRecord;
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();

    for (uint32_t i = 0; i < totalCountOfSavedDTC; i++)
    {
        DTCwithMatch = charon_getDTCSeverityMaskAddress(DTCSeverityMask,DTCStatusMask,offset);

        if (DTCwithMatch != NULL)
        {
            // Check if the new input will fit into the buffer.
            if((length + 7u ) > MAX_TX_BUFFER_SIZE)
            {
                // Too many DTC found, 
                charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);
                CHARON_ERROR("Response is too long! Maximum size is %i.", MAX_TX_BUFFER_SIZE);
                return uds_responseCode_ResponseTooLong;
            }

            // Building response buffer.
            memcpy(&s_buffer[length], &DTCwithMatch->DTCSeverityMask, 1u);
            length++;
            memcpy(&s_buffer[length], &DTCwithMatch->FunctionalGroupIdentifier, 1u);
            length++;
            memcpy(&s_buffer[length], &DTCwithMatch->DTCHighByte , lengthOfDTC);
            length += lengthOfDTC;
            memcpy(&s_buffer[length], &DTCwithMatch->statusOfDTC, 1u);
            length++;
            foundDTCCounter++;

            // Current count must not be the same as total, will break when all DTCs are found.
            if (currentCountOfSavedDTC == foundDTCCounter)
            {
                break;
            }
        }
        offset++;
    }
    
    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t SeverityInformationOfDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    DTC_t *DTCwithMatch = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    uint8_t DTCMaskRecordhigh = receiveBuffer[2];
    uint8_t DTCMaskRecordmid  = receiveBuffer[3];
    uint8_t DTCMaskRecordlow  = receiveBuffer[4];
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint8_t lengthOfDTC = 3u;

    // Building response buffer.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportSeverityInformationOfDTC;
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();


    DTCwithMatch = charon_getDTCLookupByDTCNumber(DTCMaskRecordhigh,DTCMaskRecordmid,DTCMaskRecordlow, false, false, 0x00);

    if (DTCwithMatch != NULL)
    {
        // Building response buffer.
        memcpy(&s_buffer[length], &DTCwithMatch->DTCSeverityMask, 1u);
        length++;
        memcpy(&s_buffer[length], &DTCwithMatch->FunctionalGroupIdentifier, 1u);
        length++;
        memcpy(&s_buffer[length], &DTCwithMatch->DTCHighByte , lengthOfDTC);
        length += lengthOfDTC;
        memcpy(&s_buffer[length], &DTCwithMatch->statusOfDTC, 1u);
        length++;
    }
    else
    {
        // When no DTC was found an echo shall be send back.
        memcpy(&s_buffer[length], &receiveBuffer[2], lengthOfDTC);
        length += lengthOfDTC;
    }


    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t SupportedDTC (void)
{
    DTC_t *matchedDTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,false);
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint8_t lengthOfDTC = 3u;
    uint8_t index;
    uint8_t number;
    DTC_header_t *DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t countOfSavedDTC = DTC_header->totalDTCCounter;
    uint32_t currentAmountOfSavedDTC = DTC_header->currentDTCCounter;
    uint32_t DTCInBufferCounter = 0x00;

    // Building response buffer.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportSupportedDTC;
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();


    // Check if the new input will fit into the buffer.
    if((currentAmountOfSavedDTC * 4u) > MAX_TX_BUFFER_SIZE)
    {
        charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);
        CHARON_ERROR("Response is too long! Maximum size is %i.", MAX_TX_BUFFER_SIZE);
        return uds_responseCode_ResponseTooLong;
    }


    for (uint32_t i = 0; i < countOfSavedDTC; i++)
    {
        // Validation check.
        LOOKUP_CALC(i, index, number);
        index = (uint8_t)DTC_header->nvmDTCLookupTable[index];
        number = (0x01 << number);

        if ((index & number) > 0x00)
        {
            // Building response buffer.
            memcpy(&s_buffer[length], &matchedDTC[i].DTCHighByte , lengthOfDTC);
            length += lengthOfDTC;
            memcpy(&s_buffer[length], &matchedDTC[i].statusOfDTC, 1u);
            length ++;
            DTCInBufferCounter++;
        }

        // Current count must not be the same as total, will break when all DTCs are found.
        if (currentAmountOfSavedDTC == DTCInBufferCounter)
        {
            break;
        }
    }
    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t FirstTestFailedDTC (void)
{
    DTC_t *matchedDTC;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint8_t lengthOfDTC = 3u;
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);

    // Building response buffer.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportFirstTestFailedDTC;
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();


    // Check if there is a saved DTC since header wipe.
    if ((DTC_header->FirstFailedDTC != 0) && (DTC_header->FirstFailedDTC != 0x01))
    {       
        // Get address to the first failed DTC from header.
        matchedDTC = (DTC_t*)DTC_header->FirstFailedDTC;

        // Building response buffer.
        memcpy(&s_buffer[length], &matchedDTC->DTCHighByte , lengthOfDTC);
        length += lengthOfDTC;
        memcpy(&s_buffer[length], &matchedDTC->statusOfDTC, 1u);
        length ++;
    }

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t FirstConfirmedDTC (void)
{
    DTC_t *matchedDTC;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint8_t lengthOfDTC = 3u;
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);

    // Building response buffer.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportFirstConfirmedDTC;
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();


    // Check if there is a saved DTC since header wipe.
    if ((DTC_header->FirstConfirmedDTC != 0) && (DTC_header->FirstConfirmedDTC != 0x01))
    {
        // Get address to the first confirmed DTC from header.
        matchedDTC = (DTC_t*)DTC_header->FirstConfirmedDTC;

        // Building response buffer.
        memcpy(&s_buffer[length], &matchedDTC->DTCHighByte , lengthOfDTC);
        length += lengthOfDTC;
        memcpy(&s_buffer[length], &matchedDTC->statusOfDTC, 1u);
        length ++;
    }

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t MostRecentTestFailedDTC (void)
{
    DTC_t *matchedDTC;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint8_t lengthOfDTC = 3u;
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);

    // Building response buffer.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportMostRecentTestFailedDTC;
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();

    // Check if there is a saved DTC since header wipe.
    if (DTC_header->MostRecentTestFailed != 0)
    {
        // Get address to the most recent DTC from header.
        matchedDTC = (DTC_t*)DTC_header->MostRecentTestFailed;

        // Building response buffer.
        memcpy(&s_buffer[length], &matchedDTC->DTCHighByte , lengthOfDTC);
        length += lengthOfDTC;
        memcpy(&s_buffer[length], &matchedDTC->statusOfDTC, 1u);
        length ++;
    }

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t MostRecentConfirmedDTC (void)
{
    DTC_t *matchedDTC;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint8_t lengthOfDTC = 3u;
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);

    // Building response buffer.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportMostRecentConfirmedDTC;
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();

    // Check if there is a saved DTC since header wipe.
    if (DTC_header->MostRecentConfirmedDTC != 0)
    {
        // Get address to the most recent DTC from header.
        matchedDTC = (DTC_t*)DTC_header->MostRecentConfirmedDTC;

        // Building response buffer.
        memcpy(&s_buffer[length], &matchedDTC->DTCHighByte , lengthOfDTC);
        length += lengthOfDTC;
        memcpy(&s_buffer[length], &matchedDTC->statusOfDTC, 1u);
        length ++;
    }

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uds_responseCode_t NumberOfEmissionsOBDDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    CHARON_ERROR("Subfunction not Supported.");
    return uds_responseCode_SubfunctionNotSupported;
}


static uds_responseCode_t EmissionsOBDDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    CHARON_ERROR("Subfunction not Supported.");
    return uds_responseCode_SubfunctionNotSupported;
}


static uds_responseCode_t DTCFaultDetectionCounter (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    CHARON_ERROR("Subfunction not Supported.");
    return uds_responseCode_SubfunctionNotSupported;
}


static uds_responseCode_t DTCWithPermanentStatus (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    CHARON_ERROR("Subfunction not Supported.");
    return uds_responseCode_SubfunctionNotSupported;
}


static uds_responseCode_t DTCExtDataRecordByRecordNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    DTC_t *matchedDTC = NULL;
    uint32_t pointerBuffer[1] = {0}; // Dummy.
    uint32_t* pIndex = &pointerBuffer[0];
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint8_t lengthOfDTC = 3u;
    uint8_t counter = 0u;
    uint32_t offset = 0u;
    uint8_t DTCExtDataRecordNumber = receiveBuffer[2];

    // A not supported ExtData was requested.
    for (size_t i = 0; i < sizeof(NOT_SUPPORTED_EXTDATA); i++)
    {
        if ((DTCExtDataRecordNumber > 0xEF) || (DTCExtDataRecordNumber == NOT_SUPPORTED_EXTDATA[i]))
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadDtcInformation);
            CHARON_ERROR("Request not valid, ExtendedDataRecord not supported.");
            return uds_responseCode_RequestOutOfRange;
        }
    }

    // Building response buffer, always needed even if no ExtData is saved.
    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportDTCExtDataRecordByRecordNumber;
    s_buffer[2] = DTCExtDataRecordNumber;

    // Get the total count of matching ExtData.
    counter = charon_getDTCExtDataCountByRecordNumber(DTCExtDataRecordNumber);

    if (counter != 0)
    {
        do
        {
            matchedDTC = charon_getDTCExtDataAddressByRecordNumber (DTCExtDataRecordNumber,offset,pIndex);

            if (matchedDTC != NULL)
            {
                // Skip more positive response building if no ExtData is saved.
                if (matchedDTC->NumberOfSavedExtendedData > 0)
                {
                    // Check if the new input will fit into the buffer.
                    if((length + lengthOfDTC + 1u + (uint16_t)matchedDTC->DTCExtendedDataLength[(uint32_t)*pIndex]) > MAX_TX_BUFFER_SIZE)
                    {
                        charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);
                        CHARON_ERROR("Response is too long! Maximum size is %i.", MAX_TX_BUFFER_SIZE);
                        return uds_responseCode_ResponseTooLong;
                    }

                    // Building response buffer.
                    memcpy(&s_buffer[length], &matchedDTC->DTCHighByte, lengthOfDTC);
                    length += lengthOfDTC;
                    memcpy(&s_buffer[length], &matchedDTC->statusOfDTC, 1u);
                    length++;
                    memcpy(&s_buffer[length], &matchedDTC->DTCExtendedDataAddress[(uint32_t)*pIndex]->DTCExtendedDataPayload, (uint16_t)matchedDTC->DTCExtendedDataLength[(uint32_t)*pIndex]);
                    length += (uint16_t)matchedDTC->DTCExtendedDataLength[(uint32_t)*pIndex];
                }
            }
            if (counter != 0)
            {
                counter--;
                offset++;
            }
        } while (counter != 0);
    }

    charon_sscTxMessage(s_buffer,length);
    //charon info einfügen für debugging!
    return uds_responseCode_PositiveResponse;
}


static uint8_t charon_DTC_LookupTable_getBitNumber (uint8_t input)
{
    uint8_t number;
    uint8_t mask;
    if ((input & 0x0F) == 0x0F)
    {
        // Reset not wanted bits.
        mask = input & 0xF0;
        // Slot is in high nibble, split again.
        if ((mask & 0x30) == 0x30)
        {
            // Slot is in high half nibble.
            if ((mask & 0x40) == 0x40)
            {
                // 0111 1111
                number = 7u;
            }
            else
            {
                // x011 1111
                number = 6u;
            }
        }
        else
        {
           // Slot is in low half nibble.
            if ((mask & 0x10) == 0x10)
            {
                // xx01 1111
                number = 5u;
            }
            else
            {
                // xxx0 1111
                number = 4u;
            }
        }
    }
    else
    {
        // Reset not wanted bits.
        mask = input & 0x0F;
        // Slot is in low nibble, split again.
        if ((mask & 0x03) == 0x03)
        {
            // Slot is in high half nibble.
            if ((mask & 0x07) == 0x07)
            {
                // xxxx 0111
                number = 3u;
            }
            else
            {
                // xxxx x011
                number = 2u;
            }
        }
        else
        {
            // Slot is in low half nibble.
            if ((mask & 0x01) == 0x01)
            {
                // xxxx xx01
                number = 1u;
            }
            else
            {
                // xxxx xxx0
                number = 0u;
            }
        }
    }

    return number;
}

// Write DTC data to NVM
static DTC_t* charon_StoredDataTransmissionFunctionalUnit_writeNewDTCToNvm (DTC_t DTCup)
{
    DTC_t DTCinput = DTCup;
    DTC_t* DTC = NULL;
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    uint32_t position;
    // To get out of double for loop.
    bool doubleBreak = false;

    if (DTC_header->totalDTCCounter == DTC_header->currentDTCCounter)
    {
        position = (uint16_t)DTC_header->totalDTCCounter;
        // Updating the lookup table
        DTC_header->nvmDTCLookupTable[(position / 8u)] |= (0x01 << ((position) % 8u));
        DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(position,false);

        DTC_header->totalDTCCounter++;
        DTC_header->currentDTCCounter++;
    } 
    else
    {
        for (uint32_t b = 0; b < ((uint16_t)DTC_header->sizeOfDTCLookupTable); b++)
        { 
            for (uint32_t o = 0; o < BYTE_SIZE; o++)
            {
                if (DTC_header->nvmDTCLookupTable[b] != 0xFF)
                {
                    // Split the Byte to get less comparisons. 
                    position = charon_DTC_LookupTable_getBitNumber(DTC_header->nvmDTCLookupTable[b]);
                    // Updating the lookup table
                    DTC_header->nvmDTCLookupTable[b] |= (0x01 << position);

                    // Building Storage slot number.
                    position = ((b * 8u)+ position);
                    DTC = (DTC_t*)charon_NvmDriver_getNvmAddress_for_DTC(position,false);


                    // True if a new slot is used, false if a previous deleted one is used.
                    if ((uint16_t)DTC_header->totalDTCCounter == (uint16_t)DTC_header->currentDTCCounter)
                    {
                        DTC_header->totalDTCCounter++;
                        DTC_header->currentDTCCounter++;
                    }
                    else
                    {
                        DTC_header->currentDTCCounter++;
                    }

                    doubleBreak = true;
                    break;
                }
                else
                {
                    o = 8u;
                }
            }
            if (doubleBreak)
            {
                break;
            }
        }
    }


    // Fill DTC.
    DTC->DTCStatusMask                   = DTCinput.DTCStatusMask;
    DTC->DTCHighByte                     = DTCinput.DTCHighByte;
    DTC->DTCMiddleByte                   = DTCinput.DTCMiddleByte;
    DTC->DTCLowByte                      = DTCinput.DTCLowByte;
    DTC->DTCSeverityMask                 = DTCinput.DTCSeverityMask;
    DTC->DTCSeverityMaskRecordHigh       = DTCinput.DTCSeverityMaskRecordHigh;
    DTC->DTCSeverityMaskRecordLow        = DTCinput.DTCSeverityMaskRecordLow;
    DTC->FunctionalGroupIdentifier       = DTCinput.FunctionalGroupIdentifier;
    DTC->MemorySelection                 = DTCinput.MemorySelection;
    DTC->NumberOfSavedSnapshots          = 0x00;
    DTC->NumberOfSavedDataRecords        = 0x00;
    DTC->NumberOfSavedExtendedData       = 0x00;
    DTC->statusOfDTC                     = DTCinput.statusOfDTC;
    DTC->DTCSettingType                  = DTCinput.DTCSettingType;


    // First DTC save.
    if ((DTC_header->FirstFailedDTC == 0) && ((DTC->DTCStatusMask & TEST_FAILED_CHECK) == TEST_FAILED_CHECK))
    {
        DTC_header->FirstFailedDTC = (uint32_t)DTC;
    }
    if ((DTC_header->FirstConfirmedDTC == 0) && ((DTC->DTCStatusMask & CONFIRMED_DTC_CHECK) == CONFIRMED_DTC_CHECK))
    {
        DTC_header->FirstConfirmedDTC = (uint32_t)DTC;
    }
    // Most recent DTC save.
    if ((DTC->DTCStatusMask & TEST_FAILED_CHECK) == TEST_FAILED_CHECK)
    {
        DTC_header->MostRecentTestFailed = (uint32_t)DTC;
    }
    if ((DTC->DTCStatusMask & CONFIRMED_DTC_CHECK) == CONFIRMED_DTC_CHECK)
    {
        DTC_header->MostRecentConfirmedDTC = (uint32_t)DTC;
    }

    return DTC;
}

static void charon_StoredDataTransmissionFunctionalUnit_writeUpdateDTCToNvm (DTC_t DTCup, DTC_t* address)
{
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);
    DTC_t* DTCcurrent = (DTC_t*)address;
    DTC_t DTCupdate = DTCup;

    // Writing needs to be allowed.
    if (DTCcurrent->DTCSettingType == 0x01)
    {
        if (DTCupdate.statusOfDTC != 0x00)
        {
            DTCcurrent->statusOfDTC = DTCupdate.statusOfDTC;
        }
        if (DTCupdate.DTCStatusMask != 0x00)
        {
            DTCcurrent->DTCStatusMask = DTCupdate.DTCStatusMask;
        }

        // First DTC save.
        if ((DTC_header->FirstFailedDTC == 0) && ((DTCcurrent->DTCStatusMask & TEST_FAILED_CHECK) == TEST_FAILED_CHECK))
        {
            DTC_header->FirstFailedDTC = (uint32_t)&DTCcurrent;
        }
        if ((DTC_header->FirstConfirmedDTC == 0) && ((DTCcurrent->DTCStatusMask & CONFIRMED_DTC_CHECK) == CONFIRMED_DTC_CHECK))
        {
            DTC_header->FirstConfirmedDTC = (uint32_t)&DTCcurrent;
        }
        // Most recent DTC save.
        if ((DTCcurrent->DTCStatusMask & TEST_FAILED_CHECK) == TEST_FAILED_CHECK)
        {
            DTC_header->MostRecentTestFailed = (uint32_t)&DTCcurrent;
        }
        if ((DTCcurrent->DTCStatusMask & CONFIRMED_DTC_CHECK) == CONFIRMED_DTC_CHECK)
        {
            DTC_header->MostRecentConfirmedDTC = (uint32_t)&DTCcurrent;
        }
    }
}

static void charon_StoredDataTransmissionFunctionalUnit_writeSnapshotToNvm (DTC_SnapshotData_t payload, DTC_t DTCup, DTC_t* address)
{
    // IF check before even entering this function. (payload not empty!)
    DTC_t DTCUpdate = DTCup;
    DTC_t* DTCCurrent = (DTC_t*)address;
    DTC_SnapshotData_t update = payload;
    DTC_SnapshotData_t* snapshotCurrent = NULL;
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);

    bool foundEmptyIndex = false;
    bool doubleBreak = false;
    uint16_t number;
    uint8_t arrayIndex;


    // Getting the next empty index.
    for (uint32_t i = 0; i < NVM_AMOUNT_OF_SNAPSHOTS; i++)
    {
        if (DTCCurrent->DTCSnapshotAddress[i] == 0x00)
        {
            arrayIndex = i;
            foundEmptyIndex = true;
            break;
        }
    }
    
    if (foundEmptyIndex)
    {
        // Search the lookup table for the next empty slot, runs 8x the same index if needed. 
        for (uint32_t b = 0; b < ((uint16_t)DTC_header->sizeOfSnapshotLookupTable); b++)
        { 
            for (uint32_t o = 0; o < BYTE_SIZE; o++)
            {
                if (DTC_header->nvmSnapshotLookupTable[b] != 0xFF)
                {
                    // Split the Byte to get less comparisons. 
                    number = charon_DTC_LookupTable_getBitNumber(DTC_header->nvmSnapshotLookupTable[b]);
                    // Updating the lookup table.
                    DTC_header->nvmSnapshotLookupTable[b] |= (0x01 << number);

                    // Building Storage slot number.
                    number = ((b * 8u)+ number);

                    // Filling all the new information into the DTC.
                    *(uint32_t*)&DTCCurrent->DTCSnapshotAddress[arrayIndex] = charon_NvmDriver_getNvmAddress_for_Snapshot(number);
                    DTCCurrent->DTCSnapshotLength[arrayIndex] = DTCUpdate.DTCSnapshotLength[0];       
                    DTCCurrent->DTCSnapshotRecordNumber[arrayIndex] = number;
                    DTCCurrent->NumberOfSavedSnapshots++;

                    // Filling all the new information into the snapshot.
                    *(uint32_t*)&snapshotCurrent = charon_NvmDriver_getNvmAddress_for_Snapshot(number);
                    snapshotCurrent->DTCSnapshotDataRecordNumberOfIdentifiers = update.DTCSnapshotDataRecordNumberOfIdentifiers;
                    for (uint32_t i = 0; i < (uint16_t)DTCUpdate.DTCSnapshotLength[0]; i++)
                    {
                        snapshotCurrent->DTCSnapshotDataPayload[i] = update.DTCSnapshotDataPayload[i];
                    }
                    
                    // True if a new slot is used, false if a previous deleted one is used.
                    if ((uint16_t)DTC_header->totalSnapshotRecordCounter == (uint16_t)DTC_header->currentSnapshotRecordCounter)
                    {
                        DTC_header->totalSnapshotRecordCounter++;
                        DTC_header->currentSnapshotRecordCounter++;
                    }
                    else
                    {
                        DTC_header->currentSnapshotRecordCounter++;
                    }

                    doubleBreak = true;
                    break;
                }
                else
                {
                    o = 8u;
                }
            }
            if (doubleBreak)
            {
                break;
            }
        }
    }
}

static void charon_StoredDataTransmissionFunctionalUnit_writeStoredDataToNvm (DTC_StoredData_t payload, DTC_t DTCup, DTC_t* address)
{
    // IF check before even entering this function. (payload not empty!)
    DTC_t DTCUpdate = DTCup;
    DTC_t* DTCCurrent = (DTC_t*)address;
    DTC_StoredData_t storedDataUpdate = payload;
    DTC_StoredData_t* storedDataCurrent = NULL;
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);

    bool foundEmptyIndex = false;
    bool doubleBreak = false;
    uint16_t number;
    uint8_t arrayIndex;


    // Getting the next empty index.
    for (uint32_t i = 0; i < NVM_AMOUNT_OF_DATARECORDS; i++)
    {
        if (DTCCurrent->DTCStoredDataAddress[i] == 0x00)
        {
            arrayIndex = i;
            foundEmptyIndex = true;
            break;
        }
    }
    
    if (foundEmptyIndex)
    {
        // Search the lookup table for the next empty slot, runs 8x the same index if needed. 
        for (uint32_t b = 0; b < ((uint16_t)DTC_header->sizeOfStoredDataLookupTable); b++)
        { 
            for (uint32_t o = 0; o < BYTE_SIZE; o++)
            {
                if (DTC_header->nvmStoredDataLookupTable[b] != 0xFF)
                {
                    // Split the Byte to get less comparisons. 
                    number = charon_DTC_LookupTable_getBitNumber(DTC_header->nvmStoredDataLookupTable[b]);
                    // Updating the lookup table.
                    DTC_header->nvmStoredDataLookupTable[b] |= (0x01 << number);

                    // Building Storage slot number.
                    number = ((b * 8u)+ number);

                    // Filling all the new information into the DTC.
                    *(uint32_t*)&DTCCurrent->DTCStoredDataAddress[arrayIndex] = charon_NvmDriver_getNvmAddress_for_StoredData(number);
                    DTCCurrent->DTCStoredDataLength[arrayIndex] = DTCUpdate.DTCStoredDataLength[0];       
                    DTCCurrent->DTCStoredDataRecordNumber[arrayIndex] = number;
                    DTCCurrent->NumberOfSavedDataRecords++;

                    // Filling all the new information into the storedData.
                    *(uint32_t*)&storedDataCurrent = charon_NvmDriver_getNvmAddress_for_StoredData(number);
                    storedDataCurrent->DTCStoredDataRecordNumberOfIdentifiers = storedDataUpdate.DTCStoredDataRecordNumberOfIdentifiers;
                    for (uint32_t i = 0; i < (uint16_t)DTCUpdate.DTCStoredDataLength[0]; i++)
                    {
                        storedDataCurrent->DTCStoredDataPayload[i] = storedDataUpdate.DTCStoredDataPayload[i];
                    }
                    
                    // True if a new slot is used, false if a previous deleted one is used.
                    if ((uint16_t)DTC_header->totalDataRecordCounter == (uint16_t)DTC_header->currentDataRecordCounter)
                    {
                        DTC_header->totalDataRecordCounter++;
                        DTC_header->currentDataRecordCounter++;
                    }
                    else
                    {
                        DTC_header->currentDataRecordCounter++;
                    }

                    doubleBreak = true;
                    break;
                }
                else
                {
                    o = 8u;
                }
            }
            if (doubleBreak)
            {
                break;
            }
        }
    }
}

static void charon_StoredDataTransmissionFunctionalUnit_writeExtendedDataToNvm (DTC_ExtendedData_t payload, DTC_t DTCup, DTC_t* address)
{
    // IF check before even entering this function. (payload not empty!)
    DTC_t DTCUpdate = DTCup;
    DTC_t* DTCCurrent = (DTC_t*)address;
    DTC_ExtendedData_t extendedDataUpdate = payload;
    DTC_ExtendedData_t* extendedDataCurrent = NULL;
    DTC_header_t* DTC_header = (DTC_header_t*)charon_NvmDriver_getNvmAddress_for_DTC(0,true);

    bool foundEmptyIndex = false;
    bool doubleBreak = false;
    uint16_t number;
    uint8_t arrayIndex;


    // Getting the next empty index.
    for (uint32_t i = 0; i < NVM_AMOUNT_OF_EXTENDED; i++)
    {
        if (DTCCurrent->DTCExtendedDataAddress[i] == 0x00)
        {
            arrayIndex = i;
            foundEmptyIndex = true;
            break;
        }
    }
    

    if (foundEmptyIndex)
    {
        // Search the lookup table for the next empty slot, runs 8x the same index if needed. 
        for (uint32_t b = 0; b < ((uint16_t)DTC_header->sizeOfExtDataLookupTable); b++)
        { 
            for (uint32_t o = 0; o < BYTE_SIZE; o++)
            {
                if (DTC_header->nvmExtDataLookupTable[b] != 0xFF)
                {
                    // Split the Byte to get less comparisons. 
                    number = charon_DTC_LookupTable_getBitNumber(DTC_header->nvmExtDataLookupTable[b]);
                    // Updating the lookup table.
                    DTC_header->nvmExtDataLookupTable[b] |= (0x01 << number);

                    // Building Storage slot number.
                    number = ((b * 8u)+ number);

                    // Filling all the new information into the DTC.
                    *(uint32_t*)&DTCCurrent->DTCExtendedDataAddress[arrayIndex] = charon_NvmDriver_getNvmAddress_for_ExtendedData(number);
                    DTCCurrent->DTCExtendedDataLength[arrayIndex] = DTCUpdate.DTCExtendedDataLength[0];       
                    DTCCurrent->DTCExtDataRecordNumber[arrayIndex] = number;
                    DTCCurrent->NumberOfSavedExtendedData++;

                    // Filling all the new information into the extDataRecord.
                    *(uint32_t*)&extendedDataCurrent = charon_NvmDriver_getNvmAddress_for_ExtendedData(number);
                    extendedDataCurrent->DTCExtendedDataRecordNumberOfIdentifiers = extendedDataUpdate.DTCExtendedDataRecordNumberOfIdentifiers;
                    for (uint32_t i = 0; i < (uint16_t)DTCUpdate.DTCExtendedDataLength[0]; i++)
                    {
                        extendedDataCurrent->DTCExtendedDataPayload[i] = extendedDataUpdate.DTCExtendedDataPayload[i];
                    }
                    
                    // True if a new slot is used, false if a previous deleted one is used.
                    if ((uint16_t)DTC_header->totalExtDataRecordCounter == (uint16_t)DTC_header->currentExtDataRecordCounter)
                    {
                        DTC_header->totalExtDataRecordCounter++;
                        DTC_header->currentExtDataRecordCounter++;
                    }
                    else
                    {
                        DTC_header->currentExtDataRecordCounter++;
                    }

                    doubleBreak = true;
                    break;
                }
                else
                {
                    o = 8u;
                }
            }
            if (doubleBreak)
            {
                break;
            }
        }
    }
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
