/*
 * C
 *
 * Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "LLCOMM_UART.h"
#include "LLCOMM_CDC.h"
#include "LLCOMM_impl.h"
#include "microej.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "usbh_core.h"
#include "usbh_cdc.h"
#include "interrupts.h"

/* Methods declaration -------------------------------------------------------*/

void LLCOMM_UART_new(LLCOMM_BUFFERED_CONNECTION* env);
void LLCOMM_CDC_new(LLCOMM_BUFFERED_CONNECTION* env);

/* Enumerations --------------------------------------------------------------*/

/*
 * Defines the mapping between the HAL UART instance and MicroEJ LLCOMM_UARTs
 * table. The indexes are 0-based.
 */
typedef enum
{
	UART6_INDEX = 0,
} hal_to_microej_uart_table;

/* Fields --------------------------------------------------------------------*/

/*
 * Array of all static LLCOMM UARTs.
 */
static LLCOMM_UART LLCOMM_UARTs[1];

// HAL USBH instance
static USBH_HandleTypeDef hUSBHost;

/*
 * The ST USBH implementation can manage only one connection.
 */
static LLCOMM_CDC LLCOMM_CDC0;

static SemaphoreHandle_t sync_connections_sem;

/* Private functions ---------------------------------------------------------*/

/*
 * Initialize LLCOMM_UARTs array for a specific HAL UART instance and initialize
 * corresponding MicroEJ LLCOMM_UART.
 */
static void COMMS_initialize_llcomm_uart(int32_t index, USART_TypeDef* hal_uart_instance, uint8_t platform_id)
{
	// store HAL UART instance
	LLCOMM_UARTs[index].hal_uart_handle.Instance = hal_uart_instance;

	// store HAL UART ID
	LLCOMM_UARTs[index].platformId = platform_id;

	// create MicroEJ UART connection
	LLCOMM_UART_new(&LLCOMM_UARTs[index].header);

	// add MicroEJ UART connection into MicroEJ framework as generic connection
	LLCOMM_addConnection((LLCOMM_CONNECTION*)&LLCOMM_UARTs[index].header);
}

static void COMMS_initialize_interrupt(IRQn_Type interrupt)
{
	HAL_NVIC_SetPriority(interrupt, 0xF, 0xF);
	HAL_NVIC_EnableIRQ(interrupt);
}

static void COMMS_initialize_pin(GPIO_TypeDef* port, uint16_t pin, uint32_t alternate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate = alternate;
	GPIO_InitStructure.Pin = pin;
	HAL_GPIO_Init(port, &GPIO_InitStructure);
}

static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
	switch(id)
	{
	case HOST_USER_SELECT_CONFIGURATION:
		printf("HOST_USER_SELECT_CONFIGURATION\n");
		break;

	case HOST_USER_DISCONNECTION:
		if (phost->pActiveClass == USBH_CDC_CLASS && interrupt_is_in() == MICROEJ_FALSE && LLCOMM_CDC0.status == CDC_STATUS_CONNECTED)
		{
			if (!LLCOMM_removeConnection((LLCOMM_CONNECTION*) &LLCOMM_CDC0))
			{
				printf(("[ERROR] USB CDC connection could not be removed.\n"));
			}
			LLCOMM_CDC0.status = CDC_STATUS_DISCONNECTED;
		}
		// else: unsupported class (should never occurs)
		break;

	case HOST_USER_CLASS_ACTIVE:
		if (phost->pActiveClass == USBH_CDC_CLASS && LLCOMM_CDC0.status == CDC_STATUS_IDLE)
		{
			if (!LLCOMM_addConnection((LLCOMM_CONNECTION*) &LLCOMM_CDC0))
			{
				// reached the maximum numbed of connections allowed in ECOM-COMM stack.
				printf(("[ERROR] USB CDC connection could not be added.\n"));
			}
			else
			{
				LLCOMM_CDC0.status = CDC_STATUS_CONNECTED;
			}
		}
		// else: unsupported class (should never occurs)
		break;

	case HOST_USER_CONNECTION:
		break;

	default:
		break;
	}
}

static void COMMS_initialize_USBHost(void)
{
	// store USBH pointer
	LLCOMM_CDC_new(&LLCOMM_CDC0.header);
	LLCOMM_CDC0.hUSBHost = &hUSBHost;
	LLCOMM_CDC0.id = 0;
	LLCOMM_CDC0.status = CDC_STATUS_IDLE;

	/* Init Host Library */
	USBH_Init(&hUSBHost, USBH_UserProcess, 0);

	/* Add Supported Class(es) */
	USBH_RegisterClass(&hUSBHost, USBH_CDC_CLASS);

	/* Start Host Process */
	USBH_Start(&hUSBHost);
}

/* Callback ------------------------------------------------------------------*/

void USBH_CDC_ReceiveCallback(USBH_HandleTypeDef *phost)
{
	LLCOMM_USBH_CDC_ReceiveCallback(&LLCOMM_CDC0);
}

void USBH_CDC_TransmitCallback(USBH_HandleTypeDef *phost)
{
	LLCOMM_USBH_CDC_TransmitCallback(&LLCOMM_CDC0);
}

/* Interrupt functions -------------------------------------------------------*/

void USART6_IRQHandler(void)
{
	LLCOMM_UART_callback(&LLCOMM_UARTs[UART6_INDEX]);
}

/* Public functions ----------------------------------------------------------*/

/*
 * The semaphore must be created before the first call of LLCOMM_IMPL_syncConnectionsEnter()
 * which is called BEFORE LLCOMM_IMPL_initialize().
 */
void LLCOMM_stack_initialize(void)
{
	vSemaphoreCreateBinary(sync_connections_sem);
	xSemaphoreGive(sync_connections_sem);
}

void LLCOMM_IMPL_initialize(void)
{
	// STM32F7508-DK COM virtual port 1: use USART 6
	// 	- TX: CN4 D1
	// 	- RX: CN4 D0
	__HAL_RCC_USART6_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	COMMS_initialize_pin(GPIOC, GPIO_PIN_6, GPIO_AF8_USART6);	// TX
	COMMS_initialize_pin(GPIOC, GPIO_PIN_7, GPIO_AF8_USART6);	// RX
	COMMS_initialize_llcomm_uart(UART6_INDEX, USART6, 6);
	COMMS_initialize_interrupt(USART6_IRQn);

	COMMS_initialize_USBHost();
}

void LLCOMM_IMPL_syncConnectionsEnter(void)
{
	xSemaphoreTake(sync_connections_sem, 0);
}

void LLCOMM_IMPL_syncConnectionsExit(void)
{
	if (interrupt_is_in() == MICROEJ_TRUE)
	{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(sync_connections_sem, &xHigherPriorityTaskWoken);
		if(xHigherPriorityTaskWoken != pdFALSE )
		{
			// Force a context switch here.
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}
	else
	{
		xSemaphoreGive(sync_connections_sem);
	}
}
