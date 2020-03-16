/*
 * test_uds_UploadDownloadFunctionalUnit.c
 *
 *  Created on: 16.01.2020
 *      Author: Florian Kaup
 */


#include <unity.h>
#include "uds_UploadDownloadFunctionalUnit.h"
#include "uds_negativeResponse.h"

#include "mock_uds_interface_NvmDriver.h"


void test_uds_UploadDownloadFunctionalUnit_requestDownload_wrongSize_returnsIncorrectMessageLength (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x00, 0x33, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x34, 0x13};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestDownload_alreadyExpectingDownload_returnsConditionsNotCorrect (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x34, 0x22};

    uds_NvmDriver_checkAddressRange_IgnoreAndReturn(true);

    uds_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData), outputData, sizeof(outputData));
    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(1, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestDownload_compressedAndEncrypted_returnsRequestOutOfRange (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x11, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x34, 0x31};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestDownload_invalidSizeIdentifier_returnsRequestOutOfRange (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x00, 0x66, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x34, 0x31};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestDownload_memoryAddressOutOfRange_returnsRequestOutOfRange (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x34, 0x31};

    uds_NvmDriver_checkAddressRange_ExpectAndReturn(0x1d000000, 0x10000, false);

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestDownload_ok_returnsBufferSizeInfo (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x34, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x74, 0x20, 0x0F, 0xFF};

    uds_NvmDriver_checkAddressRange_ExpectAndReturn(0x1d000000, 0x10000, true);

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestDownload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0x1d000000, uds_UploadDownloadFunctionalUnit_getCurrentMemoryAddress());
    TEST_ASSERT_EQUAL(0x10000, uds_UploadDownloadFunctionalUnit_getRemainingMemoryLength());
    TEST_ASSERT_EQUAL(1, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}






void test_uds_UploadDownloadFunctionalUnit_requestUpload_wrongSize_returnsIncorrectMessageLength (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x33, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x35, 0x13};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestUpload_alreadyExpectingUpload_returnsConditionsNotCorrect (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x35, 0x22};

    uds_NvmDriver_checkAddressRange_IgnoreAndReturn(true);

    uds_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData), outputData, sizeof(outputData));
    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(2, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestUpload_compressedAndEncrypted_returnsRequestOutOfRange (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x11, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x35, 0x31};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestUpload_invalidSizeIdentifier_returnsRequestOutOfRange (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x66, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x35, 0x31};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestUpload_memoryAddressOutOfRange_returnsRequestOutOfRange (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7f, 0x35, 0x31};

    uds_NvmDriver_checkAddressRange_ExpectAndReturn(0x1d000000, 0x10000, false);

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}

void test_uds_UploadDownloadFunctionalUnit_requestUpload_ok_returnsBufferSizeInfo (void)
{
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);

    uint8_t inputData[] = {0x35, 0x00, 0x44, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x75, 0x20, 0x0F, 0xFF};

    uds_NvmDriver_checkAddressRange_ExpectAndReturn(0x1d000000, 0x10000, true);

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestUpload(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0x1d000000, uds_UploadDownloadFunctionalUnit_getCurrentMemoryAddress());
    TEST_ASSERT_EQUAL(0x10000, uds_UploadDownloadFunctionalUnit_getRemainingMemoryLength());
    TEST_ASSERT_EQUAL(2, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}




