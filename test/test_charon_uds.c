/*
 * test_uds.c
 *
 *  Created on: 15.01.2020
 *      Author: Florian Kaup
 */


#include <unity.h>
#include "charon_uds.h"
#include "Common/charon_negativeResponse.h"

#include "HSDI/charon_interface_canisotp.h"

#include "mock_charon_interface_canisotp.h"
#include "mock_charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "mock_charon_DataTransmissionFunctionalUnit.h"
#include "mock_charon_StoredDataTransmissionFunctionalUnit.h"
#include "mock_charon_InputOutputControlFunctionalUnit.h"
#include "mock_charon_RoutineFunctionalUnit.h"
#include "mock_charon_UploadDownloadFunctionalUnit.h"

void test_uds_task_nothingToDo (void)
{
    uds_isotp_receive_ExpectAndReturn(0,0,0);
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uds_task();
}

void test_uds_task_receiveUnknown_sendNotImplemented (void)
{
    uint8_t dataToReceive [] = {0xBA, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x7F, 0xBA, 0x11};

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveDiagnosticSessionControl_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x10, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x50, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveEcuReset_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x11, 0x01};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x51, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveClearDiagnosticInformation_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x14, 0x01};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x54, 0x00};
    uds_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_IgnoreArg_transmitBuffer();
    uds_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_IgnoreArg_transmitBufferSize();
    uds_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveReadDtcInformation_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x19, 0x01};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x59, 0x00};
    uds_StoredDataTransmissionFunctionalUnit_ReadDtcInformation_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_StoredDataTransmissionFunctionalUnit_ReadDtcInformation_IgnoreArg_transmitBuffer();
    uds_StoredDataTransmissionFunctionalUnit_ReadDtcInformation_IgnoreArg_transmitBufferSize();
    uds_StoredDataTransmissionFunctionalUnit_ReadDtcInformation_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveReadDataByIdentifier_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x22, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x62, 0x00, 0x11, 0x22, 0x33, 0x44};
    uds_DataTransmissionFunctionalUnit_ReadDataByIdentifier_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DataTransmissionFunctionalUnit_ReadDataByIdentifier_IgnoreArg_transmitBuffer();
    uds_DataTransmissionFunctionalUnit_ReadDataByIdentifier_IgnoreArg_transmitBufferSize();
    uds_DataTransmissionFunctionalUnit_ReadDataByIdentifier_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveReadMemoryByAddress_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x23, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x63, 0x00, 0x11, 0x22, 0x33, 0x44};
    uds_DataTransmissionFunctionalUnit_ReadMemoryByAddress_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DataTransmissionFunctionalUnit_ReadMemoryByAddress_IgnoreArg_transmitBuffer();
    uds_DataTransmissionFunctionalUnit_ReadMemoryByAddress_IgnoreArg_transmitBufferSize();
    uds_DataTransmissionFunctionalUnit_ReadMemoryByAddress_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveReadScalingDataByIdentifier_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x24, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x64, 0x00, 0x11, 0x22, 0x33, 0x44};
    uds_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier_IgnoreArg_transmitBuffer();
    uds_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier_IgnoreArg_transmitBufferSize();
    uds_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveSecurityAccess_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x27, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x67, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveCommunicationControl_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x28, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x68, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveReadDataByPeriodicIdentifier_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x2A, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x6A, 0x00, 0x11, 0x22, 0x33, 0x44};
    uds_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_IgnoreArg_transmitBuffer();
    uds_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_IgnoreArg_transmitBufferSize();
    uds_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveDynamicallyDefineDataIdentifier_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x2C, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x6C, 0x00};
    uds_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_IgnoreArg_transmitBuffer();
    uds_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_IgnoreArg_transmitBufferSize();
    uds_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveWriteDataByIdentifier_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x2E, 0x00, 0x11, 0x22, 0x33, 0x44};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x6E, 0x00};
    uds_DataTransmissionFunctionalUnit_WriteDataByIdentifier_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DataTransmissionFunctionalUnit_WriteDataByIdentifier_IgnoreArg_transmitBuffer();
    uds_DataTransmissionFunctionalUnit_WriteDataByIdentifier_IgnoreArg_transmitBufferSize();
    uds_DataTransmissionFunctionalUnit_WriteDataByIdentifier_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveInputOutputControlByIdentifier_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x2F, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x6F, 0x00};
    uds_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier_IgnoreArg_transmitBuffer();
    uds_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier_IgnoreArg_transmitBufferSize();
    uds_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveRoutineControl_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x31, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0x71, 0x00};
    uds_RoutineFunctionalUnit_RoutineControl_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_RoutineFunctionalUnit_RoutineControl_IgnoreArg_transmitBuffer();
    uds_RoutineFunctionalUnit_RoutineControl_IgnoreArg_transmitBufferSize();
    uds_RoutineFunctionalUnit_RoutineControl_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveRequestDownload_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x34, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0x74, 0x00};
    uds_UploadDownloadFunctionalUnit_RequestDownload_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_UploadDownloadFunctionalUnit_RequestDownload_IgnoreArg_transmitBuffer();
    uds_UploadDownloadFunctionalUnit_RequestDownload_IgnoreArg_transmitBufferSize();
    uds_UploadDownloadFunctionalUnit_RequestDownload_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveRequestUpload_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x35, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0x75, 0x00};
    uds_UploadDownloadFunctionalUnit_RequestUpload_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_UploadDownloadFunctionalUnit_RequestUpload_IgnoreArg_transmitBuffer();
    uds_UploadDownloadFunctionalUnit_RequestUpload_IgnoreArg_transmitBufferSize();
    uds_UploadDownloadFunctionalUnit_RequestUpload_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveTransferData_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x36, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0x76, 0x00};
    uds_UploadDownloadFunctionalUnit_TransferData_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_UploadDownloadFunctionalUnit_TransferData_IgnoreArg_transmitBuffer();
    uds_UploadDownloadFunctionalUnit_TransferData_IgnoreArg_transmitBufferSize();
    uds_UploadDownloadFunctionalUnit_TransferData_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveRequestTransferExit_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x37, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0x77, 0x00};
    uds_UploadDownloadFunctionalUnit_RequestTransferExit_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_UploadDownloadFunctionalUnit_RequestTransferExit_IgnoreArg_transmitBuffer();
    uds_UploadDownloadFunctionalUnit_RequestTransferExit_IgnoreArg_transmitBufferSize();
    uds_UploadDownloadFunctionalUnit_RequestTransferExit_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveRequestFileTransfer_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x38, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0x78, 0x00};
    uds_UploadDownloadFunctionalUnit_RequestFileTransfer_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_UploadDownloadFunctionalUnit_RequestFileTransfer_IgnoreArg_transmitBuffer();
    uds_UploadDownloadFunctionalUnit_RequestFileTransfer_IgnoreArg_transmitBufferSize();
    uds_UploadDownloadFunctionalUnit_RequestFileTransfer_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveWriteMemoryByAddress_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x3D, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0x7D, 0x00};
    uds_DataTransmissionFunctionalUnit_WriteMemoryByAddress_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DataTransmissionFunctionalUnit_WriteMemoryByAddress_IgnoreArg_transmitBuffer();
    uds_DataTransmissionFunctionalUnit_WriteMemoryByAddress_IgnoreArg_transmitBufferSize();
    uds_DataTransmissionFunctionalUnit_WriteMemoryByAddress_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveTesterPresent_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x3E, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0x7E, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveAccessTimingParameter_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x83, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0xC3, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveSecuredDataTransmission_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x84, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0xC4, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveControlDtcSetting_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x85, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0xC5, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveResponseOnEvent_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x86, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0xC6, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

void test_uds_task_receiveLinkControl_callImplementation (void)
{
    uint8_t dataToReceive [] = {0x87, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();


    uint8_t dataToExpect [] = {0xC7, 0x00};
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl_ExpectWithArrayAndReturn(dataToReceive, sizeof(dataToReceive), sizeof(dataToReceive), 0,0,0,sizeof(dataToExpect));
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl_IgnoreArg_transmitBuffer();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl_IgnoreArg_transmitBufferSize();
    uds_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl_ReturnMemThruPtr_transmitBuffer(dataToExpect, sizeof(dataToExpect));

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}











