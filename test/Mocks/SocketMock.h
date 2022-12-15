/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Timo Kristall
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
 * @defgroup test
 * @{
 * @file SocketMock.h
 * This Module handles the Receive and Transfer of the charon uds stack data.
 * It Controls encryption and checks for all demanded Timeouts.
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 * 
 */
/*****************************************************************************/

#ifndef UNITTESTS_TEST_MOCKS_SOCKETMOCK_H_
#define UNITTESTS_TEST_MOCKS_SOCKETMOCK_H_

/* Includes ******************************************************************/

#include "ISocket.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

int32_t SocketMock_numOfData(void);
int32_t SocketMock_receive(uint8_t *buf, uint32_t len);
int32_t SocketMock_transmit(const uint8_t *buf, uint32_t len);

#endif /* UNITTESTS_TEST_MOCKS_SOCKETMOCK_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
