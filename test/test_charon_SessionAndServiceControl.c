/*
 * test_charon_SessionAndServiceControl.c
 *
 *  Created on: 01.04.2020
 *      Author: Andreas Hofmann
 */


#include <unity.h>
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "Common/charon_types.h"
#include "mock_SocketMock.h"
#include "mock_charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "mock_charon_RoutineFunctionalUnit.h"
#include "mock_charon_UploadDownloadFunctionalUnit.h"
#include "mock_charon_interface_clock.h"
#include "mock_charon_ServiceLookupTable.h"
#include "mock_charon_negativeResponse.h"

const ISocket_t SocketMock =
{
        SocketMock_numOfData,
        SocketMock_receive,
        SocketMock_transmit
};

static uds_responseCode_t dummyServiceRoutineSuccess (const uint8_t * pData, uint32_t length);
static uds_responseCode_t dummyServiceRoutinePending (const uint8_t * pData, uint32_t length);

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
            0x0F,                               /* All Sessions */
            dummyServiceRoutineSuccess,         /* Dummy Service */
            0u                                  /* No encrypt */
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u, 0u);

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_DiagnosticSessionControl, &dummyServiceObjectForSessionControl);


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
            0x0E,                               /* Programming Session and Upwards */
            NULL,                               /* Can be Null since it is not reached */
            0u                                  /* No encrypt */
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u, 0u);

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_RequestDownload, &dummyServiceObjectForDownloadRequest);
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

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_ResponseOnEvent, NULL);
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
            0x0E,                               /* Programming upward Sessions */
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
            0x0F,                               /* All Sessions */
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
            0x0F,                               /* All Sessions */
            dummyServiceRoutineSuccess,         /* Dummy Service = NULL should not be reached */
            0u                                  /* No encrypt */
    };

    /* Setup Environment */
    //NA

    /* Setup Expected Function Calls */
    SocketMock_receive_ExpectAndReturn(0, 0, sizeof(clientMsg));
    SocketMock_receive_IgnoreArg_buf();
    SocketMock_receive_IgnoreArg_len();
    SocketMock_receive_ReturnArrayThruPtr_buf(clientMsg, sizeof(clientMsg));
    charon_ServiceLookupTable_getServiceObject_ExpectAndReturn(uds_sid_TesterPresent, &dummyServiceObjectTesterPresent);

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

