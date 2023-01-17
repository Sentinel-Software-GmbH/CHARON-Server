/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Schöner
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
 * @file charon_DataTransmissionFunctionalUnit.c
 * Implementation of the Data Transmission Functional Unit Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "charon_uds.h"
#include "charon_types.h"
#include "charon_interface_debug.h"
#include "charon_interface_clock.h"
#include "charon_DataLookupTable.h" 
#include "charon_negativeResponse.h"
#include "charon_interface_NvmDriver.h"
#include "charon_SessionAndServiceControl.h"
#include "charon_DataTransmissionFunctionalUnit.h"


/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** Max allowed DID per Request */
#define MAX_PARALLEL_REQUESTED_DID      ((uint32_t) 5u)
/** Max Transmission Massage buffer size  */
#define MAX_TX_BUFFER_SIZE              ((uint32_t) 1024u)
/** Max Bytes used for memory Size parameter*/
#define MAX_MEMORY_SIZE_BYTES           ((uint32_t) 4u)
/** Max Bytes used for memory Address parameter*/
#define MAX_MEMORY_ADDRESS_BYTES        ((uint32_t) 4u)
/** the Max concurrent Periodic DID to send */ 
#define MAX_CONCURRENT_PDID             ((uint32_t) 4u)
/** the amount of cycles to send the Slow Periodic DID */
#define SLOW_PERIODIC_RATE              ((uint32_t) 480000u)
/** the amount of cycles to send the Medium Periodic DID */
#define MEDIUM_PERIODIC_RATE            ((uint32_t) 240000u)
/** the amount of cycles to send the Fast Periodic DID */
#define FAST_PERIODIC_RATE              ((uint32_t) 100000u)


/* Types *********************************************************************/

/* Variables *****************************************************************/

/** Static that keeps up with all periodic data to send. */
static uint32_t s_periodicQueueCounter = 0u;

/** Static array that holds the data for the periodic data to send. @ref PeriodicData_t */
static PeriodicData_t s_periodicArray[MAX_CONCURRENT_PDID];

/** Static that holds time between periodic massages. (perhaps delete later)*/
static uint32_t s_periodicTimestamp = 0u;


/* Private Function Definitions **********************************************/

/**
 * @brief Subfunction of the Dynamically Defined Data Identifier that defines a DID by one or more Source Data Identifiers.
 * 
 * @param receiveBuffer payload
 * @param receiveBufferSize payload size
 * @return @see @ref uds_responseCode_t 
 */
static uds_responseCode_t dynamicallyDefinedDataIdentifierDefineByIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Subfunction of the Dynamically Defined Data Identifier that defines a DID by one or more MemoryAddresses.
 * 
 * @param receiveBuffer payload 
 * @param receiveBufferSize payload size
 * @return @see @ref uds_responseCode_t 
 */
