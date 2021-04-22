/* 
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _UI_DRAWING
#define _UI_DRAWING
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Provides drawing functions called by MicroUI native drawing functions.
 *
 * Default MicroUI native drawing functions implementations manage the synchronization
 * with the graphical engine and allow a third party drawer perform the drawings.
 *
 * This header file lists all drawing functions to implement. This cut between MicroUI
 * native functions default implementation and this file allows to simplify the
 * writing of drawing functions: a drawing function has just to consider the drawing
 * itself and not the synchronization with the graphical engine.
 *
 * The drawer has the responsibility to check the graphics context (when not disabled
 * by caller) and to update the new flush dirty area.
 *
 * This cut simplifies the MicroUI native function implementation, which becomes:
 *
 * void _drawing_native_xxx(MICROUI_GraphicsContext* gc, ...)
 * {
 * 		// tell to graphical engine if drawing can be performed
 * 		if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&_drawing_native_xxx))
 * 		{
 * 			// perform the drawings and set drawing status
 * 			LLUI_DISPLAY_setDrawingStatus(_drawing_xxx(gc, ...));
 * 		}
 * 		// else: refused drawing
 * }
 *
 * When clip is checked very early, the native implementation becomes:
 *
 * void _drawing_native_xxx(MICROUI_GraphicsContext* gc, ...)
 * {
 * 		// tell to graphical engine if drawing can be performed
 * 		if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&_drawing_native_xxx))
 * 		{
 * 			DRAWING_Status status;
 *
 * 			if (LLUI_DISPLAY_isInClip(gc, ...))
 * 			{
 * 				// perform the drawings
 * 				status = _drawing_xxx(gc, ...);
 * 			}
 * 			else
 * 			{
 * 				// drawing is out of clip: nothing to draw
 * 				status = DRAWING_DONE;
 *			}
 *
 *			// set drawing status
 *			LLUI_DISPLAY_setDrawingStatus(status);
 * 		}
 * 		// else: refused drawing
 * }
 *
 * This cut allows to use a third party drawer like a dedicated CPU (hardware accelerator).
 * However, all drawing functions are already implemented in graphical engine as
 * weak implementation. This allows to override one or several functions without
 * the need to override all functions.
 *
 * Default graphical engine implementation is using software algorithms. However
 * some software algorithms may call some others drawing functions. For instance
 * to draw a vertical line, a call to "fill rectangle" is performed. If the drawing
 * implementation file implements only "fill rectangle" function over a GPU, this
 * function will be used to draw vertical lines too.
 *
 * The default graphical engine software algorithms are listed in ui_drawing_soft.h.
 * This allows to the ui_drawing.h to test if it is able to perform a drawing and
 * if not, to call the software algorithm instead. For instance a GPU may be able
 * to draw an image applying a rotation but it can apply this rotation and a mirror
 * at the same time. In this case, the drawing implementation function has to check
 * if the parameter "mirror" is false; if not, implementation has to call software
 * algorithm instead.
 *
 * The implementation of these drawings must respect the graphics context clip and
 * the flush dirty area as explained in LLUI_PAINTER_impl.h.
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

/*
 * @brief Requires LLUI_PAINTER_impl.h typedefs.
 */
#include "LLUI_PAINTER_impl.h"

// --------------------------------------------------------------------------------
// Typedefs and Structures
// --------------------------------------------------------------------------------

/*
 * @brief Drawing functions' return status.
 *
 * A function has to return DRAWING_DONE when the drawing has been fully done during
 * the function call (synchronous drawing). When the drawing is performed by an
 * asynchronous third party process (software or hardware), the function has to
 * return DRAWING_RUNNING to notify to the graphical engine the drawing is not fully
 * done yet.
 */
typedef enum
{
	/*
	 * @brief Value to return when the drawing has been synchronously performed.
	 */
	DRAWING_DONE = 0,

	/*
	 * @brief Value to return when the drawing will be asynchronously performed.
	 */
	DRAWING_RUNNING = 1

} DRAWING_Status;

// --------------------------------------------------------------------------------
// Functions that must be implemented
// --------------------------------------------------------------------------------

