/*
 * C
 *
 * Copyright 2013-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Includes ------------------------------------------------------------------*/

#include "io_task.h"
#include "touch_manager.h"
#include "buttons_manager.h"
#include "stm32f7508_discovery.h"
#include "microej.h"
#include "interrupts.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "SEGGER_SYSVIEW_configuration.h"

/* Defines -------------------------------------------------------------------*/

#define IO_TASK_PRIORITY ( 12 )

#if (defined(ENABLE_SYSTEM_VIEW)) && (1 == SEGGER_SYSVIEW_POST_MORTEM_MODE)
#define IO_TASK_STACK_SIZE (512) // IO_TASK_STACK_SIZE increased: cause stack overflow with SystemView post mortem analysis
#else
#define IO_TASK_STACK_SIZE (128) // portSTACK_TYPE is 32 bits wide, so 128 * 4 = 512 bits
#endif

/* Global --------------------------------------------------------------------*/

static xSemaphoreHandle io_task_sem = NULL;

/* Private API ---------------------------------------------------------------*/

static void IO_work(void)
{
	uint8_t poll = MICROEJ_FALSE;

	do
	{
		// manages touch before
		poll = TOUCH_MANAGER_work();

		if (poll)
		{
			vTaskDelay(20);
		}

	} while (poll == MICROEJ_TRUE);
}

static void vIoeExpanderTaskFunction(void *p_arg)
{
	while(1)
	{
		// Suspend ourselves
		xSemaphoreTake(io_task_sem, portMAX_DELAY);

		// We have been woken up, lets work !
		IO_work();

		// touch release performed: re-enable the EXTI interrupt
		HAL_NVIC_EnableIRQ(TS_INT_EXTI_IRQn);
	}
}

/* Interrupt function --------------------------------------------------------*/

void EXTI15_10_IRQHandler(void)
{
	BUTTONS_MANAGER_interrupt(BUTTON_WAKEUP);

	if (TOUCH_MANAGER_interrupt() == MICROEJ_TRUE)
	{
		// send an event to wake up the IOE interrupt management task
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

		/* Give the semaphore to wakeup LwIP task */
		xSemaphoreGiveFromISR( io_task_sem, &xHigherPriorityTaskWoken );

		/* Switch tasks if necessary. */
		if( xHigherPriorityTaskWoken != pdFALSE )
		{
			portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
		}
	}
}

/* API -----------------------------------------------------------------------*/

uint8_t IO_TASK_create_task(void)
{
	if (io_task_sem == NULL)
	{
		// create semaphore
		vSemaphoreCreateBinary(io_task_sem);
		xSemaphoreTake(io_task_sem, 0);

		// create ioe task
		xTaskCreate( vIoeExpanderTaskFunction, NULL, IO_TASK_STACK_SIZE, NULL, IO_TASK_PRIORITY , NULL );
	}

	return MICROEJ_TRUE;
}
