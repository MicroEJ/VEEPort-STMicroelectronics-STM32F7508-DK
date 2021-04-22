/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "stm32f7508_discovery_ts.h"
#include "stm32f7508_discovery_lcd.h"
#include "microej.h"
#include "io_task.h"
#include "touch_helper.h"

/* Global --------------------------------------------------------------------*/

static uint8_t touch_initialized = MICROEJ_FALSE;

/* Private API ---------------------------------------------------------------*/

static uint8_t TOUCH_MANAGER_GetState(uint16_t* x, uint16_t* y)
{
	/* Check and update the number of touches active detected */
	if(ft5336_ts_drv.DetectTouch(TS_I2C_ADDRESS) == 0)
	{
		return MICROEJ_FALSE;
	}

	/* Get each touch coordinates */
	uint16_t tx,ty;
	ft5336_ts_drv.GetXY(TS_I2C_ADDRESS, &tx, &ty);

	*(x) = ty;
	*(y) = tx;

	return MICROEJ_TRUE;
}

static uint8_t TOUCH_MANAGER_update(void)
{
	uint16_t x,y;

	if (TOUCH_MANAGER_GetState(&x, &y) == MICROEJ_TRUE)
	{
		// here, pen is down for sure
		TOUCH_HELPER_pressed(x, y);
		return MICROEJ_TRUE;
	}
	else
	{
		// here, pen is up for sure
		TOUCH_HELPER_released();
		return MICROEJ_FALSE;
	}
}

/* API -----------------------------------------------------------------------*/

void TOUCH_MANAGER_initialize(void)
{
	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
	BSP_TS_ITConfig();
	IO_TASK_create_task();
	touch_initialized = MICROEJ_TRUE;
}

uint8_t TOUCH_MANAGER_interrupt(void)
{
	if(__HAL_GPIO_EXTI_GET_IT(TS_INT_PIN) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(TS_INT_PIN);

		// disable the EXTI interrupt until next touch release
		HAL_NVIC_DisableIRQ(TS_INT_EXTI_IRQn);

		return MICROEJ_TRUE;
	}

	return MICROEJ_FALSE;
}

uint8_t TOUCH_MANAGER_work(void)
{
	if (touch_initialized == MICROEJ_FALSE)
	{
		return MICROEJ_FALSE;
	}

	// manage touch event
	uint8_t touch_pressed = TOUCH_MANAGER_update();

	// clear TS_INT_PIN IRQ in MFX
	BSP_TS_ITClear();

	return touch_pressed;
}
