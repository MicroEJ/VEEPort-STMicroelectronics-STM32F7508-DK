/*
 *	C
 *
 *	Copyright 2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"
#include "adc_manager.h"

/* Enum ----------------------------------------------------------------------*/

/*
 * List of ports used by this implementation
 * Port '0' is HAL_STM32_GPIO_PORT_MCU
 */
typedef enum
{
	/*
	 * List of MCU ports used by this implementation
	 */
	LLHAL_GPIO_PORT_A = 1,
	LLHAL_GPIO_PORT_B = 2,
	LLHAL_GPIO_PORT_F = 6,
	LLHAL_GPIO_PORT_G = 7,
	LLHAL_GPIO_PORT_H = 8,
	LLHAL_GPIO_PORT_I = 9,

	/*
	 * List of standard ports used by this implementation
	 */
	LLHAL_GPIO_PORT_ARDUINO_DIGITAL = 30,
	LLHAL_GPIO_PORT_ARDUINO_ANALOG = 31,

	/*
	 * List of specific ports used by this implementation
	 */
	LLHAL_GPIO_PORT_CN4 = 64,
	LLHAL_GPIO_PORT_CN5 = 65,
	LLHAL_GPIO_PORT_CN7 = 67,

} HAL_GPIO_IMPL_Port;

/*
 * List of DAC IDs
 */
typedef enum
{
	HAL_PB4_DAC_ID = 0,
	HAL_PI0_DAC_ID = 1,
	HAL_PH6_DAC_ID = 2,
	HAL_PA8_DAC_ID = 3,
	HAL_PB15_DAC_ID = 4,

} hal_stm32_dac_IDs;

/*
 * List of ADC IDs
 */
typedef enum
{
	HAL_ADC3_ID = 0,

} hal_stm32_adc_IDs;

/*
 * List of ADC ranks
 */
typedef enum
{
	HAL_PA0_ADC_ID = 0,
	HAL_PF10_ADC_ID = 1,
	HAL_PF9_ADC_ID = 2,
	HAL_PF8_ADC_ID = 3,
	HAL_PF7_ADC_ID = 4,
	HAL_PF6_ADC_ID = 5,

} hal_stm32_adc_ranks;


/* Fields --------------------------------------------------------------------*/

static ADC_HandleTypeDef Adc3Handle;
static DMA_HandleTypeDef hdma_adc3;

/* Public functions ----------------------------------------------------------*/

void hal_stm32_initialize(void)
{
	// GPIO clocks
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	// ADC clocks
	__HAL_RCC_ADC3_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();

	// DAC clocks
	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM5_CLK_ENABLE();
	__HAL_RCC_TIM12_CLK_ENABLE();

	Adc3Handle.Instance = ADC3;
	Adc3Handle.Init.NbrOfConversion = 6;
	hdma_adc3.Instance = DMA2_Stream0;
	hdma_adc3.Init.Channel  = DMA_CHANNEL_2;
}

int32_t dac_manager_impl_get_data(int32_t dac_ID, hal_stm32_dac_data* dac_data)
{
	switch(dac_ID)
	{
	case HAL_PB4_DAC_ID:
		// TIM3_CH1 <-> PB4 <-> ARD_D3
		dac_data->timer = TIM3;
		dac_data->channel = TIM_CHANNEL_1;
		dac_data->gpio_alternate = GPIO_AF2_TIM3;
		break;
	case HAL_PI0_DAC_ID:
		// TIM5_CH4 <-> PI0 <-> ARD_D5
		dac_data->timer = TIM5;
		dac_data->channel = TIM_CHANNEL_4;
		dac_data->gpio_alternate = GPIO_AF2_TIM5;
		break;
	case HAL_PH6_DAC_ID:
		// TIM12_CH1 <-> PH6 <-> ARD_D6
		dac_data->timer = TIM12;
		dac_data->channel = TIM_CHANNEL_1;
		dac_data->gpio_alternate = GPIO_AF9_TIM12;
		break;
	case HAL_PA8_DAC_ID:
		// TIM1_CH1 <-> PA8 <-> ARD_D10
		dac_data->timer = TIM1;
		dac_data->channel = TIM_CHANNEL_1;
		dac_data->gpio_alternate = GPIO_AF1_TIM1;
		break;
	case HAL_PB15_DAC_ID:
		// TIM12_CH2 <-> PB15 <-> ARD_D11
		dac_data->timer = TIM12;
		dac_data->channel = TIM_CHANNEL_2;
		dac_data->gpio_alternate = GPIO_AF9_TIM12;
		break;
	default:
		// invalid ID
		return HAL_GPIO_NOK;
	}
	return HAL_GPIO_OK;
}

