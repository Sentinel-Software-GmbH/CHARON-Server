/*
 * timer.h
 *
 *  Created on: 14.07.2020
 *      Author: Florian Kaup
 */

#ifndef SRC_PORT_STM32F4_DISCOVERY_INC_TIMER_H_
#define SRC_PORT_STM32F4_DISCOVERY_INC_TIMER_H_

#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_tim.h"

void timer_init(TIM_HandleTypeDef * htim);

#endif /* SRC_PORT_STM32F4_DISCOVERY_INC_TIMER_H_ */
