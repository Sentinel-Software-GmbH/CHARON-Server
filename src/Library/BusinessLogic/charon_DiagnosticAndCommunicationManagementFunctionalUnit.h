/*
 * uds_DiagnosticAndCommunicationManagementFunctionalUnit.h
 *
 *  Created on: 17.01.2020
 *      Author: Florian Kaup
 */

#ifndef UDS_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_
#define UDS_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_

#include <stdint.h>

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

#endif /* UDS_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_ */
