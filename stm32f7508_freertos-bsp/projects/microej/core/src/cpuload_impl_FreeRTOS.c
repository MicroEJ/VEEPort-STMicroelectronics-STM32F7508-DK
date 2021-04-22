/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/*
 * Implementation for FreeRTOS
 */

#include "cpuload_conf.h"
#ifdef CPULOAD_ENABLED

/* Includes ------------------------------------------------------------------*/
 
#include "FreeRTOS.h"
#include "task.h"
#include "cpuload_impl.h"

#if (configUSE_IDLE_HOOK == 0)
	#error configUSE_IDLE_HOOK must be defined in FreeRTOSConfig.h and equals to 1 when CPULOAD_ENABLED defined.
#endif

/* Globals -------------------------------------------------------------------*/

#define CPULOAD_STACK_SIZE ( 512 )
#define CPULOAD_TASK_PRIORITY      ( 15 )
#define CPULOAD_TASK_STACK_SIZE     CPULOAD_STACK_SIZE/4

/* Private API ---------------------------------------------------------------*/

static void _cpuload_task(void * pvParameters)
{
	// launch cpuload job
	cpuload_task();
	// job end, cleanup resources
	vTaskDelete(xTaskGetCurrentTaskHandle());
}

/* API -----------------------------------------------------------------------*/

int32_t cpuload_impl_start_idle_task(void)
{
	return 0;	// nothing to do (no error)
}

int32_t cpuload_impl_start_task(void)
{
	BaseType_t xReturn = xTaskCreate( _cpuload_task, "CPULOAD", CPULOAD_TASK_STACK_SIZE, NULL, CPULOAD_TASK_PRIORITY, NULL );
	return xReturn == pdPASS ? 0 : -1;
}

void cpuload_impl_sync_os_tick(void)
{
	TickType_t os_tick = xTaskGetTickCount();
	while(os_tick == xTaskGetTickCount());
}

void cpuload_impl_sleep(uint32_t ms)
{
	TickType_t ticks = ms / portTICK_PERIOD_MS;
	vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */
}

#endif	// CPULOAD_ENABLED
