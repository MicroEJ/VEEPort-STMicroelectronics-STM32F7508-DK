/*
 * C
 *
 * Copyright 2019-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief Use STM32 DMA2D (ChromART) for MicroEJ ui_drawing.h implementation.
 * @author MicroEJ Developer Team
 * @version 1.0.5
 * @date 23 September 2020
 */

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

#include "drawing_dma2d.h"
#include "ui_drawing.h"
#include "ui_drawing_soft.h"

/* Defines and Macros --------------------------------------------------------*/

/*
 * @brief Defines the DMA2D format according DRAWING_DMA2D_BPP.
 */
#if DRAWING_DMA2D_BPP == 16
#define DRAWING_DMA2D_FORMAT DMA2D_RGB565
#elif DRAWING_DMA2D_BPP == 24
#define DRAWING_DMA2D_FORMAT DMA2D_RGB888
#elif DRAWING_DMA2D_BPP == 32
#define DRAWING_DMA2D_FORMAT DMA2D_ARGB8888
#else
#error "Define 'DRAWING_DMA2D_BPP' is required (16, 24 or 32)"
#endif

/* Structs and Typedefs ------------------------------------------------------*/

/*
 * @brief Function used to notify the graphical engine about the end of DMA2D work.
 * Available values are LLUI_DISPLAY_notifyAsynchronousDrawingEnd() and LLUI_DISPLAY_flushDone().
 */
typedef void (*t_drawing_notification)(bool under_isr);

/* Private fields ------------------------------------------------------------*/

/*
 * @brief STM32 HAL DMA2D declaration.
 */
static DMA2D_HandleTypeDef g_hdma2d;

/*
 * @brief Next notification to call after DMA2D work.
 */
static t_drawing_notification g_callback_notification;

/*
 * @brief This boolean is set to true before launching a DMA2D action and back to
 * false during DMA2D interrupt. It allows to know if the DMA2D is running or not.
 */
static bool g_dma2d_running;

/*
 * @brief Binary semaphore used to lock a DMA2D user when DMA2D is already running.
 */
static void* g_dma2d_semaphore;

/* Private functions ---------------------------------------------------------*/

/*
 * @brief Ensures DMA2D previous work is done before returning.
 */
static inline void _drawing_dma2d_wait(void)
{
	while(g_dma2d_running)
	{
		LLUI_DISPLAY_IMPL_binarySemaphoreTake(g_dma2d_semaphore);
	}
}

/*
 * @brief Notify the DMA2D has finished previous job.
 */
static inline void _drawing_dma2d_done(void)
{
	g_dma2d_running = false;
	LLUI_DISPLAY_IMPL_binarySemaphoreGive(g_dma2d_semaphore, true);
}

/*
 * @brief Adjusts the given address to target the given point.
 */
static inline uint8_t* _drawing_dma2d_adjust_address(uint8_t* address, uint32_t x, uint32_t y, uint32_t stride, uint32_t bpp)
{
	return address + ((y * stride + x) * bpp / 8);
}

/*
 * @brief For A4 and A8 formats, alpha contains both the global alpha + wanted color.
 */
static inline void _drawing_dma2d_configure_alpha_image_data(MICROUI_GraphicsContext* gc, jint* alphaAndColor)
{
	// for A4 and A8 formats, alphaAndColor is both the global alpha + wanted color
	*(alphaAndColor) <<= 24;
	*(alphaAndColor) |= (gc->foreground_color & 0xffffff);
}

/* Interrupt functions -------------------------------------------------------*/

void DRAWING_DMA2D_IRQHandler(void)
{
	// notify STM32 HAL
	HAL_DMA2D_IRQHandler(&g_hdma2d);

	// notify DMA2D users
	_drawing_dma2d_done();

	// notify graphical engine
	g_callback_notification(true);
}

/* Public functions ----------------------------------------------------------*/

void DRAWING_DMA2D_initialize(void* binary_semaphore_handle)
{
	// configure globals
	g_dma2d_running = false;
	g_dma2d_semaphore = binary_semaphore_handle;

	// configure DMA2D IRQ handler
	HAL_NVIC_SetPriority(DMA2D_IRQn, 5, 3);
	HAL_NVIC_EnableIRQ(DMA2D_IRQn);

	// configure DMA2D
	g_hdma2d.Init.ColorMode = DRAWING_DMA2D_FORMAT;
	g_hdma2d.Instance = DMA2D;
}

