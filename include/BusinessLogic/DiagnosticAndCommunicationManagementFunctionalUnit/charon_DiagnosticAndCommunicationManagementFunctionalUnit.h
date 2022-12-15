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
 * @defgroup BusinessLogic Business Logic Modules
 * @{
 * @file charon_DiagnosticAndCommunicationManagementFunctionalUnit.h
 * Module handles Service Group for DCM
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_
#define CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_

/* Includes ******************************************************************/

#include <stdint.h>
#include "charon_types.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

/** @brief Resets all internal Variables and stops all ongoing Services. */
void charon_DiagnosticAndCommunicationManagementFunctionalUnit_reset (void);

/** @brief The service is used to enable different diagnostic sessions in the server.
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x10
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service is used by the client to request a server reset.
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x11
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service is to provide a means to access data and/or diagnostic services, which have restricted access for security.
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x27
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service is to switch on/off the transmission and/or the reception of certain messages of (a) server.
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x28
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service is used to indicate to a server that a client is still connected to the vehicle and that certain diagnostic services 
 *          and/or communication that have been previously activated are to remain active.
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x3E
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service is used to read and change the default timing parameters of a communication link for the duration this communication link is active.
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x83
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service is to transmit data that is protected against attacks from third parties - which could endanger data security.
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x84
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service shall be used by a client to stop or resume the updating of DTC status bits in the server(s).
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x85
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service requests a server to start or stop transmission of responses on a specified event. 
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x86
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

/** @brief The service is used to control the communication between the client and the server(s) in order to gain bus bandwidth for diagnostic purposes.
 * UDS ISO 14229-1 SE 2013-03-15
 * SID: 0x87
 *
 * @param receiveBuffer Payload
 * @param receiveBufferSize Payload Size
 * @return @see @ref uds_responseCode_t
 */
uds_responseCode_t charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl (const uint8_t * receiveBuffer, uint32_t receiveBufferSize);

#endif /* CHARON_DIAGNOSTICANDCOMMUNICATIONMANAGEMENTFUNCTIONALUNIT_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
