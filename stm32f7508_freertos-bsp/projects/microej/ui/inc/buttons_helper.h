/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _BUTTONS_HELPER
#define _BUTTONS_HELPER

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* API -----------------------------------------------------------------------*/

/*
 * Initialize the buttons helper.
 */
void BUTTONS_HELPER_initialize(void);

/*
 * Notify to the event generator a button has been pressed.
 * @param buttonId the button ID, between 0 and 255
 */
void BUTTONS_HELPER_pressed(int32_t buttonId);

/**
 * Notify to the event generator a button has been repeated.
 * @param buttonId the button ID, between 0 and 255
 */
void BUTTONS_HELPER_repeated(int32_t buttonId);

/**
 * Notify to the event generator a button has been released.
 * @param buttonId the button ID, between 0 and 255
 */
void BUTTONS_HELPER_released(int32_t buttonId);

#endif
