/*
 * NvmEmulator.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include "HSDI/charon_interface_NvmDriver.h"
#include <string.h>


static uint8_t NvmEmulator_MemorySpace[128*1024];

bool charon_NvmDriver_checkAddressRange (uint32_t address, uint32_t length)
{
    return (address + length) < sizeof(NvmEmulator_MemorySpace);
}


uds_responseCode_t charon_NvmDriver_write (uint32_t address, const uint8_t* data, uint32_t size)
{
    memcpy(&NvmEmulator_MemorySpace[address], data, size);
    return uds_responseCode_PositiveResponse;
}


void charon_NvmDriver_read (uint32_t address, uint8_t* data, uint32_t size)
{
    memcpy(data, &NvmEmulator_MemorySpace[address], size);
}


void charon_NvmDriver_erase (void)
{
    memset(NvmEmulator_MemorySpace, 0xFF, sizeof(NvmEmulator_MemorySpace));
}
