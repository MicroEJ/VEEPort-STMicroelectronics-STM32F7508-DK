
/*
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief This file implements all MicroUI drawing native functions.
 * @see LLGRAPHICS_impl.h file comment
 * @author MicroEJ Developer Team
 * @version 1.0.2
 * @date 2 September 2020
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

// implements LLUI_PAINTER_impl functions
#include "LLUI_PAINTER_impl.h"

// calls ui_drawing functions
#include "ui_drawing.h"

// use graphical engine functions to synchronize drawings
#include "LLUI_DISPLAY.h"

// --------------------------------------------------------------------------------
// Macros and Defines
// --------------------------------------------------------------------------------

// macros to log a drawing
#define LOG_DRAW_START(fn) LLUI_DISPLAY_logDrawingStart(CONCAT_DEFINES(LOG_DRAW_, fn))
#define LOG_DRAW_END(fn) LLUI_DISPLAY_logDrawingEnd(CONCAT_DEFINES(LOG_DRAW_, fn))

#define MICROUI_PAINTER_NATIVE_NAME(fn) (CONCAT_DEFINES(MICROUI_PAINTER_NATIVE_PREFIX, fn))

/*
 * LOG_DRAW_EVENT logs identifiers
 */
#define LOG_DRAW_writePixel 1
#define LOG_DRAW_drawLine 2
#define LOG_DRAW_drawHorizontalLine 3
#define LOG_DRAW_drawVerticalLine 4
#define LOG_DRAW_drawRectangle 5
#define LOG_DRAW_fillRectangle 6
#define LOG_DRAW_drawRoundedRectangle 8
#define LOG_DRAW_fillRoundedRectangle 9
#define LOG_DRAW_drawCircleArc 10
#define LOG_DRAW_fillCircleArc 11
#define LOG_DRAW_drawEllipseArc 12
#define LOG_DRAW_fillEllipseArc 13
#define LOG_DRAW_drawEllipse 14
#define LOG_DRAW_fillEllipse 15
#define LOG_DRAW_drawCircle 16
#define LOG_DRAW_fillCircle 17
#define LOG_DRAW_drawARGB 18
#define LOG_DRAW_drawImage 19

// --------------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------------

/*
 * Checks given bound to fit in bound limits: 0 and max (excluded). Updates size and
 * origin in consequence
 */
static inline void _check_bound(jint max, jint* bound, jint* size, jint* origin)
{
	if (*bound < 0)
	{
		*size += *bound; // decrease size
		*origin -= *bound; // increase origin
		*bound = 0;
	}

	if (*bound + *size > max)
	{
		*size = max - *bound; // decrease size
	}
}

// --------------------------------------------------------------------------------
// MicroUI native functions
// --------------------------------------------------------------------------------

void MICROUI_PAINTER_NATIVE(writePixel, MICROUI_GraphicsContext* gc, jint x, jint y)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(writePixel)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(writePixel);
		if (LLUI_DISPLAY_isPixelInClip(gc, x, y))
		{
			LLUI_DISPLAY_configureClip(gc, false/* point is in clip */);
			status = UI_DRAWING_writePixel(gc, x, y);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(writePixel);
	}
	// else: pixel out of clip: nothing to do (requestDrawing() has not been called)
}

void MICROUI_PAINTER_NATIVE(drawLine, MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawLine)))
	{
		LOG_DRAW_START(drawLine);
		// cannot reduce/clip line: may be endX < startX and / or endY < startY
		LLUI_DISPLAY_setDrawingStatus(UI_DRAWING_drawLine(gc, startX, startY, endX, endY));
		LOG_DRAW_END(drawLine);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(drawHorizontalLine, MICROUI_GraphicsContext* gc, jint x, jint y, jint length)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawHorizontalLine)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(drawHorizontalLine);

		jint x1 = x;
		jint x2 = x + length - 1;

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (length > 0 && LLUI_DISPLAY_clipHorizontalLine(gc, &x1, &x2, y))
		{
			LLUI_DISPLAY_configureClip(gc, false /* line has been clipped */);
			status = UI_DRAWING_drawHorizontalLine(gc, x1, x2, y);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(drawHorizontalLine);
	}
	// else: line out of clip: nothing to do (requestDrawing() has not been called)
}

void MICROUI_PAINTER_NATIVE(drawVerticalLine, MICROUI_GraphicsContext* gc, jint x, jint y, jint length)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawVerticalLine)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(drawVerticalLine);

		jint y1 = y;
		jint y2 = y + length - 1;

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (length > 0 && LLUI_DISPLAY_clipVerticalLine(gc, &y1, &y2, x))
		{
			LLUI_DISPLAY_configureClip(gc, false /* line has been clipped */);
			status = UI_DRAWING_drawVerticalLine(gc, x, y1, y2);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(drawVerticalLine);
	}
	// else: line out of clip: nothing to do (requestDrawing() has not been called)
}

void MICROUI_PAINTER_NATIVE(drawRectangle, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawRectangle)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(drawRectangle);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (width > 0 && height > 0)
		{
			jint x1 = x;
			jint x2 = x + width - 1;
			jint y1 = y;
			jint y2 = y + height - 1;

			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRectangleInClip(gc, x1, y1, x2, y2));
			status = UI_DRAWING_drawRectangle(gc, x1, y1, x2, y2);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(drawRectangle);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(fillRectangle, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(fillRectangle)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(fillRectangle);

		jint x1 = x;
		jint x2 = x + width - 1;
		jint y1 = y;
		jint y2 = y + height - 1;

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (width > 0 && height > 0 && LLUI_DISPLAY_clipRectangle(gc, &x1, &y1, &x2, &y2))
		{
			LLUI_DISPLAY_configureClip(gc, false /* rectangle has been clipped */);
			status = UI_DRAWING_fillRectangle(gc, x1, y1, x2, y2);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(fillRectangle);
	}
	// else: rectangle out of clip: nothing to do (requestDrawing() has not been called)
}

void MICROUI_PAINTER_NATIVE(drawRoundedRectangle, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawRoundedRectangle)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(drawRoundedRectangle);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (width > 0 && height > 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, width, height));
			status = UI_DRAWING_drawRoundedRectangle(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(drawRoundedRectangle);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(fillRoundedRectangle, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(fillRoundedRectangle)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(fillRoundedRectangle);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (width > 0 && height > 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, width, height));
			status = UI_DRAWING_fillRoundedRectangle(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(fillRoundedRectangle);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(drawCircleArc, MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawCircleArc)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(drawCircleArc);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (diameter > 0 && (int32_t)arcAngle != 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, diameter, diameter));
			status = UI_DRAWING_drawCircleArc(gc, x, y, diameter, startAngle, arcAngle);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(drawCircleArc);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(drawEllipseArc, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawEllipseArc)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(drawEllipseArc);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (width > 0 && height > 0 && (int32_t)arcAngle != 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, width, height));
			status = UI_DRAWING_drawEllipseArc(gc, x, y, width, height, startAngle, arcAngle);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(drawEllipseArc);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(fillCircleArc, MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(fillCircleArc)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(fillCircleArc);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (diameter > 0 && (int32_t)arcAngle != 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, diameter, diameter));
			status = UI_DRAWING_fillCircleArc(gc, x, y, diameter, startAngle, arcAngle);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(fillCircleArc);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(fillEllipseArc, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(fillEllipseArc)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(fillEllipseArc);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (width > 0 && height > 0 && (int32_t)arcAngle != 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, width, height));
			status = UI_DRAWING_fillEllipseArc(gc, x, y, width, height, startAngle, arcAngle);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(fillEllipseArc);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(drawEllipse, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawEllipse)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(drawEllipse);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (width > 0 && height > 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, width, height));
			status = UI_DRAWING_drawEllipse(gc, x, y, width, height);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(drawEllipse);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(fillEllipse, MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(fillEllipse)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(fillEllipse);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (width > 0 && height > 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, width, height));
			status = UI_DRAWING_fillEllipse(gc, x, y, width, height);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(fillEllipse);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(drawCircle, MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawCircle)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(drawCircle);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (diameter > 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, diameter, diameter));
			status = UI_DRAWING_drawCircle(gc, x, y, diameter);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(drawCircle);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(fillCircle, MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(fillCircle)))
	{
		DRAWING_Status status;
		LOG_DRAW_START(fillCircle);

		// tests on size and clip are performed after suspend to prevent to perform it several times
		if (diameter > 0)
		{
			// cannot reduce rectangle; can only check if it is fully in clip
			LLUI_DISPLAY_configureClip(gc, !LLUI_DISPLAY_isRegionInClip(gc, x, y, diameter, diameter));
			status = UI_DRAWING_fillCircle(gc, x, y, diameter);
		}
		else
		{
			// requestDrawing() has been called and accepted: notify the end of empty drawing
			status = DRAWING_DONE;
		}
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(fillCircle);
	}
	// else: refused drawing
}

void MICROUI_PAINTER_NATIVE(drawImage, MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha)
{
	if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&MICROUI_PAINTER_NATIVE_NAME(drawImage)))
	{
		DRAWING_Status status = DRAWING_DONE;
		LOG_DRAW_START(drawImage);

		// tests on parameters and clip are performed after suspend to prevent to perform it several times
		if (!LLUI_DISPLAY_isClosed(img) && alpha > 0)
		{
			alpha = alpha > 255 ? 255 : alpha;

			// compute inside image bounds
			_check_bound(img->width, &regionX, &width, &x);
			_check_bound(img->height, &regionY, &height, &y);

			// compute inside graphics context bounds
			_check_bound(gc->image.width, &x, &width, &regionX);
			_check_bound(gc->image.height, &y, &height, &regionY);

			if (width > 0 && height > 0 && LLUI_DISPLAY_clipRegion(gc, &regionX, &regionY, &width, &height, &x, &y))
			{
				LLUI_DISPLAY_configureClip(gc, false /* region has been clipped */);
				status = UI_DRAWING_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
			}
			// else: nothing to do
		}
		// else: nothing to do

		// requestDrawing() has been called and accepted: notify the end of empty drawing
		LLUI_DISPLAY_setDrawingStatus(status);
		LOG_DRAW_END(drawImage);
	}
	// else: refused drawing
}

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

