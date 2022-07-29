/*
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Kaup
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
 * @addtogroup test
 * @{
 * @file test_charon_ServiceLookupTable.c
 * Implementation of unit tests for charon_ServiceLookupTable
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include <unity.h>
#include "charon_ServiceLookupTable.h"
#include "mock_charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "mock_charon_RoutineFunctionalUnit.h"
#include "mock_charon_UploadDownloadFunctionalUnit.h"
#include "mock_charon_InputOutputControlFunctionalUnit.h"
#include "mock_charon_StoredDataTransmissionFunctionalUnit.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void test_charon_ServiceLookupTable_getServiceObject_noEntryFound_returnsNull (void)
{
    TEST_ASSERT_EQUAL(NULL, charon_ServiceLookupTable_getServiceObject(0xff));
    TEST_ASSERT_EQUAL(NULL, charon_ServiceLookupTable_getServiceObject(0x00));
}

void test_charon_ServiceLookupTable_getServiceObject_EcuReset(void)
{
    charon_serviceObject_t* serviceObject;
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_EcuReset_ExpectAndReturn(0,0,0);

    serviceObject = charon_ServiceLookupTable_getServiceObject(uds_sid_EcuReset);
    TEST_ASSERT_EQUAL(uds_sid_EcuReset, serviceObject->sid);
    serviceObject->serviceRunable(0, 0);
}

void test_charon_ServiceLookupTable_getServiceObject_AccessTimingParameter(void)
{
    charon_serviceObject_t* serviceObject;
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_AccessTimingParameter_ExpectAndReturn(0,0,0);

    serviceObject = charon_ServiceLookupTable_getServiceObject(uds_sid_AccessTimingParameter);
    TEST_ASSERT_EQUAL(uds_sid_AccessTimingParameter, serviceObject->sid);
    serviceObject->serviceRunable(0, 0);
}

void test_charon_ServiceLookupTable_getServiceObject_SecurityAccess(void)
{
    charon_serviceObject_t* serviceObject;
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_SecurityAccess_ExpectAndReturn(0,0,0);

    serviceObject = charon_ServiceLookupTable_getServiceObject(uds_sid_SecurityAccess);
    TEST_ASSERT_EQUAL(uds_sid_SecurityAccess, serviceObject->sid);
    serviceObject->serviceRunable(0, 0);
}

void test_charon_ServiceLookupTable_getServiceObject_ComunicationControl(void)
{
    charon_serviceObject_t* serviceObject;
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_CommunicationControl_ExpectAndReturn(0,0,0);

    serviceObject = charon_ServiceLookupTable_getServiceObject(uds_sid_CommunicationControl);
    TEST_ASSERT_EQUAL(uds_sid_CommunicationControl, serviceObject->sid);
    serviceObject->serviceRunable(0, 0);
}

void test_charon_ServiceLookupTable_getServiceObject_LinkControl(void)
{
    charon_serviceObject_t* serviceObject;
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_LinkControl_ExpectAndReturn(0,0,0);

    serviceObject = charon_ServiceLookupTable_getServiceObject(uds_sid_LinkControl);
    TEST_ASSERT_EQUAL(uds_sid_LinkControl, serviceObject->sid);
    serviceObject->serviceRunable(0, 0);
}

void test_charon_ServiceLookupTable_getServiceObject_DiagnosticSessionControl(void)
{
    charon_serviceObject_t* serviceObject;
    charon_DiagnosticAndCommunicationManagementFunctionalUnit_DiagnosticSessionControl_ExpectAndReturn(0,0,0);

    serviceObject = charon_ServiceLookupTable_getServiceObject(uds_sid_DiagnosticSessionControl);
    TEST_ASSERT_EQUAL(uds_sid_DiagnosticSessionControl, serviceObject->sid);
    serviceObject->serviceRunable(0, 0);
}

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/