static uds_responseCode_t dynamicallyDefinedDataIdentifierDefineByMemoryAddress (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Subfunction of the Dynamically Defined Data Identifier that clears a dynamicly defined Data Identifier.
 * 
 * @param receiveBuffer payload 
 * @param receiveBufferSize payload size
 * @return @see @ref uds_responseCode_t 
 */
static uds_responseCode_t dynamicallyDefinedDataIdentifierClearDynamicalDefinedDID (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/**
 * @brief Resets the Counter value for the Periodic Data identifier to be in schedular. 
 * 
 * @param Timing the periodic transmission mode (Slow, medium or Fast)
 * @return uint32_t returns the counter value of the specific timing aka Transmission mode 
 */
static uint32_t resetTransmitCounter (transmissionMode_t  timing);

/**
 * @brief Checks if the schedular for the periodic DID has a free slot, and if the new added periodic DID can fit.
 * 
 * @param numberOfpDID the amount of Periodic Data Identifier that the client wants to add to the periodic scheduled massages
 * @return true 
 * @return false 
 */
static bool isSchedularFree(uint32_t numberOfpDID);

/**
 * @brief Checks if the tranmission mode (periodic frequency) requested from the client is supported by the server.
 * 
 * @param TransmissionMode the Transmission mode can be Slow, medium or Fast periodic rate.
 * @return true 
 * @return false 
 */
static bool isTransmissionModeSupported (transmissionMode_t transmissionMode);

/**
 * @brief Check if the amount of Bytes for the Memory address and the memory size is tolerated.
 * 
 * @param memorySize  bytes for the size of the memory
 * @param memoryAddress bytes for the address
 * @return true 
 * @return false 
 */
static bool requestInRange (uint8_t memorySize, uint8_t memoryAddress);

/**
 * @brief Checks if the received buffer size makes sense for the request of the client.
 * 
 * @param minLength the min length of the received massage for the request
 * @param maxLength if no max length is specified by the ISO max length is =0, otherwise the max length of the received massage for the request
 * @param bufferSize the size of the received massage
 * @return true 
 * @return false 
 */
static bool lengthAndFormatCheck(uint32_t minLength, uint32_t maxLength, uint32_t bufferSize);

/**
 * @brief Checks if the the Data Identifier is allowed in the current session.
 * 
 * @param DID a pointer to the Data lookup table to the defined Data Identifier
 * @return true 
 * @return false 
 */
static bool securityCheckOkay(charon_dataIdentifierObject_t* DID);

/**
 * @brief Delete single DynamicallyDefinedDataIdentifier. (DDDDI)
 * 
 * @param highByte  of requested DDDDI
 * @param lowByte   of requested DDDDI
 */
static void deleteDynamicallyDefinedDataIdentifier (uint8_t highByte, uint8_t lowByte);


/* Interfaces  ***************************************************************/

void charon_DataTransmissionFunctionalUnit_Init (void)
{
    for(uint32_t i =0u; i< MAX_CONCURRENT_PDID; i++)
    {
        s_periodicArray[i].DID = 0u;
        s_periodicArray[i].timing = 0u;
        s_periodicArray[i].transmitCounter = 0u;
        s_periodicArray[i].isEntryFree = true;
    }
}

uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint16_t dataIdentifier;
    /* Amount of Data Identifiers (-1u for the SID not Counting) (/2u for a Data Identifier is 2 bytes) */
    uint16_t amountOfDIDs = ((receiveBufferSize -1u) /2u);
    charon_dataIdentifierObject_t* didLookupData;
    charon_dataIdentifierObject_t* didArray[amountOfDIDs];
    uint32_t lengthOfDID = 2u;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    /** Min Length is 3 Byte by ISO */
    uint32_t minLengthOfDid = 3u;
    /** Max Length of requested DIDs (SID 1 Byte + 2 Byte id per DID) */
    uint32_t maxLengthOfDid = (1u + (MAX_PARALLEL_REQUESTED_DID * 2u));
    
    /* length check for min 3 and not over the maximum amount of parallel requested DID. */
    if(!lengthAndFormatCheck(minLengthOfDid, maxLengthOfDid, receiveBufferSize))
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,uds_sid_ReadDataByIdentifier);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    
    uint8_t counter =0u;

    for(uint16_t i=0; i < amountOfDIDs; i ++ )
    {
        /* Build Data Identifier */
        dataIdentifier = *(uint16_t*) &receiveBuffer [i * lengthOfDID + 1u];
        #if !CHARON_CONFIG_IS_BIG_ENDIAN
        dataIdentifier = REV16(dataIdentifier);
        #endif
        didLookupData = charon_getDataLookupTableByDID (dataIdentifier);
        if(didLookupData != NULL)
        {
            /* check if the actual DID is allowed in the active session */ 
            if(securityCheckOkay(didLookupData))
            {
                didArray[counter] = didLookupData;
                counter ++;
            }
        }
    }

    if(counter != 0u)
    {
        uint32_t length = 1u;       /* the 1 is for the SID number */
        s_buffer[0] = (uds_sid_ReadDataByIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
        for(uint16_t i=0; i < counter; i ++ )
        {
            /* check length here, befor writing to the buffer to not disturb the dragons */
            if(MAX_TX_BUFFER_SIZE < (length + (didArray[i]->lengthOfData))) 
            {
                return uds_responseCode_ResponseTooLong;
            }

            #if !CHARON_CONFIG_IS_BIG_ENDIAN
            dataIdentifier = REV16(didArray[i]->DID);
            #endif
            memcpy(&s_buffer[length], &dataIdentifier, lengthOfDID);
            charon_NvmDriver_read(didArray[i]->addressOfData, &s_buffer[length + lengthOfDID], didArray[i]->lengthOfData);
            length +=  (didArray[i]->lengthOfData + lengthOfDID);
        }
        charon_sscTxMessage(s_buffer,length);
        return uds_responseCode_PositiveResponse;          
    }
    else
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadDataByIdentifier);
        return uds_responseCode_RequestOutOfRange;
    }
    
}

uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    /** Min Length is 4 Byte by ISO (SID 1 Byte + AddressAndLengthFormatIdentifier 1 Byte + At least 1 Byte for Memory Size + At least 1 Byte for Memory Address)*/
    uint32_t minRCVLengthOfRMBA = 4u; 
    /** Max Length of requested ReadMemoryByAddress (SID 1 Byte + 1 Byte AddressAndLengthFormatIdentifier + Max Bytes for memory Size + Max bytes for Memory Address) */
    uint32_t maxRCVLengthOfRMBA = (1u + 1u + MAX_MEMORY_SIZE_BYTES + MAX_MEMORY_ADDRESS_BYTES);
    uint8_t addressAndLengthFormatID = *(uint8_t*) &receiveBuffer[1];
    uint8_t bytesForMemorySize = ((addressAndLengthFormatID & 0xF0) >> 4u);
    uint8_t bytesForMemoryAddress = (addressAndLengthFormatID & 0x0F);
    uint32_t dataAddress;
    uint32_t dataLength = 0u; 
    uint8_t startOfAddress = 2u;
    charon_dataIdentifierObject_t* dataLookupData;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];

    if((receiveBufferSize <= minRCVLengthOfRMBA)
        || (receiveBufferSize >= maxRCVLengthOfRMBA))
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_ReadMemoryByAddress);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    if(!requestInRange(bytesForMemorySize, bytesForMemoryAddress))
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadMemoryByAddress);
        return uds_responseCode_RequestOutOfRange;
    }

    /* build Data address and data Length*/
    memcpy(&dataAddress, &receiveBuffer[startOfAddress], bytesForMemoryAddress);
    memcpy(&dataLength, &receiveBuffer[startOfAddress + bytesForMemoryAddress], bytesForMemorySize);

    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dataAddress = REV32(dataAddress);
    dataAddress = dataAddress >> ((MAX_MEMORY_ADDRESS_BYTES - bytesForMemoryAddress) * 8u);

    dataLength = REV32(dataLength);
    dataLength = dataLength >> ((MAX_MEMORY_SIZE_BYTES - bytesForMemorySize) * 8u);
    #endif

    dataLookupData = charon_getDataLookupTableByAddress (dataAddress);
    if(dataLookupData == NULL)
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadMemoryByAddress);
        return uds_responseCode_RequestOutOfRange;
    }

    if(dataLength > dataLookupData->lengthOfData)
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_ReadMemoryByAddress);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    if(!securityCheckOkay(dataLookupData))
    {
        charon_sendNegativeResponse(uds_responseCode_SecurityAccessDenied, uds_sid_ReadMemoryByAddress);
        return uds_responseCode_SecurityAccessDenied;
    }

    uint32_t length = 1u;
    s_buffer[0] = (uds_sid_ReadMemoryByAddress | (uint8_t)uds_sid_PositiveResponseMask);
    charon_NvmDriver_read(dataAddress, &s_buffer[1], dataLength);
    length += dataLength;
    charon_sscTxMessage(s_buffer,length);
    return uds_responseCode_PositiveResponse;
}

uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint16_t dataIdentifier;
    charon_dataIdentifierObject_t* didLookupData;
    /** Min Length is 3 Byte by ISO */
    uint32_t minLengthOfDid = 3u;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];


    if(!lengthAndFormatCheck(minLengthOfDid, 0u, receiveBufferSize))
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_ReadScalingDataByIdentifier);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    /* Build Data Identifier */
    dataIdentifier = *(uint16_t*) &receiveBuffer [1];
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dataIdentifier = REV16(dataIdentifier);
    #endif
    didLookupData = charon_getDataLookupTableByDID (dataIdentifier);

    if(didLookupData == NULL)
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadScalingDataByIdentifier);
        return uds_responseCode_RequestOutOfRange;
    }

    /* Scaling information available? */
    if(didLookupData->hasScalingData == false)
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadScalingDataByIdentifier);
        return uds_responseCode_RequestOutOfRange;
    }

    if(!securityCheckOkay(didLookupData))
    {
        charon_sendNegativeResponse(uds_responseCode_SecurityAccessDenied, uds_sid_ReadScalingDataByIdentifier);
        return uds_responseCode_SecurityAccessDenied;
    }

    uint32_t length = 3u;
    s_buffer[0] = (uds_sid_ReadScalingDataByIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dataIdentifier = REV16(dataIdentifier);
    #endif
    memcpy(&s_buffer[1],&dataIdentifier,2);
    charon_NvmDriver_read(didLookupData->addressOfScalingData, &s_buffer[3], didLookupData->lengthOfScalingData);
    charon_sscTxMessage(s_buffer,(length + didLookupData->lengthOfScalingData));
    return uds_responseCode_PositiveResponse;
}

uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint32_t minLength = 2u;
    uint32_t maxLength = (2u + MAX_CONCURRENT_PDID);
    uint32_t numberOfpDID = (receiveBufferSize -2u);
    transmissionMode_t transmissionMode = receiveBuffer[1];
    uint16_t dataIdentifier;
    uint8_t pDIDHighByte = 0xF2;
    charon_dataIdentifierObject_t* didLookupData;
    uint32_t counter = 0u;
    uint32_t timeChangeCounter = 0u;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint8_t pDIDArray[MAX_CONCURRENT_PDID];

    if(!lengthAndFormatCheck(minLength, maxLength, receiveBufferSize))    
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_ReadDataByPeriodicIdentifier);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    if(!isTransmissionModeSupported(transmissionMode) || (s_periodicQueueCounter > MAX_CONCURRENT_PDID))
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadDataByPeriodicIdentifier);
        return uds_responseCode_RequestOutOfRange;
    }

    if(transmissionMode == stopSending) 
    {
        if(numberOfpDID > 0u)
        {
            for(uint32_t i = 0u; i < MAX_CONCURRENT_PDID; i++)
            {
                for(uint32_t y = 0u; y < numberOfpDID; y++)
                {
                    if(s_periodicArray[i].DID == receiveBuffer [y + 2u])
                    {
                        s_periodicArray[i].DID = 0x00;
                        s_periodicArray[i].isEntryFree = true;
                        if(s_periodicQueueCounter > 0u)
                        {
                            s_periodicQueueCounter--;
                        }
                    }
                }
            }
        }
        else
        {
            for(uint32_t i = 0u; i < MAX_CONCURRENT_PDID; i++)
            {
                s_periodicArray[i].DID = 0x00;
                s_periodicArray[i].isEntryFree = true;
            }
            s_periodicQueueCounter = 0u;
        }
        /*send positive response */
        s_buffer[0] = (uds_sid_ReadDataByPeriodicIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
        charon_sscTxMessage(s_buffer,1u);
        return uds_responseCode_PositiveResponse;
    }
    else
    {
        for(uint32_t i = 0u; i < numberOfpDID; i++)
        {
            dataIdentifier = pDIDHighByte << 8u | receiveBuffer[i + 2u];

            didLookupData = charon_getDataLookupTableByDID (dataIdentifier);
            if(didLookupData != NULL)
            {
                /* check if the actual DID is allowed in the active session */ 
                if(securityCheckOkay(didLookupData))    
                {
                    pDIDArray[counter] = receiveBuffer[i + 2u];
                    counter ++;
                }
                /* check if some of the pDID´s is already running*/
                for(uint32_t y = 0u; y < MAX_CONCURRENT_PDID; y++)
                {
                    if(s_periodicArray[y].DID == pDIDArray[counter - 1u])
                    {
                        timeChangeCounter ++;
                    }
                }
            }
        }

        /* schedular free to store all supported pDID´s requested */ 
        if((s_periodicQueueCounter + counter - timeChangeCounter) > MAX_CONCURRENT_PDID)
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadDataByIdentifier);
            return uds_responseCode_RequestOutOfRange;
        }

        if(counter != 0u)
        {
            uint32_t EntryCounter = 0;
            for(uint32_t i = 0u; i < MAX_CONCURRENT_PDID; i++ )
            {
                if(s_periodicArray[i].DID == pDIDArray[EntryCounter])
                {
                    s_periodicArray[i].DID = pDIDArray[EntryCounter];
                    s_periodicArray[i].timing = transmissionMode;
                    s_periodicArray[i].transmitCounter = resetTransmitCounter(s_periodicArray[i].timing);
                    s_periodicArray[i].isEntryFree = false;
    	            EntryCounter++;
                }  
                else if(s_periodicArray[i].isEntryFree)
                {
                    s_periodicArray[i].DID = pDIDArray[EntryCounter];
                    s_periodicArray[i].timing = transmissionMode;
                    s_periodicArray[i].transmitCounter = resetTransmitCounter(s_periodicArray[i].timing);
                    s_periodicArray[i].isEntryFree = false;
    	            EntryCounter++;
                }    

                if(EntryCounter >= counter)
                {
                    break;
                }
            }
            s_periodicQueueCounter += (counter - timeChangeCounter);
            /*send positive response */
            s_buffer[0] = (uds_sid_ReadDataByPeriodicIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
            charon_sscTxMessage(s_buffer,1u);
            return uds_responseCode_PositiveResponse;
        }
        else /* not a single pDID supported in current session*/
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_ReadDataByIdentifier);
            return uds_responseCode_RequestOutOfRange;
        }
    }
}

uds_responseCode_t charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    definitionMode_t DefMode = receiveBuffer[1];
    uds_responseCode_t  RetVal;

    switch (DefMode)
    {
    case defineByIdentifier:
    {    
        RetVal = dynamicallyDefinedDataIdentifierDefineByIdentifier(receiveBuffer,receiveBufferSize);
        break;
    }
    case defineByMemoryAddress:
    {    
        RetVal = dynamicallyDefinedDataIdentifierDefineByMemoryAddress(receiveBuffer,receiveBufferSize);
        break;
    }
    case clearDynamicalDefinedDID:
    {    
        RetVal = dynamicallyDefinedDataIdentifierClearDynamicalDefinedDID(receiveBuffer,receiveBufferSize);
        break;
    }
    default:
    {
        charon_sendNegativeResponse(uds_responseCode_SubfunctionNotSupported, uds_sid_DynamicallyDefineDataIdentifier);
        RetVal = uds_responseCode_SubfunctionNotSupported;
        break;
    }
    }/*end of Switch*/
    return RetVal;
}

