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
    uint16_t AmountOfDIDs = ((receiveBufferSize -1u) /2u);
    uint8_t counter =0u;
    charon_dataIdentifierObject_t* DidLookupData;
    charon_dataIdentifierObject_t* DidArray[AmountOfDIDs];
    uint32_t SessionCheck;
    static uint8_t s_buffer[MAX_TX_BUFFER_SIZE];


    if((receiveBufferSize > MAX_RCV_LENGTH)
        || (receiveBufferSize < MIN_RCV_LENGTH))
    {
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    else
    {
        for(uint16_t i=0; i < AmountOfDIDs; i ++ )
        {
            /* Build Data Identifier */
            dataIdentifier = *(uint16_t*) &receiveBuffer [i * 2u + 1u];
            dataIdentifier = __rev16(dataIdentifier);
            
            DidLookupData = charon_getDidLookupTable(dataIdentifier);
            SessionCheck = DidLookupData->sessionMask & ((uint32_t)(1u << charon_sscGetSession()));

            /* check if the actual DID is allowed in the active session */
            if(SessionCheck != 0)    
            {
                /** @Todo Implement security check 
                check if DID security check ok?
                return uds_responseCode_SecurityAccessDenied;
                */   
                DidArray[counter] = dataIdentifier;
                counter ++;
            }             
        }

        if(counter != 0u)
        {
            /* the 1 is for the SID number */
            uint32_t length = 1u;
            /** @todo check if needed to add to Response code enum */
            s_buffer[0] = 0x62u;

            for(uint16_t i=0; i < counter; i ++ )
            {
                /* check length here, befor writing to the buffer to protect an unintended writing anywhere */
                if(MAX_TX_BUFFER_SIZE < (length + (DidArray[i]->lengthOfData)))
                {
                    return uds_responseCode_ResponseTooLong;
                }
                memcpy(&s_buffer[length], DidArray[i]->DID, 2u);
                memcpy(&s_buffer[length+2u], DidArray[i]->AddressOfData, DidArray[i]->lengthOfData);
                /* the + 2 is for the DID number itself, because it is always the same length */
                length +=  (DidArray[i]->lengthOfData + 2u);
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


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
