/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2020 Andreas Hofmann
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
 * @addtogroup CharonUDS
 * @{
 * @addtogroup ComLogic
 * @{
 * @file charon_ServiceLookupTable
 * Implementation of the Service Lookup Table
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "ComLogic/charon_ServiceLookupTable.h"

/* Imports *******************************************************************/

/* Macros ********************************************************************/

#define SESSION_DEFAULT			((uint32_t) 1u << charon_sscType_default)
#define SESSION_PROGRAMMING  	((uint32_t) 1u << charon_sscType_programming)
#define SESSION_EXTENDED		((uint32_t) 1u << charon_sscType_extended)
#define SESSION_SECURED			((uint32_t) 1u << charon_sscType_secured)

/* Constants *****************************************************************/

static const charon_serviceObject_t serviceLookupTable[] =
{ uds_sid_DiagnosticSessionControl = 0x10, uds_sid_EcuReset = 0x11,
        uds_sid_ClearDiagnosticInformation = 0x14, uds_sid_ReadDtcInformation =
                0x19, uds_sid_ReadDataByIdentifier = 0x22,
        uds_sid_ReadMemoryByAddress = 0x23,
        uds_sid_ReadScalingDataByIdentifier = 0x24, uds_sid_SecurityAccess =
                0x27, uds_sid_CommunicationControl = 0x28,
        uds_sid_ReadDataByPeriodicIdentifier = 0x2A,
        uds_sid_DynamicallyDefineDataIdentifier = 0x2C,
        uds_sid_WriteDataByIdentifier = 0x2E,
        uds_sid_InputOutputControlByIdentifier = 0x2F, uds_sid_RoutineControl =
                0x31, uds_sid_RequestDownload = 0x34, uds_sid_RequestUpload =
                0x35, uds_sid_TransferData = 0x36, uds_sid_RequestTransferExit =
                0x37, uds_sid_RequestFileTransfer = 0x38,
        uds_sid_WriteMemoryByAddress = 0x3D, uds_sid_TesterPresent = 0x3E,
        uds_sid_PositiveResponseMask = 0x40, uds_sid_NegativeResponse = 0x7F,
        uds_sid_AccessTimingParameter = 0x83, uds_sid_SecuredDataTransmission =
                0x84, uds_sid_ControlDtcSetting = 0x85,
        uds_sid_ResponseOnEvent = 0x86, uds_sid_LinkControl = 0x87, };

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
