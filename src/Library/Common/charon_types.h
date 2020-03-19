/*
 * uds_types.h
 *
 *  Created on: 16.01.2020
 *      Author: Florian Kaup
 */

#ifndef UDS_TYPES_H_
#define UDS_TYPES_H_

typedef enum
{
    uds_messageType_Diagnostics,
    uds_messageType_RemoteDiagnostics,
} uds_messageType_t;

typedef enum
{
    uds_targetAddressType_Physical,
    uds_targetAddressType_Functional,
} uds_targetAddressType_t;

typedef enum
{
    uds_sid_DiagnosticSessionControl = 0x10,
    uds_sid_EcuReset = 0x11,
    uds_sid_ClearDiagnosticInformation = 0x14,
    uds_sid_ReadDtcInformation = 0x19,
    uds_sid_ReadDataByIdentifier = 0x22,
    uds_sid_ReadMemoryByAddress = 0x23,
    uds_sid_ReadScalingDataByIdentifier = 0x24,
    uds_sid_SecurityAccess = 0x27,
    uds_sid_CommunicationControl = 0x28,
    uds_sid_ReadDataByPeriodicIdentifier = 0x2A,
    uds_sid_DynamicallyDefineDataIdentifier = 0x2C,
    uds_sid_WriteDataByIdentifier = 0x2E,
    uds_sid_InputOutputControlByIdentifier = 0x2F,
    uds_sid_RoutineControl = 0x31,
    uds_sid_RequestDownload = 0x34,
    uds_sid_RequestUpload = 0x35,
    uds_sid_TransferData = 0x36,
    uds_sid_RequestTransferExit = 0x37,
    uds_sid_RequestFileTransfer = 0x38,
    uds_sid_WriteMemoryByAddress = 0x3D,
    uds_sid_TesterPresent = 0x3E,
    uds_sid_PositiveResponseMask = 0x40,
    uds_sid_NegativeResponse = 0x7F,
    uds_sid_AccessTimingParameter = 0x83,
    uds_sid_SecuredDataTransmission = 0x84,
    uds_sid_ControlDtcSetting = 0x85,
    uds_sid_ResponseOnEvent = 0x86,
    uds_sid_LinkControl = 0x87,
} uds_sid_t;

typedef enum
{
    uds_responseCode_PositiveResponse = 0,
    uds_responseCode_GeneralReject = 0x10,
    uds_responseCode_ServiceNotSupported = 0x11,
    uds_responseCode_SubfunctionNotSupported = 0x12,
    uds_responseCode_IncorrectMessageLengthOrInvalidFormat = 0x13,
    uds_responseCode_ResponseTooLong = 0x14,
    uds_responseCode_BusyRepeatRequest = 0x21,
    uds_responseCode_ConditionsNotCorrect = 0x22,
    uds_responseCode_RequestSequenceError = 0x24,
    uds_responseCode_NoResponseFromSubnetComponent = 0x25,
    uds_responseCode_FailurePreventsExecutionOfRequestedAction = 0x26,
    uds_responseCode_RequestOutOfRange = 0x31,
    uds_responseCode_SecurityAccessDenied = 0x33,
    uds_responseCode_InvalidKey = 0x35,
    uds_responseCode_ExceededNumberOfAttempts = 0x36,
    uds_responseCode_RequiredTimeDelayNotExpired = 0x37,
    uds_responseCode_UploadDownloadNotAccepted = 0x70,
    uds_responseCode_TransferDataSuspended = 0x71,
    uds_responseCode_GeneralProgrammingFailure = 0x72,
    uds_responseCode_WrongBlockSequenceCounter = 0x73,
    uds_responseCode_RequestCorrectlyReceived_ResponsePending = 0x78,
    uds_responseCode_SubfunctionNotSupportedInActiveSession = 0x7E,
    uds_responseCode_ServiceNotSupportedInActiveSession = 0x7F,
    uds_responseCode_RpmTooHigh = 0x81,
    uds_responseCode_RpmTooLow = 0x82,
    uds_responseCode_EngineIsRunning = 0x83,
    uds_responseCode_EngineIsNotRunning = 0x84,
    uds_responseCode_EngineRunTimeTooLow = 0x85,
    uds_responseCode_TemperatureTooHigh = 0x86,
    uds_responseCode_TemperatureTooLow = 0x87,
    uds_responseCode_VehicleSpeedTooHigh = 0x88,
    uds_responseCode_VehicleSpeedTooLow = 0x89,
    uds_responseCode_ThrottlePedalTooHigh = 0x8A,
    uds_responseCode_ThrottlePedalTooLow = 0x8B,
    uds_responseCode_TransmissionNotInNeutral = 0x8C,
    uds_responseCode_TransmissionNotInGear = 0x8D,
    uds_responseCode_BrakeSwitchesNotClosed = 0x8F,
    uds_responseCode_ShifterLeverNotInPark = 0x90,
    uds_responseCode_TorqueConverterClutchLocked = 0x91,
    uds_responseCode_VoltageTooHigh = 0x92,
    uds_responseCode_VoltageTooLow = 0x93,
} uds_responseCode_t;




#endif /* UDS_TYPES_H_ */