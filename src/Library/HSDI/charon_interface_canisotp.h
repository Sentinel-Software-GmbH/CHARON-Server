/*
 * charon_interfaces.h
 *
 *  Created on: 15.01.2020
 *      Author: Florian Kaup
 */

#ifndef CHARON_INTERFACES_H_
#define CHARON_INTERFACES_H_

#include <stdint.h>

/**
 * User implementation of IsoTp receive.
 * This function must be implemented by user. It is polled for new messages
 * from UDS client.
 * @param   data    The buffer to fill with received isotp message.
 * @param   maxSize The maximum number of bytes to receive.
 * @return  The actual number of received bytes.
 */
uint16_t charon_interface_isotp_receive (uint8_t* data, uint32_t maxSize);

/**
 * User implementation of IsoTp transmit.
 * This function must be implemented by user. It is called to transmit
 * IsoTp frames to UDS client.
 * @param   data    Data to transmit.
 * @param   size    Number of bytes to transmit.
 * @return  The number of bytes successfully transmitted.
 */
uint16_t charon_interface_isotp_transmit (uint8_t* data, uint32_t size);

#endif /* CHARON_INTERFACES_H_ */
