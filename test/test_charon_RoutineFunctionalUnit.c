/*
 * test_charon_RoutineFunctionalUnit.c
 *
 *  Created on: 29.01.2020
 *      Author: Florian Kaup
 */


#include <unity.h>
#include "BusinessLogic/charon_RoutineFunctionalUnit.h"
#include "Common/charon_negativeResponse.h"

#include "mock_charon_interface_NvmDriver.h"


void test_charon_RoutineFunctionalUnit_RoutineControl_invalidSubfunctionEquals0_returnsSubfunctionNotSupported (void)
{
    uint8_t inputData[] = {0x31, 0x00, 0xff, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x31, 0x12};

    uint32_t transmitSize = charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_charon_RoutineFunctionalUnit_RoutineControl_invalidSubfunctionGreater3_returnsSubfunctionNotSupported (void)
{
    uint8_t inputData[] = {0x31, 0x04, 0xff, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x31, 0x12};

    uint32_t transmitSize = charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_charon_RoutineFunctionalUnit_RoutineControl_messageTooShort_returnsIncorrectMessageLength (void)
{
    uint8_t inputData[] = {0x31, 0x04, 0xff};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x31, 0x13};

    uint32_t transmitSize = charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_charon_RoutineFunctionalUnit_RoutineControl_invalidRoutineIdentifier_returnsRequestOutOfRange (void)
{
    uint8_t inputData[] = {0x31, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x31, 0x31};

    uint32_t transmitSize = charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_charon_RoutineFunctionalUnit_RoutineControl_RoutineEraseFlash_returnsOk (void)
{
    uint8_t inputData[] = {0x31, 0x01, 0xFF, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x71, 0x01, 0xFF, 0x00};

    charon_NvmDriver_erase_Expect();

    uint32_t transmitSize = charon_RoutineFunctionalUnit_RoutineControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