int32_t adc_manager_impl_get_data(int32_t adc_ID, ADC_HandleTypeDef** AdcHandle, DMA_HandleTypeDef** hdma_adc)
{
	switch (adc_ID)
	{
	case HAL_ADC3_ID:
		*hdma_adc = &hdma_adc3;
		*AdcHandle = &Adc3Handle;
		break;

	default:
		// invalid ID
		return HAL_GPIO_NOK;
	}
	return HAL_GPIO_OK;
}


void adc_manager_impl_configure_ranks(int32_t adc_ID, ADC_HandleTypeDef* AdcHandle)
{
	switch (adc_ID)
	{
	case HAL_ADC3_ID:
		adc_manager_configure_rank(AdcHandle, 1, ADC_CHANNEL_0);	// ADC3_CH0 <-> PA0 <-> ARD_A0
		adc_manager_configure_rank(AdcHandle, 2, ADC_CHANNEL_8);	// ADC3_CH8 <-> PF10 <-> ARD_A1
		adc_manager_configure_rank(AdcHandle, 3, ADC_CHANNEL_7);	// ADC3_CH7 <-> PF9 <-> ARD_A2
		adc_manager_configure_rank(AdcHandle, 4, ADC_CHANNEL_6);	// ADC3_CH6 <-> PF8 <-> ARD_A3
		adc_manager_configure_rank(AdcHandle, 5, ADC_CHANNEL_5);	// ADC3_CH5 <-> PF7 <-> ARD_A4
		adc_manager_configure_rank(AdcHandle, 6, ADC_CHANNEL_4);	// ADC3_CH4 <-> PF6 <-> ARD_A5
		break;
	}
}

