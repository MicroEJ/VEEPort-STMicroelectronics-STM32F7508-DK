/*
 * C
 *
 * Copyright 2018-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLMJVM implementation over FreeRTOS.
 * @author MicroEJ Developer Team
 * @version 1.1.0
 * @date 6 May 2021
 */

/*
 * This implementation uses a dedicated hardware timer for time implementation,
 * instead of internal FreeRTOS timer which is a 32bits timer.
 */

/* Includes ------------------------------------------------------------------*/
#include "misra_2004_conf.h"

MISRA_2004_DISABLE_ALL
#include <stdlib.h>
#include <stdio.h>
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
MISRA_2004_ENABLE_ALL

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
static int64_t LLMJVM_FREERTOS_next_wake_up_time = INT64_MAX;

/* Set to true when the timer expires, set to true when the timer is started. */
volatile bool LLMJVM_FREERTOS_timer_expired = false;

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
MISRA_2004_DISABLE_RULE_11_3
static void wake_up_timer_callback(TimerHandle_t timer) {
	uint32_t id = (uint32_t) pvTimerGetTimerID(timer);
	if (id == (uint32_t)WAKE_UP_TIMER_ID) {
		LLMJVM_FREERTOS_timer_expired = true;
		LLMJVM_schedule();
	}
}
MISRA_2004_ENABLE_ALL

/* Public functions ----------------------------------------------------------*/

/*
 * Implementation of functions from LLMJVM_impl.h
 * and other helping functions.
 */

/*
 * Creates the timer used to callback the LLMJVM_schedule() function.
 * After its creation, the timer is idle.
 */
MISRA_2004_DISABLE_RULE_11_3
int32_t LLMJVM_IMPL_initialize(void) {
	int32_t result = LLMJVM_OK;
	/* Create a timer to scheduler alarm for the VM */
	LLMJVM_FREERTOS_wake_up_timer = xTimerCreate(NULL, (TickType_t )100, (UBaseType_t)pdFALSE,
			(void*) WAKE_UP_TIMER_ID, wake_up_timer_callback);

	if (LLMJVM_FREERTOS_wake_up_timer == NULL) {
		result = LLMJVM_ERROR;
	} else {
		LLMJVM_FREERTOS_Semaphore = xSemaphoreCreateBinary();

		if (LLMJVM_FREERTOS_Semaphore == NULL) {
			result = LLMJVM_ERROR;
		} else {
			/* Created semaphore is in an empty state meaning that
                            it must first be given before it can be taken. */
			xSemaphoreGive(LLMJVM_FREERTOS_Semaphore);
			microej_time_init();
		}
	}
	return result;
}
MISRA_2004_ENABLE_ALL

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
	int64_t currentTime, relativeTime, relativeTick; 
	portBASE_TYPE xTimerChangePeriodResult, xTimerStartResult;

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
MISRA_2004_DISABLE_RULE_10_6
MISRA_2004_DISABLE_RULE_11_3
MISRA_2004_DISABLE_RULE_14_3
int32_t LLMJVM_IMPL_wakeupVM(void) {
	portBASE_TYPE res;

	if (IS_INSIDE_INTERRUPT()) {
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
MISRA_2004_ENABLE_ALL

/*
 * Clear the pending wake up flag and reset next wake up time
 */
int32_t LLMJVM_IMPL_ackWakeup(void) {
	return LLMJVM_OK;
}

/*
 * Gets the system or the application time in milliseconds
 */
MISRA_2004_DISABLE_RULE_11_3
int32_t LLMJVM_IMPL_getCurrentTaskID(void) {
	return (int32_t) xTaskGetCurrentTaskHandle();
}
MISRA_2004_ENABLE_ALL

/*
 * Sets the application time
 */
void LLMJVM_IMPL_setApplicationTime(int64_t t) {
	microej_time_set_application_time(t);
}

/*
 * Gets the system or the application time in milliseconds
 */
MISRA_2004_DISABLE_RULE_16_4
int64_t LLMJVM_IMPL_getCurrentTime(uint8_t sys) {
	return microej_time_get_current_time(sys);
}
MISRA_2004_ENABLE_ALL

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
