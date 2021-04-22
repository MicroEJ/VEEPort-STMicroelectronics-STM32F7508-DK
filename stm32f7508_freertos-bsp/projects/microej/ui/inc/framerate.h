/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef _FRAMERATE_INTERN
#define _FRAMERATE_INTERN

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "framerate_conf.h"

/* Defines -------------------------------------------------------------------*/

#define FRAMERATE_OK 0
#define FRAMERATE_ERROR -1

/* API -----------------------------------------------------------------------*/

/*
 * Initialize the framework
 */
int32_t framerate_init(int32_t schedule_time);

/*
 * Update the framerate counter
 */
void framerate_increment(void);

/*
 * Return the last framerate
 */
uint32_t framerate_get(void);

/* Default Java API ----------------------------------------------------------*/

#ifndef javaFramerateInit
#define javaFramerateInit		Java_com_is2t_debug_Framerate_init
#endif
#ifndef javaFramerateGet
#define javaFramerateGet		Java_com_is2t_debug_Framerate_get
#endif

#endif	// _FRAMERATE_INTERN