void DRAWING_DMA2D_configure_memcpy(uint8_t* srcAddr, uint8_t* destAddr, uint32_t xmin, uint32_t ymin, uint32_t xmax, uint32_t ymax, uint32_t stride, DRAWING_DMA2D_memcpy* memcpy_data)
{
	_drawing_dma2d_wait();

	uint32_t width = (xmax - xmin + 1);
	uint32_t height = (ymax - ymin + 1);

	// de-init DMA2D
	HAL_DMA2D_DeInit(&g_hdma2d);

	// configure foreground
	g_hdma2d.LayerCfg[1].InputOffset = stride - width;
	g_hdma2d.LayerCfg[1].InputColorMode = DRAWING_DMA2D_FORMAT;
	HAL_DMA2D_ConfigLayer(&g_hdma2d, 1);

	// configure DMA2D
	g_hdma2d.Init.Mode  = DMA2D_M2M;
	g_hdma2d.Init.OutputOffset = stride - width;
	HAL_DMA2D_Init(&g_hdma2d) ;

	// configure given structure (to give later to DRAWING_DMA2D_start_memcpy())
	memcpy_data->src_address = _drawing_dma2d_adjust_address(srcAddr, xmin, ymin, stride, DRAWING_DMA2D_BPP);
	memcpy_data->dest_address = _drawing_dma2d_adjust_address(destAddr, xmin, ymin, stride, DRAWING_DMA2D_BPP);
	memcpy_data->width = width;
	memcpy_data->height = height;

	// configure environment
	g_callback_notification = &LLUI_DISPLAY_flushDone;
	g_dma2d_running = true;
}

void DRAWING_DMA2D_start_memcpy(DRAWING_DMA2D_memcpy* memcpy_data)
{
	HAL_DMA2D_Start_IT(
			&g_hdma2d,
			(uint32_t)memcpy_data->src_address,
			(uint32_t)memcpy_data->dest_address,
			memcpy_data->width,
			memcpy_data->height
	);
}

/* ui_drawing.h functions --------------------------------------------*/

DRAWING_Status UI_DRAWING_fillRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2)
{
	if (LLUI_DISPLAY_isClipEnabled(gc) && !LLUI_DISPLAY_clipRectangle(gc, &x1, &y1, &x2, &y2))
	{
		// drawing is out of clip: nothing to do
		return DRAWING_DONE;
	}
	// else: clip disabled (means drawing is fully in clip) or drawing fits the clip

	_drawing_dma2d_wait();

	LLUI_DISPLAY_setDrawingLimits(gc, x1, y1, x2, y2);

	uint32_t rectangle_width = x2 - x1 + 1;
	uint32_t rectangle_height = y2 - y1 + 1;
	uint32_t stride = LLUI_DISPLAY_getStrideInPixels(&gc->image);

	// de-init DMA2D
	HAL_DMA2D_DeInit(&g_hdma2d);

	// configure DMA2D
	g_hdma2d.Init.Mode = DMA2D_R2M;
	g_hdma2d.Init.OutputOffset = stride - rectangle_width;
	HAL_DMA2D_Init(&g_hdma2d);

	// configure environment
	g_callback_notification = &LLUI_DISPLAY_notifyAsynchronousDrawingEnd;
	g_dma2d_running = true;

	// start DMA2D
	uint8_t* destination_address = _drawing_dma2d_adjust_address(LLUI_DISPLAY_getBufferAddress(&gc->image), x1, y1, stride, DRAWING_DMA2D_BPP);
	HAL_DMA2D_Start_IT(&g_hdma2d, gc->foreground_color, (uint32_t)destination_address, rectangle_width, rectangle_height);

	return DRAWING_RUNNING;
}

