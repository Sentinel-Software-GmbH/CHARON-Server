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
/** @brief Pointer to a UART_HandleTypeDef structure that contains the configuration information for the specified UART module. */
static UART_HandleTypeDef * handle;
/** @brief Allocate Stack for buffer in bytes*/
static uint8_t receiveBuffer[4096];
/** @brief Save size buffer on stack */
static uint16_t receiveLength;
/** @brief Flag to check if data was received */
static bool receiveData;
/** @brief Flag to check if receiving process was finished.*/
static bool receiveFinished;
/** @brief Save transmit buffer pointer on stack*/
static const uint8_t * transmitBuffer;
/** @brief Amount of data in transmit buffer*/
static uint16_t transmitLength;
/** @brief Flag to check if data transmit is ongoing*/
static bool transmitData;

/* Private Function Definitions **********************************************/

/** @brief Function  to  get number of available bytes. 
 * 
 * @return Number of bytes available.
 */
static int32_t uart_numAvailableBytes (void);

/** @brief Function to receive Date.
 * 
 * @param data buffer containing received data.
 * @param maxSize Number of maximum receivable bytes.
 * @return Actual number of bytes received.
 */
static int32_t uart_receive (uint8_t* data, uint32_t maxSize);

/** @brief Function to transmit data.  
 * 
 * @param data Contains data to transmit.
 * @param size Number of bytes to transmit.
 * @return Amount of data elements to be sent.
 */
static int32_t uart_transmit (const uint8_t* data, uint32_t size);

/** @brief The function is used to raise an Interrupt when DMA is ready and set a flag to signal receiving process is finished.
 * 
 * @param huart UART_HandleTypeDef.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

/** @brief The function is used to raise an Interrupt when DMA is ready and set a flag to signal transmission is finished.
 * 
 * @param huart UART_HandleTypeDef.
 */
void uart_transmitInterrupt (UART_HandleTypeDef *huart);

/** @brief declaration of abstract target hardware interface*/
ISocket_t uartSocket = {
        uart_numAvailableBytes,
        uart_receive,
        uart_transmit
};

/* Interfaces  ***************************************************************/
/** @brief Initialize target uart. 
 * 
 * @param huart UART_HandleTypeDef
 */
void uart_init(UART_HandleTypeDef *huart)
{
    handle = huart;
    HAL_UART_Receive_DMA(handle,receiveBuffer, sizeof(receiveLength));
}

/* Private Function **********************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (receiveData)
    {
        HAL_UART_Receive_DMA(handle,receiveBuffer, sizeof(receiveLength));
        receiveFinished = true;
        receiveData = false;
    }
    else
    {
        HAL_UART_Receive_DMA(handle,receiveBuffer,sizeof(receiveLength));
        receiveData = true;
    }
}

void uart_transmitInterrupt (UART_HandleTypeDef *huart)
{
    if (!transmitData)
    {
        HAL_UART_Transmit_DMA(handle,(uint8_t*)transmitBuffer,sizeof(transmitLength));
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
    transmitLength = size;
    HAL_UART_Transmit_DMA(handle,(uint8_t*)transmitBuffer,sizeof(transmitLength));
    return size;
}


/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
