/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _INTERRUPTS
#define _INTERRUPTS

/* Includes ------------------------------------------------------------------*/

#include "microej.h"

/* API -----------------------------------------------------------------------*/

/**
 * This function returns MICROEJ_TRUE or MICROEJ_FALSE to indicate if an ISR is currently executed.
 */
uint8_t interrupt_is_in(void);

#endif
