/*
 * C
 *
 * Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "LLCOMM_UART.h"
#define   LLCOMM_BUFFERED_CONNECTION_IMPL LLCOMM_UART
#include "LLCOMM_BUFFERED_CONNECTION_impl.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "interrupts.h"

/* Interrupt callback --------------------------------------------------------*/

void LLCOMM_UART_callback(LLCOMM_UART* llcomm)
{
	// retrieve LLCOM environment for given comIndex
	LLCOMM_BUFFERED_CONNECTION* env = &llcomm->header;
	UART_HandleTypeDef* handle = &llcomm->hal_uart_handle;

	// check RX
	if (__HAL_UART_GET_IT(handle, UART_IT_RXNE))
	{
		// read data, clear the interrupt and send data to MicroEJ framework
		LLCOMM_BUFFERED_CONNECTION_dataReceived(env, handle->Instance->RDR);
	}

	// check TX
	// warning: LLCOMM_BUFFERED_CONNECTION_transmitBufferReady() will disable the
	// TX interrupt but the a pending TX interrupt can occur after the disable
	// action -> use a boolean to ensure to not re-enter in function
	// LLCOMM_BUFFERED_CONNECTION_transmitBufferReady()
	if (llcomm->txInterruptEnabled && __HAL_UART_GET_IT(handle, UART_IT_TXE))
	{
		// transmit buffer empty -> notify MicroEJ framework (it will disable the TX interrupt)
		LLCOMM_BUFFERED_CONNECTION_transmitBufferReady(env);
	}
}

/* API -----------------------------------------------------------------------*/

