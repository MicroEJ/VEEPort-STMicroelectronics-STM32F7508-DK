/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/*
 * Implement the *_impl* functions according the CPU/OS/Compiler
 */

#ifndef _FRAMERATE_IMPL
#define _FRAMERATE_IMPL

/* Includes ------------------------------------------------------------------*/

#include "framerate.h"

/* API -----------------------------------------------------------------------*/

#ifdef FRAMERATE_ENABLED

/*
 * Framerate task have just to call this function
 */
void framerate_task_work(void);

/*
 * Create and start a framerate task
 */
int32_t framerate_impl_start_task(void);

/*
 * Sleep the framerate task
 */
void framerate_impl_sleep(uint32_t ms);

#endif	// FRAMERATE_ENABLED

#endif	// _FRAMERATE_IMPL
