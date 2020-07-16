/*
 * timer.c
 *
 *  Created on: 14.07.2020
 *      Author: Florian Kaup
 */


#include "HSDI/charon_interface_clock.h"
#include "timer.h"
#include "stm32f4xx_hal_dma.h"

static TIM_HandleTypeDef * handle;

void timer_init(TIM_HandleTypeDef * htim)
{
    handle = htim;
    __HAL_TIM_ENABLE(handle);
}

uint32_t charon_interface_clock_getTime(void)
{
    return __HAL_TIM_GET_COUNTER(handle);
}

uint32_t charon_interface_clock_getTimeElapsed(uint32_t timestamp)
{
    return __HAL_TIM_GET_COUNTER(handle) - timestamp;
}


