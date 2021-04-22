
/*
 *	H
 *
 *	Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _DAC_MANAGER
#define _DAC_MANAGER

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"

/* API -----------------------------------------------------------------------*/

/*
 * Initialize all ADCs
 */
int32_t dac_manager_initialize(void);

/*
 * Stop the DAC of the given GPIO (may be not running)
 */
void dac_manager_stop(hal_stm32_gpio_data* gpio);

/*
 * Configure DAC for given GPIO
 */
int32_t dac_manager_set_value(hal_stm32_gpio_data* gpio, int32_t percentage);

/* EOF -----------------------------------------------------------------------*/

#endif // _DAC_MANAGER
