/*
 * test_charon_ServiceLookupTable.c
 *
 *  Created on: 01.04.2020
 *      Author: Florian Kaup
 */


#include <unity.h>
#include "ComLogic/charon_ServiceLookupTable.h"



void test_charon_ServiceLookupTable_getServiceObject_noEntryFound_returnsNull (void)
{
    TEST_ASSERT_EQUAL(NULL, charon_ServiceLookupTable_getServiceObject(0xff));
}

void test_charon_ServiceLookupTable_getServiceObject_EcuReset(void)
{
    TEST_ASSERT_EQUAL(uds_sid_EcuReset, charon_ServiceLookupTable_getServiceObject(uds_sid_EcuReset)->sid);
}

void test_charon_ServiceLookupTable_getServiceObject_AccessTimingParameter(void)
{
    TEST_ASSERT_EQUAL(uds_sid_AccessTimingParameter, charon_ServiceLookupTable_getServiceObject(uds_sid_AccessTimingParameter)->sid);
}

void test_charon_ServiceLookupTable_getServiceObject_SecurityAccess(void)
{
    TEST_ASSERT_EQUAL(uds_sid_SecurityAccess, charon_ServiceLookupTable_getServiceObject(uds_sid_SecurityAccess)->sid);
}

void test_charon_ServiceLookupTable_getServiceObject_RequestDownload(void)
{
    TEST_ASSERT_EQUAL(uds_sid_RequestDownload, charon_ServiceLookupTable_getServiceObject(uds_sid_RequestDownload)->sid);
}

void test_charon_ServiceLookupTable_getServiceObject_ReadMemoryByAddress(void)
{
    TEST_ASSERT_EQUAL(uds_sid_ReadMemoryByAddress, charon_ServiceLookupTable_getServiceObject(uds_sid_ReadMemoryByAddress)->sid);
}

