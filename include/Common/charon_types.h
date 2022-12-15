/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Andreas Hofmann
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
 * @defgroup Common Common
 * @{
 * @file charon_types.h
 * Provides all needed Types and declarations used by Charon UDS.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_TYPES_H_
#define CHARON_TYPES_H_

/* Includes ******************************************************************/

#include <stdint.h>
/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** Helper to get the size of an array */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)   (sizeof(x) / sizeof(x[0]))
#endif

/** Define NULL for Compatibility across libraries and Compilers */
#ifndef NULL
#define NULL        ((void *) 0)
#endif

/** Helper Macro to define a Byte Packed struct depending on compiler */
#ifdef __GNUC__
#define PACKED_STRUCT(name)             \
struct __attribute__((packed, aligned(1))) name
#endif
#ifdef __arm__
#define PACKED_STRUCT(name)             \
struct __attribute__((packed)) name
#endif

/** Helper Function to handle the big and little endian stuff*/
#ifdef __GNUC__
#define REV32(input)        (((input & 0xFF000000) >> 24) | ((input & 0x00FF0000) >> 8) | ((input & 0x0000FF00) << 8) | ((input & 0x000000FF) << 24))                       
#endif
#ifdef __arm__
#define REV32(input)       ( __rev(input))
#endif

/** Helper Function to handle the big and little endian stuff*/
#ifdef __GNUC__
static inline uint16_t REV16 (uint16_t input)
{
    uint16_t rolled = input;
    rolled = ((rolled & 0xFF00) >> 8) | ((rolled & 0x00FF) << 8);  
    return rolled;
}
#endif
#ifdef __arm__
#define REV16(input)       ( __rev16(input))
#endif
/* Types *********************************************************************/

typedef enum
{
    charon_messageType_Diagnostics,      //!< charon_messageType_Diagnostics
    charon_messageType_RemoteDiagnostics//!< charon_messageType_RemoteDiagnostics
} charon_messageType_t;

/** @brief Target addressing  */
typedef enum
{
    /** @brief Physical addressing shall always be a dedicated message to a server implemented in one ECU. When
     * physical addressing is used, the communication is a point-to-point communication between the client and the server. 
     */
    charon_targetAddressType_Physical,    
    /** @brief Functional addressing is used by the client if it does not know the physical address of the server function that
     * shall respond to a diagnostic service request or if the server function is implemented as a distributed function
     * in several ECUs. When functional addressing is used, the communication is a broadcast communication from
     * the client to a server implemented in one or more ECUs. 
     */
    charon_targetAddressType_Functional
} charon_targetAddressType_t;

