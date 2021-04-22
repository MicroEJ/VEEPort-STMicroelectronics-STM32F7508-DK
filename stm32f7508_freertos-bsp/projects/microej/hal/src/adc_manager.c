/*
 *	C
 *
 *	Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 *	This library is provided in source code for use, modification and test, subject to license terms.
 *	Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "hal_stm32.h"
#include "adc_manager_impl.h"

/* Fields --------------------------------------------------------------------*/

/*
 * Buffer used to store ADC converted values
 */
static __IO uint32_t converted_values[ADC_NB_CHANNELS];

/* Private functions ---------------------------------------------------------*/

/*
 * Same function than HAL_ADC_Start_DMA(...) available in "stm32fxxx_hal_adc.c" but without
 * ADC and DMA interrupts configuration. In our case, the conversions are always running and
 * we don't need the interruptions calls.
 */
static HAL_StatusTypeDef _HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length)
{
	__IO uint32_t counter = 0;

	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(hadc->Init.ContinuousConvMode));
	assert_param(IS_ADC_EXT_TRIG_EDGE(hadc->Init.ExternalTrigConvEdge));

	/* Process locked */
	__HAL_LOCK(hadc);

	/* Enable the ADC peripheral */
	/* Check if ADC peripheral is disabled in order to enable it and wait during
		Tstab time the ADC's stabilization */
	if((hadc->Instance->CR2 & ADC_CR2_ADON) != ADC_CR2_ADON)
	{
		/* Enable the Peripheral */
		__HAL_ADC_ENABLE(hadc);

		/* Delay for ADC stabilization time */
		/* Compute number of CPU cycles to wait for */
		counter = (ADC_STAB_DELAY_US * (SystemCoreClock / 1000000));
		while(counter != 0)
		{
			counter--;
		}
	}

	/* Start conversion if ADC is effectively enabled */
	if(HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_ADON))
	{
		/* Set ADC state                                                          */
		/* - Clear state bitfield related to regular group conversion results     */
		/* - Set state bitfield related to regular group operation                */
		ADC_STATE_CLR_SET(hadc->State,
							HAL_ADC_STATE_READY | HAL_ADC_STATE_REG_EOC | HAL_ADC_STATE_REG_OVR,
							HAL_ADC_STATE_REG_BUSY);

		/* If conversions on group regular are also triggering group injected,    */
		/* update ADC state.                                                      */
		if (READ_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO) != RESET)
		{
			ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
		}

		/* State machine update: Check if an injected conversion is ongoing */
		if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
		{
			/* Reset ADC error code fields related to conversions on group regular */
			CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));
		}
		else
		{
			/* Reset ADC all error code fields */
			ADC_CLEAR_ERRORCODE(hadc);
		}

		/* Process unlocked */
		/* Unlock before starting ADC conversions: in case of potential           */
		/* interruption, to let the process to ADC IRQ Handler.                   */
		__HAL_UNLOCK(hadc);   

		/* Manage ADC and DMA start: ADC overrun interruption, DMA start, ADC     */
		/* start (in case of SW start):                                           */

		/* Clear regular group conversion flag and overrun flag */
		/* (To ensure of no unknown state from potential previous ADC operations) */
		__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC | ADC_FLAG_OVR);

		/* Enable ADC DMA mode */
		hadc->Instance->CR2 |= ADC_CR2_DMA;

		/* Start the DMA channel */
		HAL_DMA_Start(hadc->DMA_Handle, (uint32_t)&hadc->Instance->DR, (uint32_t)pData, Length);

		/* Check if Multimode enabled */
		if(HAL_IS_BIT_CLR(ADC->CCR, ADC_CCR_MULTI))
		{
			/* if no external trigger present enable software conversion of regular channels */
			if((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET)
			{
				/* Enable the selected ADC software conversion for regular group */
				hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
			}
		}
		else
		{
			/* if instance of handle correspond to ADC1 and  no external trigger present enable software conversion of regular channels */
			if((hadc->Instance == ADC1) && ((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET))
			{
				/* Enable the selected ADC software conversion for regular group */
				hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
			}
			/* if dual mode is selected, ADC3 works independently. */
			/* check if the mode selected is not triple */
			if( HAL_IS_BIT_CLR(ADC->CCR, ADC_CCR_MULTI_4) )
			{
				/* if instance of handle correspond to ADC3 and  no external trigger present enable software conversion of regular channels */
				if((hadc->Instance == ADC3) && ((hadc->Instance->CR2 & ADC_CR2_EXTEN) == RESET))
				{
					/* Enable the selected ADC software conversion for regular group */
					hadc->Instance->CR2 |= (uint32_t)ADC_CR2_SWSTART;
				}
			}
		}
	}

	/* Return function status */
	return HAL_OK;
}

