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
#define DEFAULT_P2_SERVER            ((uint32_t) 100u)       /**< Default P2 Server Time (see ISO Requirements) */
#define DEFAULT_P2_STAR_SERVER       ((uint32_t) 300u)       /**< Default P2* Server Time (see ISO Requirements) */
#define DEFAULT_S3_SERVER            ((uint32_t) 5000u)      /**< Default S3 Server Time (see ISO Requirements) */
/**
 * @}
 */

/**
 * Answer Requests are Request + Inidication Bit
 */
#define UDS_RESPONSE_REQUEST_INDICATION_BIT_MASK         ((uint8_t) 0x40u)

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
static charon_serviceObject_t * s_currentlyPendingService = NULL;
/** Timestamp for Pending Start */
static uint32_t s_pendingRequestStartTime = 0u;
/** Flag to indicate if the P2 Expired Message for Pending was already handled */
static bool s_p2PendingExceededHandled = false;
/** Timestamp for Diagnostic Session Timing */
static uint32_t s_diagnoticSessionTimestamp = 0u;
/** Adjustable timeouts, initialized with server default values */
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
 */
static void processReveivedMessage (uint8_t const * const pBuffer, uint32_t length);

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

/**
 * Execute given Service
 *
 * @param pExecutableService
 *      Dispatched Service Entry from Lookup Table
 * @param pUdsMessage
 *      Pointer to UDS Message Buffer
 * @param length
 *      Length of UDS Message in Byte
 *
 * @return @see @ref uds_responseCode_t
 */
static uds_responseCode_t handleService (charon_serviceObject_t * pExecutableService, uint8_t const * const pUdsMessage, uint32_t length);

/* Interfaces  ***************************************************************/

void charon_sscCyclic (void)
{
    /* Check Diagnostic Session */
    if(charon_sscType_default != s_currentDiagnosticSession)
    {
        handleDiagnosticSession();
    }

    /* Handle NRC Pending */
    if(NULL != s_currentlyPendingService)
    {
        handleResponsePending();
    }
    else
    {
        /* Reset Flag for P2 and P2* distinguish */
        s_p2PendingExceededHandled = false;
    }
}

void charon_sscRcvMessage (void)
{
    uint32_t length;

    /* Get Message from Underlying Transport LL */
    length = charon_interface_isotp_receive(s_receiveBuffer, sizeof(s_receiveBuffer));

    /**
     * FEATURE
     * Here needs to be evaluated if an encryption is active and the message needs translation.
     */

    /* Process Received Message if one was gotten and de-crypted */
    if(length > 0u)
    {
        processReveivedMessage(s_receiveBuffer, length);
    }
}

static void processReveivedMessage (uint8_t const * const pBuffer, uint32_t length)
{
    charon_serviceObject_t * pServiceObj = charon_ServiceLookupTable_getServiceObject((uint8_t)pBuffer[0]);   /* Get Service Object */
    uds_responseCode_t retVal;

    /* Is a Service Pending, do not execute any other Requests except for Tester Present */
    if((NULL == s_currentlyPendingService)
            || (pServiceObj->sid == uds_sid_TesterPresent))
    {
        retVal = handleService(pServiceObj, pBuffer, length);
        /* Check Return Value of Service Execution and Act accordingly */
        /*
         * Note: A lint error is here suppressed since there is  a quirk where you have to check on all enums,
         * even if you have a default case.
         * This would just virtually decrease coverage and make no sense, so this is disabled for this line.
         */
        switch(retVal)
        {
        case uds_responseCode_ServiceNotSupportedInActiveSession:
        {
            /* Answer NRC and Send */
            charon_sendNegativeResponse(uds_responseCode_ServiceNotSupportedInActiveSession, pServiceObj->sid);
            break;
        }
        case uds_responseCode_ServiceNotSupported:
        {
            /* Construct Answer */
            uds_sid_t castedSid;
            /*
             * At this point the SID is extracted from the first Byte of the payload and the LINT warning is deliberately suppressed
             * since the ISO Requires to answer specifically with the SID that is not supported, even though it can be, that the SID
             * is not in the Enumeration.
             */
            castedSid = (uds_sid_t)pBuffer[0];  //TODO Suppress
            /* Send NRC */
            charon_sendNegativeResponse(uds_responseCode_ServiceNotSupported, castedSid);
            break;
        }
        case uds_responseCode_RequestCorrectlyReceived_ResponsePending:
        {
            /* Mark Ongoing Service and Begin with Pending Management */
            s_currentlyPendingService = pServiceObj;
            s_pendingRequestStartTime = charon_interface_clock_getTime();
            break;
        }
        default:
        {
            //Do Nothing...
        }
        }
    }
    else
    {
        /* Send Busy */
        charon_sendNegativeResponse(uds_responseCode_BusyRepeatRequest, pServiceObj->sid);
    }
}

