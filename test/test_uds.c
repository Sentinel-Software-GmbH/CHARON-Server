/*
 * test_uds.c
 *
 *  Created on: 15.01.2020
 *      Author: Florian Kaup
 */


#include <unity.h>
#include "uds.h"

#include "mock_uds_interfaces.h"

void test_uds_task_nothingToDo (void)
{
    uds_isotp_receive_ExpectAndReturn(0,0,0);
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uds_task();
}

void test_uds_task_receiveUnknown_sendNotImplemented (void)
{
    uint8_t dataToReceive [] = {0x00, 0x33, 0x44, 0x88, 0x99, 0x00, 0xBA, 0x00};
    uds_isotp_receive_ExpectAndReturn(0,0,sizeof(dataToReceive));
    uds_isotp_receive_ReturnMemThruPtr_data(dataToReceive, sizeof(dataToReceive));
    uds_isotp_receive_IgnoreArg_data();
    uds_isotp_receive_IgnoreArg_maxSize();

    uint8_t dataToExpect [] = {0x88, 0x99, 0x33, 0x44, 0x00, 0x7F, 0xBA, 0x00};

    uds_isotp_transmit_ExpectWithArrayAndReturn(dataToExpect, sizeof(dataToExpect), sizeof(dataToExpect), sizeof(dataToExpect));

    uds_task();
}

