/*
 * charon_DiagnosticAndCommunicationManagementFunctionalUnit.h
 *
 *  Created on: 17.01.2020
 *      Author: Florian Kaup
 */

#ifndef CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_
#define CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_

#include <stdint.h>

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

int32_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_ */
