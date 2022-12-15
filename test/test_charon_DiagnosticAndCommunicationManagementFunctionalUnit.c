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
 * @file test_charon_DiagnosticAndCommunicationManagementFunctionalUnit.c
 * Implementation of unit tests for charon_DiagnosticAndCommunicationManagementFunctionalUnit
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <unity.h>
#include "charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "mock_charon_negativeResponse.h"
#include "mock_charon_SessionAndServiceControl.h"
#include "mock_charon_interface_debug.h"
#include "mock_charon_StoredDataTransmissionFunctionalUnit.h"
#include "mock_charon_ServiceLookupTable.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void test_charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_sendAdditionalParameters_returnsIncorrectMessageLength (void)
{

}

void test_charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_ok_returnsTimingParameters (void)
{

}


void test_charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting_NotSupportedInActiveSessionDefaultSession_returnsNotSupportedInActiveSession (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t receiveTestBufferLength = 4u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_sscGetSession_IgnoreAndReturn(0x02); // 0x02 is the default session state

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ServiceNotSupportedInActiveSession,charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting_NotSupportedInActiveSessionProgrammingSession_returnsNotSupportedInActiveSession (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t receiveTestBufferLength = 4u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_sscGetSession_IgnoreAndReturn(0x04); // 0x04 is the programming session state

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ServiceNotSupportedInActiveSession,charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting_SecuredOrExtendedSession_returnsPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0x00, 0x00, 0x00};
    uint8_t receiveTestBufferLength = 4u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_sscGetSession_IgnoreAndReturn(0x08); // 0x08 is the extended session state
    charon_StoredDataTransmissionFunctionalUnit_DTCSettingTypeUpdater_Ignore();
    charon_sscTxMessage_Expect(0x00,0x02);
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting(receiveTestBuffer,receiveTestBufferLength));
}



/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
