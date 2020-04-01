/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2020 Andreas Hofmann
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
 * @addtogroup ComLogic
 * @{
 * @file charon_SessionAndSerivceControl.c
 * Implementation of the Service and Session Control Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "Common/charon_negativeResponse.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** Size in Bytes an uint8_t array needs to be */
#define NRC_ARRAY_SIZE      ((uint32_t) 3u)

/* Types *********************************************************************/

/* Variables *****************************************************************/

/** Stores the Currently Active Diagnostic Session */
static charon_sessionTypes_t s_currentDiagnosticSession = charon_sscType_default;

/* Private Function Definitions **********************************************/

/**
 *
 * @param currentSession
 *      Currently Active Diagnose Session
 * @param pService
 *      Pointer to Service Object
 *
 * @return Service is allowed or disallowed in Session
 */
bool isServiceInSession (charon_sessionTypes_t currentSession, charon_serviceObject_t * pService);

/* Interfaces  ***************************************************************/

int32_t charon_sscRcvProcessMessage (uint8_t * const pBuffer, uint32_t length)
{
    /**
     * FEATURE
     * Here needs to be evaluated if an encryption is active and the message needs translation.
     */

    uint8_t sid = (uint8_t)(((uint8_t)pBuffer[0]) & 0x7Fu);       /* Get SID from Message */
    int32_t retVal = 0;
    charon_serviceObject_t * pServiceObj = charon_ServiceLookupTable_getServiceObject(sid);   /* Get Service Object */

    /* Check if Service is supported */
    if(NULL != pServiceObj)
    {
        /* Check if Service is Supported in Current Session */
        if(isServiceInSession(s_currentDiagnosticSession, pServiceObj))
        {
            /* Execute Service */
            retVal = pServiceObj->serviceRunable(pBuffer, length);
        }
        else
        {
            charon_sendNegativeResponse(uds_responseCode_SubfunctionNotSupportedInActiveSession, sid);
            retVal = -1;
        }
    }
    else
    {
        charon_sendNegativeResponse(uds_responseCode_ServiceNotSupported, sid);
        retVal = -1;
    }


    return retVal;
}

void charon_sscTxProcessMessage (uint8_t * const pBuffer, uint32_t length)
{

}

int32_t charon_sscSetSession (charon_sessionTypes_t sessionType, uint32_t timeout)
{
    /**
     * FEATURE
     * Align timeouts and check them
     * Momentary check for unused/set unused (MISRA)
     */
    uint32_t unusedTimeout = timeout;
    (void)unusedTimeout;

    s_currentDiagnosticSession = sessionType;

    return 0;
}

charon_sessionTypes_t charon_sscGetSession (void)
{
    return s_currentDiagnosticSession;
}

/* Private Function **********************************************************/

bool isServiceInSession (charon_sessionTypes_t currentSession, charon_serviceObject_t * pService)
{
    uint32_t result = 0u;
    bool retval = false;

    result = (pService->sessionMask & ((uint8_t)(1u << currentSession)));
    if(result > 0u)
    {
        retval = true;
    }

    return retval;
}


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
