/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/*
 * @file
 * @brief Implementation of all drawing functions of ui_drawing_stub.h.
 * @author MicroEJ Developer Team
 * @version 3.1.0
 * @see ui_drawing_stub.h
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include <LLUI_DISPLAY.h>

#include "ui_drawing_stub.h"

// --------------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------------

static inline DRAWING_Status not_implemented(MICROUI_GraphicsContext* gc){
	LLUI_DISPLAY_reportError(gc, DRAWING_LOG_NOT_IMPLEMENTED);
	return DRAWING_DONE;
}

// --------------------------------------------------------------------------------
// ui_drawing_stub.h functions
// --------------------------------------------------------------------------------

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_writePixel(MICROUI_GraphicsContext* gc, jint x, jint y){
	(void)gc;
	(void)x;
	(void)y;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY){
	(void)gc;
	(void)startX;
	(void)startY;
	(void)endX;
	(void)endY;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawHorizontalLine(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y){
	(void)gc;
	(void)x1;
	(void)x2;
	(void)y;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawVerticalLine(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2){
	(void)gc;
	(void)x;
	(void)y1;
	(void)y2;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	(void)gc;
	(void)x1;
	(void)y1;
	(void)x2;
	(void)y2;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_fillRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	(void)gc;
	(void)x1;
	(void)y1;
	(void)x2;
	(void)y2;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	(void)gc;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	(void)cornerEllipseWidth;
	(void)cornerEllipseHeight;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_fillRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	(void)gc;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	(void)cornerEllipseWidth;
	(void)cornerEllipseHeight;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	(void)gc;
	(void)x;
	(void)y;
	(void)diameter;
	(void)startAngle;
	(void)arcAngle;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	(void)gc;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	(void)startAngle;
	(void)arcAngle;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_fillCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	(void)gc;
	(void)x;
	(void)y;
	(void)diameter;
	(void)startAngle;
	(void)arcAngle;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_fillEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	(void)gc;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	(void)startAngle;
	(void)arcAngle;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	(void)gc;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_fillEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	(void)gc;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	(void)gc;
	(void)x;
	(void)y;
	(void)diameter;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_fillCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	(void)gc;
	(void)x;
	(void)y;
	(void)diameter;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	(void)gc;
	(void)img;
	(void)regionX;
	(void)regionY;
	(void)width;
	(void)height;
	(void)x;
	(void)y;
	(void)alpha;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_copyImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	(void)gc;
	(void)img;
	(void)regionX;
	(void)regionY;
	(void)width;
	(void)height;
	(void)x;
	(void)y;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawRegion(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	(void)gc;
	(void)regionX;
	(void)regionY;
	(void)width;
	(void)height;
	(void)x;
	(void)y;
	(void)alpha;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawThickFadedPoint(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade){
	(void)gc;
	(void)x;
	(void)y;
	(void)thickness;
	(void)fade;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawThickFadedLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap){
	(void)gc;
	(void)startX;
	(void)startY;
	(void)endX;
	(void)endY;
	(void)thickness;
	(void)fade;
	(void)startCap;
	(void)endCap;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawThickFadedCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade){
	(void)gc;
	(void)x;
	(void)y;
	(void)diameter;
	(void)thickness;
	(void)fade;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawThickFadedCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end){
	(void)gc;
	(void)x;
	(void)y;
	(void)diameter;
	(void)startAngle;
	(void)arcAngle;
	(void)thickness;
	(void)fade;
	(void)start;
	(void)end;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawThickFadedEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade){
	(void)gc;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	(void)thickness;
	(void)fade;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawThickLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness){
	(void)gc;
	(void)startX;
	(void)startY;
	(void)endX;
	(void)endY;
	(void)thickness;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawThickCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness){
	(void)gc;
	(void)x;
	(void)y;
	(void)diameter;
	(void)thickness;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawThickEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness){
	(void)gc;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	(void)thickness;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawThickCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness){
	(void)gc;
	(void)x;
	(void)y;
	(void)diameter;
	(void)startAngle;
	(void)arcAngle;
	(void)thickness;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawFlippedImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	(void)gc;
	(void)img;
	(void)regionX;
	(void)regionY;
	(void)width;
	(void)height;
	(void)x;
	(void)y;
	(void)transformation;
	(void)alpha;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	(void)gc;
	(void)img;
	(void)x;
	(void)y;
	(void)rotationX;
	(void)rotationY;
	(void)angle;
	(void)alpha;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawRotatedImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	(void)gc;
	(void)img;
	(void)x;
	(void)y;
	(void)rotationX;
	(void)rotationY;
	(void)angle;
	(void)alpha;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawScaledImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	(void)gc;
	(void)img;
	(void)x;
	(void)y;
	(void)factorX;
	(void)factorY;
	(void)alpha;
	return not_implemented(gc);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_STUB_drawScaledImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	(void)gc;
	(void)img;
	(void)x;
	(void)y;
	(void)factorX;
	(void)factorY;
	(void)alpha;
	return not_implemented(gc);
}

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------
