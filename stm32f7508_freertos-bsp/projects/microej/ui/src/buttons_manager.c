/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "stm32f7508_discovery.h"
#include "stm32f7xx_hal_cortex.h"
#include "microej.h"
#include "interrupts.h"
#include "LLMJVM.h"
#include "buttons_helper.h"

/* Global --------------------------------------------------------------------*/

static GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {WAKEUP_BUTTON_GPIO_PORT};
static const uint16_t BUTTON_PIN[BUTTONn] = {WAKEUP_BUTTON_PIN};
static const IRQn_Type BUTTON_IRQn[BUTTONn] = {WAKEUP_BUTTON_EXTI_IRQn};
static const uint8_t BUTTON_ID[BUTTONn] = {0};
static const uint8_t BUTTON_REVERSE[BUTTONn] = {MICROEJ_FALSE};

/* Private API ---------------------------------------------------------------*/

static void BUTTONS_MANAGER_event(Button_TypeDef Button)
{
	uint8_t button_pressed;

	if (HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]) == GPIO_PIN_SET)
	{
		// GPIO == 1

		if (BUTTON_REVERSE[Button] == MICROEJ_TRUE)
		{
			// GPIO == 1 means "released"
			button_pressed = MICROEJ_FALSE;
		}
		else
		{
			// GPIO == 1 means "pressed"
			button_pressed = MICROEJ_TRUE;
			LLMJVM_dump();
		}
	}
	else
	{
		// GPIO == 0

		if (BUTTON_REVERSE[Button] == MICROEJ_TRUE)
		{
			// GPIO == 0 means "pressed"
			button_pressed = MICROEJ_TRUE;
		}
		else
		{
			// GPIO == 0 means "released"
			button_pressed = MICROEJ_FALSE;
		}
	}

	if (button_pressed == MICROEJ_TRUE)
	{
		BUTTONS_HELPER_pressed(BUTTON_ID[Button]);
	}
	else
	{
		BUTTONS_HELPER_released(BUTTON_ID[Button]);
	}
}

static void BUTTONS_MANAGER_init(Button_TypeDef Button)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Enable the BUTTON Clock */
	BUTTONx_GPIO_CLK_ENABLE(Button);

	/* Configure Button pin as input with external interrupt */
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pin = BUTTON_PIN[Button];
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);

	/* Configure Button EXTI Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(BUTTON_IRQn[Button], 0xf, 0);
}

/* API -----------------------------------------------------------------------*/

void BUTTONS_MANAGER_interrupt(Button_TypeDef Button)
{
  if(__HAL_GPIO_EXTI_GET_IT(BUTTON_PIN[Button]) != RESET)
  {
    BUTTONS_MANAGER_event(Button);
    __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_PIN[Button]);
  }
}

void BUTTONS_MANAGER_enable_interrupts(void)
{
	HAL_NVIC_EnableIRQ(BUTTON_IRQn[BUTTON_WAKEUP]);
}

void BUTTONS_MANAGER_disable_interrupts(void)
{
	HAL_NVIC_DisableIRQ(BUTTON_IRQn[BUTTON_WAKEUP]);
}

void BUTTONS_MANAGER_initialize(void)
{
	BUTTONS_HELPER_initialize();
	BUTTONS_MANAGER_init(BUTTON_WAKEUP);
}
