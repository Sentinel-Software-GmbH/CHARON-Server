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
 * @addtogroup CharonUDS_Server
 * @{
 * @addtogroup STM32F4Port stm32f4 discovery porting
 * @{
 * @file timer.h
 * This Module handles needed timer functionalities
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 */
/*****************************************************************************/

#ifndef SRC_PORT_STM32F4_DISCOVERY_INC_TIMER_H_
#define SRC_PORT_STM32F4_DISCOVERY_INC_TIMER_H_

/* Includes ******************************************************************/

#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_tim.h"

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Interfaces ****************************************************************/

void timer_init(TIM_HandleTypeDef * htim);

#endif /* SRC_PORT_STM32F4_DISCOVERY_INC_TIMER_H_ */

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
