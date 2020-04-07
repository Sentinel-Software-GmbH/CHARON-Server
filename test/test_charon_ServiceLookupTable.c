/*
 * test_charon_ServiceLookupTable.c
 *
 *  Created on: 01.04.2020
 *      Author: Florian Kaup
 */


#include <unity.h>
#include "ComLogic/charon_ServiceLookupTable.h"
#include "mock_charon_DiagnosticAndCommunicationManagementFunctionalUnit.h"
#include "mock_charon_RoutineFunctionalUnit.h"
#include "mock_charon_UploadDownloadFunctionalUnit.h"

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
