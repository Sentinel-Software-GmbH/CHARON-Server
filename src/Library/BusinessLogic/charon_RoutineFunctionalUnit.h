/*
 * charon_RoutineFunctionalUnit.h
 *
 *  Created on: 20.01.2020
 *      Author: Florian Kaup
 */

#ifndef CHARON_ROUTINEFUNCTIONALUNIT_H_
#define CHARON_ROUTINEFUNCTIONALUNIT_H_

#include <stdint.h>

uint32_t charon_RoutineFunctionalUnit_RoutineControl (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

#endif /* CHARON_ROUTINEFUNCTIONALUNIT_H_ */
