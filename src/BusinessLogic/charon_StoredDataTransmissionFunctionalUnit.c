/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022
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
#include "charon_types.h"
#include "charon_SessionAndSerivceControl.h"
#include "charon_StoredDataTransmissionFunctionalUnit.h"
#include "Charon_DTC_LookupTable.h"
#include "charon_negativeResponse.h"
#include "Common/charon_types.h"
#include "charon_interface_NvmDriver.h"

/* Imports *******************************************************************/

/* Macros ********************************************************************/

/** Max Transmission Massage buffer size  */
#define MAX_TX_BUFFER_SIZE              ((uint32_t) 1024u)

/** safe area in Nvm */
#define STORAGE_HEADER ((uint8_t) 16u) 

/* Types *********************************************************************/

/* Variables *****************************************************************/
static uint32_t DTCcounter = 0;

/* Private Function Definitions **********************************************/

uds_responseCode_t NumberOfDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t DTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t DTCSnapshotIdentification (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t DTCSnapshotRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t DTCStoredDataByRecordNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t DTCExtDataRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t NumberOfDTCBySeverityMaskRecord (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t DTCBySeverityMaskRecord (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t SeverityInformationOfDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t SupportedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t FirstTestFailedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t FirstConfirmedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t MostRecentTestFailedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t MostRecentConfirmedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t MirrorMemoryDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t MirrorMemoryDTCExtDataRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t NumberOfMirrorMemoryDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t NumberOfEmissionsOBDDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t EmissionsOBDDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t DTCFaultDetectionCounter (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t DTCWithPermanentStatus (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t DTCExtDataRecordByRecordNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t UserDefMemoryDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t UserDefMemoryDTCSnapshotRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

uds_responseCode_t UserDefMemoryDTCExtDataRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/* Interfaces  ***************************************************************/

/* Private Function **********************************************************/

uds_responseCode_t charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{

    DTCSubfunction_t subfunction =0u;


    /* Build Data Identifier */
    subfunction = receiveBuffer [1];
  
    switch (subfunction)
    {
    case reportNumberOfDTCByStatusMask:
    { 
        return NumberOfDTCByStatusMask(receiveBuffer, receiveBufferSize);
        break;
    }

    case reportDTCByStatusMask:   
    { 
        return DTCByStatusMask(receiveBuffer, receiveBufferSize);
        break;
    }  

    case reportDTCSnapshotIdentification:    
    { 
        return DTCSnapshotIdentification(receiveBuffer, receiveBufferSize);
        break;
    } 

    case reportDTCSnapshotRecordByDTCNumber:   
    { 
        return DTCSnapshotRecordByDTCNumber(receiveBuffer, receiveBufferSize);
        break;
    } 

    case reportDTCStoredDataByRecordNumber:  
    { 
        return DTCStoredDataByRecordNumber(receiveBuffer, receiveBufferSize);
        break;
    } 

    case reportDTCExtDataRecordByDTCNumber:    
    { 
        return DTCExtDataRecordByDTCNumber(receiveBuffer, receiveBufferSize);
        break;
    }    

    case reportNumberOfDTCBySeverityMaskRecord:  
    { 
        return NumberOfDTCBySeverityMaskRecord(receiveBuffer, receiveBufferSize);
        break;
    }   

    case reportDTCBySeverityMaskRecord:     
    { 
        return DTCBySeverityMaskRecord(receiveBuffer, receiveBufferSize);
        break;
    }    

    case reportSeverityInformationOfDTC:  
    { 
        return SeverityInformationOfDTC(receiveBuffer, receiveBufferSize);
        break;
    }  

    case reportSupportedDTC:       
    { 
        return SupportedDTC(receiveBuffer, receiveBufferSize);
        break;
    }     

    case reportFirstTestFailedDTC:   
    { 
        return FirstTestFailedDTC(receiveBuffer, receiveBufferSize);
        break;
    }     

    case reportFirstConfirmedDTC:
    { 
        return FirstConfirmedDTC(receiveBuffer, receiveBufferSize);
        break;
    }

    case reportMostRecentTestFailedDTC:
    { 
        return MostRecentTestFailedDTC(receiveBuffer, receiveBufferSize);
        break;
    }   

    case reportMostRecentConfirmedDTC:      
    { 
        return MostRecentConfirmedDTC(receiveBuffer, receiveBufferSize);
        break;
    }   

    case reportMirrorMemoryDTCByStatusMask:    
    { 
        return MirrorMemoryDTCByStatusMask(receiveBuffer, receiveBufferSize);
        break;
    }     

    case reportMirrorMemoryDTCExtDataRecordByDTCNumber:     
    { 
        return MirrorMemoryDTCExtDataRecordByDTCNumber(receiveBuffer, receiveBufferSize);
        break;
    }  

    case reportNumberOfMirrorMemoryDTCByStatusMask:    
    { 
        return NumberOfMirrorMemoryDTCByStatusMask(receiveBuffer, receiveBufferSize);
        break;
    }  

    case reportNumberOfEmissionsOBDDTCByStatusMask:  
    { 
        return NumberOfEmissionsOBDDTCByStatusMask(receiveBuffer, receiveBufferSize);
        break;
    }   

    case reportEmissionsOBDDTCByStatusMask:  
    { 
        return EmissionsOBDDTCByStatusMask(receiveBuffer, receiveBufferSize);
        break;
    }     

    case reportDTCFaultDetectionCounter:    
    { 
        return DTCFaultDetectionCounter(receiveBuffer, receiveBufferSize);
        break;
    }     

    case reportDTCWithPermanentStatus:  
    { 
        return DTCWithPermanentStatus(receiveBuffer, receiveBufferSize);
        break;
    }      

    case reportDTCExtDataRecordByRecordNumber:   
    { 
        return DTCExtDataRecordByRecordNumber(receiveBuffer, receiveBufferSize);
        break;
    }       
         
    case reportUserDefMemoryDTCByStatusMask:   
    { 
        return UserDefMemoryDTCByStatusMask(receiveBuffer, receiveBufferSize);
        break;
    }      

    case reportUserDefMemoryDTCSnapshotRecordByDTCNumber:   
    { 
        return UserDefMemoryDTCSnapshotRecordByDTCNumber(receiveBuffer, receiveBufferSize);
        break;
    }  

    case reportUserDefMemoryDTCExtDataRecordByDTCNumber:
    { 
        return UserDefMemoryDTCExtDataRecordByDTCNumber(receiveBuffer, receiveBufferSize);
        break;
    }

    default:
  
        // Subfunction not supported.
        charon_sendNegativeResponse(uds_responseCode_SubfunctionNotSupported, uds_sid_ReadDtcInformation);

        return uds_responseCode_SubfunctionNotSupported;
        break;
    }
}


uds_responseCode_t NumberOfDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint8_t StatusMask = receiveBuffer[2];
    uint16_t countOfMatchedDTC = charon_getDTCCountByStatusMask(StatusMask); 
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    /* length of the buffer is alway 6 bytes. due to consistent response*/
    uint32_t length = 6u; 

    if(countOfMatchedDTC == 0u)
    {
        // No DTC with matching mask found.
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadDtcInformation);

        return uds_responseCode_RequestOutOfRange;
    }    
    else
    {
        s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
        s_buffer[1] = reportNumberOfDTCByStatusMask;
        s_buffer[2] = charon_getDTCStatusAvailabilityMask();
        s_buffer[3] = DTC_Format_Identifier;
        s_buffer[4] = countOfMatchedDTC;

        charon_sscTxMessage(s_buffer,length);
        //charon info einfügen für debuging!
        return uds_responseCode_PositiveResponse;
    }
}

uds_responseCode_t DTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint8_t StatusMask = receiveBuffer[2];
    DTC_t *matchedDTC = (DTC_t*)charon_NvmDriver_getNvmAddress();
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t length = 3u;
    uint16_t countOfMatchedDTC = charon_getDTCCountByStatusMask(StatusMask);
    uint8_t lengthOfDTC = 3u;


    s_buffer[0] = (uds_sid_ReadDtcInformation | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = reportDTCByStatusMask;
    s_buffer[2] = charon_getDTCStatusAvailabilityMask();


    for(uint32_t i = 0u; ((i < countOfMatchedDTC) && (matchedDTC != NULL)); i++)
    {
        if((length + 4u) > MAX_TX_BUFFER_SIZE)
        {
            // Too many DTC found, 
            charon_sendNegativeResponse(uds_responseCode_ResponseTooLong, uds_sid_ReadDtcInformation);

            return uds_responseCode_ResponseTooLong;
        }

        matchedDTC = charon_getDTCLookupByStatusMask(StatusMask,(i+1));
        if(matchedDTC != NULL)
        {
            memcpy(&s_buffer[length], &matchedDTC[0].DTCHighByte , lengthOfDTC);
            length += lengthOfDTC;
            memcpy(&s_buffer[length], &matchedDTC[0].DTCStatusMask, 1u);
            length ++;
        }
    }

    if(length > 3u)
    {
        charon_sscTxMessage(s_buffer,length);
        //charon info einfügen für debuging!
        return uds_responseCode_PositiveResponse;
    }
    else
    {
        // No DTC with matching mask found.
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadDtcInformation);

        return uds_responseCode_RequestOutOfRange;
    }

}


uds_responseCode_t DTCSnapshotIdentification (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint8_t StatusMask = receiveBuffer[2];
    DTC_t *matchedDTC = (DTC_t*)charon_NvmDriver_getNvmAddress();
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    
    uint16_t countOfMatchedDTC = charon_getDTCCountByStatusMask(StatusMask);


    return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t DTCSnapshotRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t DTCStoredDataByRecordNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t DTCExtDataRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t NumberOfDTCBySeverityMaskRecord (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t DTCBySeverityMaskRecord (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t SeverityInformationOfDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t SupportedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t FirstTestFailedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t FirstConfirmedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t MostRecentTestFailedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t MostRecentConfirmedDTC (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t MirrorMemoryDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t MirrorMemoryDTCExtDataRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t NumberOfMirrorMemoryDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t NumberOfEmissionsOBDDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t EmissionsOBDDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t DTCFaultDetectionCounter (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t DTCWithPermanentStatus (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t DTCExtDataRecordByRecordNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t UserDefMemoryDTCByStatusMask (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t UserDefMemoryDTCSnapshotRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}

uds_responseCode_t UserDefMemoryDTCExtDataRecordByDTCNumber (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
return uds_responseCode_SubfunctionNotSupported;
}


uds_responseCode_t charon_StoredDataTransmissionFunctionalUnit_writeDTCto_Nvm (uint32_t DTCnumber, uint8_t StatusMask, uint8_t HighByte, uint8_t MiddleByte, uint8_t LowByte, uint8_t SnapshotRecorNumber, uint8_t StoredDataRecordNumber, uint8_t ExtDataRecordNumber, uint8_t SeverityMask, uint8_t FunctionalGroupidentifier)
{
    uint8_t data[sizeof(DTC_t)];
    uint8_t length = sizeof(DTC_t);
    uint32_t position;
    DTC_t DTC;


    // DTCnumber cant be allowed to generate a new DTC
    if (DTCnumber > DTCcounter)
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadDtcInformation);
        return uds_responseCode_GeneralReject;
    }
    
    // DTCnumber can be used to change a DTC
    if (DTCnumber != 0)
    {
        position = DTCnumber;
        position--;
    }
    else
    {
        position = DTCcounter;
        DTCcounter++;
    }


    // fill DTC type
    DTC.DTCStatusMask                   = StatusMask;
    DTC.DTCHighByte                     = HighByte;
    DTC.DTCMiddleByte                   = MiddleByte;
    DTC.DTCLowByte                      = LowByte;
    DTC.DTCSnapshotRecorNumber          = SnapshotRecorNumber;
    DTC.DTCStoredDataRecordNumber       = StoredDataRecordNumber;
    DTC.DTCExtDataRecordNumber          = ExtDataRecordNumber;
    DTC.DTCSeverityMask                 = SeverityMask;
    DTC.DTCSeverityMaskRecordHigh       = DTC.DTCSeverityMask;
    DTC.DTCSeverityMaskRecordLow        = DTC.DTCStatusMask;
    DTC.FunctionalGroupidentifier       = FunctionalGroupidentifier;
    DTC.MemorySelection                 = charon_NvmDriver_getAddress(position);


    // cast DTC typ to uint8_t array
    memcpy(data,&DTC,length);
    
    if (charon_NvmDriver_write(((position*length) + STORAGE_HEADER), data, length) == uds_responseCode_PositiveResponse)
    {
        return uds_responseCode_PositiveResponse;
    }
    else
    {
        return uds_responseCode_GeneralReject;
    }
}

uint32_t charon_StoredDataTransmissionFunctionalUnit_getDTCcount (void)
{
    return (uint32_t) DTCcounter;
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
