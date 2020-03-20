/*
 * charon_StoredDataTransmissionFunctionalUnit.h
 *
 *  Created on: 20.01.2020
 *      Author: Florian Kaup
 */

#ifndef CHARON_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_
#define CHARON_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_

uint32_t charon_StoredDataTransmissionFunctionalUnit_ClearDiagnosticInformation (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t charon_StoredDataTransmissionFunctionalUnit_ReadDtcInformation (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

#endif /* CHARON_STOREDDATATRANSMISSIONFUNCTIONALUNIT_H_ */
