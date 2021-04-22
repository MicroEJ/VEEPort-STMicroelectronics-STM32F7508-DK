/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
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

/* Defines -------------------------------------------------------------------*/

#define IO_TASK_PRIORITY ( 12 )
#define IO_TASK_STACK_SIZE (128) // portSTACK_TYPE is 32 bits wide, so 128 * 4 = 512 bits

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
