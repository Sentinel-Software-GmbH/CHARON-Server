/*
 * test_uds_RoutineFunctionalUnit.c
 *
 *  Created on: 29.01.2020
 *      Author: Florian Kaup
 */


#include <unity.h>
#include "uds_RoutineFunctionalUnit.h"

#include "mock_uds_interface_NvmDriver.h"


void test_uds_RoutineFunctionalUnit_RoutineControl_invalidSubfunction_returnsSubfunctionNotSupported (void)
{
    uint8_t inputData[] = {0x31, 0x04, 0xff, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x31, 0x12};

    uint32_t transmitSize = uds_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_uds_RoutineFunctionalUnit_RoutineControl_messageTooShort_returnsIncorrectMessageLength (void)
{
    uint8_t inputData[] = {0x31, 0x04, 0xff};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x31, 0x13};

    uint32_t transmitSize = uds_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_uds_RoutineFunctionalUnit_RoutineControl_invalidRoutineIdentifier_returnsRequestOutOfRange (void)
{
    uint8_t inputData[] = {0x31, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x31, 0x31};

    uint32_t transmitSize = uds_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

