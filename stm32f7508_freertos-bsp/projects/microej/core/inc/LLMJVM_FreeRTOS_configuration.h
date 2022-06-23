/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLMJVM_FREERTOS_CONFIGURATION_H
#define _LLMJVM_FREERTOS_CONFIGURATION_H

/* Defines used to include FreeRTOS API header files. Update it if header file location is different. */

#define FREERTOS_HEADER				"FreeRTOS.h"
#define FREERTOS_TIMERS_HEADER		"timers.h"
#define FREERTOS_SEMPHR_HEADER		"semphr.h"
#define FREERTOS_TASK_HEADER		"task.h"
#define YIELD_FROM_ISR(x)			portYIELD_FROM_ISR(x)
#define IS_INSIDE_INTERRUPT			xPortIsInsideInterrupt

#endif /* _LLMJVM_FREERTOS_CONFIGURATION_H */