DRAWING_Status UI_DRAWING_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* image, jint x_src, jint y_src, jint width, jint height, jint x_dest, jint y_dest, jint alpha)
{
	if (LLUI_DISPLAY_isClipEnabled(gc) && !!LLUI_DISPLAY_clipRegion(gc, &x_src, &y_src, &width, &height, &x_dest, &y_dest))
	{
		// clip disabled( means drawing is fully in clip) or drawing is out of clip
		// nothing to do
		return DRAWING_DONE;
	}

	if ((uint32_t)&(gc->image) == (uint32_t)image) {
		// image in image or display in display
		
		if ((y_dest > y_src && y_dest < (y_src + height)) || (y_dest == y_src && x_dest > x_src && x_dest < (x_src + width))) {
			// overlap bottom and/or right: DMA2D is not able to perform the drawing
			UI_DRAWING_SOFT_drawImage(gc, image, x_src, y_src, width, height, x_dest, y_dest, alpha);
			return DRAWING_DONE;
		}
	}

	_drawing_dma2d_wait();

	uint8_t format;
	uint8_t bpp;

	switch(image->format)
	{
	case MICROUI_IMAGE_FORMAT_RGB565:
		format = CM_RGB565;
		bpp = 16;
		break;
	case MICROUI_IMAGE_FORMAT_ARGB8888:
		format = CM_ARGB8888;
		bpp = 32;
		break;
	case MICROUI_IMAGE_FORMAT_RGB888:
		format = CM_RGB888;
		bpp = 24;
		break;
	case MICROUI_IMAGE_FORMAT_ARGB1555:
		format = CM_ARGB1555;
		bpp = 16;
		break;
	case MICROUI_IMAGE_FORMAT_ARGB4444:
		format = CM_ARGB4444;
		bpp = 16;
		break;
	case MICROUI_IMAGE_FORMAT_A4:
	{
		// check DMA2D limitations: first and last vertical lines addresses must be aligned on 8 bits

		jint xAlign = x_src & 1;
		jint wAlign = width & 1;

		if (xAlign == 0) {
			if (wAlign != 0) {
				// hard cannot draw last vertical line
				--width;
				UI_DRAWING_SOFT_drawImage(gc, image, x_src + width, y_src, 1, height, x_dest + width, y_dest, alpha);
			}
			// else: easy case: first and last vertical lines are aligned on 8 bits
		}
		else {
			if (wAlign == 0) {
				// worst case: hard cannot draw first and last vertical lines

				// first line
				UI_DRAWING_SOFT_drawImage(gc, image, x_src, y_src, 1, height, x_dest, y_dest, alpha);

				// last line
				--width;
				UI_DRAWING_SOFT_drawImage(gc, image, x_src + width, y_src, 1, height, x_dest + width, y_dest, alpha);

				++x_src;
				++x_dest;
				--width;
			}
			else {
				// cannot draw first vertical line
				UI_DRAWING_SOFT_drawImage(gc, image, x_src, y_src, 1, height, x_dest, y_dest, alpha);
				++x_src;
				++x_dest;
				--width;
			}
		}

		_drawing_dma2d_configure_alpha_image_data(gc, &alpha);
		format = CM_A4;
		bpp = 4;
		break;
	}
	case MICROUI_IMAGE_FORMAT_A8:
		_drawing_dma2d_configure_alpha_image_data(gc, &alpha);
		format = CM_A8;
		bpp = 8;
		break;
	default:
		// unsupported image format
		UI_DRAWING_SOFT_drawImage(gc, image, x_src, y_src, width, height, x_dest, y_dest, alpha);
		return DRAWING_DONE;
	}

	uint32_t srcStride = LLUI_DISPLAY_getStrideInPixels(image);
	uint32_t destStride = LLUI_DISPLAY_getStrideInPixels(&gc->image);
	uint8_t* srcAddr = _drawing_dma2d_adjust_address(LLUI_DISPLAY_getBufferAddress(image), x_src, y_src, srcStride, bpp);
	uint8_t* destAddr = _drawing_dma2d_adjust_address(LLUI_DISPLAY_getBufferAddress(&gc->image), x_dest, y_dest, destStride, DRAWING_DMA2D_BPP);

	// de-init DMA2D
	HAL_DMA2D_DeInit(&g_hdma2d);

	// configure background
	g_hdma2d.LayerCfg[0].InputOffset = destStride - width;
	g_hdma2d.LayerCfg[0].InputColorMode = DRAWING_DMA2D_FORMAT;
	g_hdma2d.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	g_hdma2d.LayerCfg[0].InputAlpha = 255;
	HAL_DMA2D_ConfigLayer(&g_hdma2d, 0);

	// configure foreground
	HAL_DMA2D_DisableCLUT(&g_hdma2d, 1);
	g_hdma2d.LayerCfg[1].InputOffset = srcStride - width;
	g_hdma2d.LayerCfg[1].InputColorMode = format;
	g_hdma2d.LayerCfg[1].AlphaMode = DMA2D_COMBINE_ALPHA;
	g_hdma2d.LayerCfg[1].InputAlpha = alpha;
	HAL_DMA2D_ConfigLayer(&g_hdma2d, 1);

	// configure DMA2D
	g_hdma2d.Init.Mode = DMA2D_M2M_BLEND;
	g_hdma2d.Init.OutputOffset = destStride - width;
	HAL_DMA2D_Init(&g_hdma2d) ;

	// configure environment
	g_dma2d_running = true;
	g_callback_notification = &LLUI_DISPLAY_notifyAsynchronousDrawingEnd;
	LLUI_DISPLAY_setDrawingLimits(gc, x_dest, y_dest, x_dest + width - 1, y_dest + height - 1);

	// start DMA2D
	HAL_DMA2D_BlendingStart_IT(&g_hdma2d, (uint32_t)srcAddr, (uint32_t)destAddr, (uint32_t)destAddr, width, height);

	return DRAWING_RUNNING;
}

/* EOF -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif


