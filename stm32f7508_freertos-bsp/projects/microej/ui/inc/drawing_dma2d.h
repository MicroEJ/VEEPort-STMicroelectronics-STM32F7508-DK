/*
 * C
 *
 * Copyright 2019-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef DRAWING_DMA2D_H
#define DRAWING_DMA2D_H

/**
 * @file
 * @brief Use STM32 DMA2D (ChromART) for MicroEJ ui_drawing.h implementation.
 *
 * 		This library provides the implementation of ui_drawing.h "UI_DRAWING_fillRectangle()"
 * 		and "UI_DRAWING_drawImage()" functions. The third feature "memcpy" is useful when a copy
 * 		from frame buffer to back buffer is required after the call to "LLUI_DISPLAY_IMPL_flush()".
 *
 * 		How to use this library:
 * 		 - Set the define DRAWING_DMA2D_BPP to 16, 24 or 32
 * 		 - Set the define STM32F4XX or STM32F7XX
 * 		 - Call "DRAWING_DMA2D_initialize()" during "LLUI_DISPLAY_IMPL_initialize()"
 * 		 - Redirect the STM32 DMA2D interrupt routine to "DRAWING_DMA2D_IRQHandler()"
 * 		 - Call "DRAWING_DMA2D_configure_memcpy()" in "LLUI_DISPLAY_IMPL_flush()" before enabling
 * 		 LCD interrupt (optional).
 * 		 - Call "DRAWING_DMA2D_start_memcpy()" in LCD interrupt (optional).
 *
 * @author MicroEJ Developer Team
 * @version 3.1.0
 * @date 13 April 2023
 */

#ifdef __cplusplus
	extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "LLUI_DISPLAY_impl.h"
#include "drawing_dma2d_configuration.h"


#ifdef STM32F4XX
#include "stm32f4xx_hal.h"
#endif

#ifdef STM32F7XX
#include "stm32f7xx_hal.h"
#endif

#ifdef STM32H7XX
#include "stm32h7xx_hal.h"
#endif

#include "sni.h"

/* Structs -------------------------------------------------------------------*/

/*
 * @brief Feature "memcpy" data. Function "DRAWING_DMA2D_configure_memcpy()" fills
 * it with given arguments. In a second time, this structure must be given to
 * "DRAWING_DMA2D_start_memcpy()".
 */
typedef struct
{
	uint8_t* src_address;
	uint8_t* dest_address;
	uint16_t width;
	uint16_t height;
} DRAWING_DMA2D_memcpy;

/* API -----------------------------------------------------------------------*/

/*
 * @brief Initializes this library.
 *
 * @param[in] binary_semaphore_handle a binary semaphore. It must initialized in such
 * case the semaphore must first be 'given' before it can be 'taken'.
 */
void DRAWING_DMA2D_initialize(void* binary_semaphore_handle);

/*
 * @brief STM32 HAL DMA2D implementation. DMA2D IRQ handler must call this function.
 * This function calls STM32 HAL DMA2D "HAL_DMA2D_IRQHandler()" function. Then it
 * calls the LLUI_DISPLAY "LLUI_DISPLAY_notifyAsynchronousDrawingEnd" or "LLUI_DISPLAY_flushDone"
 * callbacks to notify the graphical engine about current status.
 */
void DRAWING_DMA2D_IRQHandler(void);

/*
 * @brief Configures the copy from frame buffer to back buffer just after a flush.
 *
 * @param[in] srcAddr the address of the buffer to copy.
 * @param[in] destAddr the address of the destination buffer.
 * @param[in] xmin the top-left X coordinate of rectangle to copy.
 * @param[in] ymin the top-left Y coordinate of rectangle to copy.
 * @param[in] xmax the bottom-right X coordinate of rectangle to copy.
 * @param[in] ymax the bottom-right Y coordinate of rectangle to copy.
 * @param[in] stride the buffer row stride in pixels (usually equals to buffer width)
 * @param[in] memcpy_data the internal representation of memcpy to perform.
 */
void DRAWING_DMA2D_configure_memcpy(uint8_t* srcAddr, uint8_t* destAddr, uint32_t xmin, uint32_t ymin, uint32_t xmax, uint32_t ymax, uint32_t stride, DRAWING_DMA2D_memcpy* memcpy_data);

/*
 * @brief Starts the copy previously configured by a call to "DRAWING_DMA2D_configure_memcpy()".
 *
 * @param[in] memcpy_data the internal representation of memcpy to perform.
 */
void DRAWING_DMA2D_start_memcpy(DRAWING_DMA2D_memcpy* memcyp_data);

/* EOF -----------------------------------------------------------------------*/

#ifdef __cplusplus
	}
#endif

#endif // DRAWING_DMA2D_H

