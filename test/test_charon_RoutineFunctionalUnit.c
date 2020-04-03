/*
 * test_charon_RoutineFunctionalUnit.c
 *
 *  Created on: 29.01.2020
 *      Author: Florian Kaup
 */


#include <unity.h>
#include "BusinessLogic/charon_RoutineFunctionalUnit.h"

#include "mock_charon_interface_NvmDriver.h"
#include "mock_charon_negativeResponse.h"
#include "mock_charon_SessionAndSerivceControl.h"


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

    charon_sscTxProcessMessage_ExpectWithArray(expectedOutputData, sizeof(expectedOutputData), sizeof(expectedOutputData));
    charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData));
}

