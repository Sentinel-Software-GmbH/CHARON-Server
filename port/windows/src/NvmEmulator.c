/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Rene Mutscheller
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
 * @addtogroup Windows Port
 * @{
 * @file NVMEmulator.c
 * Implementation of NVM Emulation Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 * NvmEmulator.c
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "charon_interface_NvmDriver.h"
#include <string.h>

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/
#define STORAGE_HEADER ((uint8_t) 48u) 

/* Types *********************************************************************/


/* Variables *****************************************************************/

static uint8_t NvmEmulator_MemorySpace[2*1024*1024];

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

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

uint32_t charon_NvmDriver_getAddress (uint32_t DTCnumber)
{
    uint8_t position = DTCnumber;
    uint8_t length = 48u;
    uint32_t address = 0u;


    address = (uint32_t)&NvmEmulator_MemorySpace[((position*length)+STORAGE_HEADER)];


    return address;
}

uint32_t charon_NvmDriver_getNvmAddress (void)
{
    return (uint32_t) &NvmEmulator_MemorySpace[0];
}
/* Private Function **********************************************************/

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
