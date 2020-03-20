/*
 * test_charon_DiagnosticAndCommunicationManagementFunctionalUnit.c
 *
 *  Created on: 30.01.2020
 *      Author: Florian Kaup
 */


#include <unity.h>
#include "BusinessLogic/charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "Common/charon_negativeResponse.h"



void test_charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_sendAdditionalParameters_returnsIncorrectMessageLength (void)
{
    uint8_t inputData[] = {0x10, 0x01, 0xff, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x10, 0x13};

    uint32_t transmitSize = charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_ok_returnsTimingParameters (void)
{
    uint8_t inputData[] = {0x10, 0x02};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x50, 0x02, 0x00, 0x32, 0x01, 0xF4};

    uint32_t transmitSize = charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}
