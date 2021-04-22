
/*
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief This file implements all "Drawing" (MicroUI extended library) drawing native functions.
 * @see LLGRAPHICS_EXT_impl.h file comment
 * @author MicroEJ Developer Team
 * @version 1.0.2
 * @date 2 September 2020
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

// implements LLDW_PAINTER_impl functions
#include "LLDW_PAINTER_impl.h"

// calls dw_drawing functions
#include "dw_drawing.h"

// use graphical engine functions to synchronize drawings
#include "LLUI_DISPLAY.h"

// --------------------------------------------------------------------------------
// Macros and Defines
// --------------------------------------------------------------------------------

// macros to log a drawing
#define LOG_DRAW_START(fn) LLUI_DISPLAY_logDrawingStart(CONCAT_DEFINES(LOG_DRAW_, fn))
#define LOG_DRAW_END(fn) LLUI_DISPLAY_logDrawingEnd(CONCAT_DEFINES(LOG_DRAW_, fn))

#define DRAWING_PAINTER_NATIVE_NAME(fn) (CONCAT_DEFINES(DRAWING_PAINTER_NATIVE_PREFIX, fn))

/*
 * LOG_DRAW_EVENT logs identifiers
 */
#define LOG_DRAW_drawThickFadedPoint 100
#define LOG_DRAW_drawThickFadedLine 101
#define LOG_DRAW_drawThickFadedCircle 102
#define LOG_DRAW_drawThickFadedCircleArc 103
#define LOG_DRAW_drawThickFadedEllipse 104
#define LOG_DRAW_drawThickLine 105
#define LOG_DRAW_drawThickCircle 106
#define LOG_DRAW_drawThickEllipse 107
#define LOG_DRAW_drawThickCircleArc 108

#define LOG_DRAW_drawFlippedImage 200
#define LOG_DRAW_drawRotatedImageNearestNeighbor 201
#define LOG_DRAW_drawRotatedImageBilinear 202
#define LOG_DRAW_drawScaledImageNearestNeighbor 203
#define LOG_DRAW_drawScaledImageBilinear 204

// --------------------------------------------------------------------------------
// MicroUI Extended native functions
// --------------------------------------------------------------------------------

void DRAWING_PAINTER_NATIVE(drawThickFadedPoint, MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawThickFadedPoint)))
	{
		LOG_DRAW_START(drawThickFadedPoint);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawThickFadedPoint(gc, x, y, thickness, fade));
		LOG_DRAW_END(drawThickFadedPoint);
	}
}

void DRAWING_PAINTER_NATIVE(drawThickFadedLine, MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawThickFadedLine)))
	{
		LOG_DRAW_START(drawThickFadedLine);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawThickFadedLine(gc, startX, startY, endX, endY, thickness, fade, startCap, endCap));
		LOG_DRAW_END(drawThickFadedLine);
	}
}

void DRAWING_PAINTER_NATIVE(drawThickFadedCircle, MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade)
{
	if (diameter > 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawThickFadedCircle)))
	{
		LOG_DRAW_START(drawThickFadedCircle);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawThickFadedCircle(gc, x, y, diameter, thickness, fade));
		LOG_DRAW_END(drawThickFadedCircle);
	}
}

void DRAWING_PAINTER_NATIVE(drawThickFadedCircleArc, MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end)
{
	if (diameter > 0 && (int32_t)arcAngle != 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawThickFadedCircleArc)))
	{
		LOG_DRAW_START(drawThickFadedCircleArc);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawThickFadedCircleArc(gc, x, y, diameter, startAngle, arcAngle, thickness, fade, start, end));
		LOG_DRAW_END(drawThickFadedCircleArc);
	}
}

void DRAWING_PAINTER_NATIVE(drawThickFadedEllipse, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade)
{
	if (width > 0 && height > 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawThickFadedEllipse)))
	{
		LOG_DRAW_START(drawThickFadedEllipse);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawThickFadedEllipse(gc, x, y, width, height, thickness, fade));
		LOG_DRAW_END(drawThickFadedEllipse);
	}
}

void DRAWING_PAINTER_NATIVE(drawThickLine, MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawThickLine)))
	{
		LOG_DRAW_START(drawThickLine);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawThickLine(gc, startX, startY, endX, endY, thickness));
		LOG_DRAW_END(drawThickLine);
	}
}

void DRAWING_PAINTER_NATIVE(drawThickCircle, MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness)
{
	if (diameter > 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawThickCircle)))
	{
		LOG_DRAW_START(drawThickCircle);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawThickCircle(gc, x, y, diameter, thickness));
		LOG_DRAW_END(drawThickCircle);
	}
}

void DRAWING_PAINTER_NATIVE(drawThickEllipse, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness)
{
	if (width > 0 && height > 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawThickEllipse)))
	{
		LOG_DRAW_START(drawThickEllipse);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawThickEllipse(gc, x, y, width, height, thickness));
		LOG_DRAW_END(drawThickEllipse);
	}
}

void DRAWING_PAINTER_NATIVE(drawThickCircleArc, MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness)
{
	if (diameter > 0 && (int32_t)arcAngle != 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawThickCircleArc)))
	{
		LOG_DRAW_START(drawThickCircleArc);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawThickCircleArc(gc, x, y, diameter, startAngle, arcAngle, thickness));
		LOG_DRAW_END(drawThickCircleArc);
	}
}

void DRAWING_PAINTER_NATIVE(drawFlippedImage, MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha)
{
	if (!LLUI_DISPLAY_isClosed(img) && alpha > 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawFlippedImage)))
	{
		LOG_DRAW_START(drawFlippedImage);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha));
		LOG_DRAW_END(drawFlippedImage);
	}
}

void DRAWING_PAINTER_NATIVE(drawRotatedImageNearestNeighbor, MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha)
{
	if (!LLUI_DISPLAY_isClosed(img) && alpha > 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawRotatedImageNearestNeighbor)))
	{
		LOG_DRAW_START(drawRotatedImageNearestNeighbor);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha));
		LOG_DRAW_END(drawRotatedImageNearestNeighbor);
	}
}

void DRAWING_PAINTER_NATIVE(drawRotatedImageBilinear, MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha)
{
	if (!LLUI_DISPLAY_isClosed(img) && alpha > 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawRotatedImageBilinear)))
	{
		LOG_DRAW_START(drawRotatedImageBilinear);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha));
		LOG_DRAW_END(drawRotatedImageBilinear);
	}
}

void DRAWING_PAINTER_NATIVE(drawScaledImageNearestNeighbor, MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha)
{
	if (!LLUI_DISPLAY_isClosed(img) && alpha > 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawScaledImageNearestNeighbor)))
	{
		LOG_DRAW_START(drawScaledImageNearestNeighbor);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha));
		LOG_DRAW_END(drawScaledImageNearestNeighbor);
	}
}

void DRAWING_PAINTER_NATIVE(drawScaledImageBilinear, MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha)
{
	if (!LLUI_DISPLAY_isClosed(img) && alpha > 0 && LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&DRAWING_PAINTER_NATIVE_NAME(drawScaledImageBilinear)))
	{
		LOG_DRAW_START(drawScaledImageBilinear);
		LLUI_DISPLAY_setDrawingStatus(DW_DRAWING_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha));
		LOG_DRAW_END(drawScaledImageBilinear);
	}
}

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

