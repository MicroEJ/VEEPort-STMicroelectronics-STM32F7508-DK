
/*
 *	H
 *
 *	Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _ADC_MANAGER
#define _ADC_MANAGER

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"

/* API -----------------------------------------------------------------------*/

/*
 * Initialize all ADCs
 */
int32_t adc_manager_initialize(void);

/*
 * Initialize an ADC rank. Must be called by ADC manager implementation during the call of
 * "adc_manager_impl_configure_ranks()"
 */
void adc_manager_configure_rank(ADC_HandleTypeDef* AdcHandle, uint32_t rank, uint32_t channel);

/*
 * Return the ADC value for given GPIO
 */
int32_t adc_manager_get_value(hal_stm32_gpio_data* gpio);

/* EOF -----------------------------------------------------------------------*/

#endif // _ADC_MANAGER
