/* 
 * Copyright 2020-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#if !defined UI_DRAWING_H
#define UI_DRAWING_H
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Provides drawing functions called by MicroUI native drawing functions.
 *
 * Default MicroUI native drawing functions implementations manage the synchronization
 * with the Graphics Engine and allow a third party drawer perform the drawings.
 *
 * This header file lists all drawing functions to implement. This cut between MicroUI
 * native functions default implementation and this file allows to simplify the
 * writing of drawing functions: a drawing function has just to consider the drawing
 * itself and not the synchronization with the Graphics Engine.
 *
 * The drawer has the responsibility to check the graphics context (when not disabled
 * by caller) and to update the new flush dirty area.
 *
 * This cut simplifies the MicroUI native function implementation, which becomes:
 *
 * void _drawing_native_xxx(MICROUI_GraphicsContext* gc, ...) {
 * 		// tell to the Graphics Engine if drawing can be performed
 * 		if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&_drawing_native_xxx)) {
 * 			// perform the drawings and set drawing status
 * 			LLUI_DISPLAY_setDrawingStatus(_drawing_xxx(gc, ...));
 * 		}
 * 		// else: refused drawing
 * }
 *
 * When clip is checked very early, the native implementation becomes:
 *
 * void _drawing_native_xxx(MICROUI_GraphicsContext* gc, ...) {
 * 		// tell to the Graphics Engine if drawing can be performed
 * 		if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&_drawing_native_xxx)) {
 * 			DRAWING_Status status;
 *
 * 			if (LLUI_DISPLAY_isInClip(gc, ...)) {
 * 				// perform the drawings
 * 				status = _drawing_xxx(gc, ...);
 * 			}
 * 			else {
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
 * However, all drawing functions are already implemented in ui_drawing.c as
 * weak implementation. This allows to override one or several functions without
 * the need to override all functions. Default ui_drawing.c implementation calls the
 * Graphics Engine software algorithms.
 *
 * The Graphics Engine software algorithms are listed in ui_drawing_soft.h.
 * This allows to the ui_drawing.h to test if it is able to perform a drawing and
 * if not, to call the software algorithm instead. For instance a GPU may be able
 * to draw an image applying a rotation but it can't apply this rotation and an opacity
 * at the same time. In this case, the drawing implementation function has to check
 * the parameter "alpha" and calls or not Graphics Engine software algorithm.
 *
 * The implementation of these drawings must respect the graphics context clip and
 * the flush dirty area as explained in LLUI_PAINTER_impl.h.
 *
 * @author MicroEJ Developer Team
 * @version 3.1.0
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include <LLUI_PAINTER_impl.h>
#include <LLDW_PAINTER_impl.h>

// --------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------

/*
 * @brief Concat two defines
 */
#ifndef CONCAT
#define CONCAT0(p,s) p ## s
#define CONCAT(p,s) CONCAT0(p,s)
#endif

// --------------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------------

#if defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

/*
 * @brief Tells if drawer "1" matches the destination format. The VEE port must implement
 * this function when it includes the support to target the destination format. It not, the
 * default weak implementation in ui_drawing.c redirects all the drawings for this destination
 * format in the stub implementation.
 */
bool UI_DRAWING_is_drawer_1(jbyte image_format);

#if (LLUI_GC_SUPPORTED_FORMATS > 2)
/*
 * @brief Tells drawer "2" matches the destination format. See UI_DRAWING_is_drawer_1.
 */
bool UI_DRAWING_is_drawer_2(jbyte image_format);

#endif // #if (LLUI_GC_SUPPORTED_FORMATS > 2)

#endif // #if defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

/*
 * @brief Returns the new image row stride in bytes.
 *
 * @param[in] image_format the new RAW image format. The format is one value from the
 * MICROUI_ImageFormat enumeration.
 * @param[in] image_width the new image width (in pixels).
 * @param[in] image_height the new image height (in pixels).
 * @param[in] default_stride the minimal row stride (in bytes)
 *
 * @return expected row stride (in bytes)
 *
 * @see LLUI_DISPLAY_IMPL_getNewImageStrideInBytes()
 */
uint32_t UI_DRAWING_getNewImageStrideInBytes(jbyte image_format, uint32_t image_width, uint32_t image_height, uint32_t default_stride);

/*
 * @brief Adjusts the new image characteristics: data size and alignment.
 *
 * @param[in] image_format the new RAW image format. The format is one value from the
 * MICROUI_ImageFormat enumeration.
 * @param[in] width the new image width (in pixels).
 * @param[in] height the new image height (in pixels).
 * @param[in/out] data_size the minimal data size (in bytes).
 * @param[in/out] data_alignment the minimal data alignment to respect (in bytes).
 *
 * @see LLUI_DISPLAY_IMPL_adjustNewImageCharacteristics()
 */
void UI_DRAWING_adjustNewImageCharacteristics(jbyte image_format, uint32_t width, uint32_t height, uint32_t* data_size, uint32_t* data_alignment);

/*
 * @brief Initializes the image's buffer.
 *
 * @param[in] image the MicroUI Image to initialize.
 *
 * @see LLUI_DISPLAY_IMPL_initializeNewImage()
 */
void UI_DRAWING_initializeNewImage(MICROUI_Image* image);

/*
 * @brief Frees the image's resources (if any). This function is called just before releasing
 * the image buffer and closing the image.
 *
 * @param[in] image the MicroUI Image to close.
 *
 * @see LLUI_DISPLAY_IMPL_freeImageResources()
 */
void UI_DRAWING_freeImageResources(MICROUI_Image* image);

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
 * The image and the destination (MICROUI_GraphicsContext) never target the same image. See
 * UI_DRAWING_copyImage() and UI_DRAWING_drawRegion().
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
 * @param[in] regionY the y coordinate of the upper-left corner of the region to copy.
 * @param[in] width the width of the region to copy.
 * @param[in] height the height of the region to copy.
 * @param[in] x the x coordinate of the top-left point in the destination.
 * @param[in] y the y coordinate of the top-left point in the destination.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

/*
 * @brief Copy a region of an image in another image with the same pixel format.
 *
 * Contrary to UI_DRAWING_drawImage(), the opacity is not an option. As the source and destination
 * have the same pixel representation, this function has just to perform a "memory copy".
 *
 * The region of the image to draw is given relative to the image (origin at the
 * upper-left corner) as a rectangle.
 *
 * If the specified source region exceeds the image bounds, the copied region is
 * limited to the image boundary. If the copied region goes out of the bounds of
 * the graphics context area, pixels out of the range will not be drawn.
 *
 * The image and the destination (MICROUI_GraphicsContext) may target the same image. By consequence,
 * the implementation has to check the "overlap" use-case: the region to copy overlaps the destination
 * region.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to copy.
 * @param[in] regionX the x coordinate of the upper-left corner of the region to copy.
 * @param[in] regionY the y coordinate of the upper-left corner of the region to copy.
 * @param[in] width the width of the region to copy.
 * @param[in] height the height of the region to copy.
 * @param[in] x the x coordinate of the top-left point in the destination.
 * @param[in] y the y coordinate of the top-left point in the destination.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_copyImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);

/*
 * @brief Draw a region of an image in the same image.
 *
 * Contrary to UI_DRAWING_drawImage(), the implementation has to check the "overlap" use-case:
 * the region to copy overlaps the destination region.
 *
 * The region of the image to draw is given relative to the image (origin at the
 * upper-left corner) as a rectangle.
 *
 * If the specified source region exceeds the image bounds, the copied region is
 * limited to the image boundary. If the copied region goes out of the bounds of
 * the graphics context area, pixels out of the range will not be drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext source and target.
 * @param[in] regionX the x coordinate of the upper-left corner of the region to copy.
 * @param[in] regionY the y coordinate of the upper-left corner of the region to copy.
 * @param[in] width the width of the region to copy.
 * @param[in] height the height of the region to copy.
 * @param[in] x the x coordinate of the top-left point in the destination.
 * @param[in] y the y coordinate of the top-left point in the destination.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawRegion(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

/*
 * @brief Draws a thick point with fade at given position.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the point X coordinate.
 * @param[in] y the point Y coordinate.
 * @param[in] thickness the point thickness.
 * @param[in] fade the fade to apply.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawThickFadedPoint(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade);

/*
 * @brief Draws a thick line with fade between given points.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the x coordinate of the start of the line
 * @param[in] startY the y coordinate of the start of the line
 * @param[in] endX the x coordinate of the end of the line
 * @param[in] endY the y coordinate of the end of the line
 * @param[in] thickness the line thickness.
 * @param[in] fade the fade to apply.
 * @param[in] startCap cap representation of start of shape
 * @param[in] endCap cap representation of end of shape
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawThickFadedLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap);

/*
 * @brief Draws a thick circle with fade covering the square specified by its diameter.
 *
 * If diameter is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] thickness the circle thickness.
 * @param[in] fade the fade to apply.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawThickFadedCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade);

/*
 * @brief Draws a thick circle with fade arc covering the specified square.
 *
 * The arc is drawn from startAngle up to arcAngle degrees. The center of the arc
 * is defined as the center of the square whose origin is at (x,y) (upper-left
 * corner) and whose dimension is given by diameter.
 *
 * Angles are interpreted such that 0 degrees is at the 3 o'clock position. A positive
 * value indicates a counter-clockwise rotation while a negative value indicates
 * a clockwise rotation.
 *
 * If diameter is negative or zero, nothing is drawn.
 *
 * The angles are given relative to the square. For instance an angle of 45 degrees
 * is always defined by the line from the center of the square to the upper right
 * corner of the square. Thus for a non squared square angles are skewed along
 * either height or width.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the arc is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the arc is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 * @param[in] thickness the arc thickness.
 * @param[in] fade the fade to apply.
 * @param[in] start cap representation of start of shape
 * @param[in] end cap representation of end of shape
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawThickFadedCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end);

/*
 * @brief Draws a thick ellipse with fade covering the specified rectangle.
 *
 * The center of the ellipse is defined as the center of the rectangle whose origin
 * is at (x,y) (upper-left corner) and whose dimension is given by width and height.
 *
 * If either width or height is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the rectangle where the ellipse is drawn
 * @param[in] y the y coordinate of the upper-left corner of the rectangle where the ellipse is drawn
 * @param[in] width the width of the ellipse to draw
 * @param[in] height the height of the ellipse to draw
 * @param[in] thickness the ellipse thickness.
 * @param[in] fade the fade to apply.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawThickFadedEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade);

/*
 * @brief Draws a thick line between given points.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the x coordinate of the start of the line
 * @param[in] startY the y coordinate of the start of the line
 * @param[in] endX the x coordinate of the end of the line
 * @param[in] endY the y coordinate of the end of the line
 * @param[in] thickness the line thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawThickLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness);

/*
 * @brief Draws a thick circle covering the square specified by its diameter.
 *
 * If diameter is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] thickness the circle thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawThickCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness);

/*
 * @brief Draws a thick ellipse covering the specified rectangle.
 *
 * The center of the ellipse is defined as the center of the rectangle whose origin
 * is at (x,y) (upper-left corner) and whose dimension is given by width and height.
 *
 * If either width or height is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] width the width of the ellipse to draw
 * @param[in] height the height of the ellipse to draw
 * @param[in] thickness the circle thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawThickEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness);

/*
 * @brief Draws a thick arc covering the square specified by its diameter.
 *
 * The arc is drawn from startAngle up to arcAngle degrees. The center of the arc is
 * defined as the center of the square whose origin is at (x,y) (upper-left corner)
 * and whose dimension is given by diameter.
 *
 * Angles are interpreted such that 0 degrees is at the 3 o'clock position. A positive
 * value indicates a counter-clockwise rotation while a negative value indicates a
 * clockwise rotation.
 *
 * If diameter is negative, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the arc is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the arc is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 * @param[in] thickness the arc thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawThickCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness);

/*
 * @brief Draws an image applying a flip (0, 90, 180 or 270 degrees with or without
 * mirror).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] regionX the x coordinate of the upper-left corner of the region to draw.
 * @param[in] regionY the y coordinate of the upper-left corner of the region to draw.
 * @param[in] width the width of the region to copy.
 * @param[in] height the height of the region to copy.
 * @param[in] x the x coordinate of the top-left point in the destination.
 * @param[in] y the y coordinate of the top-left point in the destination.
 * @param[in] transformation the flip to apply.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawFlippedImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);

/*
 * @brief Draws an image applying a free rotation (0 to 360 degrees).
 *
 * The rotation is specified by the center and the angle. The reference point is
 * the graphical object top-left corner. The rotation point is relative where the
 * graphical object will be drawn.
 *
 * This method uses the nearest neighbor algorithm to render the content. This algorithm
 * is faster than bilinear algorithm but its rendering is faster.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] x the x coordinate of the image reference anchor top-left point.
 * @param[in] y the y coordinate of the image reference anchor top-left point.
 * @param[in] rotationX the x coordinate of the rotation center.
 * @param[in] rotationY the y coordinate of the rotation center.
 * @param[in] angle the rotation angle.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawRotatedImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

/*
 * @brief Draws an image applying a free rotation (0 to 360 degrees).
 *
 * The rotation is specified by the center and the angle. The reference point is
 * the graphical object top-left corner. The rotation point is relative where the
 * graphical object will be drawn.
 *
 * This method uses the bilinear algorithm to render the content. This algorithm
 * performs better rendering than nearest neighbor algorithm but it is slower to
 * apply.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] x the x coordinate of the image reference anchor top-left point.
 * @param[in] y the y coordinate of the image reference anchor top-left point.
 * @param[in] rotationX the x coordinate of the rotation center.
 * @param[in] rotationY the y coordinate of the rotation center.
 * @param[in] angle the rotation angle.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawRotatedImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

/*
 * @brief Draws an image applying a scaling.
 *
 * This method uses the nearest neighbor algorithm to render the content. This algorithm
 * is faster than bilinear algorithm but its rendering is faster.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] x the x coordinate of the image reference anchor top-left point.
 * @param[in] y the y coordinate of the image reference anchor top-left point.
 * @param[in] factorX scaling X factor.
 * @param[in] factorY scaling Y factor.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawScaledImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

/*
 * @brief Draws an image applying a scaling.
 *
 * This method uses the bilinear algorithm to render the content. This algorithm
 * performs better rendering than nearest neighbor algorithm but it is slower to
 * apply.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] x the x coordinate of the image reference anchor top-left point.
 * @param[in] y the y coordinate of the image reference anchor top-left point.
 * @param[in] factorX scaling X factor.
 * @param[in] factorY scaling Y factor.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_drawScaledImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif // UI_DRAWING_H
