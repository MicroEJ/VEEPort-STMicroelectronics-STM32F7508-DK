
/*
 *	H
 *
 *	Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _HAL_STM32_CONFIGURATION
#define _HAL_STM32_CONFIGURATION

/* Includes ------------------------------------------------------------------*/

#include "stm32f7xx_hal.h"

/* Defines -------------------------------------------------------------------*/

/*
 * DAC timers input clock (use default value)
 */
// #define DAC_TIMER_IN_FREQUENCY	20000000	// 20 MHz

/*
 * DAC timers output clock (use default value)
 */
// #define DAC_TIMER_OUT_FREQUENCY	1000		// 1 KHz

/*
 * Available number of channels
 */
#define ADC_NB_CHANNELS 6	// 6 (ADC3)

/* EOF -----------------------------------------------------------------------*/

#endif // _HAL_STM32_CONFIGURATION
