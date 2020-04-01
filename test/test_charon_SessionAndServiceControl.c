/*
 * test_charon_SessionAndServiceControl.c
 *
 *  Created on: 01.04.2020
 *      Author: Andreas Hofmann
 */


#include <unity.h>
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "Common/charon_negativeResponse.h"
#include "Common/charon_types.h"
#include "ComLogic/charon_ServiceLookupTable.h"

#include "mock_charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "mock_charon_RoutineFunctionalUnit.h"
#include "mock_charon_UploadDownloadFunctionalUnit.h"
#include "mock_charon_interface_clock.h"


void test_charon_SessionAndServiceControl_executeServiceInDefaultSession_Success (void)
{
    /* Setup Variables */
    uint32_t retVal;
    uint8_t clientMsg[] =
    {
            /* Client Request to Change to Programming Session */
            0x10u, 0x00, 0x02
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u);

    /* Setup Expected Function Calls */
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_ExpectAndReturn(clientMsg, sizeof(clientMsg), uds_responseCode_PositiveResponse);

    /* Run Function Test */
    retVal = charon_sscRcvProcessMessage(clientMsg, sizeof(clientMsg));

    /* Check Results */
    TEST_ASSERT_EQUAL(0, retVal);
    //TEST_ASSERT_EQUAL(charon_sscType_programming, charon_sscGetSession()); ENABLE THIS UPON SESSION CONTROL IN DCM
    //CHECK ANSWER FRAME ?
}

void test_charon_SessionAndServiceControl_executeServiceInDefaultSession_Fail(void)
{
    /* Setup Variables */
    uint32_t retVal;
    uint8_t clientMsg[] =
    {
            /* Client Request to Download of 4k to address 0x00000000 */
            0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u);

    /* Setup Expected Function Calls */
    //NA

    /* Run Function Test */
    retVal = charon_sscRcvProcessMessage(clientMsg, sizeof(clientMsg));

    /* Check Results */
    TEST_ASSERT_EQUAL(-1, retVal);
    //CHECK ANSWER FRAME ?
}

void test_charon_SessionAndServiceControl_executeService_WithSuppressedAnswer(void)
{
    /* Setup Variables */
    uint32_t retVal;
    uint8_t clientMsg[] =
    {
            /* Client Request to Reset without Answer */
            (0x11u | 0x80u), 0x00, 0x02
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u);

    /* Setup Expected Function Calls */
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset_ExpectAndReturn(clientMsg, sizeof(clientMsg), uds_responseCode_PositiveResponse);

    /* Run Function Test */
    retVal = charon_sscRcvProcessMessage(clientMsg, sizeof(clientMsg));

    /* Check Results */
    TEST_ASSERT_EQUAL(0, retVal);
    //CHECK ANSWER FRAME ?
}

