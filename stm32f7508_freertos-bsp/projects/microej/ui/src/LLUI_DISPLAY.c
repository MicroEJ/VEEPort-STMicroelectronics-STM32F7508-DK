/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdbool.h>
#include "LLDISPLAY_configuration.h"

#include "stm32f7508_discovery_lcd.h"
#include "LLUI_DISPLAY_impl.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "microej.h"
#include "bsp_util.h"
#include "framerate.h"
#include "interrupts.h"
#include "drawing_dma2d.h"

/* Defines -------------------------------------------------------------------*/

// Define size to allocate for Display Buffer
#define DISPLAY_MEM_SIZE 0x80000 /* Back and Frame buffers: (480 * 272 * (16 / 8)) * 2. Rounded up to fill completely an MPU region */

// Declare Display Buffer
uint8_t display_mem[DISPLAY_MEM_SIZE] __ALIGNED(DISPLAY_MEM_SIZE) __attribute__((section(".DisplayMem")));

#define BUFFER_SIZE (RK043FN48H_WIDTH * RK043FN48H_HEIGHT * (DRAWING_DMA2D_BPP / 8))

#define BACK_BUFFER ((int32_t)&display_mem[0])
#define FRAME_BUFFER (BACK_BUFFER + BUFFER_SIZE)

/* Global --------------------------------------------------------------------*/

static DRAWING_DMA2D_memcpy dma2d_memcpy;
static SemaphoreHandle_t dma2d_sem;

extern LTDC_HandleTypeDef hLtdcHandler;

/* Private API ---------------------------------------------------------------*/

static void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();

	/* Set common MPU attributes */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;

	/* Configure the MPU region for SDRAM display buffers (strongly-ordered) */
	/* See AN4861 LCD-TFT display controller (LTDC) on STM32 MCUs, Chapter 4.6.2 */
	MPU_InitStruct.BaseAddress = BACK_BUFFER;
	MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER5;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void lcd_enable_interrupt(void)
{
	// ask to reload buffer address
	// (it will throw an interrupt)
	HAL_LTDC_Reload(&hLtdcHandler, LTDC_RELOAD_VERTICAL_BLANKING);
}

/* Interrupt functions -------------------------------------------------------*/

void DMA2D_IRQHandler(void)
{
	DRAWING_DMA2D_IRQHandler();
}

void LTDC_IRQHandler(void)
{
	// notify STM32 HAL
	HAL_LTDC_IRQHandler(&hLtdcHandler);
}

void LTDC_ER_IRQHandler(void)
{
	// LTDC error interrupt
}

/* Callback functions --------------------------------------------------------*/

void HAL_LTDC_ErrorCallback(LTDC_HandleTypeDef *hltdc)
{
	if (hltdc->ErrorCode & HAL_LTDC_ERROR_TE)
	{
		// LTDC transfer error
		__HAL_LTDC_ENABLE_IT(hltdc, LTDC_IT_TE);
	}
	else if (hltdc->ErrorCode & HAL_LTDC_ERROR_FU)
	{
		// LTDC FIFO underrun error
		__HAL_LTDC_ENABLE_IT(hltdc, LTDC_IT_FU);
	}
	else
	{
		// LTDC unknown error
	}
}

void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc)
{
	// LTDC register reload
	__HAL_LTDC_ENABLE_IT(hltdc, LTDC_IT_RR);

	// launch the copy from backbuffer to lcd buffer
	DRAWING_DMA2D_start_memcpy(&dma2d_memcpy);
}

/* API -----------------------------------------------------------------------*/

void LLUI_DISPLAY_IMPL_initialize(LLUI_DISPLAY_SInitData* init_data)
{
	MPU_Config();

	BSP_LCD_Init();

	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, FRAME_BUFFER);

#if LLDISPLAY_BPP == 16
	HAL_LTDC_SetPixelFormat(&hLtdcHandler, LTDC_PIXEL_FORMAT_RGB565, LTDC_ACTIVE_LAYER);
#elif LLDISPLAY_BPP == 24
	HAL_LTDC_SetPixelFormat(&hLtdcHandler, LTDC_PIXEL_FORMAT_RGB888, LTDC_ACTIVE_LAYER);
#elif LLDISPLAY_BPP == 32
	HAL_LTDC_SetPixelFormat(&hLtdcHandler, LTDC_PIXEL_FORMAT_ARGB8888, LTDC_ACTIVE_LAYER);
#else
	#error "Define 'LLDISPLAY_BPP' is required (16, 24 or 32)"
#endif

	init_data->lcd_width = RK043FN48H_WIDTH;
	init_data->lcd_height = RK043FN48H_HEIGHT;
	init_data->back_buffer_address = (uint8_t*)BACK_BUFFER;
	init_data->binary_semaphore_0 = (LLUI_DISPLAY_binary_semaphore*)xSemaphoreCreateBinary();
	init_data->binary_semaphore_1 = (LLUI_DISPLAY_binary_semaphore*)xSemaphoreCreateBinary();
	dma2d_sem = xSemaphoreCreateBinary();
	
	// interruptions
	HAL_NVIC_SetPriority(LTDC_IRQn, 5, 3);
	HAL_NVIC_EnableIRQ(LTDC_IRQn);

	DRAWING_DMA2D_initialize((void*)dma2d_sem);
}

void LLUI_DISPLAY_IMPL_binarySemaphoreTake(void* sem)
{
	xSemaphoreTake((xSemaphoreHandle)sem, portMAX_DELAY);
}

void LLUI_DISPLAY_IMPL_binarySemaphoreGive(void* sem, bool under_isr)
{
	if (under_isr)
	{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR((xSemaphoreHandle)sem, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken != pdFALSE)
		{
			// Force a context switch here.
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}
	else
	{
		xSemaphoreGive((xSemaphoreHandle)sem);
	}
}

uint8_t* LLUI_DISPLAY_IMPL_flush(MICROUI_GraphicsContext* gc, uint8_t* srcAddr, uint32_t xmin, uint32_t ymin, uint32_t xmax, uint32_t ymax)
{
	uint8_t* destAddr = (srcAddr == (uint8_t*)BACK_BUFFER) ? (uint8_t*)FRAME_BUFFER : (uint8_t*)BACK_BUFFER;

#ifdef FRAMERATE_ENABLED
	framerate_increment();
#endif

	HAL_LTDC_SetAddress(&hLtdcHandler, (uint32_t)srcAddr, LTDC_ACTIVE_LAYER);
	DRAWING_DMA2D_configure_memcpy(srcAddr, destAddr, xmin, ymin, xmax, ymax, RK043FN48H_WIDTH, &dma2d_memcpy);
	lcd_enable_interrupt();

	return destAddr;
}
