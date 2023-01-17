/*
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
 * @addtogroup CharonUDS_Server
 * @{
 * @addtogroup test
 * @{
 * @file test_charon_SessionAndServiceControl.c
 * Implementation of unit tests for charon_SessionAndServiceControl
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <unity.h>
#include "charon_SessionAndServiceControl.h"
#include "charon_types.h"
#include "mock_SocketMock.h"
#include "mock_charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "mock_charon_RoutineFunctionalUnit.h"
#include "mock_charon_UploadDownloadFunctionalUnit.h"
#include "mock_charon_interface_clock.h"
#include "mock_charon_ServiceLookupTable.h"
#include "mock_charon_negativeResponse.h"
#include "mock_charon_DataTransmissionFunctionalUnit.h"
#include "mock_charon_interface_debug.h"


/* Imports *******************************************************************/

/* Constants *****************************************************************/

const ISocket_t SocketMock =
{
        SocketMock_numOfData,
        SocketMock_receive,
        SocketMock_transmit
};

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

static uds_responseCode_t dummyServiceRoutineSuccess (const uint8_t * pData, uint32_t length);
static uds_responseCode_t dummyServiceRoutinePending (const uint8_t * pData, uint32_t length);

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void setUp (void)
{
	charon_sscInit(SocketMock);
}

void test_charon_SessionAndServiceControl_SetSessionAndTimeouts (void)
{
    /* Setup Variables */
    //NA

    /* Setup Environment */
    //NA

    /* Setup Expected Function Calls */
    charon_interface_clock_getTime_ExpectAndReturn(0x12345678u);

    /* Run Function Test */
    charon_sscSetSession(charon_sscType_programming, 100u, 100u);

    /* Check Results */
    TEST_ASSERT_EQUAL(charon_sscType_programming, charon_sscGetSession());
}

void test_charon_SessionAndServiceControl_noUdsPackageReceived (void)
{
    /* Setup Variables */
    //NA

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u, 0u);

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0u, 0u, 0u);
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();

    /* Run Function Test */
    charon_sscRcvMessage();

    /* Check Results */
    //NA
}

void test_charon_SessionAndServiceControl_executeServiceInDefaultSession_Success (void)
{
    /* Setup Variables */
    uint8_t clientMsg[] =
    {
            /* Client Request to Change to Programming Session */
            0x10u, 0x00, 0x02
    };
    charon_serviceObject_t dummyServiceObjectForSessionControl =
    {
            uds_sid_DiagnosticSessionControl,   /* ID */
            0x1E,                               /* All Sessions */
            dummyServiceRoutineSuccess,         /* Dummy Service */
            0u                                  /* No encrypt */
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u, 0u);
    charon_interface_clock_getTime_IgnoreAndReturn(0x12345678u);

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_DiagnosticSessionControl, &dummyServiceObjectForSessionControl);
    charon_ServiceLookupTable_getNameForServiceSid_ExpectAndReturn(uds_sid_DiagnosticSessionControl,"DiagnosticSessionControl");
    CHARON_INFO_Ignore();
    charon_ServiceLookupTable_getNameForReturnCode_ExpectAndReturn(uds_responseCode_PositiveResponse,"PositiveResponse");

    /* Run Function Test */
    charon_sscRcvMessage();

    /* Check Results */
    //NA
}

void test_charon_SessionAndServiceControl_executeServiceInDefaultSession_Fail(void)
{
    /* Setup Variables */
    uint8_t clientMsg[] =
    {
            /* Client Request to Download of 4k to address 0x00000000 */
            0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
    };
    charon_serviceObject_t dummyServiceObjectForDownloadRequest =
    {
            uds_sid_RequestDownload,            /* ID */
            0x1C,                               /* Programming Session and Upwards */
            NULL,                               /* Can be Null since it is not reached */
            0u                                  /* No encrypt */
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u, 0u);
    charon_interface_clock_getTime_IgnoreAndReturn(0x12345678u);

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_RequestDownload, &dummyServiceObjectForDownloadRequest);
    charon_ServiceLookupTable_getNameForServiceSid_ExpectAndReturn(uds_sid_RequestDownload, "RequestDownload");
    CHARON_INFO_Ignore();
    CHARON_WARNING_Ignore();
    charon_sendNegativeResponse_Expect(uds_responseCode_ServiceNotSupportedInActiveSession ,uds_sid_RequestDownload);

    /* Run Function Test */
    charon_sscRcvMessage();

    /* Check Results */
    //NA
}

