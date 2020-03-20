/*
 * uds.c
 *
 *  Created on: 15.01.2020
 *      Author: Florian Kaup
 */


#include "charon_uds.h"
#include "Common/charon_negativeResponse.h"
#include "Common/charon_types.h"
#include "HSDI/charon_interface_canisotp.h"

#include "BusinessLogic/charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "BusinessLogic/charon_DataTransmissionFunctionalUnit.h"
#include "BusinessLogic/charon_StoredDataTransmissionFunctionalUnit.h"
#include "BusinessLogic/charon_InputOutputControlFunctionalUnit.h"
#include "BusinessLogic/charon_RoutineFunctionalUnit.h"
#include "BusinessLogic/charon_UploadDownloadFunctionalUnit.h"

#include <stdint.h>

static uint8_t s_receiveBuffer[4096];
static uint8_t s_transmitBuffer[4096];



void charon_task (void)
{
    uint32_t recevieByteCount = charon_isotp_receive(s_receiveBuffer, sizeof(s_receiveBuffer));

    if (recevieByteCount != 0u)
    {
        uint32_t transmitByteCount = 0;
        switch (s_receiveBuffer[0])
        {
        case charon_sid_DiagnosticSessionControl:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_EcuReset:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_ClearDiagnosticInformation:
            transmitByteCount = charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_ReadDtcInformation:
            transmitByteCount = charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_ReadDataByIdentifier:
            transmitByteCount = charon_DataTransmissionFunctionalUnit_ReadDataByIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_ReadMemoryByAddress:
            transmitByteCount = charon_DataTransmissionFunctionalUnit_ReadMemoryByAddress(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_ReadScalingDataByIdentifier:
            transmitByteCount = charon_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_SecurityAccess:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_CommunicationControl:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_ReadDataByPeriodicIdentifier:
            transmitByteCount = charon_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_DynamicallyDefineDataIdentifier:
            transmitByteCount = charon_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_WriteDataByIdentifier:
            transmitByteCount = charon_DataTransmissionFunctionalUnit_WriteDataByIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_InputOutputControlByIdentifier:
            transmitByteCount = charon_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_RoutineControl:
            transmitByteCount = charon_RoutineFunctionalUnit_RoutineControl(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_RequestDownload:
            transmitByteCount = charon_UploadDownloadFunctionalUnit_RequestDownload(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_RequestUpload:
            transmitByteCount = charon_UploadDownloadFunctionalUnit_RequestUpload(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_TransferData:
            transmitByteCount = charon_UploadDownloadFunctionalUnit_TransferData(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_RequestTransferExit:
            transmitByteCount = charon_UploadDownloadFunctionalUnit_RequestTransferExit(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_RequestFileTransfer:
            transmitByteCount = charon_UploadDownloadFunctionalUnit_RequestFileTransfer(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_WriteMemoryByAddress:
            transmitByteCount = charon_DataTransmissionFunctionalUnit_WriteMemoryByAddress(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_TesterPresent:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_AccessTimingParameter:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_SecuredDataTransmission:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_ControlDtcSetting:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_ResponseOnEvent:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        case charon_sid_LinkControl:
            transmitByteCount = charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl(s_receiveBuffer, recevieByteCount, s_transmitBuffer, sizeof(s_transmitBuffer));
            break;
        default:
            transmitByteCount = charon_generateNegativeResponse(charon_responseCode_ServiceNotSupported, s_receiveBuffer[0], s_transmitBuffer);
            break;
        }

        if (transmitByteCount > 0u)
        {
            (void)charon_isotp_transmit(s_transmitBuffer,transmitByteCount);
        }
    }

    return;
}

