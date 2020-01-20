/*
 * uds_DataTransmissionFunctionalUnit.h
 *
 *  Created on: 20.01.2020
 *      Author: Florian Kaup
 */

#ifndef UDS_DATATRANSMISSIONFUNCTIONALUNIT_H_
#define UDS_DATATRANSMISSIONFUNCTIONALUNIT_H_

uint32_t uds_DataTransmissionFunctionalUnit_ReadDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DataTransmissionFunctionalUnit_ReadMemoryByAddress (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DataTransmissionFunctionalUnit_ReadScalingDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DataTransmissionFunctionalUnit_ReadDataByPeriodicIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DataTransmissionFunctionalUnit_DynamicallyDefineDataIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DataTransmissionFunctionalUnit_WriteDataByIdentifier (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

uint32_t uds_DataTransmissionFunctionalUnit_WriteMemoryByAddress (uint8_t * receiveBuffer, uint32_t receiveBufferSize, uint8_t * transmitBuffer, uint32_t transmitBufferSize);

#endif /* UDS_DATATRANSMISSIONFUNCTIONALUNIT_H_ */
