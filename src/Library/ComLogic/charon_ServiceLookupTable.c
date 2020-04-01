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
#include <stddef.h>
#include "ComLogic/charon_ServiceLookupTable.h"
#include "BusinessLogic/charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "BusinessLogic/charon_RoutineFunctionalUnit.h"
#include "BusinessLogic/charon_UploadDownloadFunctionalUnit.h"

/* Imports *******************************************************************/

/* Macros ********************************************************************/

#define SESSION_DEFAULT			((uint32_t) 1u << (uint8_t)charon_sscType_default)
#define SESSION_PROGRAMMING  	((uint32_t) 1u << (uint8_t)charon_sscType_programming)
#define SESSION_EXTENDED		((uint32_t) 1u << (uint8_t)charon_sscType_extended)
#define SESSION_SECURED			((uint32_t) 1u << (uint8_t)charon_sscType_secured)

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
        {uds_sid_SecurityAccess,                            (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess,                       0u},
        {uds_sid_CommunicationControl,                      (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl,                 0u},
        {uds_sid_ReadDataByPeriodicIdentifier,              (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_DynamicallyDefineDataIdentifier,           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_WriteDataByIdentifier,                     (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_InputOutputControlByIdentifier,            (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_RoutineControl,                            (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_RoutineFunctionalUnit_RoutineControl,                                                    0u},
        {uds_sid_RequestDownload,                           (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_RequestDownload,                                            0u},
        {uds_sid_RequestUpload,                             (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_RequestUpload,                                              0u},
        {uds_sid_TransferData,                              (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_TransferData,                                               0u},
        {uds_sid_RequestTransferExit,                       (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_RequestTransferExit,                                        0u},
        {uds_sid_RequestFileTransfer,                       (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_UploadDownloadFunctionalUnit_RequestFileTransfer,                                        0u},
        {uds_sid_WriteMemoryByAddress,                      (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                                                                           0u},
        {uds_sid_TesterPresent,                             (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_TesterPresent,                        0u},
        {uds_sid_AccessTimingParameter,                     (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter,                0u},
        {uds_sid_SecuredDataTransmission,                   (0u              | 0u                  | 0u               | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecuredDataTransmission,              0u},
        {uds_sid_ControlDtcSetting,                         (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_ControlDtcSetting,                    0u},
        {uds_sid_ResponseOnEvent,                           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_ResponseOnEvent,                      0u},
        {uds_sid_LinkControl,                               (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl,                          0u},
};

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

charon_serviceObject_t* charon_ServiceLookupTable_getServiceObject (uint8_t sid)
{
    const uint8_t TableSize = (sizeof(serviceLookupTable) / sizeof(serviceLookupTable[0]));
    uint8_t numIterations = 32 - __builtin_clz(TableSize);
    uint8_t iterationSize = TableSize / 2u;
    int16_t TableIndex = iterationSize;
    charon_serviceObject_t * result = NULL;

    for (;numIterations > 0u;numIterations--)
    {
        iterationSize = (iterationSize+1u) / 2u;

        uint8_t castedSidEnumeration = (uint8_t)serviceLookupTable[TableIndex].sid;// TODO(Check for Misra at this point)
        if (castedSidEnumeration == sid)
        {
            result = &serviceLookupTable[TableIndex];
            break;
        }
        else if (castedSidEnumeration > sid)
        {
            TableIndex -= (int16_t)iterationSize;
            if (TableIndex < 0)
            {
                TableIndex = 0;
            }
        }
        else
        {
            TableIndex += (int16_t)iterationSize;
            if (TableIndex > ((int16_t)TableSize-1) )
            {
                TableIndex = TableSize-1u;
            }
        }
    }
    return result;
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
