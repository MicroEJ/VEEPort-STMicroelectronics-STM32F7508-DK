
/*
 *	H
 *
 *	Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _HAL_STM32
#define _HAL_STM32

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include "hal_stm32_configuration.h"
#include "LLHAL_GPIO_impl.h"

/* Defines -------------------------------------------------------------------*/

/*
 * DAC timers input clock
 */
#ifndef DAC_TIMER_IN_FREQUENCY
#define DAC_TIMER_IN_FREQUENCY	20000000	// 20 MHz
#endif

/*
 * DAC timers output clock
 */
#ifndef DAC_TIMER_OUT_FREQUENCY
#define DAC_TIMER_OUT_FREQUENCY	1000		// 1 KHz
#endif

/*
 * Available number of channels
 */
#ifndef ADC_NB_CHANNELS
#define ADC_NB_CHANNELS 0
#endif

/*
 * Global virtual MCU port
 */
#define HAL_STM32_GPIO_PORT_MCU 0

/* Structs -------------------------------------------------------------------*/

/*
 * DAC data
 */
typedef struct
{
	TIM_TypeDef *timer;
	uint32_t channel;
	uint8_t gpio_alternate;

} hal_stm32_dac_data;

/*
 * MCU pin data
 */
typedef struct
{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	hal_stm32_dac_data dac_data;
	int8_t adc_rank;

} hal_stm32_gpio_data;

/* API -----------------------------------------------------------------------*/

/*
 * Initialize specific implementation data (clocks etc.)
 */
void hal_stm32_initialize(void);

/*
 * Fill the structure for the given port and pin
 */
int32_t hal_stm32_get_gpio_data(int32_t port, int32_t pin, hal_stm32_gpio_data* gpio);

/* EOF -----------------------------------------------------------------------*/

#endif // _HAL_STM32
