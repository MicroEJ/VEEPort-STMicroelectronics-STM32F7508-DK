/*
 *	C
 *
 *	Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"
#include "dac_manager_impl.h"

/* Fields --------------------------------------------------------------------*/

/*
 * Global common DAC timer channel configuration
 */
static TIM_OC_InitTypeDef hal_dac_sConfig;

/*
 * Global common DAC timer handler.
 */
static TIM_HandleTypeDef hal_dac_timer_handler;

/* API -----------------------------------------------------------------------*/

/*
 * A timer can drive several channels. So the initialization step is performed
 * at startup and only a start/stop action is made on each channel.
 */
int32_t dac_manager_initialize(void)
{
	// compute the prescaler value to have timer counter clock equal to 20 MHz
	uint32_t uhPrescalerValue = (uint32_t)((SystemCoreClock/2) / DAC_TIMER_IN_FREQUENCY) - 1;

	// common configuration for all channels
	hal_dac_sConfig.OCMode       = TIM_OCMODE_PWM1;
	hal_dac_sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	hal_dac_sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	hal_dac_sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	hal_dac_sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	hal_dac_sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

	// common configuration for all timers
	hal_dac_timer_handler.Init.Prescaler         = uhPrescalerValue;
	hal_dac_timer_handler.Init.Period            = (DAC_TIMER_IN_FREQUENCY / DAC_TIMER_OUT_FREQUENCY) - 1;
	hal_dac_timer_handler.Init.ClockDivision     = 0;
	hal_dac_timer_handler.Init.CounterMode       = TIM_COUNTERMODE_UP;
	hal_dac_timer_handler.Init.RepetitionCounter = 0;

	uint8_t dac_ID = 0;
	while(1)
	{
		// get specific DAC data
		hal_stm32_dac_data data;
		if (dac_manager_impl_get_data(dac_ID, &data) != HAL_GPIO_OK)
		{
			// no more DAC timers
			return HAL_GPIO_OK;
		}

		// update global timer handler
		hal_dac_timer_handler.Instance = data.timer;

		// start it
		HAL_TIM_PWM_Init(&hal_dac_timer_handler);

		++dac_ID;
	}
}

void dac_manager_stop(hal_stm32_gpio_data* gpio)
{
	hal_dac_timer_handler.Instance = gpio->dac_data.timer;
	HAL_TIM_PWM_Stop(&hal_dac_timer_handler, gpio->dac_data.channel);
}

int32_t dac_manager_set_value(hal_stm32_gpio_data* gpio, int32_t percentage)
{
	// set the pulse value for channel
	hal_dac_sConfig.Pulse = (hal_dac_timer_handler.Init.Period * percentage) / 100;
	hal_dac_timer_handler.Instance = gpio->dac_data.timer;
	if (HAL_TIM_PWM_ConfigChannel(&hal_dac_timer_handler, &hal_dac_sConfig, gpio->dac_data.channel) != HAL_OK)
	{
		// configuration error
		return HAL_GPIO_NOK;
	}

	// start channel
	if (HAL_TIM_PWM_Start(&hal_dac_timer_handler, gpio->dac_data.channel) != HAL_OK)
	{
		// unknwon error
		return HAL_GPIO_NOK;
	}

	return HAL_GPIO_OK;
}