/*
 * @brief Draws a pixel at given position.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the pixel X coordinate.
 * @param[in] y the pixel Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_writePixel(MICROUI_GraphicsContext* gc, jint x, jint y);

/*
 * @brief Draws a line at between points startX,startY (included) and endX,endY (included).
 * Note: startX may be higher than endX (and/or startY  may be higher than endY).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the first pixel line X coordinate.
 * @param[in] startY the first pixel line Y coordinate.
 * @param[in] endX the last pixel line X coordinate.
 * @param[in] endY the last pixel line Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY);

/*
 * @brief Draws a horizontal line at between points x1,y (included) and x2,y (included).
 * Caller ensures that x1 <= x2.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the first pixel line X coordinate.
 * @param[in] x2 the last pixel line X coordinate.
 * @param[in] y the both pixels line Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawHorizontalLine(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y);

/*
 * @brief Draws a vertical line at between points x,y1 (included) and x,y2 (included).
 * Caller ensures that y1 <= y2.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the both pixels line X coordinate.
 * @param[in] y1 the first pixel line Y coordinate.
 * @param[in] y2 the last pixel line Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawVerticalLine(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2);

/*
 * @brief Draws an orthogonal rectangle at from top-left point x1,y1 (included) and bottom-right
 * point x2,y2 (included).
 * Caller ensures that x1 <= x2 and y1 <= y2.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the top-left pixel X coordinate.
 * @param[in] y1 the top-left pixel Y coordinate.
 * @param[in] x2 the bottom-right pixel X coordinate.
 * @param[in] y2 the top-right pixel Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);

/*
 * @brief Fills a rectangle at from top-left point x1,y1 (included) and bottom-right
 * point x2,y2 (included).
 * Caller ensures that x1 <= x2 and y1 <= y2.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the top-left pixel X coordinate.
 * @param[in] y1 the top-left pixel Y coordinate.
 * @param[in] x2 the bottom-right pixel X coordinate.
 * @param[in] y2 the top-right pixel Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_fillRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);

/*
 * @brief Draws a rounded rectangle at from top-left point x,y (included) and bottom-right
 * point x+width-1,y+height-1 (included).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 * @param[in] cornerEllipseWidth  the horizontal diameter of the arc at the corners.
 * @param[in] cornerEllipseHeight the vertical diameter of the arc at the corners.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/*
 * @brief Fills a rounded rectangle at from top-left point x,y (included) and bottom-right
 * point x+width-1,y+height-1 (included).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 * @param[in] cornerEllipseWidth  the horizontal diameter of the arc at the corners.
 * @param[in] cornerEllipseHeight the vertical diameter of the arc at the corners.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_fillRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/*
 * @brief Draws a circular arc covering the square defined by top-left point x,y (included)
 * and bottom-right point x+diameter-1,y+diameter-1 (included).
 *
 * The arc is drawn from startAngle up to arcAngle degrees. The center of the arc is
 * defined as the center of the square whose origin is at (x,y) (upper-left corner)
 * and whose dimension is given by width and height.
 *
 * Angles are interpreted such that 0 degrees is at the 3 o'clock position. A positive
 * value indicates a counter-clockwise rotation while a negative value indicates
 * a clockwise rotation.
 *
 * If either the given diameter is negative or zero, or if arcAngle is zero,
 * nothing is drawn.
 *
 * The angles are given relative to the square. For instance an angle of 45
 * degrees is always defined by the line from the center of the square to the
 * upper right corner of the square.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] square the diameter of the arc to draw.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);

/*
 * @brief Draws an elliptical arc covering the rectangle defined by top-left point x,y (included)
 * and bottom-right point x+width-1,y+height-1 (included).
 *
 * The arc is drawn from startAngle up to arcAngle degrees. The center of the arc is
 * defined as the center of the rectangle whose origin is at (x,y) (upper-left corner)
 * and whose dimension is given by width and height.
 *
 * Angles are interpreted such that 0 degrees is at the 3 o'clock position. A positive
 * value indicates a counter-clockwise rotation while a negative value indicates
 * a clockwise rotation.
 *
 * If either the given width or height is negative or zero, or if arcAngle is zero,
 * nothing is drawn.
 *
 * The angles are given relative to the rectangle. For instance an angle of 45
 * degrees is always defined by the line from the center of the rectangle to the
 * upper right corner of the rectangle. Thus for a non squared rectangle
 * angles are skewed along either height or width.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);

/*
 * @brief Fills a circular arc covering the square defined by top-left point x,y (included)
 * and bottom-right point x+diameter-1,y+diameter-1 (included).
 *
 * The arc is drawn from startAngle up to arcAngle degrees. The center of the arc is
 * defined as the center of the rectangle whose origin is at (x,y) (upper-left corner)
 * and whose dimension is given by width and height.
 *
 * Angles are interpreted such that 0 degrees is at the 3 o'clock position. A positive
 * value indicates a counter-clockwise rotation while a negative value indicates
 * a clockwise rotation.
 *
 * If either the given diameter is negative or zero, or if arcAngle is zero,
 * nothing is drawn.
 *
 * The angles are given relative to the square. For instance an angle of 45
 * degrees is always defined by the line from the center of the square to the
 * upper right corner of the square.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the diameter of the arc to draw.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_fillCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);

/*
 * @brief Fills an elliptical arc covering the rectangle defined by top-left point x,y (included)
 * and bottom-right point x+width-1,y+height-1 (included).
 *
 * The arc is drawn from startAngle up to arcAngle degrees. The center of the arc is
 * defined as the center of the rectangle whose origin is at (x,y) (upper-left corner)
 * and whose dimension is given by width and height.
 *
 * Angles are interpreted such that 0 degrees is at the 3 o'clock position. A positive
 * value indicates a counter-clockwise rotation while a negative value indicates
 * a clockwise rotation.
 *
 * If either the given width or height is negative or zero, or if arcAngle is zero,
 * nothing is drawn.
 *
 * The angles are given relative to the rectangle. For instance an angle of 45
 * degrees is always defined by the line from the center of the rectangle to the
 * upper right corner of the rectangle. Thus for a non squared rectangle
 * angles are skewed along either height or width.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_fillEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);

/*
 * @brief Draws an ellipse covering the rectangle defined by top-left point x,y (included)
 * and bottom-right point x+width-1,y+height-1 (included). Ellipses which focal points are not
 * on the same axis are not supported.
 *
 * If either the given width or height is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the ellipse width.
 * @param[in] height the ellipse height.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

/*
 * @brief Fills an ellipse covering the rectangle defined by top-left point x,y (included)
 * and bottom-right point x+width-1,y+height-1 (included).
 *
 * If either the given width or height is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the ellipse width.
 * @param[in] height the ellipse height.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_fillEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

/*
 * @brief Draws a circle covering the square defined by top-left point x,y (included)
 * and bottom-right point x+diameter-1,y+diameter-1 (included).
 *
 * If the given diameter is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the circle square size.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);

/*
 * @brief Fills a circle covering the square defined by top-left point x,y (included)
 * and bottom-right point x+diameter-1,y+diameter-1 (included)
 *
 * If the given diameter is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the circle square size.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_fillCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);

/*
 * @brief Draws a region of an image.
 *
 * The region of the image to draw is given relative to the image (origin at the
 * upper-left corner) as a rectangle.
 *
 * If the specified source region exceeds the image bounds, the copied region is
 * limited to the image boundary. If the copied region goes out of the bounds of
 * the graphics context area, pixels out of the range will not be drawn.
 *
 * A global opacity value is given. When this value is 0xff (255, opaque), that means the image
 * is drawn on the graphics context without managing an extra opacity. Only the image transparent
 * pixels must have been merged with destination. All image opaque pixels override destination.
 *
 * When this value is a value between 0 and 0xff, that means each pixel of the image must be merged
 * with destination in addition with the image transparent pixels. An image opaque pixel becomes
 * transparent (its opacity is the given alpha) and the opacity of an image transparent pixel becomes
 * (alpha * alpha(pixel)) / 255.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] regionX the x coordinate of the upper-left corner of the region to copy.
 * @param[in] regionY the x coordinate of the upper-left corner of the region to copy.
 * @param[in] width the width of the region to copy.
 * @param[in] height the height of the region to copy.
 * @param[in] x the x coordinate of the top-left point in the destination.
 * @param[in] y the y coordinate of the top-left point in the destination.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
