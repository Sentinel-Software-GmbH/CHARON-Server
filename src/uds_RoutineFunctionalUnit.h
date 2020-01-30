/*
 * uds_RoutineFunctionalUnit.h
 *
 *  Created on: 20.01.2020
 *      Author: Florian Kaup
 */

#ifndef UDS_ROUTINEFUNCTIONALUNIT_H_
#define UDS_ROUTINEFUNCTIONALUNIT_H_

#include <stdint.h>

uint32_t uds_RoutineFunctionalUnit_RoutineControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

#endif /* UDS_ROUTINEFUNCTIONALUNIT_H_ */
