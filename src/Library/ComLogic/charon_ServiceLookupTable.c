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

#include <stddef.h>
#include "ComLogic/charon_ServiceLookupTable.h"
#include "BusinessLogic/charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"

/* Imports *******************************************************************/

/* Macros ********************************************************************/

#define SESSION_DEFAULT			((uint32_t) 1u << charon_sscType_default)
#define SESSION_PROGRAMMING  	((uint32_t) 1u << charon_sscType_programming)
#define SESSION_EXTENDED		((uint32_t) 1u << charon_sscType_extended)
#define SESSION_SECURED			((uint32_t) 1u << charon_sscType_secured)

/* Constants *****************************************************************/

static charon_serviceObject_t serviceLookupTable[] =
        /* SID */                                           /* Allowed Sessions */                                                          /* Service Function */                                                                          /* Encryption */
{       {uds_sid_DiagnosticSessionControl,                  (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl,             0u},
        {uds_sid_EcuReset,                                  (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset,                             0u},
        {uds_sid_ClearDiagnosticInformation,                (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_ReadDtcInformation,                        (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_ReadDataByIdentifier,                      (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_ReadMemoryByAddress,                       (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_ReadScalingDataByIdentifier,               (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_SecurityAccess,                            (0               | 0                   | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess,                       0u},
        {uds_sid_CommunicationControl,                      (0               | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl,                                   0u},
        {uds_sid_ReadDataByPeriodicIdentifier,              (0               | 0                   | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_DynamicallyDefineDataIdentifier,           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_WriteDataByIdentifier,                     (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_InputOutputControlByIdentifier,            (0               | 0                   | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_RoutineControl,                            (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_RequestDownload,                           (0               | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_RequestUpload,                             (0               | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_TransferData,                              (0               | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_RequestTransferExit,                       (0               | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_RequestFileTransfer,                       (0               | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_WriteMemoryByAddress,                      (0               | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_TesterPresent,                             (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent,                        0u},
        {uds_sid_AccessTimingParameter,                     (0               | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter,                0u},
        {uds_sid_SecuredDataTransmission,                   (0               | 0                   | 0                | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission,              0u},
        {uds_sid_ControlDtcSetting,                         (0               | 0                   | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting,                    0u},
        {uds_sid_ResponseOnEvent,                           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent,                      0u},
        {uds_sid_LinkControl,                               (0               | 0                   | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl,                          0u},
};

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

charon_serviceObject_t* charon_ServiceLookupTable_getServiceObject( uds_sid_t sid)
{
    const uint8_t TableSize = (sizeof(serviceLookupTable) / sizeof(serviceLookupTable[0]));
    charon_serviceObject_t * result = NULL;
    uint8_t TableIndex;
    for (TableIndex = 0; TableIndex < TableSize; TableIndex++)
    {
        if (serviceLookupTable[TableIndex].sid == sid)
        {
            result = &serviceLookupTable[TableIndex];
            break;
        }
    }

    return result;
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
