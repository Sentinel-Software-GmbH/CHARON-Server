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
#include "CHARON_DATATRANSMISSIONFUNCTIONALUNIT.h"
#include "charon_DataIdentifier.h" 


/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** Max allowed DID per Request */
#define MAX_PARALLEL_REQUESTED_DID      ((uint32_t) 5u)
/** Min Length is 3 Byte by ISO */
#define MIN_RCV_LENGTH                  ((uint32_t) 3u)
/** Max Length of requested DIDs (SID 1 Byte + 2 Byte id per DID) */
#define MAX_RCV_LENGTH                  ((uint32_t) 1u + (MAX_PARALLEL_REQUESTED_DID * 2u))
/** Max Transmission Massage buffer size  */
#define MAX_TX_BUFFER_SIZE              ((uint32_t) 1024u)

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

/* Private Function **********************************************************/

uds_responseCode_t charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    uint16_t dataIdentifier ;
    /* Amount of Data Identifiers (-1u for the SID not Counting) (/2u for a Data Identifier is 2 bytes) */
    uint16_t amountOfDIDs = ((receiveBufferSize -1u) /2u);
    uint8_t counter =0u;
    charon_dataIdentifierObject_t* didLookupData;
    charon_dataIdentifierObject_t* didArray[amountOfDIDs];
    uint32_t sessionCheck;
    uint8_t lengthOfDID = 2u;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];


    if((receiveBufferSize > MAX_RCV_LENGTH)
        || (receiveBufferSize < MIN_RCV_LENGTH))
    {
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else
    {
        for(uint16_t i=0; i < amountOfDIDs; i ++ )
        {
            /* Build Data Identifier */
            dataIdentifier = *(uint16_t*) &receiveBuffer [i * lengthOfDID + 1u];
            dataIdentifier = __rev16(dataIdentifier);
            
            didLookupData = charon_getDidLookupTable(dataIdentifier);
            sessionCheck = didLookupData->sessionMask & ((uint32_t)(1u << charon_sscGetSession()));

            /* check if the actual DID is allowed in the active session */
            if(sessionCheck != 0)    
            {
                /** @Todo Implement security check 
                check if DID security check ok?
                return uds_responseCode_SecurityAccessDenied;
                */   
                didArray[counter] = dataIdentifier;
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
                memcpy(&s_buffer[length], didArray[i]->DID, lengthOfDID);
                memcpy(&s_buffer[length+lengthOfDID], didArray[i]->AddressOfData, didArray[i]->lengthOfData);
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
    
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