void charon_sscTxMessage (uint8_t const * const pBuffer, uint32_t length)
{
    uint32_t txLength;
    uint8_t responeRequestId;

    /* Check if a Request is pending */
    if(NULL != s_currentlyPendingService)
    {
        /* Copy First byte and Align to Request ID Matching to check if it was an ongoing Service that is now answered */
        responeRequestId = (uint8_t)(((uint8_t)pBuffer[0]) & ((uint8_t)~UDS_RESPONSE_REQUEST_INDICATION_BIT_MASK));
    }

    /**
     * FEATURE
     * Here needs to be evaluated if an encryption is active and the message needs encoding.
     */

    /* Transmit Message if it fits, otherwise trim */
    if(length >= CHARON_TX_BUFFER_SIZE)      //TODO Error Case?
    {
        txLength = CHARON_TX_BUFFER_SIZE;
    }
    else
    {
        txLength = length;
    }

    /* Copy to Buffer and start transfer */
    memcpy(s_sendBuffer, pBuffer, txLength);
    charon_interface_isotp_transmit(s_sendBuffer, txLength);
}

void charon_sscSetSession (charon_sessionTypes_t sessionType, uint32_t timeoutP2, uint32_t timeoutP2extended)
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
}

charon_sessionTypes_t charon_sscGetSession (void)
{
    return s_currentDiagnosticSession;
}

void charon_sscTesterPresentHeartbeat (void)
{
    s_diagnoticSessionTimestamp = charon_interface_clock_getTime();
}

/* Private Function **********************************************************/

static bool isServiceInSession (charon_sessionTypes_t currentSession, charon_serviceObject_t * pService)
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

static void handleDiagnosticSession (void)
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
    if(!(s_p2PendingExceededHandled))
    {
        if(tmp >= s_ttl.p2Server)
        {
            s_p2PendingExceededHandled = true;
            charon_sendNegativeResponse(uds_responseCode_RequestCorrectlyReceived_ResponsePending, s_currentlyPendingService->sid);
            s_pendingRequestStartTime = charon_interface_clock_getTime();
        }
    }
    else /* Handle the P2* exceedings */
    {
        if(tmp >= s_ttl.p2StarServer)
        {
            charon_sendNegativeResponse(uds_responseCode_RequestCorrectlyReceived_ResponsePending, s_currentlyPendingService->sid);
            s_pendingRequestStartTime = charon_interface_clock_getTime();
        }
        //TODO as far as i remember there was a maximum amount to do this, but i couldn't find it...
    }
}

static uds_responseCode_t handleService (charon_serviceObject_t * pExecutableService, uint8_t const * const pUdsMessage, uint32_t length)
{
    uds_responseCode_t retVal;

    /* Check if Service is supported */
    if (NULL != pExecutableService)
    {
        /* Check if Service is Supported in Current Session */
        if (isServiceInSession(s_currentDiagnosticSession, pExecutableService))
        {
            /* Execute Service */
            retVal = pExecutableService->serviceRunable(pUdsMessage, length);
        }
        else
        {
            /* Send Diag NRC */
            retVal = uds_responseCode_ServiceNotSupportedInActiveSession;
        }
    }
    else
    {
        /* Send not Supported NRC */
        retVal = uds_responseCode_ServiceNotSupported;
    }

    return retVal;
}


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