void LLCOMM_BUFFERED_CONNECTION_IMPL_initialize(LLCOMM_BUFFERED_CONNECTION* env)
{
	// pinout, clocks and interrupts have been already initialized
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getPlatformId(LLCOMM_BUFFERED_CONNECTION* env)
{
	return ((LLCOMM_UART*)env)->platformId;
}

void* LLCOMM_BUFFERED_CONNECTION_IMPL_getName(LLCOMM_BUFFERED_CONNECTION* env)
{
	// this kind of connection cannot be open from a name (only from a platform port number)
	return 0;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_configureDevice(LLCOMM_BUFFERED_CONNECTION* env, int32_t baudrate, int32_t bitsperchar, int32_t stopbits, int32_t parity)
{
	UART_HandleTypeDef* handle = &(((LLCOMM_UART*)env)->hal_uart_handle);

	// get number of bits
	switch (bitsperchar)
	{
	default:
	case LLCOMM_BUFFERED_CONNECTION_DATABITS_5:
	case LLCOMM_BUFFERED_CONNECTION_DATABITS_6:
	case LLCOMM_BUFFERED_CONNECTION_DATABITS_7:
		// not available
		return LLCOMM_BUFFERED_CONNECTION_ERROR_INIT_UNSUPPORTED_BITSPERCHAR;

	case LLCOMM_BUFFERED_CONNECTION_DATABITS_8:
		if(parity == LLCOMM_BUFFERED_CONNECTION_PARITY_NO)
		{
			handle->Init.WordLength = UART_WORDLENGTH_8B;
		}
		else
		{
			// Adds a bit to handle the parity and to not erase the MSB.
			handle->Init.WordLength = UART_WORDLENGTH_9B;
		}
		break;

	case LLCOMM_BUFFERED_CONNECTION_DATABITS_9:
		if(parity == LLCOMM_BUFFERED_CONNECTION_PARITY_NO)
		{
			handle->Init.WordLength = UART_WORDLENGTH_9B;;
		}
		else
		{
			// Not enough bits to add the parity bit.
			return LLCOMM_BUFFERED_CONNECTION_ERROR_INIT_UNSUPPORTED_BITSPERCHAR;
		}
		break;
	}

	// get number of stop bits
	switch (stopbits)
	{
	case LLCOMM_BUFFERED_CONNECTION_STOPBITS_1:
		handle->Init.StopBits = UART_STOPBITS_1 ;
		break ;

	case LLCOMM_BUFFERED_CONNECTION_STOPBITS_2:
		handle->Init.StopBits = UART_STOPBITS_2;
		break ;

	default:
		return LLCOMM_BUFFERED_CONNECTION_ERROR_INIT_UNSUPPORTED_STOPBITS;
	}

	// get parity
	switch (parity)
	{
	case LLCOMM_BUFFERED_CONNECTION_PARITY_EVEN:
		handle->Init.Parity = UART_PARITY_EVEN;
		break ;

	case LLCOMM_BUFFERED_CONNECTION_PARITY_NO:
		handle->Init.Parity = UART_PARITY_NONE;
		break ;

	case LLCOMM_BUFFERED_CONNECTION_PARITY_ODD:
		handle->Init.Parity = UART_PARITY_ODD;
		break ;

	default:
		return LLCOMM_BUFFERED_CONNECTION_ERROR_INIT_UNSUPPORTED_PARITY;
	}

	handle->Init.BaudRate = baudrate;
	handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	handle->Init.Mode = 0;	// TX & RX modes are disabled by default

	HAL_UART_Init(handle);

	return LLCOMM_BUFFERED_CONNECTION_SUCCESS;
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_enableRX(LLCOMM_BUFFERED_CONNECTION* env)
{
	((LLCOMM_UART*)env)->hal_uart_handle.Instance->CR1 |= UART_MODE_RX;
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_disableRX(LLCOMM_BUFFERED_CONNECTION* env)
{
	((LLCOMM_UART*)env)->hal_uart_handle.Instance->CR1 &= ~UART_MODE_RX;
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_enableRXDeviceInterrupt(LLCOMM_BUFFERED_CONNECTION* env)
{
	__HAL_UART_ENABLE_IT(&(((LLCOMM_UART*)env)->hal_uart_handle), UART_IT_RXNE);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_disableRXDeviceInterrupt(LLCOMM_BUFFERED_CONNECTION* env)
{
	__HAL_UART_DISABLE_IT(&(((LLCOMM_UART*)env)->hal_uart_handle), UART_IT_RXNE);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_enableTX(LLCOMM_BUFFERED_CONNECTION* env)
{
	((LLCOMM_UART*)env)->hal_uart_handle.Instance->CR1 |= UART_MODE_TX;
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_disableTX(LLCOMM_BUFFERED_CONNECTION* env)
{
	((LLCOMM_UART*)env)->hal_uart_handle.Instance->CR1 &= ~UART_MODE_TX;
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_enableTXDeviceInterrupt(LLCOMM_BUFFERED_CONNECTION* env)
{
	((LLCOMM_UART*)env)->txInterruptEnabled = 1;
	__HAL_UART_ENABLE_IT(&(((LLCOMM_UART*)env)->hal_uart_handle), UART_IT_TXE);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_disableTXDeviceInterrupt(LLCOMM_BUFFERED_CONNECTION* env)
{
	__HAL_UART_DISABLE_IT(&(((LLCOMM_UART*)env)->hal_uart_handle), UART_IT_TXE);
	((LLCOMM_UART*)env)->txInterruptEnabled = 0;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getRXBufferStartAddress(LLCOMM_BUFFERED_CONNECTION* env)
{
	return (int32_t)(((LLCOMM_UART*)env)->rxBuffer);
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getRXBufferLength(LLCOMM_BUFFERED_CONNECTION* env)
{
	return LLCOMM_UART_RX_BUFFER_SIZE;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getTXBufferStartAddress(LLCOMM_BUFFERED_CONNECTION* env)
{
	return (int32_t)(((LLCOMM_UART*)env)->txBuffer);
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getTXBufferLength(LLCOMM_BUFFERED_CONNECTION* env)
{
	return LLCOMM_UART_TX_BUFFER_SIZE;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_useTXHardwareFIFO(LLCOMM_BUFFERED_CONNECTION* env)
{
	return LLCOMM_BUFFERED_CONNECTION_NO_HARDWARE_FIFO;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_txFifoFull(LLCOMM_BUFFERED_CONNECTION* env)
{
	return LLCOMM_BUFFERED_CONNECTION_HARDWARE_FIFO_NOT_FULL;
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_sendData(LLCOMM_BUFFERED_CONNECTION* env, int32_t data)
{
	/* Transmit Data */
	((LLCOMM_UART*)env)->hal_uart_handle.Instance->TDR = (data & (uint16_t)0x01FF);
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_isTransmissionComplete(LLCOMM_BUFFERED_CONNECTION* env)
{
	return __HAL_UART_GET_FLAG(&(((LLCOMM_UART*)env)->hal_uart_handle), UART_FLAG_TC) == SET ? LLCOMM_BUFFERED_CONNECTION_TRANSMISSION_COMPLETE : LLCOMM_BUFFERED_CONNECTION_TRANSMISSION_TIMEOUT;
}

int32_t LLCOMM_BUFFERED_CONNECTION_IMPL_getNbProperties(LLCOMM_BUFFERED_CONNECTION* env)
{
	// This implementation does not provide Device properties
	return 0;
}

void* LLCOMM_BUFFERED_CONNECTION_IMPL_getProperty(LLCOMM_BUFFERED_CONNECTION* env, void* propertyName)
{
	// This implementation does not provide Device properties
	return 0;
}

void* LLCOMM_BUFFERED_CONNECTION_IMPL_getPropertyName(LLCOMM_BUFFERED_CONNECTION* env, int32_t propertyId)
{
	// This implementation does not provide Device properties
	while(1);
}

void LLCOMM_BUFFERED_CONNECTION_IMPL_released(LLCOMM_BUFFERED_CONNECTION* env)
{
	// This implementation does not support dynamic connections add/removal.
	// To enable dynamic connections, synchronization functions must be implemented.
	// See LLCOMM_impl.h
	while(1);
}

