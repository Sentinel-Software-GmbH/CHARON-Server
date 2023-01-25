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
 * @addtogroup CharonUDS_Server
 * @{
 * @addtogroup ShowCase Show Case demo
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
#include "charon_StoredDataTransmissionFunctionalUnit.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "charon_config.h"


/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/** @brief Save area for header in the NVM. */
#define STORAGE_HEADER ((uint8_t) sizeof(DTC_header_t)) 

/** @brief NVm config, may be changed depending on targets memory. Start of Memory block. Chains the DTC memory but can also be saved independet. (still in a block!) */
#define START_OF_RESERVED_SPACE_FOR_DTC             ((uint8_t)  0u)        /** @todo change start here in merge */
/** @brief Memory size in NVM for DTC depending on AMOUNT_OF_DTC. Indicates the end. */
#define END_OF_RESERVED_SPACE_FOR_DTC               ((uint32_t)(sizeof(DTC_t)*AMOUNT_OF_DTC) + STORAGE_HEADER)

/** @brief NVm config, may be changed depending on targets memory. Start of Memory block */
#define START_OF_RESERVED_SPACE_FOR_SNAPSHOT        ((uint32_t)END_OF_RESERVED_SPACE_FOR_DTC + 1u)
/** @brief Memory size in NVM for snapshot depending on AMOUNT_OF_SNAPSHOT. Indicates the end. */
#define END_OF_RESERVED_SPACE_FOR_SNAPSHOT          ((uint32_t)START_OF_RESERVED_SPACE_FOR_SNAPSHOT + (sizeof(DTC_SnapshotData_t)*AMOUNT_OF_SNAPSHOT))

/** @brief NVm config, may be changed depending on targets memory. Start of Memory block */
#define START_OF_RESERVED_SPACE_FOR_STOREDDATA      ((uint32_t)END_OF_RESERVED_SPACE_FOR_SNAPSHOT + 1u)
/** @brief Memory size in NVM for storedData depending on AMOUNT_OF_STOREDDATA. Indicates the end. */
#define END_OF_RESERVED_SPACE_FOR_STOREDDATA        ((uint32_t)START_OF_RESERVED_SPACE_FOR_STOREDDATA + (sizeof(DTC_StoredData_t)*AMOUNT_OF_STOREDDATA))

/** @brief NVm config, may be changed depending on targets memory. Start of Memory block */
#define START_OF_RESERVED_SPACE_FOR_EXTENDEDDATA    ((uint32_t)END_OF_RESERVED_SPACE_FOR_STOREDDATA + 1u)
/** @brief Memory size in NVM for extData depending on AMOUNT_OF_EXTENDEDDATA. Indicates the end. */
#define END_OF_RESERVED_SPACE_FOR_EXTENDEDDATA      ((uint32_t)START_OF_RESERVED_SPACE_FOR_EXTENDEDDATA + (sizeof(DTC_StoredData_t)*AMOUNT_OF_EXTENDEDDATA))

/* Types *********************************************************************/


/* Variables *****************************************************************/

/** @brief Stack allocation for emulated nvm. */
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


uint32_t charon_NvmDriver_getNvmAddress (void)
{
    return (uint32_t) &NvmEmulator_MemorySpace[0];
}

uint32_t charon_NvmDriver_getMirrorNvmAddress (uint16_t input, bool header)
{
    uint8_t header_bytes = sizeof(DTC_header_t);
    uint32_t pos = sizeof(DTC_t);
    if (header)
    {
        header_bytes = 0;
    }
    return (uint32_t) &NvmEmulator_MemorySpace[START_OF_RESERVED_SPACE_FOR_DTC + header_bytes + (pos * input)]; /** @todo USER: change to your Mirror starting address.*/
}

uint32_t charon_NvmDriver_getNvmAddress_for_DTC (uint16_t input, bool header)
{
    uint8_t header_bytes = sizeof(DTC_header_t);
    uint32_t pos = sizeof(DTC_t);
    if (header)
    {
        header_bytes = 0;
    }
    return (uint32_t) &NvmEmulator_MemorySpace[START_OF_RESERVED_SPACE_FOR_DTC + header_bytes + (pos * input)];
}

uint32_t charon_NvmDriver_getNvmAddress_for_Snapshot (uint16_t input)
{
    uint32_t pos = sizeof(DTC_SnapshotData_t);
    return (uint32_t) &NvmEmulator_MemorySpace[START_OF_RESERVED_SPACE_FOR_SNAPSHOT + (pos * input)];
}

uint32_t charon_NvmDriver_getNvmAddress_for_StoredData (uint16_t input)
{
    uint32_t pos = sizeof(DTC_StoredData_t);
    return (uint32_t) &NvmEmulator_MemorySpace[START_OF_RESERVED_SPACE_FOR_STOREDDATA + (pos * input)];
}

uint32_t charon_NvmDriver_getNvmAddress_for_ExtendedData (uint16_t input)
{
    uint32_t pos = sizeof(DTC_ExtendedData_t);
    return (uint32_t) &NvmEmulator_MemorySpace[START_OF_RESERVED_SPACE_FOR_EXTENDEDDATA + (pos * input)];
}

/* Private Function **********************************************************/

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
