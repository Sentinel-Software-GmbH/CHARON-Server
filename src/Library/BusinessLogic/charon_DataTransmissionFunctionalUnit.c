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

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** Max allowed DID per Request */
#define MAX_PARALLEL_REQUESTED_DID      ((uint32_t) 5u)
/** Min Length is 3 Byte by ISO */
#define MIN_RCV_LENGTH                  ((uint32_t) 3u)
/** Max Length of requested DIDs (SID 1 Byte + 2 Byte id per DID) */
#define MAX_RCV_LENGTH                  ((uint32_t) 1u + (MAX_PARALLEL_REQUESTED_DID * 2u))

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
            dataIdentifier = ((receiveBuffer[i * 2u + 1u] <<8) | (receiveBuffer[i * 2u + 2u] ));

            //dataIdentifier = ((receiveBuffer[i * 2u + 1u]) << (receiveBuffer[i * 2u + 2u] ));
            
            /** @Todo Implement session support 
            check if DID is supported in active session
            charon_sscGetSession() ;
            if(DID allowed in session)
            {
                counter ++;
            }
            */

            /** @Todo Implement security check 
            check if DID security check ok?
            return uds_responseCode_SecurityAccessDenied;
            */

               
        }
        /* check if at least one DID is allowed in active session */
        if(counter!= 0u)
        {
            
            //charon_sscTxMessage();
        }
        else
        {
            return uds_responseCode_RequestOutOfRange;
        }

    }
    
}


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
