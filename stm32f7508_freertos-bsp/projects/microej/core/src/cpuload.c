/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "stm32f7xx.h"
#include "cpuload_impl.h"
#include "core_cm7.h"
#include <stdio.h>

/* Globals -------------------------------------------------------------------*/

#ifdef CPULOAD_ENABLED
static volatile uint32_t cpuload_schedule_time;
static volatile uint32_t cpuload_last_load;
static volatile uint32_t cpuload_reference_counter;
static volatile uint32_t cpuload_idle_counter;
static volatile uint32_t cpuload_ask_counter;
#endif

/* API -----------------------------------------------------------------------*/

void cpuload_idle(void)
{
#ifdef CPULOAD_ENABLED
	__disable_irq();
	cpuload_idle_counter++;
	__enable_irq();
#endif
}

int32_t cpuload_init(void)
{
#ifdef CPULOAD_ENABLED

	// get reference time (must be done before creating task)
	cpuload_impl_start_idle_task();
	cpuload_impl_sync_os_tick();
	cpuload_idle_counter = 0;
	cpuload_impl_sleep(CPULOAD_SCHEDULE_TIME/10);
	
	if (cpuload_idle_counter == 0){
		// it is an error: this counter must have been updated
		// during the previous sleep.
		printf("CPU load startup: invalid idle counter value: %ld\n", cpuload_idle_counter);
		return CPULOAD_INVALID_COUNTER;
	}

	// fix globals
	cpuload_schedule_time = CPULOAD_SCHEDULE_TIME;
	cpuload_reference_counter = (cpuload_idle_counter*10)+5;
	cpuload_idle_counter = 0;
	cpuload_last_load = 0;
	cpuload_ask_counter = 0;
        
	// create task
	if (cpuload_impl_start_task() != 0)
	{
		// an error has occurred
		return CPULOAD_START_TASK_ERROR;
	}

	return CPULOAD_OK;

#else
	return CPULOAD_NOT_ENABLED;
#endif
}

uint32_t cpuload_get(void)
{
#ifdef CPULOAD_ENABLED
	uint32_t ret = cpuload_last_load;
	// clear average
	cpuload_ask_counter = 0;
	return ret;
#else
	return 0;
#endif
}

void cpuload_task(void)
{
#ifdef CPULOAD_ENABLED
	while(1)
	{
		// sleep during n milliseconds
		cpuload_impl_sleep(cpuload_schedule_time);

		// save global variables into local ones as they are volatile and might change while computing the average
		uint32_t cpuload_last_load_tmp = cpuload_last_load;
		uint32_t cpuload_reference_counter_tmp = cpuload_reference_counter;
		uint32_t cpuload_idle_counter_tmp = cpuload_idle_counter;
		uint32_t cpuload_ask_counter_tmp = cpuload_ask_counter;

		// average computing
		uint32_t average_compute = cpuload_last_load_tmp * cpuload_ask_counter_tmp;
		uint32_t last_average = 100 - ((100 * cpuload_idle_counter_tmp) / cpuload_reference_counter_tmp);
		cpuload_ask_counter++;
		cpuload_last_load = (average_compute + last_average) / cpuload_ask_counter;

		// reset cpuload counter
		cpuload_idle_counter = 0;
	}
#endif
}

/* Java API ------------------------------------------------------------------*/

uint32_t javaCPULoadInit(void)
{
	return cpuload_init();
}

uint32_t javaCPULoadGet(void)
{
	return cpuload_get();
}
