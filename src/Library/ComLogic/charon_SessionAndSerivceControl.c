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
#include <string.h>
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "ComLogic/charon_ServiceLookupTable.h"
#include "Common/charon_negativeResponse.h"
#include "HSDI/charon_interface_clock.h"
#include "HSDI/charon_interface_canisotp.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** Size of the UDS Receive Buffer in byte */
#define CHARON_RCV_BUFFER_SIZE      ((uint32_t) 4096u)
/** Size of the UDS Transmit Buffer in byte */
#define CHARON_TX_BUFFER_SIZE      ((uint32_t) 4096u)

/**
 * Default Timing Parameter for the UDS Server Communication and Session Control
 * @{
 * */
#define DEFAULT_P2_SERVER            ((uint32_t) 100u)       /**< Default P2 Server Time (see ISO Requirments) */
#define DEFAULT_P2_STAR_SERVER       ((uint32_t) 300u)       /**< Default P2* Server Time (see ISO Requirments) */
#define DEFAULT_S3_SERVER            ((uint32_t) 5000u)      /**< Default S3 Server Time (see ISO Requirments) */
/**
 * @}
 */

/* Types *********************************************************************/

typedef enum ComStatus_t_private
{
    ComStatus_ok,     //!< ComStatus_ok
    ComStatus_pending,//!< ComStatus_pending
    ComStatus_process,//!< ComStatus_process
    ComStatus_error,  //!< ComStatus_error

    ComStatus_amount  //!< ComStatus_amount
} ComStatus_t;

/** Struct to store all timeing required Data */
typedef struct ComTimeoutLimits_t_private
{
    //TODO: there are many more, but for now...
    uint32_t        p2Server;
    uint32_t        p2StarServer;
    uint32_t        s3Server;
}ComTimeoutLimits_t;

/* Variables *****************************************************************/

/** Stores the Currently Active Diagnostic Session */
static charon_sessionTypes_t s_currentDiagnosticSession = charon_sscType_default;
/** Current Status of Communications (Handle Pending) */
static ComStatus_t s_currentComStatus = ComStatus_ok;
/** Store SID of Pending Request */
static uds_sid_t s_pendingRequestId = uds_sid_PositiveResponseMask;
/** Timestamp for Pending Start */
static uint32_t s_pendingRequestStartTime = 0u;
/** Flag to indicate if the P2 Expired Message for Pending was already handled */
static bool s_p2PendingExceededHandled = false;
/** Timestamp for Diagnostic Session Timing */
static uint32_t s_diagnoticSessionTimestamp = 0u;
/** Adjustable timeouts, initialized witth serrver default values */
static ComTimeoutLimits_t s_ttl =
{
        DEFAULT_P2_SERVER, DEFAULT_P2_STAR_SERVER, DEFAULT_S3_SERVER
};

/* Buffers *****************************************************************/

/** Charon Session and Communication Control Receive Buffer */
static uint8_t s_receiveBuffer[CHARON_RCV_BUFFER_SIZE];
/** Charon Session and Communication Control Transmit Buffer */
static uint8_t s_sendBuffer[CHARON_TX_BUFFER_SIZE];


/* Private Function Definitions **********************************************/

/**
 * Process an UDS Telegram and dispatch services
 * Accordingly to session, security and diagnostic requirements.
 *
 * @param pBuffer
 *      Pointer to received Message
 * @param length
 *      Length in Bytes
 * @return
 */
static int32_t processReveivedMessage (uint8_t * const pBuffer, uint32_t length);

/**
 * Checks if SID is allowed in session
 *
 * @param currentSession
 *      Currently Active Diagnose Session
 * @param pService
 *      Pointer to Service Object
 *
 * @return Service is allowed or disallowed in Session
 */
static bool isServiceInSession (charon_sessionTypes_t currentSession, charon_serviceObject_t * pService);

/**
 * Timeout and Session Control for ongoing
 * sessions other than default.
 */
static void handleDiagnosticSession (void);

/**
 * Is called during a RCRP Communication Monitor.
 */
static void handleResponsePending (void);

/* Interfaces  ***************************************************************/

void charon_sscCyclic(void)
{
    /* Check Diagnostic Session */
    if(charon_sscType_default != s_currentDiagnosticSession)
    {
        handleDiagnosticSession();
    }

    /* Handle NRC Pending */
    if(ComStatus_pending == s_currentComStatus)
    {
        handleResponsePending();
    }
    else if (ComStatus_ok == s_currentComStatus)
    {
        /* Reset Flag for P2 and P2* distinguish */
        s_p2PendingExceededHandled = false;
    }
}

