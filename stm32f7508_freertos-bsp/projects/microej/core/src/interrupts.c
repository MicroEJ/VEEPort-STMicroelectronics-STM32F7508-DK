/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "interrupts.h"
#include "FreeRTOS.h"

/* Public functions ----------------------------------------------------------*/

uint8_t interrupt_is_in(void)
{
	BaseType_t is_in_ISR;

	is_in_ISR = xPortIsInsideInterrupt();
	if (is_in_ISR == pdTRUE)
	{
		return MICROEJ_TRUE;
	}
	else
	{
		return MICROEJ_FALSE;
	}
}