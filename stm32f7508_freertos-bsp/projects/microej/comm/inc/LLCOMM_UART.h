/*
 * C
 *
 * Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLCOMM_UART
#define _LLCOMM_UART

/* Includes ------------------------------------------------------------------*/

#include "LLCOMM_BUFFERED_CONNECTION.h"
#include "LLCOMM_UART_configuration.h"
#include "stm32f7xx_hal.h"

/* Defines and Macros --------------------------------------------------------*/

/*
 * Defines the software FIFO size for transmission used by MicroEJ framework.
 * Default size is 5 bytes.
 */
#ifndef LLCOMM_UART_TX_BUFFER_SIZE
#define LLCOMM_UART_TX_BUFFER_SIZE 5
#endif

/*
 * Defines the software FIFO size for reception used by MicroEJ framework
 * Default size is 1024 bytes.
 */
#ifndef LLCOMM_UART_RX_BUFFER_SIZE
#define LLCOMM_UART_RX_BUFFER_SIZE 1*1024
#endif

/* Structures ----------------------------------------------------------------*/

typedef struct LLCOMM_UART
{
	// MicroEJ LLCOMM connection header
	struct LLCOMM_BUFFERED_CONNECTION header;

	// HAL UART instance
	UART_HandleTypeDef hal_uart_handle;

	// HAL UART ID
	uint8_t platformId;

	// Transmit interrupt status
	uint8_t txInterruptEnabled;

	// Software FIFO size for transmission used by MicroEJ framework
	uint8_t txBuffer[LLCOMM_UART_TX_BUFFER_SIZE];

	// Software FIFO size for reception used by MicroEJ framework
	uint8_t rxBuffer[LLCOMM_UART_RX_BUFFER_SIZE];

} LLCOMM_UART;

/* Public functions ----------------------------------------------------------*/

/*
 * Generic UART interrupt callback
 */
void LLCOMM_UART_callback(LLCOMM_UART* llcomm);

/* EOF -----------------------------------------------------------------------*/

#endif // _LLCOMM_UART
