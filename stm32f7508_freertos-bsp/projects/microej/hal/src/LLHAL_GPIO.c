/*
 *	C
 *
 *	Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"
#include "adc_manager.h"
#include "dac_manager.h"

/* Macros --------------------------------------------------------------------*/

/*
 * Check port and pin, update "gpio" struct
 */
#define __hal_stm32_get_gpio_data(port, pin, gpio)   do { \
		if (get_gpio_data(port, pin, gpio) == HAL_GPIO_NOK) \
		return HAL_GPIO_NOK; /* invalid port or pin */ \
		} while(0)

/* Fields --------------------------------------------------------------------*/

static uint8_t initialized = 0;

/* Private functions ---------------------------------------------------------*/

static int32_t get_gpio_data(int32_t port, int32_t pin, hal_stm32_gpio_data* gpio)
{
	// set default value of analog converters
	gpio->adc_rank = HAL_GPIO_NOK;
	gpio->dac_data.timer = NULL;

	if (port == HAL_STM32_GPIO_PORT_MCU)
	{
		/*
		 * MCU virtual port: convert port and pin using real MCU port and pin
		 *
		 * MCU port = (pin / 16) + 1
		 * MCU pin = pin % 16
		 */

		if (pin < 0)
		{
			return HAL_GPIO_NOK;	// invalid pin
		}
		return hal_stm32_get_gpio_data((pin >> 4) + 1, pin & 0xf, gpio);
	}
	else
	{
		return hal_stm32_get_gpio_data(port,  pin, gpio);
	}
}

/* API -----------------------------------------------------------------------*/

int32_t LLHAL_GPIO_IMPL_setPinMode(int32_t port, int32_t pin, int32_t mode)
{
	if (!initialized)
	{
		hal_stm32_initialize();

		if (dac_manager_initialize() != HAL_GPIO_OK)
		{
			// unknown error
			return HAL_GPIO_NOK;
		}

		if (adc_manager_initialize() != HAL_GPIO_OK)
		{
			// unknown error
			return HAL_GPIO_NOK;
		}

		initialized = !initialized;
	}

	// retrieve GPIO data
	hal_stm32_gpio_data gpio;
	__hal_stm32_get_gpio_data(port, pin, &gpio);

	// initialize GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pin = gpio.GPIO_Pin;

	switch (mode)
	{

	case LLHAL_GPIO_MODE_DIGITAL_INPUT:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		break;

	case LLHAL_GPIO_MODE_DIGITAL_INPUT_PULLUP:
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		break;

	case LLHAL_GPIO_MODE_DIGITAL_OUTPUT:
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		break;

	case LLHAL_GPIO_MODE_ANALOG_INPUT:

		if (gpio.adc_rank == HAL_GPIO_NOK)
		{
			// this is not a ADC pin
			return HAL_GPIO_NOK;
		}

		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		break;

	case LLHAL_GPIO_MODE_ANALOG_OUTPUT:
	{
		if (gpio.dac_data.timer == NULL)
		{
			// this is not a DAC pin
			return HAL_GPIO_NOK;
		}

		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Alternate = gpio.dac_data.gpio_alternate;
		break;
	}
	}

	if (mode != LLHAL_GPIO_MODE_ANALOG_OUTPUT && gpio.dac_data.timer != NULL)
	{
		// stop the DAC timer (might be not running)
		dac_manager_stop(&gpio);
	}

	HAL_GPIO_Init(gpio.GPIOx, &GPIO_InitStruct);

	// no error
	return HAL_GPIO_OK;
}

int32_t LLHAL_GPIO_IMPL_getDigitalValue(int32_t port, int32_t pin)
{
	// retrieve GPIO data
	hal_stm32_gpio_data gpio;
	__hal_stm32_get_gpio_data(port, pin, &gpio);

	return HAL_GPIO_ReadPin(gpio.GPIOx, gpio.GPIO_Pin) == GPIO_PIN_SET ? LLHAL_GPIO_DIGITAL_HIGH : LLHAL_GPIO_DIGITAL_LOW;
}

int32_t LLHAL_GPIO_IMPL_setDigitalValue(int32_t port, int32_t pin, int32_t value)
{
	// retrieve GPIO data
	hal_stm32_gpio_data gpio;
	__hal_stm32_get_gpio_data(port, pin, &gpio);

	HAL_GPIO_WritePin(gpio.GPIOx, gpio.GPIO_Pin, value == LLHAL_GPIO_DIGITAL_LOW ? GPIO_PIN_RESET : GPIO_PIN_SET);
	return HAL_GPIO_OK;
}

int32_t LLHAL_GPIO_IMPL_getAnalogValue(int32_t port, int32_t pin)
{
	// retrieve GPIO data
	hal_stm32_gpio_data gpio;
	__hal_stm32_get_gpio_data(port, pin, &gpio);

	if (gpio.adc_rank == HAL_GPIO_NOK)
	{
		// this is not an ADC pin
		return HAL_GPIO_NOK;
	}

	return adc_manager_get_value(&gpio);
}

int32_t LLHAL_GPIO_IMPL_setAnalogValue(int32_t port, int32_t pin, int32_t percentage)
{
	// retrieve GPIO data
	hal_stm32_gpio_data gpio;
	__hal_stm32_get_gpio_data(port, pin, &gpio);

	if (gpio.dac_data.timer == NULL)
	{
		// this is not a DAC pin
		return HAL_GPIO_NOK;
	}

	return dac_manager_set_value(&gpio, percentage);
}

/* EOF -----------------------------------------------------------------------*/

