/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _BUTTONS_MANAGER
#define _BUTTONS_MANAGER

/* Includes ------------------------------------------------------------------*/

#include "stm32f7508_discovery.h"

/* API -----------------------------------------------------------------------*/

void BUTTONS_MANAGER_initialize(void);
void BUTTONS_MANAGER_interrupt(Button_TypeDef Button);
void BUTTONS_MANAGER_enable_interrupts(void);
void BUTTONS_MANAGER_disable_interrupts(void);

#endif
