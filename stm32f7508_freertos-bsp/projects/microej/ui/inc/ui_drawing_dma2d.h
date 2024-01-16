/*
 * C
 *
 * Copyright 2019-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#if !defined UI_DRAWING_DMA2D_H
#define UI_DRAWING_DMA2D_H
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @file
 * @brief Implementation of a set of ui_drawing.h drawing functions (MicroUI library). These are
 * implementations over the STM32 DMA2D (ChromART) and the destination buffer format is the format
 * specified in the VEE port. When the drawing cannot be performed by the GPU, the software
 * implementation is used insted.
 *
 * This library provides the implementation of ui_drawing.h "UI_DRAWING_fillRectangle()" and
 * "UI_DRAWING_drawImage()" functions. The third feature "memcpy" is useful when a copy from frame
 * buffer to back buffer is required after the call to "LLUI_DISPLAY_IMPL_flush()".
 *
 * How to use this library:
 * - Set the define DRAWING_DMA2D_BPP to 16, 24 or 32 (project global define)
 * - Set the define STM32F4XX, STM32F7XX or STM32H7XX (project global define)
 * - Configures the cache management: see ui_drawing_dma2d_configuration.h
 * - Call "UI_DRAWING_DMA2D_initialize()" during "LLUI_DISPLAY_IMPL_initialize()"
 * - Redirect the STM32 DMA2D interrupt routine to "UI_DRAWING_DMA2D_IRQHandler()"
 * - Call "UI_DRAWING_DMA2D_configure_memcpy()" in "LLUI_DISPLAY_IMPL_flush()" before enabling LCD interrupt (optional).
 * - Call "UI_DRAWING_DMA2D_start_memcpy()" in LCD interrupt (optional).
 *
 * @author MicroEJ Developer Team
 * @version 4.1.0
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include <LLUI_DISPLAY_impl.h>

#include "ui_drawing.h"

#ifdef STM32F4XX
#include "stm32f4xx_hal.h"
#endif

#ifdef STM32F7XX
#include "stm32f7xx_hal.h"
#endif

#ifdef STM32H7XX
#include "stm32h7xx_hal.h"
#endif

// --------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------

#if !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The functions UI_DRAWING_DMA2D_xxx() are directly called by LLUI_PAINTER_impl.c
 * and LLDW_PAINTER_impl.c. This file overrides each function independently to use the
 * DMA2D.
 */
  
#define UI_DRAWING_DMA2D_fillRectangle UI_DRAWING_fillRectangle
#define UI_DRAWING_DMA2D_drawImage UI_DRAWING_drawImage
#define UI_DRAWING_DMA2D_copyImage UI_DRAWING_copyImage
#define UI_DRAWING_DMA2D_drawRegion UI_DRAWING_drawRegion

#else // !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The functions UI_DRAWING_DMA2D_xxx() are indirectly called through some tables.
 * The functions have got the identifier "0" as suffix. This file overrides each function
 * independently to use the DMA2D.
 */

#define UI_DRAWING_DMA2D_fillRectangle UI_DRAWING_fillRectangle_0
#define UI_DRAWING_DMA2D_drawImage UI_DRAWING_drawImage_0
#define UI_DRAWING_DMA2D_copyImage UI_DRAWING_copyImage_0
#define UI_DRAWING_DMA2D_drawRegion UI_DRAWING_drawRegion_0

#endif // !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

// -----------------------------------------------------------------------------
// Types
// -----------------------------------------------------------------------------

/*
 * @brief Feature "memcpy" data. The function "UI_DRAWING_DMA2D_configure_memcpy()" fills
 * it with the given arguments. In a second time, this structure must be given to
 * "UI_DRAWING_DMA2D_start_memcpy()".
 */
typedef struct {
	uint8_t* src_address;
	uint8_t* dest_address;
	uint16_t width;
	uint16_t height;
} DRAWING_DMA2D_memcpy;

// --------------------------------------------------------------------------------
// Public API
// --------------------------------------------------------------------------------

/*
 * @brief Initializes this library.
 *
 * @param[in] binary_semaphore_handle a binary semaphore. It must initialized in such
 * a way that the semaphore must first be 'given' before it can be 'taken'.
 */
void UI_DRAWING_DMA2D_initialize(void* binary_semaphore_handle);

/*
 * @brief STM32 HAL DMA2D implementation. DMA2D IRQ handler must call this function.
 * This function calls the STM32 HAL DMA2D "HAL_DMA2D_IRQHandler()" function. Then it
 * calls the LLUI_DISPLAY "LLUI_DISPLAY_notifyAsynchronousDrawingEnd" or "LLUI_DISPLAY_flushDone"
 * callbacks to notify the graphical engine about the current status.
 */
void UI_DRAWING_DMA2D_IRQHandler(void);

/*
 * @brief Configures the copy from frame buffer to back buffer just after a flush.
 *
 * @param[in] srcAddr the address of the buffer to copy.
 * @param[in] destAddr the address of the destination buffer.
 * @param[in] xmin the top-left X coordinate of the rectangle to copy.
 * @param[in] ymin the top-left Y coordinate of the rectangle to copy.
 * @param[in] xmax the bottom-right X coordinate of the rectangle to copy.
 * @param[in] ymax the bottom-right Y coordinate of the rectangle to copy.
 * @param[in] stride the buffer row stride in pixels (usually equal to the buffer width)
 * @param[in] memcpy_data the internal representation of the memcpy to perform.
 */
void UI_DRAWING_DMA2D_configure_memcpy(uint8_t* srcAddr, uint8_t* destAddr, uint32_t xmin, uint32_t ymin, uint32_t xmax, uint32_t ymax, uint32_t stride, DRAWING_DMA2D_memcpy* memcpy_data);

/*
 * @brief Starts the copy previously configured by a call to "DRAWING_DMA2D_configure_memcpy()".
 *
 * @param[in] memcpy_data the internal representation of the memcpy to perform.
 */
void UI_DRAWING_DMA2D_start_memcpy(DRAWING_DMA2D_memcpy* memcyp_data);

// --------------------------------------------------------------------------------
// ui_drawing.h API
// (the function names differ according to the available number of destination formats)
// --------------------------------------------------------------------------------

/*
 * @brief Implementation of fillRectangle over the DMA2D. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_DMA2D_fillRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);

/*
 * @brief Implementation of drawImage over the DMA2D. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_DMA2D_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

/*
 * @brief Implementation of copyImage over the DMA2D. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_DMA2D_copyImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);

/*
 * @brief Implementation of drawRegion over the DMA2D. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_DMA2D_drawRegion(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif // UI_DRAWING_DMA2D_H
