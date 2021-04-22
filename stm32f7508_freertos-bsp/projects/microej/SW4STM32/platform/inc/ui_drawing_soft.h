/* 
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _UI_DRAWING_SOFT
#define _UI_DRAWING_SOFT
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Provides the same functions than ui_drawing.h but available in graphical
 * engine. They are implemented in software and can be used by the ui_drawing.h
 * implementation. Please refer to ui_drawing.h to have more information about the
 * aim of these functions.
 *
 * Contrary to functions listed in ui_drawing.h, the graphical engine functions
 * are blocking. If a software algorithm is using a third party function listed in
 * ui_drawing.h (see ui_drawing.h file comment), a "wait end of drawing"
 * is automatically performed.
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

/*
 * @brief Uses LLUI_PAINTER_impl.h typedefs.
 */
#include "LLUI_PAINTER_impl.h"

// --------------------------------------------------------------------------------
// Functions provided by the graphical engine
// --------------------------------------------------------------------------------

/*
 * @brief Draws a pixel at given position.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the pixel X coordinate.
 * @param[in] y the pixel Y coordinate.
 */
void UI_DRAWING_SOFT_writePixel(MICROUI_GraphicsContext* gc, jint x, jint y);

/*
 * @brief Draws a line at between points startX,startY (included) and endX,endY (included).
 * Note: startX may be higher than endX (and/or startY  may be higher than endY).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the first pixel line X coordinate.
 * @param[in] startY the first pixel line Y coordinate.
 * @param[in] endX the last pixel line X coordinate.
 * @param[in] endY the last pixel line Y coordinate.
 */
void UI_DRAWING_SOFT_drawLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY);

/*
 * @brief Draws a horizontal line at between points x1,y (included) and x2,y (included).
 * Caller ensures that x1 <= x2.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the first pixel line X coordinate.
 * @param[in] x2 the last pixel line X coordinate.
 * @param[in] y the both pixels line Y coordinate.
 */
void UI_DRAWING_SOFT_drawHorizontalLine(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y);

/*
 * @brief Draws a vertical line at between points x,y1 (included) and x,y2 (included).
 * Caller ensures that y1 <= y2.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the both pixels line X coordinate.
 * @param[in] y1 the first pixel line Y coordinate.
 * @param[in] y2 the last pixel line Y coordinate.
 */
void UI_DRAWING_SOFT_drawVerticalLine(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2);

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
 */
void UI_DRAWING_SOFT_drawRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);

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
 */
void UI_DRAWING_SOFT_fillRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);

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
 */
void UI_DRAWING_SOFT_drawRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

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
 */
void UI_DRAWING_SOFT_fillRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/*
 * @brief Draws a circular arc covering the square defined by top-left point x,y (included)
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
 * @param[in] square the diameter of the arc to draw.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 */
void UI_DRAWING_SOFT_drawCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);

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
 */
void UI_DRAWING_SOFT_drawEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);

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
 */
void UI_DRAWING_SOFT_fillCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);

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
 */
void UI_DRAWING_SOFT_fillEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);

/*
 * @brief Draws an ellipse covering the rectangle defined by top-left point x,y (included)
 * and bottom-right point x+width-1,y+height-1 (included). Ellipses which focal points are not
 * on the same axis are not supported.
 *
 * The center of the ellipse is defined as the center of the rectangle whose origin
 * is at the given coordinates (upper-left corner) and whose dimension is given
 * by the width and height parameters.
 *
 * If either the given width or height is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the ellipse width.
 * @param[in] height the ellipse height.
 */
void UI_DRAWING_SOFT_drawEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

/*
 * @brief Fills an ellipse covering the rectangle defined by top-left point x,y (included)
 * and bottom-right point x+width-1,y+height-1 (included).
 *
 * The center of the ellipse is defined as the center of the rectangle whose origin
 * is at the given coordinates (upper-left corner) and whose dimension is given
 * by the width and height parameters.
 *
 * If either the given width or height is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the ellipse width.
 * @param[in] height the ellipse height.
 */
void UI_DRAWING_SOFT_fillEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

/*
 * @brief Draws a circle covering the square defined by top-left point x,y (included)
 * and bottom-right point x+diameter-1,y+diameter-1 (included).
 *
 * The center of the circle is defined as the center of the rectangle whose origin
 * is at the given coordinates (upper-left corner) and whose dimension is given
 * by the diameter parameter.
 *
 * If the given diameter is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the circle square size.
 */
void UI_DRAWING_SOFT_drawCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);

/*
 * @brief Fills a circle covering the square defined by top-left point x,y (included)
 * and bottom-right point x+diameter-1,y+diameter-1 (included).
 *
 * The center of the circle is defined as the center of the rectangle whose origin
 * is at the given coordinates (upper-left corner) and whose dimension is given
 * by the diameter parameter.
 *
 * If the given diameter is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the circle square size.
 */
void UI_DRAWING_SOFT_fillCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);

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
 */
void UI_DRAWING_SOFT_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
