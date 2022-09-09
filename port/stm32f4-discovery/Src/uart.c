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
 * @file uart.c
 * Implementation of uart Module
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
  */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "uart.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

/* Types *********************************************************************/

/* Variables *****************************************************************/

static UART_HandleTypeDef * handle;

static uint8_t receiveBuffer[4096];
static uint16_t receiveLength;
static bool receiveData;
static bool receiveFinished;

static const uint8_t * transmitBuffer;
static uint16_t transmitLenght;
static bool transmitData;

static int32_t uart_numAvailableBytes (void);
static int32_t uart_receive (uint8_t* data, uint32_t maxSize);
static int32_t uart_transmit (const uint8_t* data, uint32_t size);

ISocket_t uartSocket = {
        uart_numAvailableBytes,
        uart_receive,
        uart_transmit
};

/* Private Function Definitions **********************************************/

/* Interfaces  ***************************************************************/

void uart_init(UART_HandleTypeDef *huart)
{
    handle = huart;
    HAL_UART_Receive_DMA(handle,(uint8_t*)&receiveLength, sizeof(receiveLength));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (receiveData)
    {
        HAL_UART_Receive_DMA(handle,(uint8_t*)&receiveLength, sizeof(receiveLength));
        receiveFinished = true;
        receiveData = false;
    }
    else
    {
        HAL_UART_Receive_DMA(handle,receiveBuffer,receiveLength);
        receiveData = true;
    }
}

void uart_transmitInterrupt (void)
{
    if (!transmitData)
    {
        HAL_UART_Transmit_DMA(handle,(uint8_t*)transmitBuffer,transmitLenght);
        transmitData = true;
    }
    else
    {
        transmitData = false;
    }
}

static int32_t uart_numAvailableBytes (void)
{
    return INT32_MAX;
}

static int32_t uart_receive (uint8_t* data, uint32_t maxSize)
{
    uint16_t size = 0;
    if (receiveFinished)
    {
        size = receiveLength;
        if (size > maxSize)
        {
            size = maxSize;
        }
        memcpy(data, receiveBuffer, size);
    }
    return size;
}

static int32_t uart_transmit (const uint8_t* data, uint32_t size)
{
    transmitBuffer = data;
    transmitLenght = size;
    HAL_UART_Transmit_DMA(handle,(uint8_t*)&transmitLenght,sizeof(transmitLenght));
    return size;
}

/* Private Function **********************************************************/

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
