/*
 * SocketMock.h
 *
 *  Created on: 09.04.2019
 *      Author: Timo Kristall
 */

#ifndef UNITTESTS_TEST_MOCKS_SOCKETMOCK_H_
#define UNITTESTS_TEST_MOCKS_SOCKETMOCK_H_
#include "ISocket.h"

int32_t SocketMock_numOfData(void);
int32_t SocketMock_receive(uint8_t *buf, uint32_t len);
int32_t SocketMock_transmit(const uint8_t *buf, uint32_t len);

#endif /* UNITTESTS_TEST_MOCKS_SOCKETMOCK_H_ */
