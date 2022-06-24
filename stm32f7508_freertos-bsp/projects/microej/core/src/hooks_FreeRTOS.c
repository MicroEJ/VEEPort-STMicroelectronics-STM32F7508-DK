/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/*
 * This file implements the application hooks functions needed by FreeRTOS.
 * These functions trace problems that occurs during execution.
 * Their names are defined by FreeRTOS
 * and must be implemented (here) or desactivated (in ConfigFreeRTOS.h).
 *
 * See "Hook Functions [More Advanced]" on FreeRTOS Website.
 * http://www.freertos.org/a00016.hmtl
 */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#ifndef IPERF
#include "watchdog.h"
#include "cpuload.h"
#endif

/* Public functions ----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    if (pcTaskName != NULL) {
        printf("A stack overflow had occurred in task \"%s\"\n", pcTaskName);
    } else {
        printf("A stack overflow had occurred in task\n");
    }

    while(1)
    {} // Trap when a stack overflow occurs
}

void vApplicationMallocFailedHook(void)
{
    printf("A malloc fail had occurred\n");

    while(1)
    {} // Trap when a call to malloc fails
}

void vApplicationIdleHook(void)
{
#ifndef IPERF
	refresh_watchdog();
	cpuload_idle();
#endif
}

#ifdef DEBUG_MALLOC
static int32_t mallocSpaceUsed = 0;

void traceMallocDebug(void * pvAddress, size_t uiSize)
{
	mallocSpaceUsed += uiSize;
	printf("%d bytes used of %d heap size\n", mallocSpaceUsed, configTOTAL_HEAP_SIZE);
}

void traceFreeDebug(void * pvAddress, size_t uiSize)
{
	mallocSpaceUsed -= uiSize;
	printf("%d bytes used of %d heap size\n", mallocSpaceUsed, configTOTAL_HEAP_SIZE);
}
#endif