void test_charon_SessionAndServiceControl_serviceNotSupported(void)
{
    /* Setup Variables */
    uint8_t clientMsg[] =
    {
            /* Client Request of Response on Event */
            0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u, 0u);
    charon_interface_clock_getTime_IgnoreAndReturn(0x12345678u);

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_ResponseOnEvent, NULL);
    charon_ServiceLookupTable_getNameForServiceSid_ExpectAndReturn(uds_sid_ResponseOnEvent, "ResponseOnEvent");
    CHARON_INFO_Ignore();
    CHARON_WARNING_Ignore();
    charon_sendNegativeResponse_Expect(uds_responseCode_ServiceNotSupported, uds_sid_ResponseOnEvent);

    /* Run Function Test */
    charon_sscRcvMessage();

    /* Check Results */
    //NA
}

void test_charon_SessionAndServiceControl_executeService_AnswerPending(void)
{
    /* Setup Variables */
    uint8_t clientMsg[] =
    {
            0x3D, 0x00, 0x02
    };
    static charon_serviceObject_t dummyServiceObjectForWriteMemoryByAddress =           /* This is declared static since it has to be used */
    {                                                                                   /* for the pending ongoing test cases */
            uds_sid_WriteMemoryByAddress,       /* ID */
            0x1C,                               /* Programming upward Sessions */
            dummyServiceRoutinePending,         /* Dummy Service */
            0u                                  /* No encrypt */
    };

    /* Setup Environment */
    charon_interface_clock_getTime_ExpectAndReturn(0x100u);
    charon_sscSetSession(charon_sscType_programming, 0x100u, 0x500u);

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_WriteMemoryByAddress, &dummyServiceObjectForWriteMemoryByAddress);
    charon_ServiceLookupTable_getNameForServiceSid_ExpectAndReturn(uds_sid_WriteMemoryByAddress,"WriteMemoryByAddress");
    CHARON_INFO_Ignore();
    charon_interface_clock_getTime_ExpectAndReturn(0x100u);

    /* Run Function Test */
    charon_sscRcvMessage();

    /* Check Results */
    TEST_ASSERT_EQUAL(charon_sscType_programming, charon_sscGetSession());
}

void test_charon_SessionAndServiceControl_executeService_ServicePendingDenyService(void)
{
    /* Setup Variables */
    uint8_t clientMsg[] =
    {
            /* Client Request to Change to Programming Session */
            0x10u, 0x00, 0x02
    };
    charon_serviceObject_t dummyServiceObjectForDiagnosticSessionControl =
    {
            uds_sid_DiagnosticSessionControl,   /* ID */
            0x1E,                               /* All Sessions */
            NULL,                               /* Dummy Service = NULL should not be reached */
            0u                                  /* No encrypt */
    };

    /* Setup Environment */
    //NA

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_DiagnosticSessionControl, &dummyServiceObjectForDiagnosticSessionControl);
    charon_ServiceLookupTable_getNameForServiceSid_ExpectAndReturn(uds_sid_DiagnosticSessionControl, "DiagnosticSessionControl");
    CHARON_INFO_Ignore();
    CHARON_WARNING_Ignore();
    charon_sendNegativeResponse_Expect(uds_responseCode_BusyRepeatRequest, uds_sid_DiagnosticSessionControl);

    /* Run Function Test */
    charon_sscRcvMessage();

    /* Check Results */
    //NA
}

void test_charon_SessionAndServiceControl_executeService_ServicePendingAcceptTesterPresent(void)
{
    /* Setup Variables */
    uint8_t clientMsg[] =
    {
            /* Client sends Tester Present with suppressed answer */
            0x3Eu, 0x80u
    };
    charon_serviceObject_t dummyServiceObjectTesterPresent =
    {
            uds_sid_TesterPresent,              /* ID */
            0x1E,                               /* All Sessions */
            dummyServiceRoutineSuccess,         /* Dummy Service = NULL should not be reached */
            0u                                  /* No encrypt */
    };

    /* Setup Environment */
    charon_interface_clock_getTime_IgnoreAndReturn(0x100);

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_TesterPresent, &dummyServiceObjectTesterPresent);
    charon_ServiceLookupTable_getNameForServiceSid_ExpectAndReturn(uds_sid_TesterPresent, "TesterPresent");
    charon_ServiceLookupTable_getNameForReturnCode_ExpectAndReturn(uds_responseCode_PositiveResponse, "PositiveResponse");
    CHARON_INFO_Ignore();
    

    /* Run Function Test */
    charon_sscRcvMessage();

    /* Check Results */
    //NA
}