uds_responseCode_t charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint32_t minLengthOfBuffer = 4u;  /*Min receive length of this function is 4 byte due to ISO*/
    uint16_t dataIdentifier;
    uint8_t minLength = 4u; 
    uint16_t sizeOfData = receiveBufferSize - 3u;
    uint16_t sanityCounter = 0u;
    charon_dataIdentifierObject_t* didLookupData;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint8_t couldDataBeWritten[MAX_TX_BUFFER_SIZE] = {0}; // no random value for check

    if(!lengthAndFormatCheck(minLength, 0, receiveBufferSize))  
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_ReadDataByPeriodicIdentifier);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    /* Build Data Identifier */
    dataIdentifier = *(uint16_t*) &receiveBuffer [1];
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dataIdentifier = REV16(dataIdentifier);
    #endif
    didLookupData = charon_getDataLookupTableByDID (dataIdentifier);

    if(didLookupData == NULL)
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_WriteDataByIdentifier);
        return uds_responseCode_RequestOutOfRange;
    }

    if(!lengthAndFormatCheck(minLengthOfBuffer, (didLookupData->lengthOfData + 3u), receiveBufferSize))    
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_WriteDataByIdentifier);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    if(!securityCheckOkay(didLookupData))
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_WriteDataByIdentifier);
        return uds_responseCode_RequestOutOfRange;
    }

    if(didLookupData->readOnlyDID == true)
    {
        charon_sendNegativeResponse(uds_responseCode_ConditionsNotCorrect, uds_sid_WriteDataByIdentifier);
        return uds_responseCode_ConditionsNotCorrect;
    }

    charon_NvmDriver_write(didLookupData->addressOfData,&receiveBuffer[3],(sizeOfData));

    // Check if data could be written
    charon_NvmDriver_read(didLookupData->addressOfData,&couldDataBeWritten[0],(sizeOfData));
    for (size_t i = 0; i < sizeOfData; i++)
    {
        if (couldDataBeWritten[i] == receiveBuffer[i + 3])
        {
            sanityCounter++;
        }
    }
    if (sanityCounter != sizeOfData)
    {
        charon_sendNegativeResponse(uds_responseCode_GeneralProgrammingFailure, uds_sid_WriteDataByIdentifier);
        return uds_responseCode_GeneralProgrammingFailure;
    }
    
    uint32_t length = 3u;
    s_buffer[0] = (uds_sid_WriteDataByIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dataIdentifier = REV16(dataIdentifier);
    #endif
    memcpy(&s_buffer[1],&dataIdentifier,2);   /** @todo uint32_t could roll over when 0xFFFFFFFF is put in and will result in some sanity failures (happened in unit test while checking did length), may need a check here for max sizes or in later functions.  */ 
    charon_sscTxMessage(s_buffer,length);
    return uds_responseCode_PositiveResponse;
}

uds_responseCode_t charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    /** Min Length is 4 Byte by ISO (SID 1 Byte + AddressAndLengthFormatIdentifier 1 Byte + At least 1 Byte for Memory Size + At least 1 Byte for Memory Address + At least 1 Byte for Data)*/
    uint32_t minRCVLengthOfWMBA = 5u; 
    uint8_t addressAndLengthFormatID = *(uint8_t*) &receiveBuffer[1];
    uint8_t bytesForMemorySize = ((addressAndLengthFormatID & 0xF0) >> 4u);
    uint8_t bytesForMemoryAddress = (addressAndLengthFormatID & 0x0F);
    uint32_t dataAddress;
    uint32_t dataLength = 0u; 
    uint8_t startOfData = (1u + 1u + bytesForMemoryAddress + bytesForMemorySize);
    uint8_t startOfAddress = 2u;
    charon_dataIdentifierObject_t* dataLookupData;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];

    if(receiveBufferSize < minRCVLengthOfWMBA)
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_WriteMemoryByAddress);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    if(!requestInRange(bytesForMemorySize, bytesForMemoryAddress))
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_WriteMemoryByAddress);
        return uds_responseCode_RequestOutOfRange;
    }

    /* build Data address and data Length*/
    memcpy(&dataAddress, &receiveBuffer[startOfAddress], bytesForMemoryAddress);
    memcpy(&dataLength, &receiveBuffer[startOfAddress + bytesForMemoryAddress], bytesForMemorySize);

    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dataAddress = REV32(dataAddress);
    dataAddress = dataAddress >> ((MAX_MEMORY_ADDRESS_BYTES - bytesForMemoryAddress) * 8u);/** @todo make this a function*/

    dataLength = REV32(dataLength);
    dataLength = dataLength >> ((MAX_MEMORY_SIZE_BYTES - bytesForMemorySize) * 8u);
    #endif

    dataLookupData = charon_getDataLookupTableByAddress (dataAddress);
    if(dataLookupData == NULL)
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_WriteMemoryByAddress);
        return uds_responseCode_RequestOutOfRange;
    }

    if(dataLength > dataLookupData->lengthOfData)
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_WriteMemoryByAddress);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    if(!securityCheckOkay(dataLookupData))
    {
        charon_sendNegativeResponse(uds_responseCode_SecurityAccessDenied, uds_sid_WriteMemoryByAddress);
        return uds_responseCode_SecurityAccessDenied;
    }

    if(dataLookupData->readOnlyDID == true)
    {
        charon_sendNegativeResponse(uds_responseCode_ConditionsNotCorrect, uds_sid_WriteMemoryByAddress);
        return uds_responseCode_ConditionsNotCorrect;
    }

    charon_NvmDriver_write(dataAddress, &receiveBuffer[startOfData], (receiveBufferSize - startOfData));

    uint32_t length = 2u;
    s_buffer[0] = (uds_sid_WriteMemoryByAddress | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = addressAndLengthFormatID;
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dataAddress = REV32(dataAddress);
    dataAddress = dataAddress >> ((MAX_MEMORY_ADDRESS_BYTES - bytesForMemoryAddress) * 8u);
    #endif
    memcpy(&s_buffer[2],&dataAddress,bytesForMemoryAddress);
    length += bytesForMemoryAddress;
    memcpy(&s_buffer[length],&dataLength, bytesForMemorySize);
    length += bytesForMemorySize;
    charon_sscTxMessage(s_buffer,length);
    return uds_responseCode_PositiveResponse;
}

