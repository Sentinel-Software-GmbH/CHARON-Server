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


void test_charon_RoutineFunctionalUnit_RoutineControl_invalidSubfunction_returnsSubfunctionNotSupported (void)
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

