/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "microej_time.h"
#include "microej.h"
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"

/* Defines -------------------------------------------------------------------*/

#define TIMx                           TIM2
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE()

// TIMER definition
#define TIMER_CHANNEL TIM_CHANNEL_1

#define MAX_TIMER_VALUE 0xFFFFFFFF
#define TC_CLOCK 1000000    // clock frequency in Hertz

/* Globals -------------------------------------------------------------------*/

// this value is updated at each interrupt
static int64_t software_counter;

/* TIM handle declaration */
static TIM_HandleTypeDef TimHandle;

/** Offset in ms from system time to application time */
static uint64_t microej_application_time_offset = 0;

/* Private functions ---------------------------------------------------------*/

static int64_t timer_get_counter_value(void)
{
	int64_t tc = __HAL_TIM_GET_COUNTER(&TimHandle);
	return tc & 0xffffffff;
}

static int64_t time_hardware_timer_getTicks(void)
{
	return software_counter + timer_get_counter_value();
}

/*
 * An interrupt can occur between the reading of software_counter
 * and timer_get_counter_value(). So the value of software_counter is not
 * coherent with the value of timer_get_counter_value().
 * Compute twice the ticks value. At least one will be correct.
 */
static int64_t time_hardware_timer_getAndCheckTicks(void)
{
	int64_t val1 = time_hardware_timer_getTicks();
	int64_t val2 = time_hardware_timer_getTicks();

	if (val2 < val1) {   // second computation get back in the past: val2 is not valid for sure
		return val1;
	} else {             // first computation may be invalid (to far in the past): val2 is valid for sure
		return val2;
	}
}

static void timer_set_interrupt_value(int64_t itVal)
{
	__HAL_TIM_SET_COMPARE(&TimHandle, TIMER_CHANNEL, (uint32_t)itVal);
	__HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_CC1);
}

static int64_t timer_get_max_counter_value(void)
{
	return MAX_TIMER_VALUE;
}

static int64_t timer_get_max_value(void)
{
	// get the max value which can be used for an interrupt
	// return the max value of the timer divided by 2
	// if the interrupt is initialize to the max value of timer, an overflow will occurs
	return timer_get_max_counter_value() >> 1; // / 2
}

/*
 * Increment the soft timer value
 */
static void timer_update_software_counter(void)
{
	int64_t currentTimerValue = timer_get_counter_value();
	software_counter += currentTimerValue;
}

/*
 * Reconfigure interrupt value
 */
static void timer_configure_next_interrupt(void)
{
	timer_set_interrupt_value(timer_get_max_value());
}

static void timer_set_counter_value(int64_t timeVal)
{
	__HAL_TIM_SET_COUNTER(&TimHandle, (uint32_t) timeVal);
}

/* Interrupt functions -------------------------------------------------------*/

void TIM2_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&TimHandle, TIM_IT_CC1);

	// increment the soft timer value and reset the hardware timer to 0
	timer_update_software_counter();
	timer_set_counter_value(0);

	timer_configure_next_interrupt();
}

/* Public functions ----------------------------------------------------------*/

void microej_time_init(void)
{
	TIM_OC_InitTypeDef sConfig;
	uint32_t uwPrescalerValue = 0;

	// initialize fields
	software_counter = 0;
	TimHandle.Instance = TIMx;

	// initialize hardware timer
	{
		// enable timer clock
		TIMx_CLK_ENABLE();

		// Compute the prescaler value to have TIMER counter clock equal to TC_CLOCK Hz
		uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / TC_CLOCK) - 1;

		// Initialize TIMER peripheral as follows:
		//   - Period = TC_CLOCK - 1
		//   - Prescaler = ((SystemCoreClock / 2)/TC_CLOCK) - 1
		//   - ClockDivision = 1
		//   - Counter direction = Up
		TimHandle.Init.Period            = MAX_TIMER_VALUE;
		TimHandle.Init.Prescaler         = uwPrescalerValue;
		TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
		TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
		TimHandle.Init.RepetitionCounter = 0;

		if (HAL_TIM_OC_Init(&TimHandle) != HAL_OK) {
			while(1); // error
		}

		// configure the Output Compare channel 1
		sConfig.OCMode       = TIM_OCMODE_ACTIVE;
		sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
		sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH /*not used*/;
		sConfig.Pulse        = MAX_TIMER_VALUE;
		sConfig.OCIdleState  = TIM_OCIDLESTATE_SET /*not used*/;
		sConfig.OCNIdleState = TIM_OCNIDLESTATE_SET /*not used*/;

		if (HAL_TIM_OC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK) {
			while(1); // error
		}

		// initialize the timer value to 0
		timer_set_counter_value(0);
		// initialize the interrupt
		timer_configure_next_interrupt();

		// start timer channel 1 in Output compare mode
		if (HAL_TIM_OC_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK) {
			while(1); // error
		}
	}

	HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0x00);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

int64_t microej_time_getcurrenttime(uint8_t isPlatformTime)
{
	/*
	 * /!\
	 * isPlatformTime == true when ej.bon.Util.platformTimeMillis
	 * isPlatformTime == false when java.lang.System.currentTimeMillis
	 * /!\
	 */

	int64_t systemTime = time_hardware_timer_getAndCheckTicks() / 1000;
	if (isPlatformTime == MICROEJ_TRUE) {
		return systemTime;
	} else {
		return systemTime + microej_application_time_offset;
	}
}

int64_t microej_time_gettimenanos(void)
{
	return time_hardware_timer_getAndCheckTicks() * 1000;
}

void microej_time_setapplicationtime(int64_t t)
{
	int64_t currentTime = (int64_t)microej_time_getcurrenttime(MICROEJ_TRUE);
	microej_application_time_offset = t - currentTime;
}

/*
 * Convert a time in milliseconds in ticks.
 * Result is round up to the next tick (ie. converting back the resulting ticks
 * to milliseconds results in a value greater than or equals to given time).
 * Return a saturated number of ticks or 0 if given time is negative.
 */
int64_t microej_time_timeToTick(int64_t time)
{
	if (time < 0) {
		return 0;
	}

	int64_t mticks = time * (int64_t)configTICK_RATE_HZ; // milli-ticks

	// Check for no overflow
	if (mticks >= 0) {
		int64_t ticks64 = (mticks + 999LL) / 1000LL;

		// Check for no overflow
		if (ticks64 >= 0) {
			portTickType ticks = (portTickType)ticks64;

			// Check for no overflow
			if (ticks == ticks64) {
				return (int64_t)ticks;
			}
		}
	}

	// An overflow occurs: saturate the value to the max value for a portTickType
	return (int64_t)portMAX_DELAY;
}
