/* 
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _DW_DRAWING_SOFT
#define _DW_DRAWING_SOFT
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Provides the same functions than dw_drawing.h but available in graphical
 * engine. They are implemented in software and can be used by the dw_drawing.h
 * implementation. Please refer to dw_drawing.h to have more information about the
 * aim of these functions.
 *
 * Contrary to functions listed in dw_drawing.h, the graphical engine functions
 * are blocking. If a software algorithm is using a third party function listed in
 * dw_drawing.h (see dw_drawing.h file comment), a "wait end of drawing"
 * is automatically performed.
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

/*
 * @brief Uses LLDW_PAINTER_impl.h typedefs.
 */
#include "LLDW_PAINTER_impl.h"

// --------------------------------------------------------------------------------
// Functions provided by the graphical engine
// --------------------------------------------------------------------------------

/*
 * @brief Draws an anti-aliased point at given position.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the point X coordinate.
 * @param[in] y the point Y coordinate.
 * @param[in] thickness the point thickness.
 * @param[in] fade the fade to apply.
 */
void DW_DRAWING_SOFT_drawThickFadedPoint(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade);

/*
 * @brief Draws an anti-aliased line between given points.
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
 */
void DW_DRAWING_SOFT_drawThickFadedLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap);

/*
 * @brief Draws an anti-aliased circle covering the square specified by its diameter.
 *
 * If diameter is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] thickness the circle thickness.
 * @param[in] fade the fade to apply.
 */
void DW_DRAWING_SOFT_drawThickFadedCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade);

/*
 * @brief Draws an anti-aliased circle arc covering the specified square.
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
 */
void DW_DRAWING_SOFT_drawThickFadedCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end);

/*
 * @brief Draws an anti-aliased ellipse covering the specified rectangle.
 *
 * The center of the ellipse is defined as the center of the rectangle whose origin
 * is at (x,y) (upper-left corner) and whose dimension is given by width and height.
 *
 * If either width or height is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] fade the fade to apply.
 * @param[in] x the x coordinate of the upper-left corner of the rectangle where the ellipse is drawn
 * @param[in] y the y coordinate of the upper-left corner of the rectangle where the ellipse is drawn
 * @param[in] width the width of the ellipse to draw
 * @param[in] height the height of the ellipse to draw
 * @param[in] thickness the ellipse thickness.
 */
void DW_DRAWING_SOFT_drawThickFadedEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade);

/*
 * @brief Draws a thick line between given points.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the x coordinate of the start of the line
 * @param[in] startY the y coordinate of the start of the line
 * @param[in] endX the x coordinate of the end of the line
 * @param[in] endY the y coordinate of the end of the line
 * @param[in] thickness the line thickness.
 */
void DW_DRAWING_SOFT_drawThickLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness);

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
 */
void DW_DRAWING_SOFT_drawThickCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness);

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
 */
void DW_DRAWING_SOFT_drawThickEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness);

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
 */
void DW_DRAWING_SOFT_drawThickCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness);

/*
 * @brief Draws an image applying a flip (0, 90, 180 or 270 degrees with or without
 * mirror).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] regionX the x coordinate of the upper-left corner of the region to draw.
 * @param[in] regionY the x coordinate of the upper-left corner of the region to draw.
 * @param[in] width the width of the region to copy.
 * @param[in] height the height of the region to copy.
 * @param[in] x the x coordinate of the top-left point in the destination.
 * @param[in] y the y coordinate of the top-left point in the destination.
 * @param[in] transformation the flip to apply.
 * @param[in] alpha the opacity level to apply to the region.
 */
void DW_DRAWING_SOFT_drawFlippedImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);

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
 */
void DW_DRAWING_SOFT_drawRotatedImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

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
 */
void DW_DRAWING_SOFT_drawRotatedImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

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
 */
void DW_DRAWING_SOFT_drawScaledImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

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
 */
void DW_DRAWING_SOFT_drawScaledImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
