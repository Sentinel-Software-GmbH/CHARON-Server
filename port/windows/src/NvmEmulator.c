/**
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
 * @addtogroup Windows Port
 * @{
 * @file NVMEmulator.c
 * Implementation of NVM Emutation Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 * NvmEmulator.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "charon_interface_NvmDriver.h"
#include <string.h>

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

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

/* Private Function **********************************************************/




/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
