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
 * @file test_charon_UploadDownloadFunctionalUnit.c
 * Implementation of unti tests for charon_UploadDownloadFunctionalUnit
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <unity.h>
#include "charon_UploadDownloadFunctionalUnit.h"
#include "mock_charon_interface_NvmDriver.h"
#include "mock_charon_negativeResponse.h"
#include "mock_charon_SessionAndServiceControl.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void test_charon_UploadDownloadFunctionalUnit_requestDownload_wrongSize_returnsIncorrectMessageLength (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x00, 0x33, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x13, 0x34);
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, charon_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(0, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestDownload_alreadyExpectingDownload_returnsConditionsNotCorrect (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_NvmDriver_checkAddressRange_IgnoreAndReturn(true);
    charon_sscTxMessage_Ignore();
    charon_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData));

    charon_sendNegativeResponse_Expect(0x22, 0x34);
    TEST_ASSERT_EQUAL(uds_responseCode_ConditionsNotCorrect, charon_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(1, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestDownload_compressedAndEncrypted_returnsRequestOutOfRange (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x11, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x31, 0x34);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(0, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestDownload_invalidSizeIdentifier_returnsRequestOutOfRange (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData1[] = {0x34, 0x00, 0x01, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x31, 0x34);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_UploadDownloadFunctionalUnit_RequestDownload(inputData1, sizeof(inputData1)));

    TEST_ASSERT_EQUAL(0, charon_UploadDownloadFunctionalUnit_getTransferDirection());

    uint8_t inputData2[] = {0x34, 0x00, 0x10, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x31, 0x34);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_UploadDownloadFunctionalUnit_RequestDownload(inputData2, sizeof(inputData2)));

    uint8_t inputData3[] = {0x34, 0x00, 0x61, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x31, 0x34);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_UploadDownloadFunctionalUnit_RequestDownload(inputData3, sizeof(inputData3)));

    uint8_t inputData4[] = {0x34, 0x00, 0x16, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x31, 0x34);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_UploadDownloadFunctionalUnit_RequestDownload(inputData4, sizeof(inputData4)));

}

