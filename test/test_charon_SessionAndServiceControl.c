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


void test_charon_SessionAndServiceControl_executeServiceInDefaultSession_Success (void)
{
    /* Setup Variables */
    uint32_t retVal;
    uint8_t clientMsg[] =
    {
            /* Client Request to Change to Programming Session */
            0x10u, 0x00, 0x01
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u);

    /* Setup Expected Function Calls */
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_ExpectAndReturn(clientMsg, sizeof(clientMsg), 0);

    /* Run Function Test */
    retVal = charon_sscRcvProcessMessage(clientMsg, sizeof(clientMsg));

    /* Check Results */
    TEST_ASSERT_EQUAL(0, retVal);
    //TEST_ASSERT_EQUAL(charon_sscType_programming, charon_sscGetSession()); ENABLE THIS UPON SESSION CONTROL IN DCM
}

void test_charon_SessionAndServiceControl_executeServiceInDefaultSession_Fail(void)
{
    /* Setup Variables */
    uint32_t retVal;
    uint8_t clientMsg[] =
    {
            /* Client Request to Request Download of 4k to address 0x00000000 */
            0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
    };

    /* Setup Environment */
    charon_sscSetSession(charon_sscType_default, 0u);

    /* Setup Expected Function Calls */


    /* Run Function Test */
    retVal = charon_sscRcvProcessMessage(clientMsg, sizeof(clientMsg));

    /* Check Results */
    TEST_ASSERT_EQUAL(-1, retVal);
    //CHECK ANSWER FRAME
}

