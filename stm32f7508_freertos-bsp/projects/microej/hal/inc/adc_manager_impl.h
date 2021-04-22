
/*
 *	H
 *
 *	Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _ADC_MANAGER_IMPL
#define _ADC_MANAGER_IMPL

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"

/* API -----------------------------------------------------------------------*/

/*
 * Fill the structures addresses for the given ADC
 */
int32_t adc_manager_impl_get_data(int32_t adc_ID, ADC_HandleTypeDef** AdcHandle, DMA_HandleTypeDef** hdma_adc);

/*
 * Configures all ranks for given ADC. Implementation has to call "adc_manager_configure_rank()".
 */
void adc_manager_impl_configure_ranks(int32_t adc_ID, ADC_HandleTypeDef* AdcHandle);

/* EOF -----------------------------------------------------------------------*/

#endif // _ADC_MANAGER_IMPL