void test_charon_UploadDownloadFunctionalUnit_requestDownload_memoryAddressOutOfRange_returnsRequestOutOfRange (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_NvmDriver_checkAddressRange_ExpectAndReturn(0x1d000000, 0x10000, false);

    charon_sendNegativeResponse_Expect(0x31, 0x34);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(0, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestDownload_ok_returnsBufferSizeInfo (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x00, 0x34, 0x1d, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t expectedOutputData[] = {0x74, 0x20, 0x0F, 0xFF};

    charon_NvmDriver_checkAddressRange_ExpectAndReturn(0x1d000000, 0x10000, true);
    charon_sscTxMessage_ExpectWithArray(expectedOutputData, sizeof(expectedOutputData), sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse, charon_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(0x1d000000, charon_UploadDownloadFunctionalUnit_getCurrentMemoryAddress());
    TEST_ASSERT_EQUAL(0x10000, charon_UploadDownloadFunctionalUnit_getRemainingMemoryLength());
    TEST_ASSERT_EQUAL(1, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestUpload_wrongSize_returnsIncorrectMessageLength (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x33, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x13, 0x35);
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, charon_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(0, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestUpload_alreadyExpectingUpload_returnsConditionsNotCorrect (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_NvmDriver_checkAddressRange_IgnoreAndReturn(true);
    charon_sscTxMessage_Ignore();
    charon_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData));

    charon_sendNegativeResponse_Expect(0x22, 0x35);
    TEST_ASSERT_EQUAL(uds_responseCode_ConditionsNotCorrect, charon_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(2, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestUpload_compressedAndEncrypted_returnsRequestOutOfRange (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x11, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x31, 0x35);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(0, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestUpload_invalidSizeIdentifier_returnsRequestOutOfRange (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x66, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_sendNegativeResponse_Expect(0x31, 0x35);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(0, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestUpload_memoryAddressOutOfRange_returnsRequestOutOfRange (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

    charon_NvmDriver_checkAddressRange_ExpectAndReturn(0x1d000000, 0x10000, false);

    charon_sendNegativeResponse_Expect(0x31, 0x35);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestOutOfRange, charon_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(0, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_requestUpload_ok_returnsBufferSizeInfo (void)
{
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x34, 0x1d, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t expectedOutputData[] = {0x75, 0x20, 0x0F, 0xFF};

    charon_NvmDriver_checkAddressRange_ExpectAndReturn(0x1d000000, 0x10000, true);

    charon_sscTxMessage_ExpectWithArray(expectedOutputData, sizeof(expectedOutputData), sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse, charon_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(0x1d000000, charon_UploadDownloadFunctionalUnit_getCurrentMemoryAddress());
    TEST_ASSERT_EQUAL(0x10000, charon_UploadDownloadFunctionalUnit_getRemainingMemoryLength());
    TEST_ASSERT_EQUAL(2, charon_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_charon_UploadDownloadFunctionalUnit_transferData_bufferTooBig_returnsIncorrectMessageLength (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(1);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10000);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[9000] = {0x36, 0x01};

    charon_sendNegativeResponse_Expect(0x13, 0x36);
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, charon_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData)));
}

void test_charon_UploadDownloadFunctionalUnit_transferData_notInTransferMode_returnsRequestSequenceError (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[1000] = {0x36, 0x01};

    charon_sendNegativeResponse_Expect(0x24, 0x36);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestSequenceError, charon_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData)));
}

void test_charon_UploadDownloadFunctionalUnit_transferData_tooMuchData_returnsTransferDataSuspended (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(1);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[1000] = {0x36, 0x01};

    charon_sendNegativeResponse_Expect(0x71, 0x36);
    TEST_ASSERT_EQUAL(uds_responseCode_TransferDataSuspended, charon_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData)));

}

void test_charon_UploadDownloadFunctionalUnit_transferData_wrongSequenceNumber_returnsWrongBlockSequenceCounter (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(1);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10000);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(10);

    uint8_t inputData[1000] = {0x36, 0x01};

    charon_sendNegativeResponse_Expect(0x73, 0x36);
    TEST_ASSERT_EQUAL(uds_responseCode_WrongBlockSequenceCounter, charon_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData)));
}

void test_charon_UploadDownloadFunctionalUnit_transferData_DownloadOk_returnsOk (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(1);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10000);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[4095] = {0x36, 0x01};
    uint8_t expectedOutputData[] = {0x76, 0x01};

    charon_NvmDriver_write_ExpectAndReturn(0x1d000000, inputData + 2, sizeof(inputData) - 2, uds_responseCode_PositiveResponse);

    charon_sscTxMessage_ExpectWithArray(expectedOutputData, sizeof(expectedOutputData), sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse, charon_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData)));

    TEST_ASSERT_EQUAL(2, charon_UploadDownloadFunctionalUnit_getNextSequenceCounter());
    TEST_ASSERT_EQUAL(0x1d000000 + sizeof(inputData) - 2, charon_UploadDownloadFunctionalUnit_getCurrentMemoryAddress());
    TEST_ASSERT_EQUAL(0x10000 - sizeof(inputData) + 2, charon_UploadDownloadFunctionalUnit_getRemainingMemoryLength());
}

void test_charon_UploadDownloadFunctionalUnit_transferData_DownloadOkInterfaceError_returnsCorrespondingError (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(1);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10000);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[4095] = {0x36, 0x01};

    charon_NvmDriver_write_ExpectAndReturn(0x1d000000, inputData + 2, sizeof(inputData) - 2, uds_responseCode_VoltageTooLow);

    charon_sendNegativeResponse_Expect(0x93, 0x36);
    TEST_ASSERT_EQUAL(uds_responseCode_VoltageTooLow, charon_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData)));
}

