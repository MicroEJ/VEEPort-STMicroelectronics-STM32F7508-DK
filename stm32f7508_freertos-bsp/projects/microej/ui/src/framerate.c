/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include "framerate_impl.h"

/* Globals -------------------------------------------------------------------*/

#ifdef FRAMERATE_ENABLED
static uint32_t framerate_schedule_time = 0;	// means "not initialised"
static uint32_t framerate_counter;
static uint32_t framerate_last;
#endif

/* API -----------------------------------------------------------------------*/

int32_t framerate_init(int32_t schedule_time)
{
#ifdef FRAMERATE_ENABLED

	if (framerate_schedule_time == 0)	// means "not initialized"
	{
		// fix globals
		framerate_schedule_time = schedule_time;
		framerate_counter = 0;
		framerate_last = 0;

		int32_t ret = framerate_impl_start_task();
		if (ret != FRAMERATE_OK)
		{
			// an error has occurred
			return ret;
		}
	}
	// else: no error because already initialized

	return FRAMERATE_OK;

#else
	return FRAMERATE_ERROR;
#endif
}

void framerate_increment(void)
{
#ifdef FRAMERATE_ENABLED
	framerate_counter++;
#endif
}

uint32_t framerate_get(void)
{
#ifdef FRAMERATE_ENABLED
	return framerate_last;
#else
	return 0;
#endif
}

void framerate_task_work(void)
{
#ifdef FRAMERATE_ENABLED
	while(1)
	{
		// sleep during n milliseconds
		framerate_impl_sleep(framerate_schedule_time);

		// update global counter
		framerate_last = (int32_t)((float)framerate_counter * ((float)((float)1000 / (float)framerate_schedule_time)));

		// reset framerate counter
		framerate_counter = 0;
	}
#endif
}

/* Java API ------------------------------------------------------------------*/

int32_t javaFramerateInit(int32_t schedule_time)
{
	return framerate_init(schedule_time);
}

uint32_t javaFramerateGet(void)
{
	return framerate_get();
}
