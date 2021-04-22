/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 *
 */

#include "watchdog.h"
#include "watchdog_config.h"
#include "stm32f7xx_hal.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

static IWDG_HandleTypeDef hiwdg;
TIM_HandleTypeDef  htim_input_capture;
__IO uint32_t uwCaptureNumber = 0;
__IO uint32_t uwPeriodValue = 0;
__IO uint32_t uwMeasurementDone = 0;

/**
 * @brief This function configures TIM5 to measure the LSI oscillator frequency.
 *
 * @return LSI Frequency if success, 0 if error
 */
static uint32_t GetLSIFrequency(void);

/**
 * @brief This function set the prescaler and reload values to work with.
 *        Note that the reset signal is generated after the entire period has elapsed.
 *
 * @param period watchdog period in milliseconds.
 *
 * @return see watchdog_t for possible return codes
 */
static watchdog_t set_watchdog_counting_time(uint32_t period);

watchdog_t start_watchdog(void) {

	/* If the watchdog is disabled by the application, do nothing */
	if (!MEJ_WATCHDOG_ENABLED()) {
		return WATCHDOG_OK;
	}

	/* The watchdog is either enabled by default or enabled by the application */
	hiwdg.Instance = IWDG;
	hiwdg.Init.Window = 0xFFF;

	if (MEJ_WATCHDOG_PERIOD() != 0) {
		/* User set watchdog period, so use it to configure the watchdog */
		if (set_watchdog_counting_time(MEJ_WATCHDOG_PERIOD()) != WATCHDOG_OK) {
			return WATCHDOG_ERROR;
		}

	} else {
		/* User did not set the watchdog period, so use the maximum period (WATCHDOG_MAX_DELAY_LSI_32KHz) to configure the watchdog */
		hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
		hiwdg.Init.Reload = 0xFFF;
	}

	if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
		printf("ERROR watchdog start failed\n");
		return WATCHDOG_DRIVER_ERROR;
	} else {
		printf("watchdog started\n");
		return WATCHDOG_OK;
	}
}

watchdog_t refresh_watchdog(void) {

	if (!MEJ_WATCHDOG_ENABLED()) {
		return WATCHDOG_OK;
	}

	if (HAL_IWDG_Refresh(&hiwdg) != HAL_OK) {
		printf("ERROR watchdog refresh failed\n");
		return WATCHDOG_DRIVER_ERROR;
	} else {
		return WATCHDOG_OK;
	}
}

bool is_watchdog_reset(void) {
	/* IWDGRST flag set */
	return (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET);
}


void clear_watchdog_flags(void) {

	__HAL_RCC_CLEAR_RESET_FLAGS();
}

#if defined(WD_SUSPENDED_IN_STANDBY_MODE) || defined(WD_SUSPENDED_IN_STOP_MODE)

void watchdog_suspend(watchdog_mode_type_t wd_mode) {

	if (wd_mode == WATCHDOG_STOP_MODE) {
		FLASH->OPTCR &= ~(FLASH_OPTCR_IWDG_STOP);
	} else if (wd_mode == WATCHDOG_STANDBY_MODE) {
		FLASH->OPTCR &= ~(FLASH_OPTCR_IWDG_STDBY);
	}
}

void watchdog_resume(watchdog_mode_type_t wd_mode) {
	if (wd_mode == WATCHDOG_STOP_MODE) {
		FLASH->OPTCR |= (FLASH_OPTCR_IWDG_STOP);
	} else if (wd_mode == WATCHDOG_STANDBY_MODE) {
		FLASH->OPTCR |= (FLASH_OPTCR_IWDG_STDBY);
	}
}

#endif /* WD_SUSPENDED_IN_STANDBY_MODE || WD_SUSPENDED_IN_STOP_MODE */

