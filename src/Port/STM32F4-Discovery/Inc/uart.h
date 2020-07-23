/*
 * uart.h
 *
 *  Created on: 14.07.2020
 *      Author: Florian Kaup
 */

#ifndef SRC_PORT_STM32F4_DISCOVERY_INC_UART_H_
#define SRC_PORT_STM32F4_DISCOVERY_INC_UART_H_

#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_uart.h"
#include "Interface/Socket/ISocket.h"

extern ISocket_t uartSocket;

void uart_init (UART_HandleTypeDef *huart);

#endif /* SRC_PORT_STM32F4_DISCOVERY_INC_UART_H_ */
