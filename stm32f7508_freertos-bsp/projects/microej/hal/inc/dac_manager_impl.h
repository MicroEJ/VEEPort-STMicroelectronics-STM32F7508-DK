
/*
 *	H
 *
 *	Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _DAC_MANAGER_IMPL
#define _DAC_MANAGER_IMPL

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"

/* API -----------------------------------------------------------------------*/

/*
 * Fill the structure for the given DAC
 */
int32_t dac_manager_impl_get_data(int32_t dac_ID, hal_stm32_dac_data* dac_data);

/* EOF -----------------------------------------------------------------------*/

#endif // _DAC_MANAGER_IMPL
