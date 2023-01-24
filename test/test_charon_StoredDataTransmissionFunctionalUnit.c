/**
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
 * @file test_charon_StoredDataTransmissionFunctionalUnit.c
 * UnitTest for Stored Data Transmission Functional Unit
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <unity.h>
#include "charon_StoredDataTransmissionFunctionalUnit.h"
#include "mock_charon_crc16.h"
#include "mock_charon_types.h"
#include "mock_charon_DTC_LookupTable.h"
#include "mock_charon_interface_debug.h"
#include "mock_charon_negativeResponse.h"
#include "mock_charon_interface_NvmDriver.h"
#include "mock_charon_SessionAndServiceControl.h"


/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void setUp(void) {}
void tearDown(void) {}


void test_charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_incorrectMessageLengthOrInvalidFormatMessageToShort_returnsNegativeResponseMessage (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[3];
    uint8_t receiveTestBufferLength = 3u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_incorrectMessageLengthOrInvalidFormatMessageToLong_returnsNegativeResponseMessage (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[42];
    uint8_t receiveTestBufferLength = 42u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat,charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_requestOutOfRangeInputIsReservedByISOLow_returnsNegativeResponseMessage (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0x00, 0x00, 0x00}; // 0x000000 -> 0x0000FF are reserved
    uint8_t receiveTestBufferLength = 4u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_requestOutOfRangeInputIsReservedByISOMid_returnsNegativeResponseMessage (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0xFF, 0xFF, 0x03}; // 0xFFFF00 -> 0xFFFF32 are reserved
    uint8_t receiveTestBufferLength = 4u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_requestOutOfRangeInputIsReservedByISOHigh1_returnsNegativeResponseMessage (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0xFF, 0xFF, 0x36}; // 0xFFFF34 -> 0xFFFFCF are reserved
    uint8_t receiveTestBufferLength = 4u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_requestOutOfRangeInputIsReservedByISOHigh2_returnsNegativeResponseMessage (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0xFF, 0xFF, 0xFD}; // 0xFFFFD1 -> 0xFFFFFD are reserved
    uint8_t receiveTestBufferLength = 4u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_deleteAllRequest_returnsPositiveResponseMessage (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0xFF, 0xFF, 0xFF}; // 0xFFFFFF = delete all
    uint8_t receiveTestBufferLength = 4u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_deleteDTC_Expect(0xFF,0xFF,0xFF,0,0,false,false,false,true);
    charon_sendNegativeResponse_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_deleteSingleDTC_returnsPositiveResponseMessage (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0x01, 0x02, 0x03};
    uint8_t receiveTestBufferLength = 4u;


    // Setup Environment


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_deleteDTC_Expect(0x01,0x02,0x03,0,0,false,false,false,false);
    charon_sendNegativeResponse_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_invalidSubfunctionEqualsZero_returnsSubfunctionNotSupported (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x00};
    uint8_t receiveTestBufferLength = 2u;
    uint32_t pointerBufferForHeader[(sizeof(DTC_header_t) / 4)] = {0}; // Dummy.
    uint32_t iGiveUpNow = 0x00;
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->iniDone = 0xDEADBEEF;
    iGiveUpNow = (uint32_t)&pointerBufferForHeader[0];


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,iGiveUpNow);
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SubfunctionNotSupported,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_invalidSubfunctionEqualsHex20_returnsSubfunctionNotSupported (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x20};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SubfunctionNotSupported,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_crc16Invalid_returnFailurePreventsExecutionOfRequestedAction (void)
{
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x20};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x01; // test should yield 0x00 and so its non valid


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_FailurePreventsExecutionOfRequestedAction,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportNumberOfDTCByStatusMask_returnsPositiveResponse (void)
{
    // sub 0x01 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x01, 0xF1};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCCountByStatusMask_ExpectAndReturn(0xF1,false,false,0x00,3);

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCByStatusMaskToManySavedDTC_returnsResponseTooLong (void)
{
    // sub 0x02 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x02, 0xF1};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_testDummy->DTCHighByte = 0x01;
    DTC_testDummy->DTCMiddleByte = 0x01;
    DTC_testDummy->DTCLowByte = 0x01;
    DTC_testDummy->DTCStatusMask = 0xFF;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCCountByStatusMask_ExpectAndReturn(0xF1,false,false,0x00,280); // 280 for saved DTC, max buffer size is 1024 atm and a single DTC will need 4 byte
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_header);
    charon_getDTCLookupByStatusMask_IgnoreAndReturn(DTC_testDummy);

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ResponseTooLong,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCByStatusMask_returnsPositiveResponse (void)
{
    // sub 0x02 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x02, 0xF1};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCCountByStatusMask_ExpectAndReturn(0xF1,false,false,0x00,5);
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_header);
    charon_getDTCLookupByStatusMask_IgnoreAndReturn(DTC_testDummy);

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCSnapshotIdentificationToManySavedDTC_returnsResponseTooLong (void)
{
    // sub 0x03 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x03, 0x01, 0x02, 0x03, 0x01};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0xF0; // gives to many DTC back
    DTC_testDummy->NumberOfSavedSnapshots = 0xFF;
  

    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_getDTCSnapshotAddress_IgnoreAndReturn(DTC_testDummy);


    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ResponseTooLong,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCSnapshotIdentification_returnsPositiveResponse (void)
{
    // sub 0x03 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x03, 0x01, 0x02, 0x03, 0x01};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;
    DTC_testDummy->NumberOfSavedSnapshots = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_getDTCSnapshotAddress_IgnoreAndReturn(DTC_testDummy);

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCSnapshotRecordByDTCNumberDTCNotSupported_returnsRequestOutOfRange (void)
{
    // sub 0x04 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x04, 0x04, 0x05, 0x06, 0xFF}; // a dummy is set atm with the value 0x040506 in "NOT_SUPPORTED_DTC"
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForSnap[sizeof(DTC_SnapshotData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_SnapshotData_t* Snap_testDummy = (DTC_SnapshotData_t*)&pointerBufferForSnap[0]; // needs a address or in the test it will result in an NULL pointer happy little accident

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x00;
    DTC_testDummy->NumberOfSavedSnapshots = 0x01;
    Snap_testDummy->DTCSnapshotDataRecordNumberOfIdentifiers = 0x01;
    Snap_testDummy->DTCSnapshotDataPayload[0] = 0x01;
    for(uint8_t i = 0; i<3;i++)
    {
        DTC_testDummy->DTCSnapshotLength[i] = 0xFFFF; // will result in to many saved DTC
        DTC_testDummy->DTCSnapshotAddress[i] = (DTC_SnapshotData_t*)&pointerBufferForSnap[0];
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCSnapshotRecordByDTCNumberSnapshotNotSupported_returnsRequestOutOfRange (void)
{
    // sub 0x04 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x04, 0x01, 0x02, 0x03, 0x20}; // a dummy is set atm with the value 0x20 in "NOT_SUPPORTED_SNAPSHOT"
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForSnap[sizeof(DTC_SnapshotData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_SnapshotData_t* Snap_testDummy = (DTC_SnapshotData_t*)&pointerBufferForSnap[0]; // needs a address or in the test it will result in an NULL pointer happy little accident

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x00;
    DTC_testDummy->NumberOfSavedSnapshots = 0x01;
    Snap_testDummy->DTCSnapshotDataRecordNumberOfIdentifiers = 0x01;
    Snap_testDummy->DTCSnapshotDataPayload[0] = 0x01;
    for(uint8_t i = 0; i<3;i++)
    {
        DTC_testDummy->DTCSnapshotLength[i] = 0xFFFF; // will result in to many saved DTC
        DTC_testDummy->DTCSnapshotAddress[i] = (DTC_SnapshotData_t*)&pointerBufferForSnap[0];
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCSnapshotRecordByDTCNumberTooManySavedDTC_returnsResponseTooLong (void)
{
    // sub 0x04 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x04, 0x01, 0x02, 0x03, 0xFF};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForSnap[sizeof(DTC_SnapshotData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_SnapshotData_t* Snap_testDummy = (DTC_SnapshotData_t*)&pointerBufferForSnap[0]; // needs a address or in the test it will result in an NULL pointer happy little accident

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x00;
    DTC_testDummy->NumberOfSavedSnapshots = 0x01;
    Snap_testDummy->DTCSnapshotDataRecordNumberOfIdentifiers = 0x01;
    Snap_testDummy->DTCSnapshotDataPayload[0] = 0x01;
    for(uint8_t i = 0; i<3;i++)
    {
        DTC_testDummy->DTCSnapshotLength[i] = 0xFFFF; // will result in to many saved DTC
        DTC_testDummy->DTCSnapshotAddress[i] = (DTC_SnapshotData_t*)&pointerBufferForSnap[0];
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCLookupByDTCNumber_ExpectAnyArgsAndReturn(DTC_testDummy);
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ResponseTooLong,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCSnapshotRecordByDTCNumberNULLPointer_responsePositiveResponse (void)
{
    // sub 0x04 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x04, 0x01, 0x02, 0x03, 0xFF};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = NULL;

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCLookupByDTCNumber_ExpectAnyArgsAndReturn(DTC_testDummy);
    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCSnapshotRecordByDTCNumber_returnsPositiveResponse (void)
{
    // sub 0x04 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x04, 0x01, 0x02, 0x03, 0x01};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForSnap[sizeof(DTC_SnapshotData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_SnapshotData_t* Snap_testDummy = (DTC_SnapshotData_t*)&pointerBufferForSnap[0]; // needs a address or in the test it will result in an NULL pointer happy little accident

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x00;
    DTC_testDummy->NumberOfSavedSnapshots = 0x01;
    Snap_testDummy->DTCSnapshotDataRecordNumberOfIdentifiers = 0x01;
    Snap_testDummy->DTCSnapshotDataPayload[0] = 0x01;
    for(uint8_t i = 0; i<3;i++)
    {
        DTC_testDummy->DTCSnapshotLength[i] = 0x01;
        DTC_testDummy->DTCSnapshotAddress[i] = (DTC_SnapshotData_t*)&pointerBufferForSnap[0];
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCLookupByDTCNumber_ExpectAnyArgsAndReturn(DTC_testDummy);

    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCStoredDataByRecordNumberRequestReservedNumber_returnsServiceNotSupported (void)
{
    // sub 0x05 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x05, 0x00}; // 0x00 is a reserved number
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_testDummy);

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ServiceNotSupported,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCStoredDataByRecordNumberRequestPrintAllBufferOverflow_returnsResponseTooLong (void)
{
    // sub 0x05 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x05, 0xFF}; // 0xFF is a special request = print all
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_testDummy);
    charon_getDTCLookup_reportAllDataRecords_ExpectAnyArgsAndReturn(uds_responseCode_ResponseTooLong);

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ResponseTooLong,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCStoredDataByRecordNumberRequestPrintAll_returnsPositiveResponse (void)
{
    // sub 0x05 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x05, 0xFF}; // 0xFF is a special request = print all
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_testDummy);
    charon_getDTCLookup_reportAllDataRecords_ExpectAnyArgsAndReturn(uds_responseCode_PositiveResponse);


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCStoredDataByRecordNumber_returnsPositiveResponse (void)
{
    // sub 0x05 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x05, 0x01};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_testDummy);
    charon_getDTCLookupByRecordNumber_ExpectAndReturn(0x01,DTC_testDummy);

    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

// @todo OBD-test: Define CHARON_CONFIG_OBD_SUPPORT could be tested too 2x.
void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCExtDataRecordByDTCNumberRequestReservedNumber_returnsServiceNotSupported (void)
{
    // sub 0x06 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x06, 0x01, 0x02, 0x03, 0x91}; // 0x91 at array 6 is a reserved OBD number
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ServiceNotSupported,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCExtDataRecordByDTCNumberTooManyDTCs_returnsResponseTooLong (void)
{
    // sub 0x06 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x06, 0x01, 0x02, 0x03, 0xFF};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForExt[sizeof(DTC_ExtendedData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_ExtendedData_t* Ext_testDummy = (DTC_ExtendedData_t*)&pointerBufferForExt[0]; // needs a address or in the test it will result in an NULL pointer happy little accident

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;
    DTC_testDummy->NumberOfSavedExtendedData = 0x03;

    Ext_testDummy->DTCExtendedDataRecordNumberOfIdentifiers = 0x01;
    Ext_testDummy->DTCExtendedDataPayload[0] = 0x01;
    for(uint8_t i = 0; i<3;i++)
    {
        DTC_testDummy->DTCExtendedDataLength[i] = 0xFFFF; // Overfill buffer
        DTC_testDummy->DTCExtendedDataAddress[i] = (DTC_ExtendedData_t*)&pointerBufferForExt[0];
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCLookupByDTCNumber_ExpectAnyArgsAndReturn(DTC_testDummy);

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ResponseTooLong,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCExtDataRecordByDTCNumber_returnsPositiveResponse (void)
{
    // sub 0x06 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x06, 0x01, 0x02, 0x03, 0x01};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForExt[sizeof(DTC_ExtendedData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_ExtendedData_t* Ext_testDummy = (DTC_ExtendedData_t*)&pointerBufferForExt[0]; // needs a address or in the test it will result in an NULL pointer happy little accident

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;
    DTC_testDummy->NumberOfSavedExtendedData = 0x01;

    Ext_testDummy->DTCExtendedDataRecordNumberOfIdentifiers = 0x01;
    Ext_testDummy->DTCExtendedDataPayload[0] = 0x01;
    for(uint8_t i = 0; i<3;i++)
    {
        DTC_testDummy->DTCExtendedDataLength[i] = 0x01;
        DTC_testDummy->DTCExtendedDataAddress[i] = (DTC_ExtendedData_t*)&pointerBufferForExt[0];
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCLookupByDTCNumber_ExpectAnyArgsAndReturn(DTC_testDummy);

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportNumberOfDTCBySeverityMaskRecord_returnsPositiveResponse (void)
{
    // sub 0x07 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0x07, 0xFF, 0xFF};
    uint8_t receiveTestBufferLength = 4u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCCountBySeverityMask_ExpectAnyArgsAndReturn(3);

    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCBySeverityMaskRecordTooManyDTCs_returnsResponseTooLong (void)
{
    // sub 0x08 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0x08, 0xFF, 0xFF};
    uint8_t receiveTestBufferLength = 4u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0xFF; // will cause an overflow
    DTC_header->currentDTCCounter = 0xFF; // will cause an overflow


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    for (size_t i = 0; i < 0xAB; i++) // max buffer 1024 ... 1024 / 6 = 171 = 0xAB for overflow
    {
        charon_getDTCSeverityMaskAddress_ExpectAnyArgsAndReturn(DTC_testDummy);
    }

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ResponseTooLong,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCBySeverityMaskRecordButNoDTCFound_returnsPositiveResponse (void)
{
    // sub 0x08 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0x08, 0xFF, 0xFF};
    uint8_t receiveTestBufferLength = 4u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = NULL; // NULL pointer = no found DTC
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_getDTCSeverityMaskAddress_ExpectAnyArgsAndReturn(DTC_testDummy);

    charon_sscTxMessage_Expect(receiveTestBuffer, 3); // test will result in an length of 3 bc this is the minimum size by no matched DTC
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCBySeverityMaskRecord_returnsPositiveResponse (void)
{
    // sub 0x08 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[4] = {0x00, 0x08, 0xFF, 0xFF};
    uint8_t receiveTestBufferLength = 4u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_getDTCSeverityMaskAddress_ExpectAnyArgsAndReturn(DTC_testDummy);

    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportSeverityInformationOfDTCNoDTCFound_returnsPositiveResponse (void)
{
    // sub 0x09 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[5] = {0x00, 0x09, 0x01, 0x02, 0x03};
    uint8_t receiveTestBufferLength = 5u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = NULL;
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_header);
    charon_getDTCLookupByDTCNumber_IgnoreAndReturn(DTC_testDummy);

    charon_sscTxMessage_Expect(receiveTestBuffer, 6); // test will result in an length of 6 bc this is the minimum size by no matched DTC
    charon_sscTxMessage_IgnoreArg_pBuffer();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportSeverityInformationOfDTC_returnsPositiveResponse (void)
{
    // sub 0x09 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[5] = {0x00, 0x09, 0x01, 0x02, 0x03};
    uint8_t receiveTestBufferLength = 5u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_header);
    charon_getDTCLookupByDTCNumber_IgnoreAndReturn(DTC_testDummy);

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportSupportedDTCTooManyDTC_returnsResponseTooLong (void)
{
    // sub 0x0A ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0A};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    DTC_t* overfillBuffer[260] = {0}; // Dummy


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&overfillBuffer[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->currentDTCCounter = 0x1FFF; // will result in TX_BUFFER overflow

    for (uint32_t i = 0; i < ((260 / 8) + 1); i++)
    {
        DTC_header->nvmDTCLookupTable[i] = 0xFF;
    }
   
    for (uint32_t i = 0; i < 260; i++)
    {
        DTC_testDummy->statusOfDTC = 0x01;
        DTC_testDummy->DTCHighByte = 0x01;
        DTC_testDummy->DTCMiddleByte = 0x01;
        DTC_testDummy->DTCLowByte = 0x01;
    }
    

    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_testDummy);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);


    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ResponseTooLong,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportSupportedDTC_returnsPositiveResponse (void)
{
    // sub 0x0A ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0A};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;
    DTC_header->currentDTCCounter = 0x01;

    for (uint32_t i = 0; i < ((AMOUNT_OF_SNAPSHOT / 8) + 1); i++)
    {
        DTC_header->nvmDTCLookupTable[i] = 0xFF;
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,false,(uint32_t)DTC_testDummy);
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);

    charon_sscTxMessage_Expect(receiveTestBuffer, 7); // test will result in an length of 7 
    charon_sscTxMessage_IgnoreArg_pBuffer();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportFirstTestFailedDTCNoDTCSaved_returnsPositiveResponse (void)
{
    // sub 0x0B ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0B};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;
    
    DTC_header->FirstFailedDTC = 0;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);

    charon_sscTxMessage_Expect(receiveTestBuffer, 3); // test will result in an length of 6 
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportFirstTestFailedDTC_returnsPositiveResponse (void)
{
    // sub 0x0B ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0B};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_header->FirstFailedDTC = (uint32_t)&pointerBufferForDTC[0];


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);

    charon_sscTxMessage_Expect(receiveTestBuffer, 7); // test will result in an length of 6 
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportFirstConfirmedDTCNoDTCSaved_returnsPositiveResponse (void)
{
    // sub 0x0C ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0C};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;
    
    DTC_header->FirstConfirmedDTC = 0;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);

    charon_sscTxMessage_Expect(receiveTestBuffer, 3); // test will result in an length of 6 
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportFirstConfirmedDTC_returnsPositiveResponse (void)
{
    // sub 0x0C ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0C};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_header->FirstConfirmedDTC = (uint32_t)&pointerBufferForDTC[0];


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);

    charon_sscTxMessage_Expect(receiveTestBuffer, 7); // test will result in an length of 6 
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportMostRecentTestFailedDTCNoDTCSaved_returnsPositiveResponse (void)
{
    // sub 0x0D ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0D};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;
    
    DTC_header->MostRecentTestFailed = 0;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);

    charon_sscTxMessage_Expect(receiveTestBuffer, 3); // test will result in an length of 6 
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportMostRecentTestFailedDTC_returnsPositiveResponse (void)
{
    // sub 0x0D ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0D};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_header->MostRecentTestFailed = (uint32_t)&pointerBufferForDTC[0];


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);

    charon_sscTxMessage_Expect(receiveTestBuffer, 7); // test will result in an length of 6 
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportMostRecentConfirmedDTCNoDTCSaved_returnsPositiveResponse (void)
{
    // sub 0x0E ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0E};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;
    
    DTC_header->MostRecentConfirmedDTC = 0;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);

    charon_sscTxMessage_Expect(receiveTestBuffer, 3); // test will result in an length of 6 
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportMostRecentConfirmedDTC_returnsPositiveResponse (void)
{
    // sub 0x0E ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x0E};
    uint8_t receiveTestBufferLength = 2u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_header->MostRecentConfirmedDTC = (uint32_t)&pointerBufferForDTC[0];


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);

    charon_sscTxMessage_Expect(receiveTestBuffer, 7); // test will result in an length of 6 
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportMirrorMemoryDTCByStatusMask_returnsPositiveResponse (void)
{
    // sub 0x0F ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x0F, 0xF1};
    uint8_t receiveTestBufferLength = 3u;
    uint32_t pointerBuffer[1]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBuffer[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBuffer[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCCountByStatusMask_ExpectAndReturn(0xF1,true,false,0x00,5);
    charon_NvmDriver_getMirrorNvmAddress_ExpectAndReturn(0,false,(uint32_t)DTC_header);
    charon_getDTCLookupByStatusMask_IgnoreAndReturn(DTC_testDummy);

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportMirrorMemoryDTCExtDataRecordByDTCNumber_returnsPositiveResponse (void)
{
    // sub 0x10 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x10, 0x01, 0x02, 0x03, 0x01};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForExt[sizeof(DTC_ExtendedData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_ExtendedData_t* Ext_testDummy = (DTC_ExtendedData_t*)&pointerBufferForExt[0]; // needs a address or in the test it will result in an NULL pointer happy little accident

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;
    DTC_testDummy->NumberOfSavedExtendedData = 0x01;

    Ext_testDummy->DTCExtendedDataRecordNumberOfIdentifiers = 0x01;
    Ext_testDummy->DTCExtendedDataPayload[0] = 0x01;
    for(uint8_t i = 0; i<3;i++)
    {
        DTC_testDummy->DTCExtendedDataLength[i] = 0x01;
        DTC_testDummy->DTCExtendedDataAddress[i] = (DTC_ExtendedData_t*)&pointerBufferForExt[0];
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCLookupByDTCNumber_ExpectAndReturn(0,0,0,true,false,0,DTC_testDummy);
    charon_getDTCLookupByDTCNumber_IgnoreArg_DTCHighByte();
    charon_getDTCLookupByDTCNumber_IgnoreArg_DTCMiddleByte();
    charon_getDTCLookupByDTCNumber_IgnoreArg_DTCLowByte();
    charon_getDTCLookupByDTCNumber_IgnoreArg_MemorySelection();

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportNumberOfMirrorMemoryDTCByStatusMask_returnsPositiveResponse (void)
{
    // sub 0x11 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x11, 0xF1};
    uint8_t receiveTestBufferLength = 3u;
    uint32_t pointerBuffer[1]; // Dummy.
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBuffer[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCCountByStatusMask_ExpectAndReturn(0xF1,true,false,0x00,3);

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

// Currently not supported
void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportNumberOfEmissionsOBDDTCByStatusMask_returnsSubfunctionNotSupported (void)
{
    // sub 0x12 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x12, 0xF1};
    uint8_t receiveTestBufferLength = 3u;
    uint32_t pointerBuffer[1]; // Dummy.
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBuffer[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SubfunctionNotSupported,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

// Currently not supported
void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportEmissionsOBDDTCByStatusMask_returnsSubfunctionNotSupported (void)
{
    // sub 0x13 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x13, 0xF1};
    uint8_t receiveTestBufferLength = 3u;
    uint32_t pointerBuffer[1]; // Dummy.
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBuffer[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SubfunctionNotSupported,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

// Currently not supported
void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCFaultDetectionCounter_returnsSubfunctionNotSupported (void)
{
    // sub 0x14 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x14};
    uint8_t receiveTestBufferLength = 2u;
    uint32_t pointerBuffer[1]; // Dummy.
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBuffer[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SubfunctionNotSupported,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

// Currently not supported
void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCWithPermanentStatus_returnsSubfunctionNotSupported (void)
{
    // sub 0x15 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[2] = {0x00, 0x15};
    uint8_t receiveTestBufferLength = 2u;
    uint32_t pointerBuffer[1]; // Dummy.
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBuffer[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    CHARON_ERROR_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_SubfunctionNotSupported,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCExtDataRecordByRecordNumberRequestReservedNumber_returnsRequestOutOfRange (void)
{
    // sub 0x16 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x16, 0xFE}; // 0xFE is an reserved number
    uint8_t receiveTestBufferLength = 3u;
    uint32_t pointerBuffer[1]; // Dummy.
    

    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBuffer[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);


    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCExtDataRecordByRecordNumberTooManyDTC_returnsResponseTooLong (void)
{
    // sub 0x16 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x16, 0x01};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)] = {0}; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForExt[sizeof(DTC_ExtendedData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_testDummy->DTCExtendedDataLength[0] = 0xFFFF; // will result in overflow
    DTC_testDummy->NumberOfSavedExtendedData = 0x01;
    DTC_testDummy->statusOfDTC = 0xFF;
    DTC_testDummy->DTCExtendedDataAddress[0] = (DTC_ExtendedData_t*)&pointerBufferForExt[0];

    DTC_testDummy->DTCStatusMask= 0x01;
    DTC_testDummy->DTCHighByte = 0x01;
    DTC_testDummy->DTCMiddleByte = 0x01;
    DTC_testDummy->DTCLowByte = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCExtDataCountByRecordNumber_ExpectAndReturn(0x01,1);
    charon_getDTCExtDataAddressByRecordNumber_IgnoreAndReturn((DTC_t*)pointerBufferForDTC);

    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_ResponseTooLong,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCExtDataRecordByRecordNumberNULLPointer_returnsPositiveResponse (void)
{
    // sub 0x16 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x16, 0x01};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_header->CRC16Checksum = 0x00;

    DTC_testDummy->NumberOfSavedExtendedData = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCExtDataCountByRecordNumber_ExpectAndReturn(0x01,1);
    charon_getDTCExtDataAddressByRecordNumber_IgnoreAndReturn(NULL);

    charon_sscTxMessage_Expect(receiveTestBuffer, 3);
    charon_sscTxMessage_IgnoreArg_pBuffer();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportDTCExtDataRecordByRecordNumber_returnsPositiveResponse (void)
{
    // sub 0x16 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x16, 0x01};
    uint8_t receiveTestBufferLength = 3u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForExt[sizeof(DTC_ExtendedData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_ExtendedData_t* Ext_testDummy = (DTC_ExtendedData_t*)&pointerBufferForExt[0]; // needs a address or in the test it will result in an NULL pointer happy little accident
    DTC_testDummy->DTCHighByte = 0x01;
    DTC_testDummy->DTCMiddleByte = 0x01;
    DTC_testDummy->DTCLowByte = 0x01;
    DTC_testDummy->statusOfDTC = 0xFF;

    DTC_header->CRC16Checksum = 0x00;

    DTC_testDummy->NumberOfSavedExtendedData = 0x01;
    DTC_testDummy->DTCExtendedDataLength[0] = 0x01;
    DTC_testDummy->DTCExtendedDataAddress[0] = (DTC_ExtendedData_t*)&pointerBufferForExt[0];

    Ext_testDummy->DTCExtendedDataRecordNumberOfIdentifiers = 0x01;
    Ext_testDummy->DTCExtendedDataPayload[0] = 0x01;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCExtDataCountByRecordNumber_ExpectAndReturn(0x01,1);
    charon_getDTCExtDataAddressByRecordNumber_IgnoreAndReturn((DTC_t*)pointerBufferForDTC);


    charon_sendNegativeResponse_Ignore();
    CHARON_ERROR_Ignore();
    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportUserDefMemoryDTCByStatusMask_returnsPositiveResponse (void)
{
    // sub 0x17 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[3] = {0x00, 0x17, 0xF1};
    uint8_t receiveTestBufferLength = 3u;
    uint32_t pointerBuffer[1]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBuffer[0];
    DTC_header->CRC16Checksum = 0x00;


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCCountByStatusMask_ExpectAndReturn(0xF1,false,true,0x00,3);
    charon_getDTCCountByStatusMask_IgnoreArg_MemorySelection();

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportUserDefMemoryDTCSnapshotRecordByDTCNumber_returnsPositiveResponse (void)
{
    // sub 0x18 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x18, 0x01, 0x02, 0x03, 0x01};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForSnap[sizeof(DTC_SnapshotData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_SnapshotData_t* Snap_testDummy = (DTC_SnapshotData_t*)&pointerBufferForSnap[0]; // needs a address or in the test it will result in an NULL pointer happy little accident

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x00;
    DTC_testDummy->NumberOfSavedSnapshots = 0x01;
    Snap_testDummy->DTCSnapshotDataRecordNumberOfIdentifiers = 0x01;
    Snap_testDummy->DTCSnapshotDataPayload[0] = 0x01;
    for(uint8_t i = 0; i<3;i++)
    {
        DTC_testDummy->DTCSnapshotLength[i] = 0x01;
        DTC_testDummy->DTCSnapshotAddress[i] = (DTC_SnapshotData_t*)&pointerBufferForSnap[0];
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCLookupByDTCNumber_ExpectAndReturn(0,0,0,false,true,0,DTC_testDummy);
    charon_getDTCLookupByDTCNumber_IgnoreArg_DTCHighByte();
    charon_getDTCLookupByDTCNumber_IgnoreArg_DTCMiddleByte();
    charon_getDTCLookupByDTCNumber_IgnoreArg_DTCLowByte();
    charon_getDTCLookupByDTCNumber_IgnoreArg_MemorySelection();

    charon_sscTxMessage_Ignore();


    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________

void test_charon_StoredDataTransmissionFunctionalUnit_ReadDTCInformation_reportUserDefMemoryDTCExtDataRecordByDTCNumber_returnsPositiveResponse (void)
{
    // sub 0x19 ISO 14229-1 2013
    // Setup Variables
    uint8_t receiveTestBuffer[6] = {0x00, 0x19, 0x01, 0x02, 0x03, 0x01};
    uint8_t receiveTestBufferLength = 6u;
    uint8_t pointerBufferForDTC[sizeof(DTC_t)]; // Dummy.
    uint8_t pointerBufferForHeader[sizeof(DTC_header_t)]; // Dummy.
    uint8_t pointerBufferForExt[sizeof(DTC_ExtendedData_t)]; // Dummy.


    // Setup Environment
    DTC_header_t* DTC_header = (DTC_header_t*)&pointerBufferForHeader[0];
    DTC_t* DTC_testDummy = (DTC_t*)&pointerBufferForDTC[0];
    DTC_ExtendedData_t* Ext_testDummy = (DTC_ExtendedData_t*)&pointerBufferForExt[0]; // needs a address or in the test it will result in an NULL pointer happy little accident

    DTC_header->CRC16Checksum = 0x00;
    DTC_header->totalDTCCounter = 0x01;
    DTC_testDummy->NumberOfSavedExtendedData = 0x01;

    Ext_testDummy->DTCExtendedDataRecordNumberOfIdentifiers = 0x01;
    Ext_testDummy->DTCExtendedDataPayload[0] = 0x01;
    for(uint8_t i = 0; i<3;i++)
    {
        DTC_testDummy->DTCExtendedDataLength[i] = 0x01;
        DTC_testDummy->DTCExtendedDataAddress[i] = (DTC_ExtendedData_t*)&pointerBufferForExt[0];
    }


    // Setup Expected Function Calls
    CHARON_INFO_Ignore();
    charon_NvmDriver_getNvmAddress_for_DTC_ExpectAndReturn(0,true,(uint32_t)DTC_header);
    charon_DTC_LookupTable_header_SET_Ignore();
    charon_crc16_update_IgnoreAndReturn(0);
    charon_crc16_reflect_IgnoreAndReturn(0);
    charon_getDTCStatusAvailabilityMask_IgnoreAndReturn(0x07);

    charon_getDTCLookupByDTCNumber_ExpectAndReturn(0,0,0,false,true,0,DTC_testDummy);
    charon_getDTCLookupByDTCNumber_IgnoreArg_DTCHighByte();
    charon_getDTCLookupByDTCNumber_IgnoreArg_DTCMiddleByte();
    charon_getDTCLookupByDTCNumber_IgnoreArg_DTCLowByte();
    charon_getDTCLookupByDTCNumber_IgnoreArg_MemorySelection();

    charon_sscTxMessage_Ignore();

    // Run Function Test

    // Check Results
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse,charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(receiveTestBuffer,receiveTestBufferLength));
}
//_________________________________________________________________________________________________


/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
