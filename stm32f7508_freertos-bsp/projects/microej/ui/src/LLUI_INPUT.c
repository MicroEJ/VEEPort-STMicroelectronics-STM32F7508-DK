/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "LLUI_INPUT_impl.h"
#include "microej.h"
#include "buttons_manager.h"
#include "touch_manager.h"
#include "interrupts.h"
#include "FreeRTOS.h"
#include "semphr.h"

static xSemaphoreHandle g_sem_input;

/* API -----------------------------------------------------------------------*/

void LLUI_INPUT_IMPL_initialize(void)
{
	g_sem_input = xSemaphoreCreateBinary();
	xSemaphoreGive(g_sem_input);

	BUTTONS_MANAGER_initialize();
	TOUCH_MANAGER_initialize();
}

int32_t LLUI_INPUT_IMPL_getInitialStateValue(int32_t stateMachinesID, int32_t stateID)
{
	// no state on this BSP
	return 0;
}

void LLUI_INPUT_IMPL_enterCriticalSection()
{
	if (interrupt_is_in() == MICROEJ_FALSE)
	{
		xSemaphoreTake(g_sem_input, portMAX_DELAY);
		BUTTONS_MANAGER_disable_interrupts();
	}
}

void LLUI_INPUT_IMPL_leaveCriticalSection()
{
	if (interrupt_is_in() == MICROEJ_FALSE)
	{
		BUTTONS_MANAGER_enable_interrupts();
		xSemaphoreGive(g_sem_input);
	}
}
