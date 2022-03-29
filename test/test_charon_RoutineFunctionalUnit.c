/*
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Kaup
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
 * @addtogroup test
 * @{
 * @file  test_charon_RoutineFunctionalUnit.c
 * Implementation of unit tests for charon_RoutineFunctionalUnit
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <unity.h>
#include "BusinessLogic/charon_RoutineFunctionalUnit.h"
#include "mock_charon_interface_NvmDriver.h"
#include "mock_charon_negativeResponse.h"
#include "mock_charon_SessionAndSerivceControl.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void test_charon_RoutineFunctionalUnit_RoutineControl_invalidSubfunctionEquals0_returnsSubfunctionNotSupported (void)
{
    uint8_t inputData[] = {0x31, 0x00, 0xff, 0x00};

    charon_sendNegativeResponse_Expect(0x12, 0x31);
    charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData));
}

void test_charon_RoutineFunctionalUnit_RoutineControl_invalidSubfunctionGreater3_returnsSubfunctionNotSupported (void)
{
    uint8_t inputData[] = {0x31, 0x04, 0xff, 0x00};

    charon_sendNegativeResponse_Expect(0x12, 0x31);
    TEST_ASSERT_EQUAL(uds_responseCode_SubfunctionNotSupported, charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData)));
}

void test_charon_RoutineFunctionalUnit_RoutineControl_messageTooShort_returnsIncorrectMessageLength (void)
{
    uint8_t inputData[] = {0x31, 0x04, 0xff};

    charon_sendNegativeResponse_Expect(0x13, 0x31);
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData)));
}

void test_charon_RoutineFunctionalUnit_RoutineControl_invalidRoutineIdentifier_returnsRequestOutOfRange (void)
{
    uint8_t inputData[] = {0x31, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x31, 0x31);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData)));
}

void test_charon_RoutineFunctionalUnit_RoutineControl_RoutineEraseFlash_returnsOk (void)
{
    uint8_t inputData[] = {0x31, 0x01, 0xFF, 0x00};
    uint8_t expectedOutputData[] = {0x71, 0x01, 0xFF, 0x00};

    charon_NvmDriver_erase_Expect();

    charon_sscTxMessage_ExpectWithArray(expectedOutputData, sizeof(expectedOutputData), sizeof(expectedOutputData));
    charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData));
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/