void test_uds_UploadDownloadFunctionalUnit_transferData_bufferTooBig_returnsIncorrectMessageLength (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(1);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10000);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[9000] = {0x36, 0x01};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7F, 0x36, 0x13};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_uds_UploadDownloadFunctionalUnit_transferData_notInTransferMode_returnsRequestSequenceError (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[1000] = {0x36, 0x01};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7F, 0x36, 0x24};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_uds_UploadDownloadFunctionalUnit_transferData_tooMuchData_returnsTransferDataSuspended (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(1);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[1000] = {0x36, 0x01};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7F, 0x36, 0x71};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_uds_UploadDownloadFunctionalUnit_transferData_wrongSequenceNumber_returnsWrongBlockSequenceCounter (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(1);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10000);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(10);

    uint8_t inputData[1000] = {0x36, 0x01};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7F, 0x36, 0x73};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_uds_UploadDownloadFunctionalUnit_transferData_DownloadOk_returnsOk (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(1);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10000);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[4095] = {0x36, 0x01};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x76, 0x01};

    uds_NvmDriver_write_ExpectAndReturn(0x1d000000, inputData + 2, sizeof(inputData) - 2, uds_responseCode_PositiveResponse);

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(2, uds_UploadDownloadFunctionalUnit_getNextSequenceCounter());
    TEST_ASSERT_EQUAL(0x1d000000 + sizeof(inputData) - 2, uds_UploadDownloadFunctionalUnit_getCurrentMemoryAddress());
    TEST_ASSERT_EQUAL(0x10000 - sizeof(inputData) + 2, uds_UploadDownloadFunctionalUnit_getRemainingMemoryLength());
}

void test_uds_UploadDownloadFunctionalUnit_transferData_DownloadOkInterfaceError_returnsCorrespondingError (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(1);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10000);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[4095] = {0x36, 0x01};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7F, 0x36, 0x93};

    uds_NvmDriver_write_ExpectAndReturn(0x1d000000, inputData + 2, sizeof(inputData) - 2, uds_responseCode_VoltageTooLow);

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_uds_UploadDownloadFunctionalUnit_transferData_UploadOk_returnsOk (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(2);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x10000);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(1);

    uint8_t inputData[] = {0x36, 0x01};
    uint8_t outputData[4095];
    uint8_t expectedOutputData[] = {0x76, 0x01};

    uds_NvmDriver_read_Expect(0x1d000000, outputData + 2, sizeof(outputData) - 2);

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_TransferData(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(outputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(2, uds_UploadDownloadFunctionalUnit_getNextSequenceCounter());
    TEST_ASSERT_EQUAL(2, uds_UploadDownloadFunctionalUnit_getNextSequenceCounter());
    TEST_ASSERT_EQUAL(0x1d000000 + sizeof(outputData) - 2, uds_UploadDownloadFunctionalUnit_getCurrentMemoryAddress());
    TEST_ASSERT_EQUAL(0x10000 - sizeof(outputData) + 2, uds_UploadDownloadFunctionalUnit_getRemainingMemoryLength());
}



void test_uds_UploadDownloadFunctionalUnit_requestTransferExit_serviceNotActive_returnsRequestSequenceError (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(0);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(0);

    uint8_t inputData[] = {0x37};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7F, 0x37, 0x24};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestTransferExit(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_uds_UploadDownloadFunctionalUnit_requestTransferExit_serviceNotFinished_returnsRequestSequenceError (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(1);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x100);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(42);

    uint8_t inputData[] = {0x37};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7F, 0x37, 0x24};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestTransferExit(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}

void test_uds_UploadDownloadFunctionalUnit_requestTransferExit_correctSequence_returnsOk (void)
{
    uds_UploadDownloadFunctionalUnit_setTransferDirection(1);
    uds_UploadDownloadFunctionalUnit_setCurrentMemoryAddress(0x1d000000);
    uds_UploadDownloadFunctionalUnit_setRemainingMemoryLength(0x0);
    uds_UploadDownloadFunctionalUnit_setNextSequenceCounter(42);

    uint8_t inputData[] = {0x37};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x77};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestTransferExit(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
    TEST_ASSERT_EQUAL(0, uds_UploadDownloadFunctionalUnit_getTransferDirection());
}



void test_uds_UploadDownloadFunctionalUnit_requestFileTransfer_returnsNotSupported (void)
{
    uint8_t inputData[] = {0x38};
    uint8_t outputData[100];
    uint8_t expectedOutputData[] = {0x7F, 0x38, 0x11};

    uint32_t transmitSize = uds_UploadDownloadFunctionalUnit_RequestFileTransfer(inputData, sizeof(inputData), outputData, sizeof(outputData));

    TEST_ASSERT_EQUAL(sizeof(expectedOutputData), transmitSize);
    TEST_ASSERT_EQUAL_MEMORY(expectedOutputData, outputData, sizeof(expectedOutputData));
}


