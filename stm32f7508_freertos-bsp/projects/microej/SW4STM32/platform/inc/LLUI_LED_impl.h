/* 
 * Copyright 2012-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLUI_LED_IMPL
#define _LLUI_LED_IMPL
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief LEDs module low level API. This file lists a set of API to connect the
 * MicroUI Leds. All APIs are mandatory.
 */

#include "sni.h"

// --------------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------------

/**
 * @brief Intensity value to turn off a LED.
 */
#define LLUI_LED_MIN_INTENSITY (0)

/**
 * @brief Maximum intensity that a LED can handle. If a LED does not handle intensity,
 * any valid intensity different from LLUI_LED_MIN_INTENSITY turns the LED on.
 */
#define LLUI_LED_MAX_INTENSITY (255)

// --------------------------------------------------------------------------------
// Functions that must be implemented
// --------------------------------------------------------------------------------

/**
 * @brief Initializes the LEDs.
 *
 * @return the number of LEDs in the group. First LED will have the identifier 0
 * and last one the identifier nbLeds - 1.
 */
jint LLUI_LED_IMPL_initialize(void);

/**
 * @brief Get the intensity of the LED. The implementation should be able to return
 * the state of an LED. If it is not able to do so (for example `GPIO` is not accessible
 * in read mode), the BSP has to save the LED state in a global variable.
 *
 * @param[in] ledID the LED identifier.
 *
 * @return the intensity of the LED.
 */
jint LLUI_LED_IMPL_getIntensity(jint ledID);

/**
 * @brief Set the intensity of the LED.
 *
 * A 0 intensity turns off the LED, intensity of 255 turns on the LED to max power.
 *
 * @param[in] ledID the LED identifier.
 * @param[in] intensity the intensity of the LED.
 */
void LLUI_LED_IMPL_setIntensity(jint ledID, jint intensity);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
