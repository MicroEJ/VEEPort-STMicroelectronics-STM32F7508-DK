/*
 * C
 *
 * Copyright 2018-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLMJVM implementation over FreeRTOS.
 * @author MicroEJ Developer Team
 * @version 1.4.0
 */

/*
 * This implementation uses a dedicated hardware timer for time implementation,
 * instead of internal FreeRTOS timer which is a 32bits timer.
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>

#include "LLMJVM_FreeRTOS_configuration.h"
#include FREERTOS_HEADER
#include FREERTOS_TIMERS_HEADER
#include FREERTOS_SEMPHR_HEADER
#include FREERTOS_TASK_HEADER

#include "LLMJVM_impl.h"
#include "microej_time.h"
#include "microej.h"
#include "sni.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* Defines -------------------------------------------------------------------*/

/*
 * Useful macros and definitions
 */

/* ID for the FreeRTOS Timer */
#define WAKE_UP_TIMER_ID	42

/* Globals -------------------------------------------------------------------*/

/*
 * Shared variables
 */
/* Absolute time in ms at which timer will be launched */
// cppcheck-suppress misra-c2012-8.9 // Global variable used across native function calls.
static int64_t LLMJVM_FREERTOS_next_wake_up_time = INT64_MAX;

/* Set to true when the timer expires, set to true when the timer is started. */
static volatile bool LLMJVM_FREERTOS_timer_expired = false;

/* Timer for scheduling next alarm */
static TimerHandle_t LLMJVM_FREERTOS_wake_up_timer;

/* Binary semaphore to wakeup microJVM */
static SemaphoreHandle_t LLMJVM_FREERTOS_Semaphore;

/* Private functions ---------------------------------------------------------*/

static void wake_up_timer_callback(TimerHandle_t timer);

/*
 * Since LLMJVM_schedule() prototype does not match the timer callback prototype,
 * we create a wrapper around it and check the ID of the timer.
 */
static void wake_up_timer_callback(TimerHandle_t timer) {
	uint32_t id = (uint32_t) pvTimerGetTimerID(timer);
	if (id == (uint32_t)WAKE_UP_TIMER_ID) {
		LLMJVM_FREERTOS_timer_expired = true;
		LLMJVM_schedule();
	}
}

/* Public functions ----------------------------------------------------------*/

/*
 * Implementation of functions from LLMJVM_impl.h
 * and other helping functions.
 */

/*
 * Creates the timer used to callback the LLMJVM_schedule() function.
 * After its creation, the timer is idle.
 */
int32_t LLMJVM_IMPL_initialize(void) {
	int32_t result = LLMJVM_OK;
	/* Create a timer to schedule an alarm for the VM */
	// cppcheck-suppress misra-c2012-11.6 // Cast for matching xTimerCreate function signature.
	LLMJVM_FREERTOS_wake_up_timer = xTimerCreate(NULL, (TickType_t)100, (UBaseType_t)pdFALSE, (void*) WAKE_UP_TIMER_ID, wake_up_timer_callback);

	if (LLMJVM_FREERTOS_wake_up_timer == NULL) {
		result = LLMJVM_ERROR;
	} else {
		LLMJVM_FREERTOS_Semaphore = xSemaphoreCreateBinary();

		if (LLMJVM_FREERTOS_Semaphore == NULL) {
			result = LLMJVM_ERROR;
		} else {
			microej_time_init();
		}
	}
	return result;
}

/*
 * Once the task is started, save a handle to it.
 */
int32_t LLMJVM_IMPL_vmTaskStarted(void) {
	return LLMJVM_OK;
}

/*
 * Schedules requests from the VM
 */
