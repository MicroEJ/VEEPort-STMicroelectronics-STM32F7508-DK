/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/
#include "LLUI_LED_impl.h"
#include "stm32f7508_discovery.h"

/* Defines -------------------------------------------------------------------*/
#define MAX_NUMBER_OF_LEDS LEDn
#define LED_IS_ON 1
#define LED_IS_OFF 0

/* Global --------------------------------------------------------------------*/
static uint8_t led_state[MAX_NUMBER_OF_LEDS];

/* Private API ---------------------------------------------------------------*/
static void LED_On(Led_TypeDef ledID)
{
	if (ledID <= MAX_NUMBER_OF_LEDS)
	{
		BSP_LED_On(ledID);
		led_state[ledID] = LED_IS_ON;
	}
}

static void LED_Off(Led_TypeDef ledID)
{
	if (ledID <= MAX_NUMBER_OF_LEDS)
	{
		BSP_LED_Off(ledID);
		led_state[ledID] = LED_IS_OFF;
	}
}

/* Public functions ----------------------------------------------------------*/

/**
 * Initializes the LEDs.
 * @return the number of LEDs in the group.
 */
int32_t LLUI_LED_IMPL_initialize(void)
{
	// Initialize and set all LEDs off
	for (Led_TypeDef ledID = LED1; ledID < MAX_NUMBER_OF_LEDS; ledID++)
	{
		BSP_LED_Init(ledID);
		LED_Off(ledID);
	}

    return MAX_NUMBER_OF_LEDS;
}

/**
 * Get the intensity of the LED.
 * @return the intensity of the LED
 */
int32_t LLUI_LED_IMPL_getIntensity(int32_t ledID)
{
	if ((ledID >= LED1) && (ledID <= MAX_NUMBER_OF_LEDS))
	{
		if (led_state[ledID] == LED_IS_ON)
		{
			return LLUI_LED_MAX_INTENSITY;
		}
		else
		{
			return LLUI_LED_MIN_INTENSITY;
		}
	}
	else
	{
		return LLUI_LED_MIN_INTENSITY;
	}
}

/**
 * Set the intensity of the LED.
 * A 0 intensity turns off the LED, intensity of 255 turns on the LED to max power.
 * @param intensity the intensity of the LED
 */
void LLUI_LED_IMPL_setIntensity(int32_t ledID, int32_t intensity)
{
	if ((ledID >= 0) && (ledID <= MAX_NUMBER_OF_LEDS))
	{
		/* Saturate minimum intensity */
		if (intensity < LLUI_LED_MIN_INTENSITY)
		{
			intensity = LLUI_LED_MIN_INTENSITY;
		}

		/* Saturate maximum intensity */
		if (intensity > LLUI_LED_MAX_INTENSITY)
		{
			intensity = LLUI_LED_MAX_INTENSITY;
		}

		if (intensity == LLUI_LED_MIN_INTENSITY)
		{
			LED_Off((Led_TypeDef)ledID);
		}
		else
		{
			LED_On((Led_TypeDef)ledID);
		}
	}
}
