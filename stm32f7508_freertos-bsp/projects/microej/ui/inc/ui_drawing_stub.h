/* 
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef UI_DRAWING_STUB_H
#define UI_DRAWING_STUB_H
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @file
 * @brief Implementation of all drawing functions of ui_drawing.h (MicroUI and Drawing libraries).
 * These are stubbed implementations, there is no error but the drawings are not performed. Useful to
 * stub a drawing on a custom destination (custom GraphicsContext format not supported by the Graphics
 * Engine).
 * @author MicroEJ Developer Team
 * @version 3.1.0
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include "ui_drawing.h"

// --------------------------------------------------------------------------------
// Public API
// --------------------------------------------------------------------------------

/*
 * @brief Stubbed implementation of writePixel. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_writePixel(MICROUI_GraphicsContext* gc, jint x, jint y);

/*
 * @brief Stubbed implementation of drawLine. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY);

/*
 * @brief Stubbed implementation of drawHorizontalLine. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawHorizontalLine(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y);

/*
 * @brief Stubbed implementation of drawVerticalLine. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawVerticalLine(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2);

/*
 * @brief Stubbed implementation of drawRectangle. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);

/*
 * @brief Stubbed implementation of fillRectangle. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_fillRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);

/*
 * @brief Stubbed implementation of drawRoundedRectangle. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/*
 * @brief Stubbed implementation of fillRoundedRectangle. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_fillRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/*
 * @brief Stubbed implementation of drawCircleArc. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);

/*
 * @brief Stubbed implementation of drawEllipseArc. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);

/*
 * @brief Stubbed implementation of fillCircleArc. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_fillCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);

/*
 * @brief Stubbed implementation of fillEllipseArc. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_fillEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);

/*
 * @brief Stubbed implementation of drawEllipse. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

/*
 * @brief Stubbed implementation of fillEllipse. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_fillEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

/*
 * @brief Stubbed implementation of drawCircle. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);

/*
 * @brief Stubbed implementation of fillCircle. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_fillCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);

/*
 * @brief Stubbed implementation of drawImage. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

/*
 * @brief Stubbed implementation of copyImage. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_copyImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);

/*
 * @brief Stubbed implementation of drawRegion. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawRegion(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

/*
 * @brief Stubbed implementation of drawThickFadedPoint. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawThickFadedPoint(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade);

/*
 * @brief Stubbed implementation of drawThickFadedLine. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawThickFadedLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap);

/*
 * @brief Stubbed implementation of drawThickFadedCircle. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawThickFadedCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade);

/*
 * @brief Stubbed implementation of drawThickFadedCircleArc. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawThickFadedCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end);

/*
 * @brief Stubbed implementation of drawThickFadedEllipse. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawThickFadedEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade);

/*
 * @brief Stubbed implementation of drawThickLine. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawThickLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness);

/*
 * @brief Stubbed implementation of drawThickCircle. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawThickCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness);

/*
 * @brief Stubbed implementation of drawThickEllipse. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawThickEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness);

/*
 * @brief Stubbed implementation of drawThickCircleArc. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawThickCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness);

/*
 * @brief Stubbed implementation of drawFlippedImage. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawFlippedImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);

/*
 * @brief Stubbed implementation of drawRotatedImageNearestNeighbor. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

/*
 * @brief Stubbed implementation of drawRotatedImageBilinear. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawRotatedImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

/*
 * @brief Stubbed implementation of drawScaledImageNearestNeighbor. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawScaledImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

/*
 * @brief Stubbed implementation of drawScaledImageBilinear. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_STUB_drawScaledImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif // UI_DRAWING_STUB_H