/** @brief Services IDs which are allowed during the defaultSession and the non-defaultSession. */
typedef enum
{
    uds_sid_DiagnosticSessionControl = 0x10u,           /**< The service is used to enable different diagnostic sessions in the server. */
    uds_sid_EcuReset = 0x11u,                           /**< The service is used by the client to request a server reset. */ 
    uds_sid_ClearDiagnosticInformation = 0x14u,         /**< The service is used by the client to clear diagnostic information in one or multiple servers’ memory. */
    uds_sid_ReadDtcInformation = 0x19u,                 /**< The service allows a client to read the status of server resident Diagnostic Trouble Code information. */
    uds_sid_ReadDataByIdentifier = 0x22u,               /**< The service allows the client to request data record values from the server identified by one or more dataIdentifiers. */
    uds_sid_ReadMemoryByAddress = 0x23u,                /**< The service allows the client to request memory data from the server via provided starting address and size of memory to be read. */
    uds_sid_ReadScalingDataByIdentifier = 0x24u,        /**< The service allows the client to request scaling data record information from the server identified by a dataIdentifier. */    
    uds_sid_SecurityAccess = 0x27u,                     /**< The service is to provide a means to access data and/or diagnostic services, which have restricted access for security. */
    uds_sid_CommunicationControl = 0x28u,               /**< The service is to switch on/off the transmission and/or the reception of certain messages of (a) server. */        
    uds_sid_ReadDataByPeriodicIdentifier = 0x2Au,       /**< The service allows the client to request the periodic transmission of data record values from the server identified by one or more periodicDataIdentifiers. */
    uds_sid_DynamicallyDefineDataIdentifier = 0x2Cu,    /**< The service allows the client to dynamically define in a server a data identifier that can be read via the ReadDataByIdentifier service at a later time.*/    
    uds_sid_WriteDataByIdentifier = 0x2Eu,              /**< The service allows the client to write information into the server at an internal location specified by the provided data identifier. */
    uds_sid_InputOutputControlByIdentifier = 0x2Fu,     /**< The service is used by the client to substitute a value for an input signal, internal server function and/or force control to a value for an output of an electronic system.*/
    uds_sid_RoutineControl = 0x31u,                     /**< The service is used by the client to execute a defined sequence of steps and obtain any relevant results.*/
    uds_sid_RequestDownload = 0x34u,                    /**< The service is used by the client to initiate a data transfer from the client to the server (download).*/
    uds_sid_RequestUpload = 0x35u,                      /**< The service is used by the client to initiate a data transfer from the server to the client (upload). */
    uds_sid_TransferData = 0x36u,                       /**< The service is used by the client to transfer data either from the client to the server (download) or from the server to the client (upload).*/
    uds_sid_RequestTransferExit = 0x37u,                /**< The service is used by the client to terminate a data transfer between client and server (upload or download). */
    uds_sid_RequestFileTransfer = 0x38u,                /**< The service is used by the client to initiate a file data transfer from either the client to the server or from the server to the client (download or upload).*/
    uds_sid_WriteMemoryByAddress = 0x3Du,               /**< The service allows the client to write information into the server at one or more contiguous memory locations. */
    uds_sid_TesterPresent = 0x3Eu,                      /**< The service is used to indicate to a server that a client is still connected to the vehicle and that certain diagnostic services and/or communication that have been previously activated are to remain active. */
    uds_sid_PositiveResponseMask = 0x40u,               /**< The mask for positive response message.*/
    uds_sid_NegativeResponse = 0x7Fu,                   /**< The is a special parameter identifying negative service responses / confirmations.*/
    uds_sid_AccessTimingParameter = 0x83u,              /**< The service is used to read and change the default timing parameters of a communication link for the duration this communication link is active.*/
    uds_sid_SecuredDataTransmission = 0x84u,            /**< The service is to transmit data that is protected against attacks from third parties - which could endanger data security. */
    uds_sid_ControlDtcSetting = 0x85u,                  /**< The service shall be used by a client to stop or resume the updating of DTC status bits in the server(s). */
    uds_sid_ResponseOnEvent = 0x86u,                    /**< The service requests a server to start or stop transmission of responses on a specified event. */
    uds_sid_LinkControl = 0x87u,                        /**< The service is used to control the communication between the client and the server(s) in order to gain bus bandwidth for diagnostic purposes. */

    uds_sid_amount                                      /**< maximum number of available services.*/
} uds_sid_t;

