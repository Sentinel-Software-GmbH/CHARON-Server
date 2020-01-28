/*
 * uds_interface_NvmDriver.h
 *
 *  Created on: 27.01.2020
 *      Author: Florian Kaup
 */

#ifndef SRC_UDS_INTERFACE_NVMDRIVER_H_
#define SRC_UDS_INTERFACE_NVMDRIVER_H_

#include <stdint.h>
#include <stdbool.h>

bool uds_NvmDriver_checkAddressRange (uint32_t address, uint32_t length);

void uds_NvmDriver_write (uint32_t address, uint8_t* data, uint32_t size);

void uds_NvmDriver_read (uint32_t address, uint8_t* data, uint32_t size);

#endif /* SRC_UDS_INTERFACE_FLASHDRIVER_H_ */