void charon_DataTransmissionFunctionalUnit_SendPeriodic (void)
{
    if(s_periodicQueueCounter > 0u)
    {
        bool sendThisTurn = false;
        static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
        uint32_t length = 0u;
        uint16_t dataIdentifier;
        uint8_t pDIDHighByte = 0xF2;
        charon_dataIdentifierObject_t* didLookupData;

        for(uint32_t i = 0u; i < MAX_CONCURRENT_PDID; i++)
        {
            if(s_periodicArray[i].isEntryFree == false)
            {
                if((s_periodicArray[i].transmitCounter == 0u) && !sendThisTurn)
                {
                    s_buffer[0] = s_periodicArray[i].DID;
                    dataIdentifier = pDIDHighByte << 8u | s_periodicArray[i].DID;
                    s_periodicArray[i].transmitCounter = resetTransmitCounter(s_periodicArray[i].timing);

                    didLookupData = charon_getDataLookupTableByDID (dataIdentifier);
                    charon_NvmDriver_read(didLookupData->addressOfData, &s_buffer[1], didLookupData->lengthOfData);
                    length = didLookupData->lengthOfData + 1u;
                    
                    sendThisTurn = true;
                }
                else
                {
                    if(0u != s_periodicArray[i].transmitCounter)
                    {
                        s_periodicArray[i].transmitCounter --;
                    }
                }
            }
        }
        if(length > 0u)
        {
            CHARON_INFO("time %x", charon_interface_clock_getTimeElapsed(s_periodicTimestamp));
            charon_sscTxMessage(s_buffer,length);
            s_periodicTimestamp = charon_interface_clock_getTime();
        }
    }
}

#ifdef TEST
periodicRateGetter_t charon_DataTransmissionFunctionalUnit_getDefinedPeriodic (void)
{
    periodicRateGetter_t rates;

    rates.slow_rate = SLOW_PERIODIC_RATE;
    rates.medium_rate = MEDIUM_PERIODIC_RATE;
    rates.fast_rate = FAST_PERIODIC_RATE;

    return rates;
}
#endif


/* Private Function **********************************************************/

uds_responseCode_t dynamicallyDefinedDataIdentifierDefineByIdentifier (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    definitionMode_t DefMode = receiveBuffer[1];
    uint16_t dynamicDataIdentifier;
    uint16_t dataIdentifier;
    charon_dataIdentifierObject_t* didLookupData;
    uint16_t minLength = 8u; /* from ISO 14229-1, (SID + SubFunc + DDDID + Source DID + PosInSource + MemSize)=(1+1+2+2+1+1)Bytes */
    uint8_t sourceDIDOffset = 4u;
    uint8_t amountOfSourceDID = ((receiveBufferSize - sourceDIDOffset) / 4);
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint16_t lengthOfBuffer = 0u;
    uint16_t lengthOfAlreadyWrittenDID = 0u;
    uint8_t PosInSource;
    uint8_t memSize;

    if(!lengthAndFormatCheck(minLength, 0u, receiveBufferSize))
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_DynamicallyDefineDataIdentifier);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    /* Build Dynamic Data Identifier */
    dynamicDataIdentifier = *(uint16_t*) &receiveBuffer[2];
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dynamicDataIdentifier = REV16(dynamicDataIdentifier);
    #endif
    didLookupData = charon_getDataLookupTableByDID (dynamicDataIdentifier);
    if(didLookupData == NULL)
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_DynamicallyDefineDataIdentifier);
        return uds_responseCode_RequestOutOfRange;
    }
    /** check if data is already in DID */
    lengthOfAlreadyWrittenDID = didLookupData->lengthOfData;

    for(uint16_t i = 0u; i < amountOfSourceDID; i++)
    {
        dataIdentifier = *(uint16_t*) &receiveBuffer[((i + 1u) * sourceDIDOffset)];
        #if !CHARON_CONFIG_IS_BIG_ENDIAN
        dataIdentifier = REV16(dataIdentifier);
        #endif
        didLookupData = charon_getDataLookupTableByDID (dataIdentifier);
        if(didLookupData == NULL)
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_DynamicallyDefineDataIdentifier);
            return uds_responseCode_RequestOutOfRange;
        }
        PosInSource = receiveBuffer[(((i + 1u) * sourceDIDOffset)+2u)];     
        memSize = receiveBuffer[(((i + 1u) * sourceDIDOffset)+3u)]; 
        if(((lengthOfAlreadyWrittenDID + lengthOfBuffer + memSize) > MAX_VARIABLE_DATA_LENGTH) /** @todo ask andy if better as getter function*/
            || (PosInSource == 0u) 
                || (memSize == 0u))
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_DynamicallyDefineDataIdentifier);
            return uds_responseCode_RequestOutOfRange;
        }
        /*build the buffer*/
        charon_NvmDriver_read(didLookupData->addressOfData + (PosInSource - 1u), &s_buffer[lengthOfBuffer], memSize);
        lengthOfBuffer += memSize; 
    }/* end of for loop */

    didLookupData = charon_getDataLookupTableByDID (dynamicDataIdentifier);
    didLookupData->lengthOfData = lengthOfBuffer + lengthOfAlreadyWrittenDID;
    didLookupData->isDynamicallyDID = true;
    charon_NvmDriver_write((didLookupData->addressOfData + lengthOfAlreadyWrittenDID),&s_buffer[0],lengthOfBuffer);

    uint8_t length = 4u;
    s_buffer[0] = (uds_sid_DynamicallyDefineDataIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = DefMode;
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dynamicDataIdentifier = REV16(dynamicDataIdentifier);
    #endif
    memcpy(&s_buffer[2],&dynamicDataIdentifier,2);
    charon_sscTxMessage(s_buffer,length);
    return uds_responseCode_PositiveResponse;
}