int32_t LLMJVM_IMPL_scheduleRequest(int64_t absoluteTime) {
	int32_t result = LLMJVM_OK;
	int64_t currentTime;
	int64_t relativeTime;
	int64_t relativeTick;
	portBASE_TYPE xTimerChangePeriodResult;
	portBASE_TYPE xTimerStartResult;

	currentTime = LLMJVM_IMPL_getCurrentTime(MICROEJ_TRUE);

	relativeTime = absoluteTime - currentTime;
	/* Determine relative time/tick */
	relativeTick = microej_time_time_to_tick(relativeTime);

	if (relativeTick <= 0) {
		/* 'absoluteTime' has been reached yet */

		/* No pending request anymore */
		LLMJVM_FREERTOS_next_wake_up_time = INT64_MAX;

		/* Stop current timer (no delay) */
		xTimerStop(LLMJVM_FREERTOS_wake_up_timer, (TickType_t )0);

		/* Notify the VM now */
		result = LLMJVM_schedule();
	} else if ((LLMJVM_FREERTOS_timer_expired == true)
			|| (absoluteTime < LLMJVM_FREERTOS_next_wake_up_time)
			|| (LLMJVM_FREERTOS_next_wake_up_time <= currentTime)) {
		/* We want to schedule a request in the future but before the existing request
		   or the existing request is already done */

		/* Save new alarm absolute time */
		LLMJVM_FREERTOS_next_wake_up_time = absoluteTime;

		/* Stop current timer (no delay) */
		xTimerStop(LLMJVM_FREERTOS_wake_up_timer, (TickType_t )0);
		LLMJVM_FREERTOS_timer_expired = false;

		/* Schedule the new alarm */
		xTimerChangePeriodResult = xTimerChangePeriod(LLMJVM_FREERTOS_wake_up_timer, (TickType_t)relativeTick, (TickType_t)0);
		xTimerStartResult = xTimerStart(LLMJVM_FREERTOS_wake_up_timer, (TickType_t )0);

		if ((xTimerChangePeriodResult != pdPASS)
		|| (xTimerStartResult != pdPASS)) {
			result = LLMJVM_ERROR;
		}
	} else {
		/* else: there is a pending request that will occur before the new one -> do nothing */
	}

	return result;
}

/*
 * Suspends the VM task if the pending flag is not set
 */
int32_t LLMJVM_IMPL_idleVM(void) {
	portBASE_TYPE res = xSemaphoreTake(LLMJVM_FREERTOS_Semaphore,
			portMAX_DELAY);

	return (res == pdTRUE) ? (int32_t) LLMJVM_OK : (int32_t) LLMJVM_ERROR;
}

/* 
 * Wakes up the VM task 
 */
int32_t LLMJVM_IMPL_wakeupVM(void) {
	portBASE_TYPE res;

	if (IS_INSIDE_INTERRUPT() == pdTRUE) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		res = xSemaphoreGiveFromISR(LLMJVM_FREERTOS_Semaphore,
				&xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken != pdFALSE) {
			/* Force a context switch here. */
			YIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	} else {
		res = xSemaphoreGive(LLMJVM_FREERTOS_Semaphore);
	}

	return (res == pdTRUE) ? (int32_t) LLMJVM_OK : (int32_t) LLMJVM_ERROR;
}

/*
 * Clear the pending wake up flag and reset next wake up time
 */
int32_t LLMJVM_IMPL_ackWakeup(void) {
	return LLMJVM_OK;
}

/*
 * Gets the system or the application time in milliseconds
 */
int32_t LLMJVM_IMPL_getCurrentTaskID(void) {
	return (int32_t) xTaskGetCurrentTaskHandle();
}

/*
 * Sets the application time
 */
void LLMJVM_IMPL_setApplicationTime(int64_t t) {
	microej_time_set_application_time(t);
}

/*
 * Gets the system or the application time in milliseconds
 */
// cppcheck-suppress misra-c2012-8.7 // External API which is called also internally, cannot be made static.
int64_t LLMJVM_IMPL_getCurrentTime(uint8_t sys) {
	return microej_time_get_current_time(sys);
}

/*
 * Gets the system time in nanoseconds
 */
int64_t LLMJVM_IMPL_getTimeNanos(void) {
	return microej_time_get_time_nanos();
}

/*
 * Shuts the system down
 */
int32_t LLMJVM_IMPL_shutdown(void) {
	return LLMJVM_OK;
}

#ifdef __cplusplus
    }
#endif
