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
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "charon_uds.h"
#include "charon_SessionAndServiceControl.h"
#include "CHARON_DATATRANSMISSIONFUNCTIONALUNIT.h"
#include "charon_DataLookupTable.h" 


/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** Max allowed DID per Request */
#define MAX_PARALLEL_REQUESTED_DID      ((uint32_t) 5u)
/** Min Length is 3 Byte by ISO */
#define MIN_RCV_LENGTH_OF_DID           ((uint32_t) 3u)
/** Max Length of requested DIDs (SID 1 Byte + 2 Byte id per DID) */
#define MAX_RCV_LENGTH_OF_DID           ((uint32_t) 1u + (MAX_PARALLEL_REQUESTED_DID * 2u))
/** Max Transmission Massage buffer size  */
#define MAX_TX_BUFFER_SIZE              ((uint32_t) 1024u)

/** Max Bytes used for memory Size parameter*/
#define MAX_MEMORY_SIZE_RMBA            ((uint32_t) 4u)
/** Max Bytes used for memory Address parameter*/
#define MAX_MEMORY_ADDRESS_RMBA         ((uint32_t) 4u)
/** Min Length is 4 Byte by ISO (SID 1 Byte + AddressAndLengthFormatIdentifier 1 Byte + At least 1 Byte for Memory Size + At least 1 Byte for Memory Address)*/
#define MIN_RCV_LENGTH_OF_RMBA          ((uint32_t) 4u)
/** Max Length of requested ReadMemoryByAddress (SID 1 Byte + 1 Byte AddressAndLengthFormatIdentifier + Max Bytes for memory Size + Max bytes for Memory Address) */
#define MAX_RCV_LENGTH_OF_RMBA          ((uint32_t) 1u + 1u + MAX_MEMORY_SIZE_RMBA + MAX_MEMORY_ADDRESS_RMBA)

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

bool requestInRange (uint8_t memorySize, uint8_t memoryAddress);

/* Interfaces  ***************************************************************/

/* Private Function **********************************************************/

uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint16_t dataIdentifier;
    /* Amount of Data Identifiers (-1u for the SID not Counting) (/2u for a Data Identifier is 2 bytes) */
    uint16_t amountOfDIDs = ((receiveBufferSize -1u) /2u);
    charon_dataIdentifierObject_t* didLookupData;
    charon_dataIdentifierObject_t* didArray[amountOfDIDs];
    uint32_t sessionCheck;
    uint8_t lengthOfDID = 2u;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];


    if((receiveBufferSize > MAX_RCV_LENGTH_OF_DID)
        || (receiveBufferSize < MIN_RCV_LENGTH_OF_DID))
    {
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else
    {
        uint8_t counter =0u;
        
        for(uint16_t i=0; i < amountOfDIDs; i ++ )
        {
            /* Build Data Identifier */
            dataIdentifier = *(uint16_t*) &receiveBuffer [i * lengthOfDID + 1u];
            #if !CHARON_CONFIG_IS_BIG_ENDIAN
            dataIdentifier = __rev16(dataIdentifier);
            #endif
            didLookupData = charon_getDataLookupTableByDID (dataIdentifier);
            sessionCheck = didLookupData->sessionMask & ((uint32_t)(1u << charon_sscGetSession()));

            /* check if the actual DID is allowed in the active session */
            if(sessionCheck != 0)    
            {
                /** @Todo Implement security check 
                check if DID security check ok?
                return uds_responseCode_SecurityAccessDenied;
                */   
                didArray[counter]->DID = dataIdentifier;
                counter ++;
            }             
        }

        if(counter != 0u)
        {
            /* the 1 is for the SID number */
            uint32_t length = 1u;
            s_buffer[0] = (uds_sid_ReadDataByIdentifier | (uint8_t)uds_sid_PositiveResponseMask);

            for(uint16_t i=0; i < counter; i ++ )
            {
                /* check length here, befor writing to the buffer to not disturb the dragons */
                if(MAX_TX_BUFFER_SIZE < (length + (didArray[i]->lengthOfData)))
                {
                    return uds_responseCode_ResponseTooLong;
                }
                memcpy(&s_buffer[length], &didArray[i]->DID, lengthOfDID);
                memcpy(&s_buffer[length + lengthOfDID], &didArray[i]->AddressOfData, didArray[i]->lengthOfData);
                length +=  (didArray[i]->lengthOfData + lengthOfDID);
            }

            charon_sscTxMessage(s_buffer,length);
            //charon info einfügen für debuging!
            return uds_responseCode_PositiveResponse;          
        }
        else
        {
            return uds_responseCode_RequestOutOfRange;
        }
    }
    
}

uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress (uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint8_t addressAndLengthFormatID = *(uint8_t*) &receiveBuffer[1];
    uint8_t bytesForMemorySize = ((addressAndLengthFormatID & 0xF0) >> 4u);
    uint8_t bytesForMemoryAddress = (addressAndLengthFormatID & 0x0F);
    uint32_t dataAddress;
    uint32_t datalength = 0u; 
    uint8_t startOfAddress = 3u;
    charon_dataIdentifierObject_t* dataLookupData;
    uint32_t sessionCheck;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];

    if((receiveBufferSize < MIN_RCV_LENGTH_OF_RMBA)
        || (receiveBufferSize > MAX_RCV_LENGTH_OF_RMBA))
    {
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    if(!requestInRange(bytesForMemorySize, bytesForMemoryAddress))
    {
        return uds_responseCode_RequestOutOfRange;
    }

    /* build Data address and data Length*/
    memcpy(&dataAddress, &receiveBuffer[startOfAddress], bytesForMemoryAddress);
    memcpy(&datalength, &receiveBuffer[startOfAddress + bytesForMemoryAddress], bytesForMemorySize);
    
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dataAddress = __rev32(dataAddress);
    datalength = __rev32(datalength);
    #endif

    dataLookupData = charon_getDataLookupTableByAddress (dataAddress);

    if(datalength > MAX_TX_BUFFER_SIZE)
    {
        return uds_responseCode_RequestOutOfRange;
    }

    sessionCheck = dataLookupData->sessionMask & ((uint32_t)(1u << charon_sscGetSession()));
    if(sessionCheck == 0u)
    {
        return uds_responseCode_RequestOutOfRange;
    }

    /** @todo check security when implemented */

    s_buffer[0] = (uds_sid_ReadMemoryByAddress | (uint8_t)uds_sid_PositiveResponseMask);
    memcpy(&s_buffer[1], &dataAddress,datalength);
    //charon info einfügen für debuging!
    return uds_responseCode_PositiveResponse;

}

uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint16_t dataIdentifier;
    charon_dataIdentifierObject_t* didLookupData;
    uint32_t sessionCheck;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];


    if(receiveBufferSize == MIN_RCV_LENGTH_OF_DID)    
    {
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else
    {
        /* Build Data Identifier */
        dataIdentifier = *(uint16_t*) &receiveBuffer [2];
        #if !CHARON_CONFIG_IS_BIG_ENDIAN
        dataIdentifier = __rev16(dataIdentifier);
        #endif
        didLookupData = charon_getDataLookupTableByDID (dataIdentifier);
        sessionCheck = didLookupData->sessionMask & ((uint32_t)(1u << charon_sscGetSession()));

        /* Scaling information available */
        if((didLookupData->HasScalingData != false) 
            || (sessionCheck != 0u))
        {
            /* the 1 is for the SID number */
            uint32_t length = 3u;
            s_buffer[0] = (uds_sid_ReadScalingDataByIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
            s_buffer[1] = dataIdentifier;
            memcpy(&s_buffer[3],&didLookupData->AddressOfScalingData,didLookupData->lengthOfScalingData);

            charon_sscTxMessage(s_buffer,(length + didLookupData->lengthOfScalingData));
            //charon info einfügen für debuging!
            return uds_responseCode_PositiveResponse;
        }
        else
        {
            return uds_responseCode_RequestOutOfRange;
        }
    }
}

bool requestInRange (uint8_t memorySize, uint8_t memoryAddress)
{
    bool SizeInRange = false;

    if((memorySize != 0u)
        && (memorySize > MAX_MEMORY_SIZE_RMBA)
            && (memoryAddress != 0u)
                &&  (memoryAddress > MAX_MEMORY_ADDRESS_RMBA))
    {
        SizeInRange = true;
    }
    

    return SizeInRange;
}
/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
