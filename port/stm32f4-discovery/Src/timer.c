/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Florian Kaup
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
 * @addtogroup CharonUDS
 * @{
 * @addtogroup STM32F4Port
 * @{
 * @file timer.c
 * Implementation of timer Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
  */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "charon_interface_clock.h"
#include "timer.h"
#include "stm32f4xx_hal_dma.h"

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

static TIM_HandleTypeDef * handle;

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

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

/* Private Function **********************************************************/

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
