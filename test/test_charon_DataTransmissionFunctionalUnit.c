/*
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Rene Mutscheller
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
 * @file test_charon_DataTransmissionFunctionalUnit.c
 * Implementation of unit tests for charon_DataTransmissionFunctionalUnit
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <unity.h>
#include <stdbool.h>
#include "charon_DataTransmissionFunctionalUnit.h"
#include "mock_charon_types.h"
#include "mock_charon_uds.h"
#include "mock_charon_SessionAndServiceControl.h"
#include "mock_charon_DataLookupTable.h" 
#include "mock_charon_negativeResponse.h"
#include "mock_charon_interface_NvmDriver.h"
#include "mock_charon_interface_debug.h"
#include "mock_charon_interface_clock.h"


/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void setUp(void) {} 
void tearDown(void) {}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier_minLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x22, 0x20};
    uint8_t receiveTestBufferLength = 2u;                       // must be >= 3

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier_maxLengthOfDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x22, 0x20};
    uint32_t receiveTestBufferLength = 0xFFFF0000;              // way to many did requests

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier_noMatchingDIDFoundResultsInNULLPointer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x22, 0x20, 0x22};
    uint32_t receiveTestBufferLength = 0x03;

    // Setup Environment

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,NULL);       // NULL pointer
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,NULL);       // NULL pointer
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier_DIDFailsTheSecurityCheckOkay_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x22, 0x20, 0x22};
    uint32_t receiveTestBufferLength = 0x03;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0xFFFFFFFF);           // will result in a failure
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier_DIDLengthOfDataCheckFailedAndIsToLong_returnResponseTooLong (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x22, 0x20, 0x22};
    uint32_t receiveTestBufferLength = 0x03;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->sessionMask = 0xFFFFFFFF;
    didLookupData->lengthOfData = 0x0FFFFFFF;                   // is bigger than the max allowed buffer   

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);    
    charon_sendNegativeResponse_Ignore();      

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ResponseTooLong,charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier_ReadDataByIdentifier_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x22, 0x20, 0x22};
    uint32_t receiveTestBufferLength = 0x03;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->sessionMask = 0xFFFFFFFF;
    didLookupData->lengthOfData = 0x0000000F; 

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);   
    charon_NvmDriver_read_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress_minLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x23, 0x20, 0x20, 0x20};
    uint8_t receiveTestBufferLength = 3u;                       // must be >= 4

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress_maxLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x23, 0x20, 0x20, 0x20};
    uint8_t receiveTestBufferLength = 0xFF;                     // way to long

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress_requestIsNotSupportedOrOutOfRange_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x23, 0x05, 0x20, 0x20};    // 0x05 is no valid input bc in low AND high byte must be a value > 0
    uint8_t receiveTestBufferLength = 0x06;    

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress_noMatchFoundResultsInNULLPointer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x23, 0x22, 0x20, 0x22, 0x20, 0x22};
    uint8_t receiveTestBufferLength = 0x06;   

    // Setup Environment

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x2022,NULL);       // NULL pointer 
    }
    else
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x2220,NULL);       // NULL pointer 
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress_requestSizeIsBiggerThanSaved_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x23, 0x22, 0x20, 0x22, 0x20, 0x22};
    uint8_t receiveTestBufferLength = 0x06;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;                         // requested data (0x20 0x22) is longer than saved

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x2022,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x2220,didLookupData);
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress_requestFailsTheSecurityCheckOkay_returnSecurityAccessDenied (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x23, 0x22,  0x00,0x20,  0x00,0x20};
    uint8_t receiveTestBufferLength = 0x06;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x21;

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0020,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x2000,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0xFFFFFFFF);           // will result in a failure
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SecurityAccessDenied,charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress_requestReadMemoryByAddress_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x23, 0x22,  0x00,0x20,  0x00,0x20};
    uint8_t receiveTestBufferLength = 0x06;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x21;
    didLookupData->sessionMask = 0xFFFFFFFF;

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0020,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x2000,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_NvmDriver_read_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier_minLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x24, 0x20, 0x20};
    uint8_t receiveTestBufferLength = 2u;                       // must be >= 3

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier_noMatchFoundResultsInNULLPointer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x24, 0x20, 0x22};
    uint8_t receiveTestBufferLength = 3u;

    // Setup Environment

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,NULL);    // NULL pointer 
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,NULL);    // NULL pointer 
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier_requestHasNoScalingData_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x24, 0x20, 0x22};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.

    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData ->hasScalingData = false;                     // needs scaling data to read

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,didLookupData);
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier_requestFailsTheSecurityCheckOkay_returnSecurityAccessDenied (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x24, 0x20, 0x22};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData ->hasScalingData = true;

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0xFFFFFFFF);           // will result in a failure
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SecurityAccessDenied,charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier_ReadScalingDataByIdentifier_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x24, 0x20, 0x22};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->hasScalingData = true;
    didLookupData->sessionMask = 0xFFFFFFFF;

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_NvmDriver_read_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_minLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x2A, 0x20, 0x20};
    uint8_t receiveTestBufferLength = 1u;                       // must be at least = 2

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_maxLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x2A, 0x20, 0x20};
    uint8_t receiveTestBufferLength = 0xFF;                     // way to big
    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_transmissionModeNotSupported_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x2A, 0x20, 0x20};          // 0x20 is no valid mode
    uint8_t receiveTestBufferLength = 0x04;
    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_ReadDataByPeriodicIdentifierStopSendingRequestedDID_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x2A, 0x04, 0x20, 0x20};    // 0x04 stop sending
    uint8_t receiveTestBufferLength = 0x04;
    // Setup Environment

    // Setup Expected Function Calls
    charon_NvmDriver_read_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_ReadDataByPeriodicIdentifierStopSendingAllDID_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x2A, 0x04};                // 0x04 stop sending
    uint8_t receiveTestBufferLength = 0x02;                     // empty request = send all
    // Setup Environment

    // Setup Expected Function Calls
    charon_NvmDriver_read_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_noMatchFoundResultsInNULLPointer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x2A, 0x02, 0x20};
    uint8_t receiveTestBufferLength = 0x03;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->sessionMask = 0xFFFFFFFF;

    // Setup Expected Function Calls
    charon_getDataLookupTableByDID_ExpectAndReturn(0xF220,NULL);// NULL pointer, 0xF2 is set
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_toManyDIDInScheduler_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer1[6] = {0x2A, 0x02, 0x20, 0x21, 0x22, 0x23};
    uint8_t receiveTestBufferLength1 = 0x06;
    uint8_t receiveTestBuffer2[6] = {0x2A, 0x02, 0x24, 0x25, 0x26, 0x27};
    uint8_t receiveTestBufferLength2 = 0x06;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->sessionMask = 0xFFFFFFFF;

    // Setup Expected Function Calls
    for (uint8_t i = 0x20; i < (0x20 + 0x08); i++)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn((0xF2 << 8u |  i), didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_NvmDriver_read_Ignore();
    charon_sscTxMessage_Ignore();
    charon_sendNegativeResponse_Ignore();

    // Run Function Test
    charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer1,receiveTestBufferLength1);    // sending buffer is now full and will overflow on the next request

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer2,receiveTestBufferLength2));
}

void test_charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_ReadDataByPeriodicIdentifier_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x2A, 0x02, 0x20};
    uint8_t receiveTestBufferLength = 0x03;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->sessionMask = 0xFFFFFFFF;

    // Setup Expected Function Calls
    charon_getDataLookupTableByDID_ExpectAndReturn(0xF220, didLookupData);
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_requestANonValidSubfunction_returnSubfunctionNotSupported (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x2C, 0x00};                // 0x00 is not a valid request  
    uint8_t receiveTestBufferLength = 2u;

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SubfunctionNotSupported,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_defineByIdentifier__minLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[8] = {0x2C, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 7u;                       // buffer needs to be at least 8 bytes        

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_defineByIdentifier__noMatchFoundResultsInNULLPointer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[8] = {0x2C, 0x01, 0x20, 0x22, 0x02, 0x02, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 8u;

    // Setup Environment

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,NULL);    // Null pointer
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,NULL);    // Null pointer
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_defineByIdentifier__noMatchFoundResultsInNULLPointer2_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[8] = {0x2C, 0x01, 0x20, 0x22, 0x22, 0x20, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 8u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,didLookupData);
    }
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,NULL);    // Null pointer
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,NULL);    // Null pointer
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_defineByIdentifier__requestedDIDIsToLongForBuffer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[8] = {0x2C, 0x01, 0x20, 0x22, 0x22, 0x20, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 8u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0xFFFF;                       // to big for buffer

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,didLookupData);
    }
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,didLookupData);
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_defineByIdentifier__defineByIdentifier_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[8] = {0x2C, 0x01, 0x20, 0x22, 0x22, 0x20, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 8u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x02;

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();
    charon_getDataLookupTableByDID_IgnoreAndReturn(didLookupData);
    charon_getDataLookupTableByDID_IgnoreAndReturn(didLookupData);
    charon_NvmDriver_read_Ignore();
    charon_NvmDriver_write_IgnoreAndReturn(0x00);
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_defineByMemoryAddress__minLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[7] = {0x2C, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 6u;                       // buffer needs to be at least 7 bytes        

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_defineByMemoryAddress__noMatchFoundResultsInNULLPointer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[7] = {0x2C, 0x02, 0x20, 0x22, 0x02, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 7u;

    // Setup Environment

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2022,NULL);    // NULL pointer
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x2220,NULL);    // NULL pointer
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_defineByMemoryAddress__requestedDIDIsToLongForBuffer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[7] = {0x2C, 0x02, 0x11, 0x10, 0x10, 0x11, 0x02};
    uint8_t receiveTestBufferLength = 7u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0xFFFF;                       // to big for buffer

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x1110,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x1011,didLookupData);
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_defineByMemoryAddress__defineByMemoryAddress_PositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[7] = {0x2C, 0x02, 0x11, 0x11, 0x11, 0x11, 0x11};
    uint8_t receiveTestBufferLength = 7u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;

    // Setup Expected Function Calls
    charon_getDataLookupTableByDID_IgnoreAndReturn(didLookupData);
    charon_NvmDriver_read_Ignore();
    charon_NvmDriver_write_IgnoreAndReturn(0x00);
    charon_sscTxMessage_Ignore();
    
    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_clearDynamicalDefinedDID__notSupportedInputLength_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x2C, 0x03, 0x02};
    uint8_t receiveTestBufferLength = 3u;                       // buffer needs to be 2 or 4        

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_clearDynamicalDefinedDID__clearDynamicalDefinedDIDAllDID_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x2C, 0x03};
    uint8_t receiveTestBufferLength = 2u;                       // size 2 for all deletion
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;

    // Setup Expected Function Calls
    charon_getDataLookupTableByDID_IgnoreAndReturn(didLookupData);
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_clearDynamicalDefinedDID__clearDynamicalDefinedDIDSingleDID_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x2C, 0x03, 0xF3, 0x11};
    uint8_t receiveTestBufferLength = 4u;                       // size 4 for requested deletion
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x11F3,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0xF311,didLookupData);
    }
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier_minLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x2E, 0x03, 0x02};
    uint8_t receiveTestBufferLength = 3u;                       // needs to be at least 4 bytes

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier_noMatchFoundResultsInNULLPointer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x2E, 0x03, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 4u;      

    // Setup Environment

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0302,NULL);       // Null pointer
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0203,NULL);       // Null pointer
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier_checkForSavedDataInDID_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x2E, 0x03, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 4u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x00;                         // no saved data in found did

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0302,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0203,didLookupData);
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier_requestFailsTheSecurityCheckOkay_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x2E, 0x03, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 4u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;
    didLookupData->sessionMask = 0x00;

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0302,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0203,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x01);           // will result in a failure 
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier_checkIfDIDcanBeWritten_returnConditionsNotCorrect (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x2E, 0x03, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 4u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;
    didLookupData->sessionMask = 0xFFFFFFFF;
    didLookupData->readOnlyDID = true;                          // must be false or condition fails

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0302,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0203,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ConditionsNotCorrect,charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier_checkIfDataWasNotWritten_returnGeneralProgrammingFailure (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x2E, 0x03, 0x02, 0x02};    // data will not be written in unit test and will stay at 0x00 but input data is 0x02 so its not valid
    uint8_t receiveTestBufferLength = 4u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;
    didLookupData->sessionMask = 0xFFFFFFFF;
    didLookupData->readOnlyDID = false;      

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0302,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x0203,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_NvmDriver_write_IgnoreAndReturn(0x00);
    charon_NvmDriver_read_Ignore();
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_GeneralProgrammingFailure,charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier_WriteDataByIdentifier_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x2E, 0xF2, 0x90, 0x00};    // index 3 = 0x00 to be valid
    uint8_t receiveTestBufferLength = 4u;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;
    didLookupData->sessionMask = 0xFFFFFFFF;
    didLookupData->readOnlyDID = false;      

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0xF290,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByDID_ExpectAndReturn(0x90F2,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_NvmDriver_write_IgnoreAndReturn(0x00);
    charon_NvmDriver_read_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress_minLengthOfRequestDidTriggered_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x3D, 0x03, 0x02, 0x02};
    uint8_t receiveTestBufferLength = 4u;                       // needs to be at least 5 bytes

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress_noValidAddressAndLengthFormatIdentifier_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[5] = {0x3D, 0x01, 0x11, 0x11, 0x11};  // 0x01 is not valid bc both high and low bit must be >=1 
    uint8_t receiveTestBufferLength = 5u;      

    // Setup Environment

    // Setup Expected Function Calls
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress_noMatchFoundResultsInNULLPointer_returnRequestOutOfRange (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[5] = {0x3D, 0x11, 0x11, 0x12, 0x21};
    uint8_t receiveTestBufferLength = 5u;      

    // Setup Environment

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0011,NULL);    // Null pointer
    }
    else
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x1100,NULL);    // Null pointer
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress_dataLengthIsNotCorrect_returnIncorrectMessageLengthOrInvalidFormat (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[5] = {0x3D, 0x11, 0x01, 0x01, 0x11};
    uint8_t receiveTestBufferLength = 5u;      
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x00;                         // does not match the requested length

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0001,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0100,didLookupData);
    }
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress_requestFailsTheSecurityCheckOkay_returnSecurityAccessDenied (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[5] = {0x3D, 0x11, 0x01, 0x01, 0x11};
    uint8_t receiveTestBufferLength = 5u;      
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;
    didLookupData->sessionMask = 0x00;

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0001,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0100,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x01);           // will result in a failure 
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SecurityAccessDenied,charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress_checkIfDIDcanBeWritten_returnConditionsNotCorrect (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[5] = {0x3D, 0x11, 0x01, 0x01, 0x11};
    uint8_t receiveTestBufferLength = 5u;      
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;
    didLookupData->sessionMask = 0xFFFFFFFF;
    didLookupData->readOnlyDID = true;                         // must be false or the condition fails

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0001,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0100,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_sendNegativeResponse_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ConditionsNotCorrect,charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress_WriteMemoryByAddress_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[5] = {0x3D, 0x11, 0x01, 0x01, 0x11};
    uint8_t receiveTestBufferLength = 5u;      
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->lengthOfData = 0x01;
    didLookupData->sessionMask = 0xFFFFFFFF;
    didLookupData->readOnlyDID = false;

    // Setup Expected Function Calls
    if (CHARON_CONFIG_IS_BIG_ENDIAN == 0)
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0001,didLookupData);
    }
    else
    {
        charon_getDataLookupTableByAddress_ExpectAndReturn(0x0100,didLookupData);
    }
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_NvmDriver_write_IgnoreAndReturn(0x00);
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress(receiveTestBuffer,receiveTestBufferLength));
}
//_____________________________________________________________________________

// charon_DataTransmissionFunctionalUnit_Init needs no testing
//_____________________________________________________________________________

void test_charon_DataTransmissionFunctionalUnit_SendPeriodic_SendPeriodic_returnPositiveResponse (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer1[6] = {0x2A, 0x02, 0x20, 0x21, 0x22, 0x23};
    uint8_t receiveTestBufferLength1 = 0x06;
    uint8_t pointerBufferForDID[sizeof(charon_dataIdentifierObject_t)]; // Dummy.
    uint32_t mediumRate = 0;
    uint16_t savedDID1 = 0xF220;
    uint16_t savedDID2 = 0xF221;
    uint16_t savedDID3 = 0xF222;
    uint16_t savedDID4 = 0xF223;
    
    // Setup Environment
    charon_dataIdentifierObject_t* didLookupData = (charon_dataIdentifierObject_t*)&pointerBufferForDID[0];
    didLookupData->sessionMask = 0xFFFFFFFF;

    // Setup Expected Function Calls
    charon_getDataLookupTableByDID_IgnoreAndReturn(didLookupData);
    charon_sscGetSession_IgnoreAndReturn(0x00);
    charon_NvmDriver_read_Ignore();
    charon_sscTxMessage_Ignore();
    CHARON_INFO_Ignore();

    // Run Function Test
    charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(receiveTestBuffer1,receiveTestBufferLength1);    // sending buffer is now full and will overflow on the next request

    // preparation will run x amount of times the function to reduce the counter to 0 and will then check the correct function calls
    mediumRate = charon_DataTransmissionFunctionalUnit_getDefinedPeriodic().medium_rate;
    for (size_t i = 0; i < (uint32_t)mediumRate; i++)
    {
        charon_DataTransmissionFunctionalUnit_SendPeriodic();
    }
    charon_getDataLookupTableByDID_ExpectAndReturn(savedDID1,didLookupData);
    charon_interface_clock_getTimeElapsed_IgnoreAndReturn(0x00);
    charon_sscTxMessage_Ignore();
    charon_interface_clock_getTime_IgnoreAndReturn(0x00);
    charon_DataTransmissionFunctionalUnit_SendPeriodic();

    charon_getDataLookupTableByDID_ExpectAndReturn(savedDID2,didLookupData);
    charon_interface_clock_getTimeElapsed_IgnoreAndReturn(0x00);
    charon_sscTxMessage_Ignore();
    charon_interface_clock_getTime_IgnoreAndReturn(0x00);
    charon_DataTransmissionFunctionalUnit_SendPeriodic();

    charon_getDataLookupTableByDID_ExpectAndReturn(savedDID3,didLookupData);
    charon_interface_clock_getTimeElapsed_IgnoreAndReturn(0x00);
    charon_sscTxMessage_Ignore();
    charon_interface_clock_getTime_IgnoreAndReturn(0x00);
    charon_DataTransmissionFunctionalUnit_SendPeriodic();

    charon_getDataLookupTableByDID_ExpectAndReturn(savedDID4,didLookupData);
    charon_interface_clock_getTimeElapsed_IgnoreAndReturn(0x00);
    charon_sscTxMessage_Ignore();
    charon_interface_clock_getTime_IgnoreAndReturn(0x00);
    charon_DataTransmissionFunctionalUnit_SendPeriodic();
}
//_____________________________________________________________________________


/* Private Function **********************************************************/

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