/* API -----------------------------------------------------------------------*/

int32_t adc_manager_initialize(void)
{
	uint8_t adc_ID = 0;
	uint32_t* array_ptr = (uint32_t*)converted_values;

	while(1)
	{
		ADC_HandleTypeDef* AdcHandle;
		DMA_HandleTypeDef* hdma_adc;

		// get specific ADC data
		if (adc_manager_impl_get_data(adc_ID, &AdcHandle, &hdma_adc) != HAL_GPIO_OK)
		{
			// no more ADC
			return HAL_GPIO_OK;
		}

		hdma_adc->Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc->Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc->Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adc->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_adc->Init.Mode = DMA_CIRCULAR;
		hdma_adc->Init.Priority = DMA_PRIORITY_HIGH;
		hdma_adc->Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hdma_adc->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_adc->Init.MemBurst = DMA_MBURST_SINGLE;
		hdma_adc->Init.PeriphBurst = DMA_PBURST_SINGLE;
		HAL_DMA_Init(hdma_adc);

		// associate the initialized DMA handle to the ADC handle
		AdcHandle->DMA_Handle = hdma_adc;
		hdma_adc->Parent = AdcHandle;

		AdcHandle->Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
		AdcHandle->Init.Resolution            = ADC_RESOLUTION_12B;
		AdcHandle->Init.ScanConvMode          = ENABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
		AdcHandle->Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig */
		AdcHandle->Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
		AdcHandle->Init.NbrOfDiscConversion   = 0;
		AdcHandle->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start trigged at each external event */
		AdcHandle->Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
		AdcHandle->Init.DataAlign             = ADC_DATAALIGN_RIGHT;
		AdcHandle->Init.DMAContinuousRequests = ENABLE;
		AdcHandle->Init.EOCSelection          = DISABLE;

		// reduce the available number of channels if needed
		if(AdcHandle->Init.NbrOfConversion > ADC_NB_CHANNELS){
			AdcHandle->Init.NbrOfConversion = ADC_NB_CHANNELS;
		}

		if (HAL_ADC_Init(AdcHandle) != HAL_OK)
		{
			// ADC initialization Error
			return HAL_GPIO_NOK;
		}

		// initialize all ranks for this ADC
		adc_manager_impl_configure_ranks(adc_ID, AdcHandle);

		// start the conversion process
		if(_HAL_ADC_Start_DMA(AdcHandle, array_ptr, AdcHandle->Init.NbrOfConversion) != HAL_OK)
		{
			// start Conversation Error
			return HAL_GPIO_NOK;
		}

		++adc_ID;
		array_ptr += AdcHandle->Init.NbrOfConversion;
	}
}

void adc_manager_configure_rank(ADC_HandleTypeDef* AdcHandle, uint32_t rank, uint32_t channel)
{
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel      = channel;
	sConfig.Rank         = rank;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(AdcHandle, &sConfig);
}

int32_t adc_manager_get_value(hal_stm32_gpio_data* gpio)
{
	return converted_values[gpio->adc_rank];
}