void test_charon_UploadDownloadFunctionalUnit_transferData_UploadOk_returnsOk (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(2);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x1800);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData1[] = {0x36, 0x01};
    uint8_t expectedOutputData1[] = {0x76, 0x01};

    charon_NvmDriver_read_Expect(0x1d000000, 0, 4093);
    charon_NvmDriver_read_IgnoreArg_data();

    charon_sscTxMessage_ExpectWithArray(expectedOutputData1, sizeof(expectedOutputData1), 4095);
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse, charon_UploadDownloadFunctionalUnit_TransferData(inputData1, sizeof(inputData1)));

    TEST_ASSERT_EQUAL(2, charon_UploadDownloadFunctionalUnit_getNextSequenceCounter());
    TEST_ASSERT_EQUAL(0x1d000000 + 4093, charon_UploadDownloadFunctionalUnit_getCurrentMemoryAddress());
    TEST_ASSERT_EQUAL(0x1800 - 4093, charon_UploadDownloadFunctionalUnit_getRemainingMemoryLength());

    uint8_t inputData2[] = {0x36, 0x02};
    uint8_t expectedOutputData2[] = {0x76, 0x02};

    charon_NvmDriver_read_Expect(0x1d000000 + 4093, 0, 0x803);
    charon_NvmDriver_read_IgnoreArg_data();

    charon_sscTxMessage_ExpectWithArray(expectedOutputData2, sizeof(expectedOutputData2), 0x805);
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse, charon_UploadDownloadFunctionalUnit_TransferData(inputData2, sizeof(inputData2)));

    TEST_ASSERT_EQUAL(3, charon_UploadDownloadFunctionalUnit_getNextSequenceCounter());
    TEST_ASSERT_EQUAL(0x1d001800, charon_UploadDownloadFunctionalUnit_getCurrentMemoryAddress());
    TEST_ASSERT_EQUAL(0, charon_UploadDownloadFunctionalUnit_getRemainingMemoryLength());

}

void test_charon_UploadDownloadFunctionalUnit_requestTransferExit_incorrectMessageLength_returnsIncorrectMessageLength (void)
{
    uint8_t inputData[] = {0x37, 0x00};

    charon_sendNegativeResponse_Expect(0x13, 0x37);
    TEST_ASSERT_EQUAL(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, charon_UploadDownloadFunctionalUnit_RequestTransferExit(inputData, sizeof(inputData)));
}

void test_charon_UploadDownloadFunctionalUnit_requestTransferExit_serviceNotActive_returnsRequestSequenceError (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(0);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(0);

    uint8_t inputData[] = {0x37};

    charon_sendNegativeResponse_Expect(0x24, 0x37);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestSequenceError, charon_UploadDownloadFunctionalUnit_RequestTransferExit(inputData, sizeof(inputData)));
}

void test_charon_UploadDownloadFunctionalUnit_requestTransferExit_serviceNotFinished_returnsRequestSequenceError (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(1);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x100);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(42);

    uint8_t inputData[] = {0x37};

    charon_sendNegativeResponse_Expect(0x24, 0x37);
    TEST_ASSERT_EQUAL(uds_responseCode_RequestSequenceError, charon_UploadDownloadFunctionalUnit_RequestTransferExit(inputData, sizeof(inputData)));
}

void test_charon_UploadDownloadFunctionalUnit_requestTransferExit_correctSequence_returnsOk (void)
{
    charon_UploadDownloadFunctionalUnit_setTransferDirection(1);
    charon_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    charon_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x0);
    charon_UploadDownloadFunctionalUnit_setNextSequenceCounter(42);

    uint8_t inputData[] = {0x37};
    uint8_t expectedOutputData[] = {0x77};

    charon_sscTxMessage_ExpectWithArray(expectedOutputData, sizeof(expectedOutputData), sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(uds_responseCode_PositiveResponse, charon_UploadDownloadFunctionalUnit_RequestTransferExit(inputData, sizeof(inputData)));
}

void test_charon_UploadDownloadFunctionalUnit_requestFileTransfer_returnsNotSupported (void)
{
    uint8_t inputData[] = {0x38};

    charon_sendNegativeResponse_Expect(0x11, 0x38);
    TEST_ASSERT_EQUAL(uds_responseCode_ServiceNotSupported, charon_UploadDownloadFunctionalUnit_RequestFileTransfer(inputData, sizeof(inputData)));
}
/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/


