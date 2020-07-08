/*
 * timer.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include "HSDI/charon_interface_clock.h"

#include "stm32f407xx.h"


void timer_init (void)
{
    TIM2->PSC = 7999;
    TIM2->CR1 = 1;
}
__attribute__((section(".init_array"))) void(*timer_init_array_entry)(void) = timer_init;

uint32_t charon_interface_clock_getTime(void)
{
    return TIM2->CNT;
}

uint32_t charon_interface_clock_getTimeElapsed(uint32_t timestamp)
{
    return TIM2->CNT - timestamp;
}

