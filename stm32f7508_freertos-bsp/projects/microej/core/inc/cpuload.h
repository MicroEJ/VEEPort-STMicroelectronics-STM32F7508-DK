/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef _CPULOAD_INTERN
#define _CPULOAD_INTERN

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "cpuload_conf.h"

/* Defines -------------------------------------------------------------------*/

#ifndef CPULOAD_SCHEDULE_TIME
#define CPULOAD_SCHEDULE_TIME 1000	// ms
#endif

#define CPULOAD_OK 					 0
#define CPULOAD_NOT_ENABLED			-1
#define CPULOAD_INVALID_COUNTER 	-2
#define CPULOAD_START_TASK_ERROR 	-3

/* API -----------------------------------------------------------------------*/

/*
 * Initialize the framework
 * Must be called in very first OS stack. No more task must run
 * at same time.
 */
int32_t cpuload_init(void);

/*
 * Must be called by the OS idle function
 */
void cpuload_idle(void);

/*
 * Return the last CPU load measure
 */
uint32_t cpuload_get(void);

/* Default Java API ----------------------------------------------------------*/

#ifndef javaCPULoadInit
#define javaCPULoadInit		Java_com_is2t_debug_CPULoad_init
#endif

#ifndef javaCPULoadGet
#define javaCPULoadGet		Java_com_is2t_debug_CPULoad_get
#endif

#endif	// _CPULOAD_INTERN
