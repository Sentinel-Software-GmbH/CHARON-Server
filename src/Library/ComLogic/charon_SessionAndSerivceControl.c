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
#include "ComLogic/charon_ServiceLookupTable.h"
#include "Common/charon_negativeResponse.h"
#include "HSDI/charon_interface_clock.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

typedef enum ComStatus_t_private
{
    ComStatus_ok,
    ComStatus_pending,
    ComStatus_process,
    ComStatus_error,

    ComStatus_amount
} ComStatus_t;

/* Variables *****************************************************************/

/** Stores the Currently Active Diagnostic Session */
static charon_sessionTypes_t s_currentDiagnosticSession = charon_sscType_default;
/** Current Status of Communications (Handle Pending) */
static ComStatus_t s_currentComStatus = ComStatus_ok;
/** Store SID of Pending Request */
static uds_sid_t s_pendingRequestId = uds_sid_PositiveResponseMask;
/** Timestamp for Pending Iterations (P2 and P2*) */
static uint32_t s_pendingRequestTimestamp = 0u;


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
    uds_responseCode_t retVal = uds_responseCode_PositiveResponse;
    charon_serviceObject_t * pServiceObj = charon_ServiceLookupTable_getServiceObject(sid);   /* Get Service Object */

    /* Is a Service Pending, do not execute any other Requests */
    if(ComStatus_ok == s_currentComStatus)
    {
        /* Lock this if scope for the moment to prevent user misuse to float the receive channel */
        s_currentComStatus = ComStatus_process;
        /* Check if Service is supported */
        if(NULL != pServiceObj)
        {
            /* Check if Service is Supported in Current Session */
            if(isServiceInSession(s_currentDiagnosticSession, pServiceObj))
            {
                /* Execute Service */
                retVal = pServiceObj->serviceRunable(pBuffer, length);
                /* Check for Pending Service */
                if(uds_responseCode_RequestCorrectlyReceived_ResponsePending == retVal)
                {
                    /* Mark Status Pending and store all releveant Data to Process Asynchronous Handling */
                    s_currentComStatus = ComStatus_pending;
                    s_pendingRequestTimestamp = charon_interface_clock_getTime();
                    s_pendingRequestId = sid;
                }
                else
                {
                    s_currentComStatus = ComStatus_ok;
                }
            }
            else
            {
                /* Send NRC and Reset COM Status */
                charon_sendNegativeResponse(uds_responseCode_SubfunctionNotSupportedInActiveSession, sid);
                s_currentComStatus = ComStatus_ok;
                retVal = -1;
            }
        }
        else
        {
            charon_sendNegativeResponse(uds_responseCode_ServiceNotSupported, sid);
            s_currentComStatus = ComStatus_ok;
            retVal = -1;
        }
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
