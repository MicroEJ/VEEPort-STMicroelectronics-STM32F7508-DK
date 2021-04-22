/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "stm32f7xx_hal.h"
#include "stm32f7508_discovery.h"
#include <stdio.h>

/* Defines -------------------------------------------------------------------*/

#define UART_TRANSMIT_TIMEOUT 0xFFFF

// USARTx clock resources
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

// USARTx RX/TX Pins
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_7
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF7_USART1


/* Global --------------------------------------------------------------------*/

UART_HandleTypeDef UartHandle;

static int putchar_initialized = 0;

/* Private API ---------------------------------------------------------------*/

static void PUTCHAR_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	// enable GPIO TX/RX clock
	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_RX_GPIO_CLK_ENABLE();

	// Enable USARTx clock
	USARTx_CLK_ENABLE();

	// UART TX GPIO pin configuration
	GPIO_InitStruct.Pin       = USARTx_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = USARTx_TX_AF;

	HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

	// UARTx RX GPIO pin configuration
	GPIO_InitStruct.Pin = USARTx_RX_PIN;
	GPIO_InitStruct.Alternate = USARTx_RX_AF;

	HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

	UartHandle.Instance        = USARTx;
	UartHandle.Init.BaudRate   = 115200;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity     = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode       = UART_MODE_TX_RX;

	if (HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		while(1); // error
	}

}

/* Public functions ----------------------------------------------------------*/

int fputc(int ch, FILE *f)
{
	if(!putchar_initialized)
	{
		PUTCHAR_init();
		putchar_initialized = 1;
	}

	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, UART_TRANSMIT_TIMEOUT);

	return ch;
}

int putchar(int ch)
{
	if (!putchar_initialized)
	{
		PUTCHAR_init();
		putchar_initialized = 1;
	}

	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, UART_TRANSMIT_TIMEOUT);

	return ch;
}

int _write(int file, char *data, int len)
{
    int bytes_written;

    for (bytes_written = 0; bytes_written < len; bytes_written++)
    {
		putchar(*data);
		data++;
    }

    return bytes_written;
}