void test_charon_SessionAndServiceControl_monitorOngoingService_withinTimeNothingTodo(void)
{
    /* Setup Variables */
    //NA

    /* Setup Environment */
    //NA

    /* Setup Expected Function Calls */
    charon_interface_clock_getTimeElapsed_ExpectAndReturn(0x100u, 10u);
    charon_interface_clock_getTimeElapsed_ExpectAndReturn(0x100u, 10u);
    charon_DataTransmissionFunctionalUnit_SendPeriodic_Expect();

    /* Run Function Test */
    charon_sscCyclic();

    /* Check Results */
    //NA
}

void test_charon_SessionAndServiceControl_monitorOngoingService_diagnosticSessionTimedOut(void)
{
    /* Setup Variables */
    //NA

    /* Setup Environment */
    //NA

    /* Setup Expected Function Calls */
    charon_interface_clock_getTimeElapsed_ExpectAndReturn(0x100u, 5001u);
    charon_interface_clock_getTimeElapsed_ExpectAndReturn(0x100u, 10u);
    charon_DataTransmissionFunctionalUnit_SendPeriodic_Expect();
    CHARON_WARNING_Ignore();

    /* Run Function Test */
    charon_sscCyclic();

    /* Check Results */
    TEST_ASSERT_EQUAL(charon_sscType_timedOut, charon_sscGetSession());
}

void test_charon_SessionAndServiceControl_monitorOngoingService_pendingP2Service(void)
{
    /* Setup Variables */
    //NA

    /* Setup Environment */
    //NA

    /* Setup Expected Function Calls */
//    charon_interface_clock_getTimeElapsed_ExpectAndReturn(0x100u, 100u);
//    charon_sendNegativeResponse_Expect(uds_responseCode_RequestCorrectlyReceived_ResponsePending, uds_sid_WriteMemoryByAddress);
//    charon_interface_clock_getTime_ExpectAndReturn(400u);

    /* Run Function Test */
//    charon_sscCyclic();

    /* Check Results */
    //NA
}

void test_charon_SessionAndServiceControl_monitorOngoingService_pendingP2StarService(void)
{
    /* Setup Variables */
    //NA

    /* Setup Environment */
    //NA

    /* Setup Expected Function Calls */
//    charon_interface_clock_getTimeElapsed_ExpectAndReturn(400u, 100u);

    /* Run Function Test */
//    charon_sscCyclic();

    /* Check Results */
    //NA
}

void test_charon_SessionAndServiceControl_monitorOngoingService_pendingP2StarService_responsePending(void)
{
    /* Setup Variables */
    //NA

    /* Setup Environment */
    //NA

    /* Setup Expected Function Calls */
//    charon_interface_clock_getTimeElapsed_ExpectAndReturn(400u, 350u);
//    charon_sendNegativeResponse_Expect(uds_responseCode_RequestCorrectlyReceived_ResponsePending, uds_sid_WriteMemoryByAddress);
//    charon_interface_clock_getTime_ExpectAndReturn(600u);

    /* Run Function Test */
//    charon_sscCyclic();

    /* Check Results */
    //NA
}

static uds_responseCode_t dummyServiceRoutineSuccess (const uint8_t * pData, uint32_t length)
{
    __attribute__((unused)) const uint8_t * ptr = pData;
    __attribute__((unused)) const uint32_t size = length;

    return uds_responseCode_PositiveResponse;
}

static uds_responseCode_t dummyServiceRoutinePending (const uint8_t * pData, uint32_t length)
{
    __attribute__((unused)) const uint8_t * ptr = pData;
    __attribute__((unused)) const uint32_t size = length;

    return uds_responseCode_RequestCorrectlyReceived_ResponsePending;
}
/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/

