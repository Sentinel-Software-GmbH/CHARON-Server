/*
 * charon_types.h
 *
 *  Created on: 16.01.2020
 *      Author: Florian Kaup
 */

#ifndef CHARON_TYPES_H_
#define CHARON_TYPES_H_

typedef enum
{
    charon_messageType_Diagnostics,
    charon_messageType_RemoteDiagnostics,
} charon_messageType_t;

typedef enum
{
    charon_targetAddressType_Physical,
    charon_targetAddressType_Functional,
} charon_targetAddressType_t;

typedef enum
{
    charon_sid_DiagnosticSessionControl = 0x10,
    charon_sid_EcuReset = 0x11,
    charon_sid_ClearDiagnosticInformation = 0x14,
    charon_sid_ReadDtcInformation = 0x19,
    charon_sid_ReadDataByIdentifier = 0x22,
    charon_sid_ReadMemoryByAddress = 0x23,
    charon_sid_ReadScalingDataByIdentifier = 0x24,
    charon_sid_SecurityAccess = 0x27,
    charon_sid_CommunicationControl = 0x28,
    charon_sid_ReadDataByPeriodicIdentifier = 0x2A,
    charon_sid_DynamicallyDefineDataIdentifier = 0x2C,
    charon_sid_WriteDataByIdentifier = 0x2E,
    charon_sid_InputOutputControlByIdentifier = 0x2F,
    charon_sid_RoutineControl = 0x31,
    charon_sid_RequestDownload = 0x34,
    charon_sid_RequestUpload = 0x35,
    charon_sid_TransferData = 0x36,
    charon_sid_RequestTransferExit = 0x37,
    charon_sid_RequestFileTransfer = 0x38,
    charon_sid_WriteMemoryByAddress = 0x3D,
    charon_sid_TesterPresent = 0x3E,
    charon_sid_PositiveResponseMask = 0x40,
    charon_sid_NegativeResponse = 0x7F,
    charon_sid_AccessTimingParameter = 0x83,
    charon_sid_SecuredDataTransmission = 0x84,
    charon_sid_ControlDtcSetting = 0x85,
    charon_sid_ResponseOnEvent = 0x86,
    charon_sid_LinkControl = 0x87,
} charon_sid_t;

typedef enum
{
    charon_responseCode_PositiveResponse = 0,
    charon_responseCode_GeneralReject = 0x10,
    charon_responseCode_ServiceNotSupported = 0x11,
    charon_responseCode_SubfunctionNotSupported = 0x12,
    charon_responseCode_IncorrectMessageLengthOrInvalidFormat = 0x13,
    charon_responseCode_ResponseTooLong = 0x14,
    charon_responseCode_BusyRepeatRequest = 0x21,
    charon_responseCode_ConditionsNotCorrect = 0x22,
    charon_responseCode_RequestSequenceError = 0x24,
    charon_responseCode_NoResponseFromSubnetComponent = 0x25,
    charon_responseCode_FailurePreventsExecutionOfRequestedAction = 0x26,
    charon_responseCode_RequestOutOfRange = 0x31,
    charon_responseCode_SecurityAccessDenied = 0x33,
    charon_responseCode_InvalidKey = 0x35,
    charon_responseCode_ExceededNumberOfAttempts = 0x36,
    charon_responseCode_RequiredTimeDelayNotExpired = 0x37,
    charon_responseCode_UploadDownloadNotAccepted = 0x70,
    charon_responseCode_TransferDataSuspended = 0x71,
    charon_responseCode_GeneralProgrammingFailure = 0x72,
    charon_responseCode_WrongBlockSequenceCounter = 0x73,
    charon_responseCode_RequestCorrectlyReceived_ResponsePending = 0x78,
    charon_responseCode_SubfunctionNotSupportedInActiveSession = 0x7E,
    charon_responseCode_ServiceNotSupportedInActiveSession = 0x7F,
    charon_responseCode_RpmTooHigh = 0x81,
    charon_responseCode_RpmTooLow = 0x82,
    charon_responseCode_EngineIsRunning = 0x83,
    charon_responseCode_EngineIsNotRunning = 0x84,
    charon_responseCode_EngineRunTimeTooLow = 0x85,
    charon_responseCode_TemperatureTooHigh = 0x86,
    charon_responseCode_TemperatureTooLow = 0x87,
    charon_responseCode_VehicleSpeedTooHigh = 0x88,
    charon_responseCode_VehicleSpeedTooLow = 0x89,
    charon_responseCode_ThrottlePedalTooHigh = 0x8A,
    charon_responseCode_ThrottlePedalTooLow = 0x8B,
    charon_responseCode_TransmissionNotInNeutral = 0x8C,
    charon_responseCode_TransmissionNotInGear = 0x8D,
    charon_responseCode_BrakeSwitchesNotClosed = 0x8F,
    charon_responseCode_ShifterLeverNotInPark = 0x90,
    charon_responseCode_TorqueConverterClutchLocked = 0x91,
    charon_responseCode_VoltageTooHigh = 0x92,
    charon_responseCode_VoltageTooLow = 0x93,
} charon_responseCode_t;




#endif /* CHARON_TYPES_H_ */