int32_t hal_stm32_get_gpio_data(int32_t port, int32_t pin, hal_stm32_gpio_data* gpio)
{
	switch(port)
	{
	case LLHAL_GPIO_PORT_A:
	{
		/*
		 * GPIOA check and initialization
		 */
		switch(pin)
		{
		case 0:
			gpio->adc_rank = HAL_PA0_ADC_ID;
			break;
		case 8:
			dac_manager_impl_get_data(HAL_PA8_DAC_ID, &gpio->dac_data);
			break;
		case 15:
			// only digital pin
			break;
		default:
			// invalid pin
			return HAL_GPIO_NOK;
		}

		gpio->GPIOx = GPIOA;
		gpio->GPIO_Pin = 1 << pin;
		break;
	}

	case LLHAL_GPIO_PORT_B:
	{
		/*
		 * GPIOB check and initialization
		 */
		switch(pin)
		{
		case 4:
			dac_manager_impl_get_data(HAL_PB4_DAC_ID, &gpio->dac_data);
			break;
		case 14:
			// only digital pin
			break;
		case 15:
			dac_manager_impl_get_data(HAL_PB15_DAC_ID, &gpio->dac_data);
			break;
		default:
			// invalid pin
			return HAL_GPIO_NOK;
		}

		gpio->GPIOx = GPIOB;
		gpio->GPIO_Pin = 1 << pin;
		break;
	}

	case LLHAL_GPIO_PORT_F:
	{
		/*
		 * GPIOF check and initialization
		 */
		switch(pin)
		{
		case 6:
			gpio->adc_rank = HAL_PF6_ADC_ID;
			break;
		case 7:
			gpio->adc_rank = HAL_PF7_ADC_ID;
			break;
		case 8:
			gpio->adc_rank = HAL_PF8_ADC_ID;
			break;
		case 9:
			gpio->adc_rank = HAL_PF9_ADC_ID;
			break;
		case 10:
			gpio->adc_rank = HAL_PF10_ADC_ID;
			break;
		default:
			// invalid pin
			return HAL_GPIO_NOK;
		}

		gpio->GPIOx = GPIOF;
		gpio->GPIO_Pin = 1 << pin;
		break;
	}

	case LLHAL_GPIO_PORT_G:
	{
		/*
		 * GPIOG check and initialization
		 */
		switch(pin)
		{
		case 6:
		case 7:
			// only digital pin
			break;
		default:
			// invalid pin
			return HAL_GPIO_NOK;
		}

		gpio->GPIOx = GPIOG;
		gpio->GPIO_Pin = 1 << pin;
		break;
	}

	case LLHAL_GPIO_PORT_H:
	{
		/*
		 * GPIOH check and initialization
		 */
		switch(pin)
		{
		case 6:
			dac_manager_impl_get_data(HAL_PH6_DAC_ID, &gpio->dac_data);
			break;
		default:
			// invalid pin
			return HAL_GPIO_NOK;
		}

		gpio->GPIOx = GPIOH;
		gpio->GPIO_Pin = 1 << pin;
		break;
	}

	case LLHAL_GPIO_PORT_I:
	{
		/*
		 * GPIOI check and initialization
		 */
		switch(pin)
		{
		case 0:
			dac_manager_impl_get_data(HAL_PI0_DAC_ID, &gpio->dac_data);
			break;
		case 1:
		case 2:
		case 3:
			// only digital pin
			break;
		default:
			// invalid pin
			return HAL_GPIO_NOK;
		}

		gpio->GPIOx = GPIOI;
		gpio->GPIO_Pin = 1 << pin;
		break;
	}

	case LLHAL_GPIO_PORT_ARDUINO_DIGITAL:
	{
		/*
		 * ARDUINO digital port: convert port and pin using MCU port and pin
		 */
		switch(pin)
		{
		// pin 0 and 1 are used by LLCOMM_UART
		//		case 0:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_C, 7, gpio);
		//		case 1:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_C, 6, gpio);
		case 2:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_G, 6, gpio);
		case 3:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 4, gpio);
		case 4:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_G, 7, gpio);
		case 5:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_I, 0, gpio);
		case 6:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_H, 6, gpio);
		case 7:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_I, 3, gpio);
		case 8:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_I, 2, gpio);
		case 9:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_A, 15, gpio);
		case 10:	return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_A, 8, gpio);
		case 11:	return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 15, gpio);
		case 12:	return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 14, gpio);
		case 13:	return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_I, 1, gpio);
		// pin 14 and 15 are used by I2C1
		//		case 14:	return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 9, gpio);
		//		case 15:	return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 8, gpio);
		default:	return HAL_GPIO_NOK;	// invalid pin
		}
	}

	case LLHAL_GPIO_PORT_ARDUINO_ANALOG:
	{
		/*
		 * ARDUINO analog port: convert port and pin using MCU port and pin
		 */
		switch(pin)
		{
		case 0:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_A, 0, gpio);
		case 1:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 10, gpio);
		case 2:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 9, gpio);
		case 3:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 8, gpio);
		case 4:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 7, gpio);
		case 5:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 6, gpio);
		default:	return HAL_GPIO_NOK;	// invalid pin
		}
	}

	case LLHAL_GPIO_PORT_CN4:
	{
		/*
		 * CN4 port: convert port and pin using MCU port and pin
		 */
		switch(pin)
		{
		// pin 1 and 2 are used by LLCOMM_UART
		//		case 1:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_C, 7, gpio);
		//		case 2:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_C, 6, gpio);
		case 3:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_G, 6, gpio);
		case 4:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 4, gpio);
		case 5:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_G, 7, gpio);
		case 6:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_I, 0, gpio);
		case 7:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_H, 6, gpio);
		case 8:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_I, 3, gpio);
		default:	return HAL_GPIO_NOK;	// invalid pin
		}
	}

	case LLHAL_GPIO_PORT_CN5:
	{
		/*
		 * CN5 port: convert port and pin using MCU port and pin
		 */
		switch(pin)
		{
		case 1:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_A, 0, gpio);
		case 2:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 10, gpio);
		case 3:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 9, gpio);
		case 4:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 8, gpio);
		case 5:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 7, gpio);
		case 6:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_F, 6, gpio);
		default:	return HAL_GPIO_NOK;	// invalid pin
		}
	}

	case LLHAL_GPIO_PORT_CN7:
	{
		/*
		 * CN7 port: convert port and pin using MCU port and pin
		 */
		switch(pin)
		{
		case 1:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_I, 2, gpio);
		case 2:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_A, 15, gpio);
		case 3:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_A, 8, gpio);
		case 4:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 15, gpio);
		case 5:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 14, gpio);
		case 6:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_I, 1, gpio);
		// pin 7 and 8 are used by I2C1
		//		case 7:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 9, gpio);
		//		case 8:		return hal_stm32_get_gpio_data(LLHAL_GPIO_PORT_B, 8, gpio);
		default:	return HAL_GPIO_NOK;	// invalid pin
		}
	}

	default:
		// invalid port
		return HAL_GPIO_NOK;
	}
	return HAL_GPIO_OK;
}

/* EOF -----------------------------------------------------------------------*/
