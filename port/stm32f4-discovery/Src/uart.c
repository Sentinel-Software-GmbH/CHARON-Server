/*
 * uart.c
 *
 *  Created on: 14.07.2020
 *      Author: Florian Kaup
 */

#include "uart.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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