int32_t charon_sscRcvMessage (void)
{
    uint32_t length;
    int32_t retVal = 0;

    /* Get Message from Underlying Transport LL */
    length = charon_interface_isotp_receive(s_receiveBuffer, sizeof(s_receiveBuffer));

    /**
     * FEATURE
     * Here needs to be evaluated if an encryption is active and the message needs translation.
     */

    /* Process Received Message if one was gotten and de-crypted */
    if(length > 0)
    {
        retVal = processReveivedMessage(s_receiveBuffer, length);
    }

    return retVal;
}

static int32_t processReveivedMessage (uint8_t * const pBuffer, uint32_t length)
{
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
                    s_pendingRequestStartTime = charon_interface_clock_getTime();
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
    else
    {
        /* Send Busy */
        charon_sendNegativeResponse(uds_responseCode_BusyRepeatRequest, sid);
    }

    return retVal;
}

void charon_sscTxMessage (uint8_t * const pBuffer, uint32_t length)
{
    /**
     * FEATURE
     * Here needs to be evaluated if an encryption is active and the message needs encoding.
     */

    /* Reset Com Status */
    s_currentComStatus = ComStatus_ok;

    /* Transmit Message if it fits, otherwise trim */
    if(length >= CHARON_TX_BUFFER_SIZE)      //TODO Error Case?
    {
        length = CHARON_TX_BUFFER_SIZE;
    }

    /* Copy to Buffer and start transfer */
    memcpy(s_sendBuffer, pBuffer, length);
    charon_interface_isotp_transmit(s_sendBuffer, length);
}

int32_t charon_sscSetSession (charon_sessionTypes_t sessionType, uint32_t timeoutP2, uint32_t timeoutP2extended)
{
    if(charon_sscType_default != sessionType)
    {
        /* Mark Beginning of an Diagnostic session that is not default session */
        s_diagnoticSessionTimestamp = charon_interface_clock_getTime();
        /* Set new timings */
        s_ttl.p2Server = timeoutP2;
        s_ttl.p2StarServer = timeoutP2extended;
    }
    else
    {
        /* Fall Back to default timings */
        s_ttl.p2Server = DEFAULT_P2_SERVER;
        s_ttl.p2StarServer = DEFAULT_P2_STAR_SERVER;
    }

    /* Assign new Session Type */
    s_currentDiagnosticSession = sessionType;

    return 0;
}

charon_sessionTypes_t charon_sscGetSession (void)
{
    return s_currentDiagnosticSession;
}

void charon_sscTesterPresentHeartbeat(void)
{
    s_diagnoticSessionTimestamp = charon_interface_clock_getTime();
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

static void handleDiagnosticSession(void)
{
    /* Copy current P3 Server Timing onto stack */
    uint32_t p3TimeoutLimit = s_ttl.s3Server;
    /* Check if Session Timed Out */
    if(charon_interface_clock_getTimeElapsed(s_diagnoticSessionTimestamp) >= p3TimeoutLimit)
    {
        /* Reset Session */
        charon_sscSetSession(charon_sscType_default, 0u, 0u);
        //TODO Signal any ongoing services to stop whatever they are doing
    }
}

static void handleResponsePending (void)
{
    uint32_t tmp;

    /* Get all Time since Pending Request is started */
    tmp = charon_interface_clock_getTimeElapsed(s_pendingRequestStartTime);

    /* Check if P2 was exceeded */
    if(s_p2PendingExceededHandled == false)
    {
        if(tmp >= s_ttl.p2Server)
        {
            s_p2PendingExceededHandled = true;
            charon_sendNegativeResponse(uds_responseCode_RequestCorrectlyReceived_ResponsePending, s_pendingRequestId);
            s_pendingRequestStartTime = charon_interface_clock_getTime();
        }
    }
    else /* Handle the P2* exceedings */
    {
        if(tmp >= s_ttl.p2StarServer)
        {
            charon_sendNegativeResponse(uds_responseCode_RequestCorrectlyReceived_ResponsePending, s_pendingRequestId);
            s_pendingRequestStartTime = charon_interface_clock_getTime();
        }
        //TODO as far as i remember there was a maximum amount to do this, but i couldn't find it...
    }

}


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
