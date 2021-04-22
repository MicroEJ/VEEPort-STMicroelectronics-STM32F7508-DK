/*
 * C
 *
 * Copyright 2018-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLMJVM implementation over FreeRTOS.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 22 July 2020
 */

/*
 * This implementation uses a dedicated hardware timer for time implementation,
 * instead of internal FreeRTOS timer which is a 32bits timer.
 */

/* Includes ------------------------------------------------------------------*/

#include <microej_time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "semphr.h"

#include "LLMJVM_impl.h"
#include "microej.h"
#include "sni.h"

/* Defines -------------------------------------------------------------------*/

/*
 * Useful macros and definitions
 */

// ID for the FreeRTOS Timer
#define WAKE_UP_TIMER_ID	42

/* Globals -------------------------------------------------------------------*/

/*
 * Shared variables
 */
// Absolute time in ms at which timer will be launched
static int64_t LLMJVM_FREERTOS_next_wake_up_time = INT64_MAX;

// Set to true when the timer expires, set to true when the timer is started.
volatile bool LLMJVM_FREERTOS_timer_expired = false;

// Timer for scheduling next alarm
static TimerHandle_t LLMJVM_FREERTOS_wake_up_timer;

// Binary semaphore to wakeup microJVM
static SemaphoreHandle_t LLMJVM_FREERTOS_Semaphore;

/* Private functions ---------------------------------------------------------*/

// Since LLMJVM_schedule() prototype does not match the timer callback prototype,
// we create a wrapper around it and check the ID of the timer.
static void wake_up_timer_callback(TimerHandle_t timer) {
	uint32_t id = (uint32_t) pvTimerGetTimerID(timer);
	if (id == WAKE_UP_TIMER_ID) {
		LLMJVM_FREERTOS_timer_expired = true;
		LLMJVM_schedule();
	}
}

/* Public functions ----------------------------------------------------------*/

/*
 * Implementation of functions from LLMJVM_impl.h
 * and other helping functions.
 */

// Creates the timer used to callback the LLMJVM_schedule() function.
// After its creation, the timer is idle.
int32_t LLMJVM_IMPL_initialize() {
	// Create a timer to scheduler alarm for the VM
	LLMJVM_FREERTOS_wake_up_timer = xTimerCreate(NULL, 100, pdFALSE,
			(void*) WAKE_UP_TIMER_ID, wake_up_timer_callback);

	if (LLMJVM_FREERTOS_wake_up_timer == 0) {
		return LLMJVM_ERROR;
	}

	vSemaphoreCreateBinary(LLMJVM_FREERTOS_Semaphore);

	if (LLMJVM_FREERTOS_Semaphore == NULL) {
		return LLMJVM_ERROR;
	}

	// Created semaphore is in a state such that the first call to 'take' the semaphore would pass.
	// Just execute a take to init it at 0.
	// Note: From FreeRTOS version 7.6.0, the function xSemaphoreCreateBinary can be used to avoid
	// the call to xSemaphoreTake.
	xSemaphoreTake(LLMJVM_FREERTOS_Semaphore, portMAX_DELAY);

	microej_time_init();
	return LLMJVM_OK;
}

// Once the task is started, save a handle to it.
int32_t LLMJVM_IMPL_vmTaskStarted() {
	return LLMJVM_OK;
}

// Schedules requests from the VM
int32_t LLMJVM_IMPL_scheduleRequest(int64_t absoluteTime) {
	int64_t relativeTime; // relative from current time to 'absoluteTime'
	int64_t relativeTick;
	portBASE_TYPE xTimerChangePeriodResult;
	portBASE_TYPE xTimerStartResult;

	int64_t currentTime = LLMJVM_IMPL_getCurrentTime(MICROEJ_TRUE);

	relativeTime = absoluteTime - currentTime;
	// Determine relative time/tick
	relativeTick = microej_time_timeToTick(relativeTime);

	if (relativeTick <= 0) {
		// 'absoluteTime' has been reached yet

		// No pending request anymore
		LLMJVM_FREERTOS_next_wake_up_time = INT64_MAX;

		// Stop current timer (no delay)
		xTimerStop(LLMJVM_FREERTOS_wake_up_timer, 0);

		// Notify the VM now
		return LLMJVM_schedule();
	}

	// We want to schedule a request in the future
	if (LLMJVM_FREERTOS_timer_expired == true
			|| absoluteTime < LLMJVM_FREERTOS_next_wake_up_time
			|| LLMJVM_FREERTOS_next_wake_up_time <= currentTime) {
		// We want to schedule a request in the future but before the existing request
		// or the existing request is already done

		// Save new alarm absolute time
		LLMJVM_FREERTOS_next_wake_up_time = absoluteTime;

		// Stop current timer (no delay)
		xTimerStop(LLMJVM_FREERTOS_wake_up_timer, 0);
		LLMJVM_FREERTOS_timer_expired = false;

		// Schedule the new alarm
		xTimerChangePeriodResult = xTimerChangePeriod(
				LLMJVM_FREERTOS_wake_up_timer, relativeTick, 0);
		xTimerStartResult = xTimerStart(LLMJVM_FREERTOS_wake_up_timer, 0);

		if ((xTimerChangePeriodResult != pdPASS)
				|| (xTimerStartResult != pdPASS)) {
			return LLMJVM_ERROR;
		}
	}
	// else: there is a pending request that will occur before the new one -> do nothing

	return LLMJVM_OK;
}

// Suspends the VM task if the pending flag is not set
int32_t LLMJVM_IMPL_idleVM() {
	portBASE_TYPE res = xSemaphoreTake(LLMJVM_FREERTOS_Semaphore,
			portMAX_DELAY);

	return res == pdTRUE ? LLMJVM_OK : LLMJVM_ERROR;
}

// Wakes up the VM task
int32_t LLMJVM_IMPL_wakeupVM() {
	portBASE_TYPE res;
	if (xPortIsInsideInterrupt()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		res = xSemaphoreGiveFromISR(LLMJVM_FREERTOS_Semaphore,
				&xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken != pdFALSE) {
			// Force a context switch here.
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	} else {
		res = xSemaphoreGive(LLMJVM_FREERTOS_Semaphore);
	}

	return res == pdTRUE ? LLMJVM_OK : LLMJVM_ERROR;
}

// Clear the pending wake up flag and reset next wake up time
int32_t LLMJVM_IMPL_ackWakeup() {
	return LLMJVM_OK;
}

int32_t LLMJVM_IMPL_getCurrentTaskID() {
	return (int32_t) xTaskGetCurrentTaskHandle();
}

// Sets application time
void LLMJVM_IMPL_setApplicationTime(int64_t t) {
	microej_time_setapplicationtime(t);
}

// Gets the system or the application time in milliseconds
int64_t LLMJVM_IMPL_getCurrentTime(uint8_t system) {
	return microej_time_getcurrenttime(system);
}

// Gets the current system time in nanoseconds
int64_t LLMJVM_IMPL_getTimeNanos() {
	return microej_time_gettimenanos();
}

int32_t LLMJVM_IMPL_shutdown(void) {
	// nothing to do
	return LLMJVM_OK;
}