uds_responseCode_t dynamicallyDefinedDataIdentifierDefineByMemoryAddress (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    definitionMode_t DefMode = receiveBuffer[1];
    uint16_t dynamicDataIdentifier;
    charon_dataIdentifierObject_t* didLookupData;
    uint16_t lengthOfBuffer = 0u;
    uint8_t addressAndLengthFormatID = *(uint8_t*) &receiveBuffer[4];
    uint8_t bytesForMemorySize = ((addressAndLengthFormatID & 0xF0) >> 4u);
    uint8_t bytesForMemoryAddress = (addressAndLengthFormatID & 0x0F);
    uint32_t dataAddress = 0u;
    uint32_t dataSize = 0u;
    uint32_t addressOffset = 5u;
    uint32_t LengthOffset = addressOffset + bytesForMemoryAddress;
    uint32_t multiplierOffset = bytesForMemoryAddress + bytesForMemorySize;
    uint32_t amountOfAddresses = ((receiveBufferSize - addressOffset) / multiplierOffset);
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];
    uint32_t minLength = 7u; /* from ISO 14229-1, (SID + SubFunc + DDDID + ALFID + Adress + MemSize)=(1+1+2+1+1+1)Bytes */
    uint16_t lengthOfAlreadyWrittenDID = 0u;

    if(!lengthAndFormatCheck(minLength,0u,receiveBufferSize))
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_DynamicallyDefineDataIdentifier);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }

    dynamicDataIdentifier = *(uint16_t*) &receiveBuffer[2];
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dynamicDataIdentifier = REV16(dynamicDataIdentifier);
    #endif
    didLookupData = charon_getDataLookupTableByDID (dynamicDataIdentifier);
    if((didLookupData == NULL) || (!requestInRange(bytesForMemorySize,bytesForMemoryAddress)))
    {
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_DynamicallyDefineDataIdentifier);
        return uds_responseCode_RequestOutOfRange;
    }
    /** check if data is already in DID */
    lengthOfAlreadyWrittenDID = didLookupData->lengthOfData;

    for(uint32_t i = 0u; i < amountOfAddresses; i++)
    {
        /* build Data address */
        memcpy(&dataAddress, &receiveBuffer[(i * multiplierOffset) + addressOffset], bytesForMemoryAddress);
        memcpy(&dataSize, &(receiveBuffer[(i * multiplierOffset) + LengthOffset]), bytesForMemorySize);
         #if !CHARON_CONFIG_IS_BIG_ENDIAN
        dataAddress = REV32(dataAddress);
        dataAddress = dataAddress >> ((MAX_MEMORY_ADDRESS_BYTES - bytesForMemoryAddress) * 8u);/** @todo make this a function*/

        dataSize = REV32(dataSize);
        dataSize = dataSize >> ((MAX_MEMORY_SIZE_BYTES - bytesForMemorySize) * 8u);
        #endif
        if((lengthOfAlreadyWrittenDID + lengthOfBuffer + dataSize) > MAX_VARIABLE_DATA_LENGTH)
        {
            charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_DynamicallyDefineDataIdentifier);
            return uds_responseCode_RequestOutOfRange;
        }

        /*build the buffer*/ /** @todo check if there is a possible to check if address is correct */
        charon_NvmDriver_read(dataAddress, &s_buffer[lengthOfBuffer], dataSize);
        lengthOfBuffer += dataSize; 
    }
    
    didLookupData = charon_getDataLookupTableByDID (dynamicDataIdentifier);
    didLookupData->lengthOfData = lengthOfBuffer + lengthOfAlreadyWrittenDID;
    didLookupData->isDynamicallyDID = true;

    charon_NvmDriver_write((didLookupData->addressOfData + lengthOfAlreadyWrittenDID),&s_buffer[0],lengthOfBuffer);

    uint8_t length = 4u;
    s_buffer[0] = (uds_sid_DynamicallyDefineDataIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
    s_buffer[1] = DefMode;
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dynamicDataIdentifier = REV16(dynamicDataIdentifier);
    #endif
    memcpy(&s_buffer[2],&dynamicDataIdentifier,2);
    charon_sscTxMessage(s_buffer,length);
    return uds_responseCode_PositiveResponse;
}