/** @brief Defines all negative response codes used within this standard. Annex A ISO 14229-1:2013(E) */
typedef enum
{
    uds_responseCode_PositiveResponse = 0u,                                 /**< The parameter value is reserved for server internal implementation. */
    uds_responseCode_GeneralReject = 0x10u,                                 /**< The requested action has been rejected by the server. */
    uds_responseCode_ServiceNotSupported = 0x11u,                           /**< The requested action will not be taken because the server does not support the requested service. */
    uds_responseCode_SubfunctionNotSupported = 0x12u,                       /**< The requested action will not be taken because the server does not support the service specific parameters of the request message. */
    uds_responseCode_IncorrectMessageLengthOrInvalidFormat = 0x13u,         /**< The requested action will not be taken because the length of the received request message does not match the prescribed length. */
    uds_responseCode_ResponseTooLong = 0x14u,                               /**< Shall be reported by the server if the response to be generated exceeds the maximum number of bytes available by the underlying network layer. */
    uds_responseCode_BusyRepeatRequest = 0x21u,                             /**< The server is temporarily too busy to perform the requested operation. */
    uds_responseCode_ConditionsNotCorrect = 0x22u,                          /**< The requested action will not be taken because the server prerequisite conditions are not met. */
    uds_responseCode_RequestSequenceError = 0x24u,                          /**< The requested action will not be taken because the server expects a different sequence. */
    uds_responseCode_NoResponseFromSubnetComponent = 0x25u,                 /**< The server has received the request but the requested action could not be performed by the server as a subnet. */
    uds_responseCode_FailurePreventsExecutionOfRequestedAction = 0x26u,     /**< The requested action will not be taken because a failure condition, identified by a DTC, has occurred and that this failure condition prevents the server from performing the requested action. */
    uds_responseCode_RequestOutOfRange = 0x31u,                             /**< The requested action will not be taken because the server has detected that the request message contains a parameter which attempts to substitute a value beyond its range of authority. */
    uds_responseCode_SecurityAccessDenied = 0x33u,                          /**< The requested action will not be taken because the server's security strategy has not been satisfied by the client. */
    uds_responseCode_InvalidKey = 0x35u,                                    /**< The server has not given security access because the key sent by the client did not match with the key in the server's memory. */
    uds_responseCode_ExceededNumberOfAttempts = 0x36u,                      /**< The requested action will not be taken because the client has unsuccessfully attempted to gain security access more times than the server's security strategy will allow. */
    uds_responseCode_RequiredTimeDelayNotExpired = 0x37u,                   /**< The requested action will not be taken because the client's latest attempt to gain security access was initiated before the server's required timeout period had elapsed. */
    uds_responseCode_UploadDownloadNotAccepted = 0x70u,                     /**< An attempt to upload/download to a server's memory cannot be accomplished due to some fault conditions. */
    uds_responseCode_TransferDataSuspended = 0x71u,                         /**< A data transfer operation was halted due to some fault. The active transferData sequence shall be aborted. */
    uds_responseCode_GeneralProgrammingFailure = 0x72u,                     /**< The server detected an error when erasing or programming a memory location in the permanent memory device. */
    uds_responseCode_WrongBlockSequenceCounter = 0x73u,                     /**< The server detected an error in the sequence of blockSequenceCounter values. */
    uds_responseCode_RequestCorrectlyReceived_ResponsePending = 0x78u,      /**< The request message was received correctly, and that all parameters in the request message were valid, but the action to be performed is not yet completed and the server is not yet ready to receive another request. */
    uds_responseCode_SubfunctionNotSupportedInActiveSession = 0x7Eu,        /**< The requested action will not be taken because the server does not support the requested sub-function in the session currently active. */
    uds_responseCode_ServiceNotSupportedInActiveSession = 0x7Fu,            /**< The requested action will not be taken because the server does not support the requested service in the session currently active. */
    uds_responseCode_RpmTooHigh = 0x81u,                                    /**< The requested action will not be taken because the server prerequisite condition for RPM is not met. */
    uds_responseCode_RpmTooLow = 0x82u,                                     /**< The requested action will not be taken because the server prerequisite condition for RPM is not met. */
    uds_responseCode_EngineIsRunning = 0x83u,                               /**< Is required for those actuator tests which cannot be actuated while the Engine is running. */
    uds_responseCode_EngineIsNotRunning = 0x84u,                            /**< Is required for those actuator tests which cannot be actuated unless the Engine is running. */
    uds_responseCode_EngineRunTimeTooLow = 0x85u,                           /**< The requested action will not be taken because the server prerequisite condition for engine run time is not met. */
    uds_responseCode_TemperatureTooHigh = 0x86u,                            /**< The requested action will not be taken because the server prerequisite condition for temperature is not met. */
    uds_responseCode_TemperatureTooLow = 0x87u,                             /**< The requested action will not be taken because the server prerequisite condition for temperature is not met. */
    uds_responseCode_VehicleSpeedTooHigh = 0x88u,                           /**< The requested action will not be taken because the server prerequisite condition for vehicle speed is not met. */
    uds_responseCode_VehicleSpeedTooLow = 0x89u,                            /**< The requested action will not be taken because the server prerequisite condition for vehicle speed is not met. */
    uds_responseCode_ThrottlePedalTooHigh = 0x8Au,                          /**< The requested action will not be taken because the server prerequisite condition for throttle/pedal position is not met. */
    uds_responseCode_ThrottlePedalTooLow = 0x8Bu,                           /**< The requested action will not be taken because the server prerequisite condition for throttle/pedal position is not met. */
    uds_responseCode_TransmissionNotInNeutral = 0x8Cu,                      /**< The requested action will not be taken because the server prerequisite condition for being in neutral is not met. */
    uds_responseCode_TransmissionNotInGear = 0x8Du,                         /**< The requested action will not be taken because the server prerequisite condition for being in gear is not met. */
    uds_responseCode_BrakeSwitchesNotClosed = 0x8Fu,                        /**< For safety reasons, this is required for certain tests before it begins, and must be maintained for the entire duration of the test. */
    uds_responseCode_ShifterLeverNotInPark = 0x90u,                         /**< For safety reasons, this is required for certain tests before it begins, and must be maintained for the entire duration of the test. */
    uds_responseCode_TorqueConverterClutchLocked = 0x91u,                   /**< The requested action will not be taken because the server prerequisite condition for torque converter clutch is not met. */
    uds_responseCode_VoltageTooHigh = 0x92u,                                /**< The requested action will not be taken because the server prerequisite condition for voltage at the primary pin of the server. */
    uds_responseCode_VoltageTooLow = 0x93u                                  /**< The requested action will not be taken because the server prerequisite condition for voltage at the primary pin of the server. */
} uds_responseCode_t;

/* Interfaces ****************************************************************/

#endif /* CHARON_TYPES_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/

