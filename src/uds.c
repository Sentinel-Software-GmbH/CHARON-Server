/*
 * uds.c
 *
 *  Created on: 15.01.2020
 *      Author: Florian Kaup
 */


#include "uds.h"
#include "uds_types.h"
#include "uds_interfaces.h"

#include "uds_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "uds_DataTransmissionFunctionalUnit.h"
#include "uds_StoredDataTransmissionFunctionalUnit.h"
#include "uds_InputOutputControlFunctionalUnit.h"
#include "uds_RoutineFunctionalUnit.h"
#include "uds_UploadDownloadFunctionalUnit.h"

#include <stdint.h>

static uint8_t s_receiveBuffer[4096];
static uint8_t s_transmitBuffer[4096];


uint32_t uds_SidNotSupported (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize)
{
    transmitBuffer[0] = uds_sid_NegativeResponse;
    transmitBuffer[1] = s_receiveBuffer[0];
    transmitBuffer[2] = uds_responseCode_ServiceNotSupported;
    return 3;
}

void uds_task (void)
{
    uint32_t recevieByteCount = uds_isotp_receive(s_receiveBuffer, sizeof(s_receiveBuffer));
    uint32_t transmitByteCount = 0;

    if (recevieByteCount != 0)
    {
        switch (s_receiveBuffer[0])
        {
        case uds_sid_DiagnosticSessionControl:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_EcuReset:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_ClearDiagnosticInformation:
            transmitByteCount = uds_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_ReadDtcInformation:
            transmitByteCount = uds_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_ReadDataByIdentifier:
            transmitByteCount = uds_DataTransmissionFunctionalUnit_ReadDataByIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_ReadMemoryByAddress:
            transmitByteCount = uds_DataTransmissionFunctionalUnit_ReadMemoryByAddress(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_ReadScalingDataByIdentifier:
            transmitByteCount = uds_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_SecurityAccess:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_CommunicationControl:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_ReadDataByPeriodicIdentifier:
            transmitByteCount = uds_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_DynamicallyDefineDataIdentifier:
            transmitByteCount = uds_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_WriteDataByIdentifier:
            transmitByteCount = uds_DataTransmissionFunctionalUnit_WriteDataByIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_InputOutputControlByIdentifier:
            transmitByteCount = uds_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_RoutineControl:
            transmitByteCount = uds_RoutineFunctionalUnit_RoutineControl(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_RequestDownload:
            transmitByteCount = uds_UploadDownloadFunctionalUnit_RequestDownload(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_RequestUpload:
            transmitByteCount = uds_UploadDownloadFunctionalUnit_RequestUpload(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_TransferData:
            transmitByteCount = uds_UploadDownloadFunctionalUnit_TransferData(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_RequestTransferExit:
            transmitByteCount = uds_UploadDownloadFunctionalUnit_RequestTransferExit(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_RequestFileTransfer:
            transmitByteCount = uds_UploadDownloadFunctionalUnit_RequestFileTransfer(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_WriteMemoryByAddress:
            transmitByteCount = uds_DataTransmissionFunctionalUnit_WriteMemoryByAddress(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_TesterPresent:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_AccessTimingParameter:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_SecuredDataTransmission:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_ControlDtcSetting:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_ResponseOnEvent:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case uds_sid_LinkControl:
            transmitByteCount = uds_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        default:
            transmitByteCount = uds_SidNotSupported(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        }

        if (transmitByteCount > 0)
        {
            uds_isotp_transmit(s_transmitBuffer,transmitByteCount);
        }
    }

    return;
}

