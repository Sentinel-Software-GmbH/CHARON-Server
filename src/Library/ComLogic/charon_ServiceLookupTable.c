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

/* Imports *******************************************************************/

/* Macros ********************************************************************/

#define SESSION_DEFAULT			((uint32_t) 1u << (uint8_t)charon_sscType_default)
#define SESSION_PROGRAMMING  	((uint32_t) 1u << (uint8_t)charon_sscType_programming)
#define SESSION_EXTENDED		((uint32_t) 1u << (uint8_t)charon_sscType_extended)
#define SESSION_SECURED			((uint32_t) 1u << (uint8_t)charon_sscType_secured)

/* Constants *****************************************************************/

static charon_serviceObject_t serviceLookupTable[] =
        /* SID */                                           /* Allowed Sessions */                                                          /* Service Function */                  /* Encryption */
{       {uds_sid_DiagnosticSessionControl,                  (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_EcuReset,                                  (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_ClearDiagnosticInformation,                (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_ReadDtcInformation,                        (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_ReadDataByIdentifier,                      (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_ReadMemoryByAddress,                       (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_ReadScalingDataByIdentifier,               (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_SecurityAccess,                            (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_CommunicationControl,                      (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_ReadDataByPeriodicIdentifier,              (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_DynamicallyDefineDataIdentifier,           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_WriteDataByIdentifier,                     (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_InputOutputControlByIdentifier,            (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_RoutineControl,                            (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_RequestDownload,                           (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_RequestUpload,                             (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_TransferData,                              (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_RequestTransferExit,                       (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_RequestFileTransfer,                       (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_WriteMemoryByAddress,                      (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_TesterPresent,                             (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_AccessTimingParameter,                     (0u              | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_SecuredDataTransmission,                   (0u              | 0u                  | 0u               | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_ControlDtcSetting,                         (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_ResponseOnEvent,                           (SESSION_DEFAULT | SESSION_PROGRAMMING | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
        {uds_sid_LinkControl,                               (0u              | 0u                  | SESSION_EXTENDED | SESSION_SECURED),   NULL,                                   0u},
};

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

charon_serviceObject_t* charon_ServiceLookupTable_getServiceObject( uds_sid_t sid)
{
    const uint32_t TableSize = (sizeof(serviceLookupTable) / sizeof(serviceLookupTable[0]));
    uint32_t numIterations = 32 - __builtin_clz(TableSize);
    uint32_t iterationSize = TableSize / 2u;

    charon_serviceObject_t * result = NULL;

    uint32_t TableIndex = iterationSize;

    for (;numIterations > 0u;numIterations--)
    {
        iterationSize = (iterationSize+1u) / 2u;
        if (serviceLookupTable[TableIndex].sid == sid)
        {
            result = &serviceLookupTable[TableIndex];
            break;
        }
        else if (serviceLookupTable[TableIndex].sid > sid)
        {
            TableIndex -= iterationSize;
        }
        else
        {
            TableIndex += iterationSize;
        }
    }
    return result;
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