static uint32_t GetLSIFrequency(void) {

	uint32_t uwLsiFreq = 0;
	uint32_t pclk1 = 0;
	TIM_IC_InitTypeDef timinputconfig;

	/* Enable the LSI oscillator */
	__HAL_RCC_LSI_ENABLE();

	/* Wait till LSI is ready */
	while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET)
	{
	}

	/* Configure the TIM peripheral */
	/* Set TIMx instance */
	htim_input_capture.Instance = TIM5;

	/* TIM5 configuration: Input Capture mode ---------------------
	The LSI oscillator is connected to TIM5 CH4.
	The Rising edge is used as active edge.
	The TIM5 CCR4 is used to compute the frequency value.
	------------------------------------------------------------ */
	htim_input_capture.Init.Prescaler         = 0;
	htim_input_capture.Init.CounterMode       = TIM_COUNTERMODE_UP;
	htim_input_capture.Init.Period            = 0xFFFF;
	htim_input_capture.Init.ClockDivision     = 0;
	htim_input_capture.Init.RepetitionCounter = 0;
	if (HAL_TIM_IC_Init(&htim_input_capture) != HAL_OK) {
		/* Initialization Error */
		printf("ERROR watchdog GetLSIFrequency\n");
		return uwLsiFreq;
	}
	/* Connect internally the TIM5_CH4 Input Capture to the LSI clock output */
	HAL_TIMEx_RemapConfig(&htim_input_capture, TIM_TIM5_LSI);

	/* Configure the Input Capture of channel 4 */
	timinputconfig.ICPolarity  = TIM_ICPOLARITY_RISING;
	timinputconfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	timinputconfig.ICPrescaler = TIM_ICPSC_DIV8;
	timinputconfig.ICFilter    = 0;

	if (HAL_TIM_IC_ConfigChannel(&htim_input_capture, &timinputconfig, TIM_CHANNEL_4) != HAL_OK) {
		/* Initialization Error */
		printf("ERROR watchdog GetLSIFrequency\n");
		return uwLsiFreq;
	}

	/* Reset the flags */
	htim_input_capture.Instance->SR = 0;

	/* Start the TIM Input Capture measurement in interrupt mode */
	if (HAL_TIM_IC_Start_IT(&htim_input_capture, TIM_CHANNEL_4) != HAL_OK) {
		printf("ERROR watchdog GetLSIFrequency\n");
		return uwLsiFreq;
	}

	/* Wait until the TIM5 get 2 LSI edges (refer to TIM5_IRQHandler() in
	stm32f7xx_it.c file) */
	while (uwMeasurementDone == 0)
	{
	}
	uwCaptureNumber = 0;

	/* Deinitialize the TIM5 peripheral registers to their default reset values */
	HAL_TIM_IC_DeInit(&htim_input_capture);

	/* Compute the LSI frequency, depending on TIM5 input clock frequency (PCLK1)*/
	/* Get PCLK1 frequency */
	pclk1 = HAL_RCC_GetPCLK1Freq();

	/* Get PCLK1 prescaler */
	if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0) {
		/* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
		uwLsiFreq = ((pclk1 / uwPeriodValue) * 8);
	} else {
		/* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
		uwLsiFreq = (((2 * pclk1) / uwPeriodValue) * 8) ;
	}

	return uwLsiFreq;
}

static watchdog_t set_watchdog_counting_time(uint32_t period) {

	uint32_t uwLsiFreq = GetLSIFrequency();

	if (uwLsiFreq == 0) {
		return WATCHDOG_DRIVER_ERROR;
	}

	/* Compute the time in us for 1 cycle, depending on LSI oscillator frequency
	 * 4 corresponding to IWDG_PRESCALER_4
	 * 1 s = 1000000 us
	 */
	uint32_t min_time_step = 4000000 / uwLsiFreq;

	/* Set the lowest prescaler allowed for the period
	 * Compute minimum waiting time for the prescaler got
	 *
	 * Note that reload register has 12 bits, so maximum waiting time
	 * is minimum waiting time multiplied by 2^12
	 * 2^12 us = 4.096 ms
	 */
	if (period == 0) {
		printf("ERROR watchdog minimum supported period is 1 ms\n");
		return WATCHDDOG_INVALID_PARAM;
	} else if (period < (min_time_step * 4096 / 1000)) {
		hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
		min_time_step *= 1;
	} else if (period < (min_time_step * 4096 * 2 / 1000)) {
		hiwdg.Init.Prescaler = IWDG_PRESCALER_8;
		min_time_step *= 2;
	} else if (period < (min_time_step * 4096 * 4 / 1000)) {
		hiwdg.Init.Prescaler = IWDG_PRESCALER_16;
		min_time_step *= 4;
	} else if (period < (min_time_step * 4096 * 8 / 1000)) {
		hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
		min_time_step *= 8;
	} else if (period < (min_time_step * 4096 * 16 / 1000)) {
		hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
		min_time_step *= 16;
	} else if (period < (min_time_step * 4096 * 32 / 1000)) {
		hiwdg.Init.Prescaler = IWDG_PRESCALER_128;
		min_time_step *= 32;
	} else if (period < (min_time_step * 4096 * 64 / 1000)) {
		hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
		min_time_step *= 64;
	} else {
		printf("ERROR watchdog maximum supported period is %ld ms\n", (uint32_t)(min_time_step * 4096 * 64 / 1000));
		return WATCHDDOG_INVALID_PARAM;
	}

	/* Add 1 to ensure that the entire period has elapsed */
	hiwdg.Init.Reload = (uint32_t)((period * 1000) / min_time_step) + 1;
	/* Avoid reload register overflow */
	if (hiwdg.Init.Reload > 0xFFF) {
		hiwdg.Init.Reload = 0xFFF;
	}

	return WATCHDOG_OK;
}


#ifdef __cplusplus
}
#endif
