/*
 * uart.c
 *
 *  Created on: 08.07.2020
 *      Author: Florian Kaup
 */

#include "HSDI/charon_interface_canisotp.h"

#include "stm32f407xx.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint16_t receivelength;
static bool receiveData;
static bool receiveAvailable;
static uint8_t receivebuffer[4096];

static uint16_t transmitlength;
static bool transmitData;
static const uint8_t * transmitpointer;


void uart_init (void)
{
    // enable uart 2 on PA2 (tx) and PA3 (rx)
    RCC->AHB1ENR |= 0x1;
    GPIOA->MODER |= 0xA0;
    GPIOA->AFR[0] |= 0x7700;

    // dma1 stream5(rx) stream6(tx) on trigger channel 4
    RCC->AHB1ENR |= 0x200000;
    DMA1_Stream5->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream5->M0AR = (uint32_t)&receivelength;
    DMA1_Stream5->NDTR = 2;
    DMA1_Stream5->CR = 0x09800411;

    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;
    DMA1_Stream6->CR = 0x098004D0;

    // initialize uart 2
    RCC->APB1ENR |= 0x20000;
    USART2->BRR = 0x85;         // 8Mhz source clock, 115.2 kbaud, over8 = 1
    USART2->CR3 = 0xC0;         // enable dma usage
    USART2->CR1 = 0xA00C;       // enable hardware

    // enable interrupts
    NVIC->IP[DMA1_Stream5_IRQn] = 0x30;
    NVIC->IP[DMA1_Stream6_IRQn] = 0x30;
    NVIC->ISER[DMA1_Stream5_IRQn/32] |= 1uL << (DMA1_Stream5_IRQn%32);
    NVIC->ISER[DMA1_Stream6_IRQn/32] |= 1uL << (DMA1_Stream6_IRQn%32);
}
__attribute__((section(".init_array"))) void(*uart_init_array_entry)(void) = uart_init;

void uart_receiveInterrupt (void)
{
    DMA1->HIFCR = 0x0800;
    if (!receiveData)
    {
        DMA1_Stream5->M0AR = (uint32_t)receivebuffer;
        DMA1_Stream5->NDTR = receivelength;
        DMA1_Stream5->CR |= 1;
        receiveData = true;
    }
    else
    {
        receiveAvailable = true;
        DMA1_Stream5->M0AR = (uint32_t)&receivelength;
        DMA1_Stream5->NDTR = 2;
    }
}

void uart_transmitInterrupt (void)
{
    DMA1->HIFCR = 0x200000;
    if (!transmitData)
    {
        DMA1_Stream6->M0AR = (uint32_t)transmitpointer;
        DMA1_Stream6->NDTR = transmitlength;
        DMA1_Stream6->CR |= 1;
        transmitData = true;
    }
    else
    {
        transmitData = false;
    }
}

uint16_t charon_interface_isotp_receive (uint8_t* data, uint32_t maxSize)
{
    uint16_t size = 0;
    if (receiveAvailable)
    {
        size = receivelength;
        if (size > maxSize)
        {
            size = maxSize;
        }
        memcpy(data, receivebuffer, size);
    }
    return size;
}

uint16_t charon_interface_isotp_transmit (uint8_t* data, uint32_t size)
{
    transmitpointer = data;
    transmitlength = size;
    DMA1_Stream6->M0AR = (uint32_t)&transmitlength;
    DMA1_Stream6->NDTR = 2;
    DMA1_Stream6->CR |= 1;

    return size;
}