uds_responseCode_t dynamicallyDefinedDataIdentifierClearDynamicalDefinedDID (const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    definitionMode_t DefMode = receiveBuffer[1];
    uint16_t dynamicDataIdentifier;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];

    if ((receiveBufferSize == 2))      // for all dynamic did deletion
    {
        for (uint8_t i = 0; i < 0xFF; i++)
        {
            deleteDynamicallyDefinedDataIdentifier(0xF2,i);     // F2 range for periodicDataIdentifier (DDDDi may be saved here)
            deleteDynamicallyDefinedDataIdentifier(0xF3,i);     // F3 range for DynamicallyDefinedDataIdentifier
        }
        uint8_t length = 2u;
        s_buffer[0] = (uds_sid_DynamicallyDefineDataIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
        s_buffer[1] = DefMode;
        charon_sscTxMessage(s_buffer,length);
        return uds_responseCode_PositiveResponse;
    }
    else if (receiveBufferSize == 4)   // for single dynamic did deletion
    {
        deleteDynamicallyDefinedDataIdentifier(receiveBuffer[2],receiveBuffer[3]);

        uint8_t length = 4u;
        s_buffer[0] = (uds_sid_DynamicallyDefineDataIdentifier | (uint8_t)uds_sid_PositiveResponseMask);
        s_buffer[1] = DefMode;

        dynamicDataIdentifier = *(uint16_t*) &receiveBuffer[2];
        #if !CHARON_CONFIG_IS_BIG_ENDIAN
        dynamicDataIdentifier = REV16(dynamicDataIdentifier);
        #endif
        memcpy(&s_buffer[2],&dynamicDataIdentifier,2);

        charon_sscTxMessage(s_buffer,length);
        return uds_responseCode_PositiveResponse;
    }
    else
    {
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_DynamicallyDefineDataIdentifier);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
}

uint32_t resetTransmitCounter (transmissionMode_t  timing)
{
    uint32_t retval = 0u;

    if(timing == sendSlowRate )
    {
        retval = SLOW_PERIODIC_RATE;
    }
    else if(timing == sendMediumRate)
    {
        retval = MEDIUM_PERIODIC_RATE;
    }    
    else if(timing == sendFastRate)
    {
        retval = FAST_PERIODIC_RATE;
    }

    return retval;
}

bool isSchedularFree(uint32_t numberOfpDID)
{
    uint32_t SchedulerRequest = (s_periodicQueueCounter + numberOfpDID);
    if(MAX_CONCURRENT_PDID <= SchedulerRequest)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isTransmissionModeSupported (transmissionMode_t transmissionMode)
{
    bool retval = false;
    
    if(transmissionMode == sendSlowRate)
    {
        retval = true;  
    }
    if(transmissionMode == sendMediumRate)
    {
        retval = true;  
    }
    if(transmissionMode == sendFastRate)
    {
        retval = true;  
    }
    if(transmissionMode == stopSending)
    {
        retval = true;  
    }

    return retval;
}

bool requestInRange (uint8_t memorySize, uint8_t memoryAddress)
{
    bool SizeInRange = false;

    if((memorySize != 0u)
        && (memorySize <= MAX_MEMORY_SIZE_BYTES)
            && (memoryAddress != 0u)
                &&  (memoryAddress <= MAX_MEMORY_ADDRESS_BYTES))
    {
        SizeInRange = true;
    }
    return SizeInRange;
}

bool lengthAndFormatCheck(uint32_t minLength, uint32_t maxLength, uint32_t bufferSize)
{
    bool retval = false;

    if(maxLength != 0u)
    {
        if((bufferSize >= minLength)
            && (bufferSize <= maxLength))
        {
            retval = true;
        }
    }
    else
    {
        if(bufferSize >= minLength)
        {
            retval = true;
        }
    }
    return retval;
}

bool securityCheckOkay(charon_dataIdentifierObject_t* DID)
{
    bool retval = false;

    retval = ((DID->sessionMask & ((uint32_t)(1u << charon_sscGetSession()))) != 0u);             //debugCheck
    return retval;
}

void deleteDynamicallyDefinedDataIdentifier (uint8_t highByte, uint8_t lowByte)
{
    uint16_t dynamicDataIdentifier;
    charon_dataIdentifierObject_t* didLookupData;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];

    dynamicDataIdentifier = ((highByte << 8) | (lowByte));
    #if !CHARON_CONFIG_IS_BIG_ENDIAN
    dynamicDataIdentifier = REV16(dynamicDataIdentifier);
    #endif

    didLookupData = charon_getDataLookupTableByDID (dynamicDataIdentifier);
    if ((didLookupData != NULL) && (didLookupData->isDynamicallyDID) && (!(didLookupData->readOnlyDID)))
    {
        for(uint32_t i = 0u; i< didLookupData->lengthOfData; i++)
        {
            s_buffer[i] = 0u;
        }
        /** @todo check if necessary */
        charon_NvmDriver_write(didLookupData->addressOfData,&s_buffer[0],didLookupData->lengthOfData);
        didLookupData->lengthOfData = 0u;
    }
}


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
