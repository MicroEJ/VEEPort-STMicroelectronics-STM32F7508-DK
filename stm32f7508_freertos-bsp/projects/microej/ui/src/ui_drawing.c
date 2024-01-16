/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/*
 * @file
 * @brief Redirection of all MicroUI and Drawing libraries drawing functions. This file
 * is required (must be compiled in the C project) but should not be modified (except when
 * the VEE port uses more than 3 destination formats, see below).
 *
 * According to the available number of destination formats, this file redirects all drawings
 * to the Graphics Engine software algorithms or uses some tables to target the right drawer
 * according to the destination format.
 *
 * When only one destination format is available and no hardware acceleration (GPU) is
 * available, the C compiler must just include this file. The default weak implementations
 * call the Graphics Engine sofware algorithms.
 *
 * When only one destination format is available and a hardware acceleration (GPU) is
 * available, a custom implementation over the GPU must override a set of these default
 * weak implementations (function UI_DRAWING_xxx()). This allows to override only a set
 * of functions which are GPU compatible.
 *
 * When several destination formats are available, this file uses some tables to target
 * the right drawer. Each drawer (that targets a specific destination format) has to
 * specify the table index and has to override a maximum of drawing functions. When a
 * function is not overridden for a given destination format, the default weak implementation
 * is used. This implementation uses the stub implementation.
 *
 * The BSP should specify the define "LLUI_GC_SUPPORTED_FORMATS". When not set or smaller than
 * "2", this file considers only one destination format is available: the same format than
 * the buffer of the display.
 *
 * When this define is "2" or "3", this file uses the tables indirections. The format "0" is
 * reserved for the display buffer format (and for the GraphicsContext that uses the same
 * format). The formats "1" and "2" are specific to the VEE Port.
 *
 * This file does not manages more than 3 destination formats ("0" == display buffer format
 * and custom formats "1" and "2"). However the file can be easily modified to target more
 * than 3 destination formats. See format "2" support and apply same patterns to add the
 * format "3".
 *
 * @author MicroEJ Developer Team
 * @version 3.1.0
 * @see ui_drawing.h
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <LLUI_DISPLAY.h>

#include "ui_drawing.h"
#include "ui_drawing_stub.h"
#include "ui_drawing_soft.h"
#include "dw_drawing_soft.h"
#include "ui_image_drawing.h"
#include "bsp_util.h"

// --------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------

#if !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The functions UI_DRAWING_DEFAULT_xxx() are directly called by LLUI_DISPLAY_impl.c,
 * LLUI_PAINTER_impl.c and LLDW_PAINTER_impl.c. Other files can override each weak
 * function independently to use a GPU.
 */

#define UI_DRAWING_DEFAULT_writePixel UI_DRAWING_writePixel
#define UI_DRAWING_DEFAULT_drawLine UI_DRAWING_drawLine
#define UI_DRAWING_DEFAULT_drawHorizontalLine UI_DRAWING_drawHorizontalLine
#define UI_DRAWING_DEFAULT_drawVerticalLine UI_DRAWING_drawVerticalLine
#define UI_DRAWING_DEFAULT_drawRectangle UI_DRAWING_drawRectangle
#define UI_DRAWING_DEFAULT_fillRectangle UI_DRAWING_fillRectangle
#define UI_DRAWING_DEFAULT_drawRoundedRectangle UI_DRAWING_drawRoundedRectangle
#define UI_DRAWING_DEFAULT_fillRoundedRectangle UI_DRAWING_fillRoundedRectangle
#define UI_DRAWING_DEFAULT_drawCircleArc UI_DRAWING_drawCircleArc
#define UI_DRAWING_DEFAULT_drawEllipseArc UI_DRAWING_drawEllipseArc
#define UI_DRAWING_DEFAULT_fillCircleArc UI_DRAWING_fillCircleArc
#define UI_DRAWING_DEFAULT_fillEllipseArc UI_DRAWING_fillEllipseArc
#define UI_DRAWING_DEFAULT_drawEllipse UI_DRAWING_drawEllipse
#define UI_DRAWING_DEFAULT_fillEllipse UI_DRAWING_fillEllipse
#define UI_DRAWING_DEFAULT_drawCircle UI_DRAWING_drawCircle
#define UI_DRAWING_DEFAULT_fillCircle UI_DRAWING_fillCircle
#define UI_DRAWING_DEFAULT_drawImage UI_DRAWING_drawImage
#define UI_DRAWING_DEFAULT_copyImage UI_DRAWING_copyImage
#define UI_DRAWING_DEFAULT_drawRegion UI_DRAWING_drawRegion

#define UI_DRAWING_DEFAULT_drawThickFadedPoint UI_DRAWING_drawThickFadedPoint
#define UI_DRAWING_DEFAULT_drawThickFadedLine UI_DRAWING_drawThickFadedLine
#define UI_DRAWING_DEFAULT_drawThickFadedCircle UI_DRAWING_drawThickFadedCircle
#define UI_DRAWING_DEFAULT_drawThickFadedCircleArc UI_DRAWING_drawThickFadedCircleArc
#define UI_DRAWING_DEFAULT_drawThickFadedEllipse UI_DRAWING_drawThickFadedEllipse
#define UI_DRAWING_DEFAULT_drawThickLine UI_DRAWING_drawThickLine
#define UI_DRAWING_DEFAULT_drawThickCircle UI_DRAWING_drawThickCircle
#define UI_DRAWING_DEFAULT_drawThickEllipse UI_DRAWING_drawThickEllipse
#define UI_DRAWING_DEFAULT_drawThickCircleArc UI_DRAWING_drawThickCircleArc
#define UI_DRAWING_DEFAULT_drawFlippedImage UI_DRAWING_drawFlippedImage
#define UI_DRAWING_DEFAULT_drawRotatedImageNearestNeighbor UI_DRAWING_drawRotatedImageNearestNeighbor
#define UI_DRAWING_DEFAULT_drawRotatedImageBilinear UI_DRAWING_drawRotatedImageBilinear
#define UI_DRAWING_DEFAULT_drawScaledImageNearestNeighbor UI_DRAWING_drawScaledImageNearestNeighbor
#define UI_DRAWING_DEFAULT_drawScaledImageBilinear UI_DRAWING_drawScaledImageBilinear

#else // !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The functions UI_DRAWING_DEFAULT_xxx() are indirectly called through some tables.
 * The functions have got the identifier "0" as suffix. Another file can override
 * each weak function independently to use a GPU.
 */

#define UI_DRAWING_DEFAULT_writePixel UI_DRAWING_writePixel_0
#define UI_DRAWING_DEFAULT_drawLine UI_DRAWING_drawLine_0
#define UI_DRAWING_DEFAULT_drawHorizontalLine UI_DRAWING_drawHorizontalLine_0
#define UI_DRAWING_DEFAULT_drawVerticalLine UI_DRAWING_drawVerticalLine_0
#define UI_DRAWING_DEFAULT_drawRectangle UI_DRAWING_drawRectangle_0
#define UI_DRAWING_DEFAULT_fillRectangle UI_DRAWING_fillRectangle_0
#define UI_DRAWING_DEFAULT_drawRoundedRectangle UI_DRAWING_drawRoundedRectangle_0
#define UI_DRAWING_DEFAULT_fillRoundedRectangle UI_DRAWING_fillRoundedRectangle_0
#define UI_DRAWING_DEFAULT_drawCircleArc UI_DRAWING_drawCircleArc_0
#define UI_DRAWING_DEFAULT_drawEllipseArc UI_DRAWING_drawEllipseArc_0
#define UI_DRAWING_DEFAULT_fillCircleArc UI_DRAWING_fillCircleArc_0
#define UI_DRAWING_DEFAULT_fillEllipseArc UI_DRAWING_fillEllipseArc_0
#define UI_DRAWING_DEFAULT_drawEllipse UI_DRAWING_drawEllipse_0
#define UI_DRAWING_DEFAULT_fillEllipse UI_DRAWING_fillEllipse_0
#define UI_DRAWING_DEFAULT_drawCircle UI_DRAWING_drawCircle_0
#define UI_DRAWING_DEFAULT_fillCircle UI_DRAWING_fillCircle_0
#define UI_DRAWING_DEFAULT_drawImage UI_DRAWING_drawImage_0
#define UI_DRAWING_DEFAULT_copyImage UI_DRAWING_copyImage_0
#define UI_DRAWING_DEFAULT_drawRegion UI_DRAWING_drawRegion_0

#define UI_DRAWING_DEFAULT_drawThickFadedPoint UI_DRAWING_drawThickFadedPoint_0
#define UI_DRAWING_DEFAULT_drawThickFadedLine UI_DRAWING_drawThickFadedLine_0
#define UI_DRAWING_DEFAULT_drawThickFadedCircle UI_DRAWING_drawThickFadedCircle_0
#define UI_DRAWING_DEFAULT_drawThickFadedCircleArc UI_DRAWING_drawThickFadedCircleArc_0
#define UI_DRAWING_DEFAULT_drawThickFadedEllipse UI_DRAWING_drawThickFadedEllipse_0
#define UI_DRAWING_DEFAULT_drawThickLine UI_DRAWING_drawThickLine_0
#define UI_DRAWING_DEFAULT_drawThickCircle UI_DRAWING_drawThickCircle_0
#define UI_DRAWING_DEFAULT_drawThickEllipse UI_DRAWING_drawThickEllipse_0
#define UI_DRAWING_DEFAULT_drawThickCircleArc UI_DRAWING_drawThickCircleArc_0
#define UI_DRAWING_DEFAULT_drawFlippedImage UI_DRAWING_drawFlippedImage_0
#define UI_DRAWING_DEFAULT_drawRotatedImageNearestNeighbor UI_DRAWING_drawRotatedImageNearestNeighbor_0
#define UI_DRAWING_DEFAULT_drawRotatedImageBilinear UI_DRAWING_drawRotatedImageBilinear_0
#define UI_DRAWING_DEFAULT_drawScaledImageNearestNeighbor UI_DRAWING_drawScaledImageNearestNeighbor_0
#define UI_DRAWING_DEFAULT_drawScaledImageBilinear UI_DRAWING_drawScaledImageBilinear_0

#endif // !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

// --------------------------------------------------------------------------------
// Extern functions
// --------------------------------------------------------------------------------

#if defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

/*
 * @brief Set of drawing functions according to the index of the destination format in
 * the drawing tables ("0", "1" or "2").
 *
 * These functions must be declared in other H files.
 */

#if (LLUI_GC_SUPPORTED_FORMATS > 3)
#error "Increase the number of following functions and update the next tables"
#endif

extern DRAWING_Status UI_DRAWING_writePixel_0(MICROUI_GraphicsContext* gc, jint x, jint y);
extern DRAWING_Status UI_DRAWING_drawLine_0(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY);
extern DRAWING_Status UI_DRAWING_drawHorizontalLine_0(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y);
extern DRAWING_Status UI_DRAWING_drawVerticalLine_0(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2);
extern DRAWING_Status UI_DRAWING_drawRectangle_0(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);
extern DRAWING_Status UI_DRAWING_fillRectangle_0(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);
extern DRAWING_Status UI_DRAWING_drawRoundedRectangle_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);
extern DRAWING_Status UI_DRAWING_fillRoundedRectangle_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);
extern DRAWING_Status UI_DRAWING_drawCircleArc_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_drawEllipseArc_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_fillCircleArc_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_fillEllipseArc_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_drawEllipse_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);
extern DRAWING_Status UI_DRAWING_fillEllipse_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);
extern DRAWING_Status UI_DRAWING_drawCircle_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);
extern DRAWING_Status UI_DRAWING_fillCircle_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);
extern DRAWING_Status UI_DRAWING_drawImage_0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_DRAWING_copyImage_0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_DRAWING_drawRegion_0(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_DRAWING_drawThickFadedPoint_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade);
extern DRAWING_Status UI_DRAWING_drawThickFadedLine_0(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap);
extern DRAWING_Status UI_DRAWING_drawThickFadedCircle_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade);
extern DRAWING_Status UI_DRAWING_drawThickFadedCircleArc_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end);
extern DRAWING_Status UI_DRAWING_drawThickFadedEllipse_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade);
extern DRAWING_Status UI_DRAWING_drawThickLine_0(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness);
extern DRAWING_Status UI_DRAWING_drawThickCircle_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness);
extern DRAWING_Status UI_DRAWING_drawThickEllipse_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness);
extern DRAWING_Status UI_DRAWING_drawThickCircleArc_0(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness);
extern DRAWING_Status UI_DRAWING_drawFlippedImage_0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_DRAWING_drawRotatedImageNearestNeighbor_0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_DRAWING_drawRotatedImageBilinear_0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_DRAWING_drawScaledImageNearestNeighbor_0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_DRAWING_drawScaledImageBilinear_0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

extern uint32_t UI_DRAWING_getNewImageStrideInBytes_1(jbyte image_format, uint32_t width, uint32_t height, uint32_t default_stride);
extern void UI_DRAWING_adjustNewImageCharacteristics_1(jbyte image_format, uint32_t width, uint32_t height, uint32_t* data_size, uint32_t* data_alignment);
extern void UI_DRAWING_initializeNewImage_1(MICROUI_Image* image);
extern void UI_DRAWING_freeImageResources_1(MICROUI_Image* image);
extern DRAWING_Status UI_DRAWING_writePixel_1(MICROUI_GraphicsContext* gc, jint x, jint y);
extern DRAWING_Status UI_DRAWING_drawLine_1(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY);
extern DRAWING_Status UI_DRAWING_drawHorizontalLine_1(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y);
extern DRAWING_Status UI_DRAWING_drawVerticalLine_1(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2);
extern DRAWING_Status UI_DRAWING_drawRectangle_1(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);
extern DRAWING_Status UI_DRAWING_fillRectangle_1(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);
extern DRAWING_Status UI_DRAWING_drawRoundedRectangle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);
extern DRAWING_Status UI_DRAWING_fillRoundedRectangle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);
extern DRAWING_Status UI_DRAWING_drawCircleArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_drawEllipseArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_fillCircleArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_fillEllipseArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_drawEllipse_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);
extern DRAWING_Status UI_DRAWING_fillEllipse_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);
extern DRAWING_Status UI_DRAWING_drawCircle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);
extern DRAWING_Status UI_DRAWING_fillCircle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);
extern DRAWING_Status UI_DRAWING_drawImage_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_DRAWING_copyImage_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_DRAWING_drawRegion_1(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_DRAWING_drawThickFadedPoint_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade);
extern DRAWING_Status UI_DRAWING_drawThickFadedLine_1(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap);
extern DRAWING_Status UI_DRAWING_drawThickFadedCircle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade);
extern DRAWING_Status UI_DRAWING_drawThickFadedCircleArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end);
extern DRAWING_Status UI_DRAWING_drawThickFadedEllipse_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade);
extern DRAWING_Status UI_DRAWING_drawThickLine_1(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness);
extern DRAWING_Status UI_DRAWING_drawThickCircle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness);
extern DRAWING_Status UI_DRAWING_drawThickEllipse_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness);
extern DRAWING_Status UI_DRAWING_drawThickCircleArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness);
extern DRAWING_Status UI_DRAWING_drawFlippedImage_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_DRAWING_drawRotatedImageNearestNeighbor_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_DRAWING_drawRotatedImageBilinear_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_DRAWING_drawScaledImageNearestNeighbor_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_DRAWING_drawScaledImageBilinear_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

#if (LLUI_GC_SUPPORTED_FORMATS > 2)
extern uint32_t UI_DRAWING_getNewImageStrideInBytes_2(jbyte image_format, uint32_t width, uint32_t height, uint32_t default_stride);
extern void UI_DRAWING_adjustNewImageCharacteristics_2(jbyte image_format, uint32_t width, uint32_t height, uint32_t* data_size, uint32_t* data_alignment);
extern void UI_DRAWING_initializeNewImage_2(MICROUI_Image* image);
extern void UI_DRAWING_freeImageResources_2(MICROUI_Image* image);
extern DRAWING_Status UI_DRAWING_writePixel_2(MICROUI_GraphicsContext* gc, jint x, jint y);
extern DRAWING_Status UI_DRAWING_drawLine_2(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY);
extern DRAWING_Status UI_DRAWING_drawHorizontalLine_2(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y);
extern DRAWING_Status UI_DRAWING_drawVerticalLine_2(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2);
extern DRAWING_Status UI_DRAWING_drawRectangle_2(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);
extern DRAWING_Status UI_DRAWING_fillRectangle_2(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);
extern DRAWING_Status UI_DRAWING_drawRoundedRectangle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);
extern DRAWING_Status UI_DRAWING_fillRoundedRectangle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);
extern DRAWING_Status UI_DRAWING_drawCircleArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_drawEllipseArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_fillCircleArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_fillEllipseArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);
extern DRAWING_Status UI_DRAWING_drawEllipse_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);
extern DRAWING_Status UI_DRAWING_fillEllipse_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);
extern DRAWING_Status UI_DRAWING_drawCircle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);
extern DRAWING_Status UI_DRAWING_fillCircle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);
extern DRAWING_Status UI_DRAWING_drawImage_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_DRAWING_copyImage_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_DRAWING_drawRegion_2(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_DRAWING_drawThickFadedPoint_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade);
extern DRAWING_Status UI_DRAWING_drawThickFadedLine_2(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap);
extern DRAWING_Status UI_DRAWING_drawThickFadedCircle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade);
extern DRAWING_Status UI_DRAWING_drawThickFadedCircleArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end);
extern DRAWING_Status UI_DRAWING_drawThickFadedEllipse_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade);
extern DRAWING_Status UI_DRAWING_drawThickLine_2(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness);
extern DRAWING_Status UI_DRAWING_drawThickCircle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness);
extern DRAWING_Status UI_DRAWING_drawThickEllipse_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness);
extern DRAWING_Status UI_DRAWING_drawThickCircleArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness);
extern DRAWING_Status UI_DRAWING_drawFlippedImage_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_DRAWING_drawRotatedImageNearestNeighbor_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_DRAWING_drawRotatedImageBilinear_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_DRAWING_drawScaledImageNearestNeighbor_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_DRAWING_drawScaledImageBilinear_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
#endif // (LLUI_GC_SUPPORTED_FORMATS > 2)

#endif // defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

// --------------------------------------------------------------------------------
// Typedef of drawing functions
// --------------------------------------------------------------------------------

#if defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

/*
 * @brief Typedef used by next tables. See the function comments in ui_drawing.h
 */

typedef uint32_t (* UI_DRAWING_getNewImageStrideInBytes_t) (jbyte image_format, uint32_t width, uint32_t height, uint32_t default_stride);
typedef void (* UI_DRAWING_adjustNewImageCharacteristics_t) (jbyte image_format, uint32_t width, uint32_t height, uint32_t* data_size, uint32_t* data_alignment);
typedef void (* UI_DRAWING_initializeNewImage_t) (MICROUI_Image* image);
typedef void (* UI_DRAWING_freeImageResources_t) (MICROUI_Image* image);
typedef DRAWING_Status (* UI_DRAWING_writePixel_t) (MICROUI_GraphicsContext* gc, jint x, jint y);
typedef DRAWING_Status (* UI_DRAWING_drawLine_t) (MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY);
typedef DRAWING_Status (* UI_DRAWING_drawHorizontalLine_t) (MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y);
typedef DRAWING_Status (* UI_DRAWING_drawVerticalLine_t) (MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2);
typedef DRAWING_Status (* UI_DRAWING_drawRectangle_t) (MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);
typedef DRAWING_Status (* UI_DRAWING_fillRectangle_t) (MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);
typedef DRAWING_Status (* UI_DRAWING_drawRoundedRectangle_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);
typedef DRAWING_Status (* UI_DRAWING_fillRoundedRectangle_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);
typedef DRAWING_Status (* UI_DRAWING_drawCircleArc_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);
typedef DRAWING_Status (* UI_DRAWING_drawEllipseArc_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);
typedef DRAWING_Status (* UI_DRAWING_fillCircleArc_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);
typedef DRAWING_Status (* UI_DRAWING_fillEllipseArc_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);
typedef DRAWING_Status (* UI_DRAWING_drawEllipse_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);
typedef DRAWING_Status (* UI_DRAWING_fillEllipse_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);
typedef DRAWING_Status (* UI_DRAWING_drawCircle_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);
typedef DRAWING_Status (* UI_DRAWING_fillCircle_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);
typedef DRAWING_Status (* UI_DRAWING_drawImage_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
typedef DRAWING_Status (* UI_DRAWING_copyImage_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
typedef DRAWING_Status (* UI_DRAWING_drawRegion_t) (MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
typedef DRAWING_Status (* UI_DRAWING_drawThickFadedPoint_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade);
typedef DRAWING_Status (* UI_DRAWING_drawThickFadedLine_t) (MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap);
typedef DRAWING_Status (* UI_DRAWING_drawThickFadedCircle_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade);
typedef DRAWING_Status (* UI_DRAWING_drawThickFadedCircleArc_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end);
typedef DRAWING_Status (* UI_DRAWING_drawThickFadedEllipse_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade);
typedef DRAWING_Status (* UI_DRAWING_drawThickLine_t) (MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness);
typedef DRAWING_Status (* UI_DRAWING_drawThickCircle_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness);
typedef DRAWING_Status (* UI_DRAWING_drawThickEllipse_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness);
typedef DRAWING_Status (* UI_DRAWING_drawThickCircleArc_t) (MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness);
typedef DRAWING_Status (* UI_DRAWING_drawFlippedImage_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
typedef DRAWING_Status (* UI_DRAWING_drawRotatedImageNearestNeighbor_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
typedef DRAWING_Status (* UI_DRAWING_drawRotatedImageBilinear_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
typedef DRAWING_Status (* UI_DRAWING_drawScaledImageNearestNeighbor_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
typedef DRAWING_Status (* UI_DRAWING_drawScaledImageBilinear_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

#endif // #if defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

// --------------------------------------------------------------------------------
// Tables according to the destination format.
// --------------------------------------------------------------------------------

#if defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

static const UI_DRAWING_getNewImageStrideInBytes_t UI_DRAWER_getNewImageStrideInBytes[] = {
	&UI_DRAWING_getNewImageStrideInBytes,
	&UI_DRAWING_getNewImageStrideInBytes_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_getNewImageStrideInBytes_2,
#endif
};

static const UI_DRAWING_adjustNewImageCharacteristics_t UI_DRAWER_adjustNewImageCharacteristics[] = {
	&UI_DRAWING_adjustNewImageCharacteristics,
	&UI_DRAWING_adjustNewImageCharacteristics_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_adjustNewImageCharacteristics_2,
#endif
};

static const UI_DRAWING_initializeNewImage_t UI_DRAWER_initializeNewImage[] = {
	&UI_DRAWING_initializeNewImage,
	&UI_DRAWING_initializeNewImage_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_initializeNewImage_2,
#endif
};

static const UI_DRAWING_freeImageResources_t UI_DRAWER_freeImageResources[] = {
	&UI_DRAWING_freeImageResources,
	&UI_DRAWING_freeImageResources_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_freeImageResources_2,
#endif
};

static const UI_DRAWING_writePixel_t UI_DRAWER_writePixel[] = {
	&UI_DRAWING_writePixel_0,
	&UI_DRAWING_writePixel_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_writePixel_2,
#endif
};

static const UI_DRAWING_drawLine_t UI_DRAWER_drawLine[] = {
	&UI_DRAWING_drawLine_0,
	&UI_DRAWING_drawLine_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawLine_2,
#endif
};

static const UI_DRAWING_drawHorizontalLine_t UI_DRAWER_drawHorizontalLine[] = {
	&UI_DRAWING_drawHorizontalLine_0,
	&UI_DRAWING_drawHorizontalLine_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawHorizontalLine_2,
#endif
};

static const UI_DRAWING_drawVerticalLine_t UI_DRAWER_drawVerticalLine[] = {
	&UI_DRAWING_drawVerticalLine_0,
	&UI_DRAWING_drawVerticalLine_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawVerticalLine_2,
#endif
};

static const UI_DRAWING_drawRectangle_t UI_DRAWER_drawRectangle[] = {
	&UI_DRAWING_drawRectangle_0,
	&UI_DRAWING_drawRectangle_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawRectangle_2,
#endif
};

static const UI_DRAWING_fillRectangle_t UI_DRAWER_fillRectangle[] = {
	&UI_DRAWING_fillRectangle_0,
	&UI_DRAWING_fillRectangle_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_fillRectangle_2,
#endif
};

static const UI_DRAWING_drawRoundedRectangle_t UI_DRAWER_drawRoundedRectangle[] = {
	&UI_DRAWING_drawRoundedRectangle_0,
	&UI_DRAWING_drawRoundedRectangle_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawRoundedRectangle_2,
#endif
};

static const UI_DRAWING_fillRoundedRectangle_t UI_DRAWER_fillRoundedRectangle[] = {
	&UI_DRAWING_fillRoundedRectangle_0,
	&UI_DRAWING_fillRoundedRectangle_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_fillRoundedRectangle_2,
#endif
};

static const UI_DRAWING_drawCircleArc_t UI_DRAWER_drawCircleArc[] = {
	&UI_DRAWING_drawCircleArc_0,
	&UI_DRAWING_drawCircleArc_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawCircleArc_2,
#endif
};

static const UI_DRAWING_drawEllipseArc_t UI_DRAWER_drawEllipseArc[] = {
	&UI_DRAWING_drawEllipseArc_0,
	&UI_DRAWING_drawEllipseArc_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawEllipseArc_2,
#endif
};

static const UI_DRAWING_fillCircleArc_t UI_DRAWER_fillCircleArc[] = {
	&UI_DRAWING_fillCircleArc_0,
	&UI_DRAWING_fillCircleArc_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_fillCircleArc_2,
#endif
};

static const UI_DRAWING_fillEllipseArc_t UI_DRAWER_fillEllipseArc[] = {
	&UI_DRAWING_fillEllipseArc_0,
	&UI_DRAWING_fillEllipseArc_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_fillEllipseArc_2,
#endif
};

static const UI_DRAWING_drawEllipse_t UI_DRAWER_drawEllipse[] = {
	&UI_DRAWING_drawEllipse_0,
	&UI_DRAWING_drawEllipse_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawEllipse_2,
#endif
};

static const UI_DRAWING_fillEllipse_t UI_DRAWER_fillEllipse[] = {
	&UI_DRAWING_fillEllipse_0,
	&UI_DRAWING_fillEllipse_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_fillEllipse_2,
#endif
};

static const UI_DRAWING_drawCircle_t UI_DRAWER_drawCircle[] = {
	&UI_DRAWING_drawCircle_0,
	&UI_DRAWING_drawCircle_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawCircle_2,
#endif
};

static const UI_DRAWING_fillCircle_t UI_DRAWER_fillCircle[] = {
	&UI_DRAWING_fillCircle_0,
	&UI_DRAWING_fillCircle_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_fillCircle_2,
#endif
};

static const UI_DRAWING_drawImage_t UI_DRAWER_drawImage[] = {
	&UI_DRAWING_drawImage_0,
	&UI_DRAWING_drawImage_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawImage_2,
#endif
};

static const UI_DRAWING_copyImage_t UI_DRAWER_copyImage[] = {
	&UI_DRAWING_copyImage_0,
	&UI_DRAWING_copyImage_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_copyImage_2,
#endif
};

static const UI_DRAWING_drawRegion_t UI_DRAWER_drawRegion[] = {
	&UI_DRAWING_drawRegion_0,
	&UI_DRAWING_drawRegion_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawRegion_2,
#endif
};

static const UI_DRAWING_drawThickFadedPoint_t UI_DRAWER_drawThickFadedPoint[] = {
	&UI_DRAWING_drawThickFadedPoint_0,
	&UI_DRAWING_drawThickFadedPoint_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawThickFadedPoint_2,
#endif
};

static const UI_DRAWING_drawThickFadedLine_t UI_DRAWER_drawThickFadedLine[] = {
	&UI_DRAWING_drawThickFadedLine_0,
	&UI_DRAWING_drawThickFadedLine_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawThickFadedLine_2,
#endif
};

static const UI_DRAWING_drawThickFadedCircle_t UI_DRAWER_drawThickFadedCircle[] = {
	&UI_DRAWING_drawThickFadedCircle_0,
	&UI_DRAWING_drawThickFadedCircle_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawThickFadedCircle_2,
#endif
};

static const UI_DRAWING_drawThickFadedCircleArc_t UI_DRAWER_drawThickFadedCircleArc[] = {
	&UI_DRAWING_drawThickFadedCircleArc_0,
	&UI_DRAWING_drawThickFadedCircleArc_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawThickFadedCircleArc_2,
#endif
};

static const UI_DRAWING_drawThickFadedEllipse_t UI_DRAWER_drawThickFadedEllipse[] = {
	&UI_DRAWING_drawThickFadedEllipse_0,
	&UI_DRAWING_drawThickFadedEllipse_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawThickFadedEllipse_2,
#endif
};

static const UI_DRAWING_drawThickLine_t UI_DRAWER_drawThickLine[] = {
	&UI_DRAWING_drawThickLine_0,
	&UI_DRAWING_drawThickLine_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawThickLine_2,
#endif
};

static const UI_DRAWING_drawThickCircle_t UI_DRAWER_drawThickCircle[] = {
	&UI_DRAWING_drawThickCircle_0,
	&UI_DRAWING_drawThickCircle_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawThickCircle_2,
#endif
};

static const UI_DRAWING_drawThickEllipse_t UI_DRAWER_drawThickEllipse[] = {
	&UI_DRAWING_drawThickEllipse_0,
	&UI_DRAWING_drawThickEllipse_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawThickEllipse_2,
#endif
};

static const UI_DRAWING_drawThickCircleArc_t UI_DRAWER_drawThickCircleArc[] = {
	&UI_DRAWING_drawThickCircleArc_0,
	&UI_DRAWING_drawThickCircleArc_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawThickCircleArc_2,
#endif
};

static const UI_DRAWING_drawFlippedImage_t UI_DRAWER_drawFlippedImage[] = {
	&UI_DRAWING_drawFlippedImage_0,
	&UI_DRAWING_drawFlippedImage_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawFlippedImage_2,
#endif
};

static const UI_DRAWING_drawRotatedImageNearestNeighbor_t UI_DRAWER_drawRotatedImageNearestNeighbor[] = {
	&UI_DRAWING_drawRotatedImageNearestNeighbor_0,
	&UI_DRAWING_drawRotatedImageNearestNeighbor_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawRotatedImageNearestNeighbor_2,
#endif
};

static const UI_DRAWING_drawRotatedImageBilinear_t UI_DRAWER_drawRotatedImageBilinear[] = {
	&UI_DRAWING_drawRotatedImageBilinear_0,
	&UI_DRAWING_drawRotatedImageBilinear_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawRotatedImageBilinear_2,
#endif
};

static const UI_DRAWING_drawScaledImageNearestNeighbor_t UI_DRAWER_drawScaledImageNearestNeighbor[] = {
	&UI_DRAWING_drawScaledImageNearestNeighbor_0,
	&UI_DRAWING_drawScaledImageNearestNeighbor_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawScaledImageNearestNeighbor_2,
#endif
};

static const UI_DRAWING_drawScaledImageBilinear_t UI_DRAWER_drawScaledImageBilinear[] = {
	&UI_DRAWING_drawScaledImageBilinear_0,
	&UI_DRAWING_drawScaledImageBilinear_1,
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	&UI_DRAWING_drawScaledImageBilinear_2,
#endif
};

#endif // defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

// --------------------------------------------------------------------------------
// LLUI_DISPLAY_impl.h functions that depend on image format
// (the functions are redirected to ui_drawing.h)
// --------------------------------------------------------------------------------

#if !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The VEE port supports only one destination format: the displat buffer format. The
 * application mutable images have the same format as the display buffer.
 */

// See the header file for the function documentation
int32_t LLUI_DISPLAY_IMPL_getDrawerIdentifier(jbyte image_format) {
	return LLUI_DISPLAY_isDisplayFormat(image_format) ? 0 /* no error */ : -1 /* means invalid */;
}

// See the header file for the function documentation
uint32_t LLUI_DISPLAY_IMPL_getNewImageStrideInBytes(jbyte image_format, uint32_t width, uint32_t height, uint32_t default_stride) {
	// just make an indirection (useful for multi destination formats)
	return UI_DRAWING_getNewImageStrideInBytes(image_format, width, height, default_stride);
}

// See the header file for the function documentation
void LLUI_DISPLAY_IMPL_adjustNewImageCharacteristics(jbyte image_format, uint32_t width, uint32_t height, uint32_t* data_size, uint32_t* data_alignment) {
	// just make an indirection (useful for multi destination formats)
	UI_DRAWING_adjustNewImageCharacteristics(image_format, width, height, data_size, data_alignment);
}

// See the header file for the function documentation
void LLUI_DISPLAY_IMPL_initializeNewImage(MICROUI_Image* image) {
	// just make an indirection (useful for multi destination formats)
	UI_DRAWING_initializeNewImage(image);
}

// See the header file for the function documentation
void LLUI_DISPLAY_IMPL_freeImageResources(MICROUI_Image* image) {
	// just make an indirection (useful for multi destination formats)
	UI_DRAWING_freeImageResources(image);
}

#else // #if !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The VEE port supports several destination formats. All drawing functions use a
 * dedicated table to redirect to the right implementation. The VEE Port must implement
 * the functions UI_DRAWING_is_drawer_X() to identify the right drawer according to
 * the destination format.
 *
 * The "DEFAULT" functions (see below) are used as element "0" of the tables (this is the
 * display buffer format).
 */

int32_t LLUI_DISPLAY_IMPL_getDrawerIdentifier(jbyte image_format) {

	int32_t index;

	if (LLUI_DISPLAY_isDisplayFormat(image_format)) {
		index = 0;
	} else if (UI_DRAWING_is_drawer_1(image_format)) {
		index = 1;
#if (LLUI_GC_SUPPORTED_FORMATS > 2)
	} else if (UI_DRAWING_is_drawer_2(image_format)) {
		index = 2;
#endif
	}
	else {
		// unknown format
		index = -1;
	}

	return index;
}

/*
 * @brief See the header file for the function documentation
 *
 * Implementation details: The new image to create is an immutable image or a mutable image.
 *
 * -	If it is a mutable image, that means a drawer is able to draw into it (see
 * 		LLUI_DISPLAY_IMPL_getDrawerIdentifier()). In this case, this function redirects the
 * 		implementation to the drawer (that should be able to adjust image characteristics if
 * 		required).
 *
 * - 	If it is an immutable image (PNG decoder, automatic image format convert, etc.), that
 * 		means (most of the time) no drawer can manage this image format. In such case, this function
 * 		redirects the implementation to the default implementation (the indirection table
 * 		UI_DRAWER_getNewImageStrideInBytes points to the default function at index 0).
 *
 * -	In the case a drawer is available for this new immutable image, this function redirects
 * 		to the drawer.
 */
uint32_t LLUI_DISPLAY_IMPL_getNewImageStrideInBytes(jbyte image_format, uint32_t width, uint32_t height, uint32_t default_stride) {
	int32_t drawer = LLUI_DISPLAY_IMPL_getDrawerIdentifier(image_format);
	drawer = (drawer >= 0) ? drawer : 0;
	return (*UI_DRAWER_getNewImageStrideInBytes[drawer])(image_format, width, height, default_stride);
}

// See the header file foar the function documentation and implementation of
// LLUI_DISPLAY_IMPL_getNewImageStrideInBytes
void LLUI_DISPLAY_IMPL_adjustNewImageCharacteristics(jbyte image_format, uint32_t width, uint32_t height, uint32_t* data_size, uint32_t* data_alignment) {
	int32_t drawer = LLUI_DISPLAY_IMPL_getDrawerIdentifier(image_format);
	drawer = (drawer >= 0) ? drawer : 0;
	(*UI_DRAWER_adjustNewImageCharacteristics[drawer])(image_format, width, height, data_size, data_alignment);
}

// See the header file for the function documentation and implementation of
// LLUI_DISPLAY_IMPL_getNewImageStrideInBytes
void LLUI_DISPLAY_IMPL_initializeNewImage(MICROUI_Image* image) {
	int32_t drawer = LLUI_DISPLAY_IMPL_getDrawerIdentifier(image->format);
	drawer = (drawer >= 0) ? drawer : 0;
	(*UI_DRAWER_initializeNewImage[drawer])(image);
}

// See the header file for the function documentation and implementation of
// LLUI_DISPLAY_IMPL_getNewImageStrideInBytes
void LLUI_DISPLAY_IMPL_freeImageResources(MICROUI_Image* image) {
	int32_t drawer = LLUI_DISPLAY_IMPL_getDrawerIdentifier(image->format);
	drawer = (drawer >= 0) ? drawer : 0;
	(*UI_DRAWER_freeImageResources[drawer])(image);
}

#endif	 // #if !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

// --------------------------------------------------------------------------------
// ui_drawing.h functions
// (the function names don't differ regardless of the available number of destination formats)
// --------------------------------------------------------------------------------

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT uint32_t UI_DRAWING_getNewImageStrideInBytes(jbyte image_format, uint32_t width, uint32_t height, uint32_t default_stride) {
	(void)image_format;
	(void)width;
	(void)height;
	// no specific stride by default
	return default_stride;
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_adjustNewImageCharacteristics(jbyte image_format, uint32_t width, uint32_t height, uint32_t* data_size, uint32_t* data_alignment){
	(void)image_format;
	(void)width;
	(void)height;
	(void)data_size;
	(void)data_alignment;
	// nothing to adjust by default
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_initializeNewImage(MICROUI_Image* image){
	(void)image;
	// nothing to initialize by default
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_freeImageResources(MICROUI_Image* image){
	(void)image;
	// nothing to initialize by default
}

// --------------------------------------------------------------------------------
// ui_drawing.h functions
// (the function names differ according to the available number of destination formats)
// --------------------------------------------------------------------------------

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_writePixel(MICROUI_GraphicsContext* gc, jint x, jint y){
	return UI_DRAWING_SOFT_writePixel(gc, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY){
	return UI_DRAWING_SOFT_drawLine(gc, startX, startY, endX, endY);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawHorizontalLine(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y){
	return UI_DRAWING_SOFT_drawHorizontalLine(gc, x1, x2, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawVerticalLine(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2){
	return UI_DRAWING_SOFT_drawVerticalLine(gc, x, y1, y2);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	return UI_DRAWING_SOFT_drawRectangle(gc, x1, y1, x2, y2);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_fillRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	return UI_DRAWING_SOFT_fillRectangle(gc, x1, y1, x2, y2);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	return UI_DRAWING_SOFT_drawRoundedRectangle(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_fillRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	return UI_DRAWING_SOFT_fillRoundedRectangle(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_SOFT_drawCircleArc(gc, x, y, diameter, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_SOFT_drawEllipseArc(gc, x, y, width, height, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_fillCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_SOFT_fillCircleArc(gc, x, y, diameter, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_fillEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_SOFT_fillEllipseArc(gc, x, y, width, height, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	return UI_DRAWING_SOFT_drawEllipse(gc, x, y, width, height);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_fillEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	return UI_DRAWING_SOFT_fillEllipse(gc, x, y, width, height);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	return UI_DRAWING_SOFT_drawCircle(gc, x, y, diameter);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_fillCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	return UI_DRAWING_SOFT_fillCircle(gc, x, y, diameter);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
#if !defined(LLUI_IMAGE_CUSTOM_FORMATS)
	return UI_DRAWING_SOFT_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
#else
	return UI_IMAGE_DRAWING_draw(gc, img, regionX, regionY, width, height, x, y, alpha);
#endif
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_copyImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
#if !defined(LLUI_IMAGE_CUSTOM_FORMATS)
	return UI_DRAWING_SOFT_copyImage(gc, img, regionX, regionY, width, height, x, y);
#else
	return UI_IMAGE_DRAWING_copy(gc, img, regionX, regionY, width, height, x, y);
#endif
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawRegion(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
#if !defined(LLUI_IMAGE_CUSTOM_FORMATS)
	return UI_DRAWING_SOFT_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
#else
	return UI_IMAGE_DRAWING_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
#endif
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawThickFadedPoint(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade){
	return DW_DRAWING_SOFT_drawThickFadedPoint(gc, x, y, thickness, fade);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawThickFadedLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap){
	return DW_DRAWING_SOFT_drawThickFadedLine(gc, startX, startY, endX, endY, thickness, fade, startCap, endCap);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawThickFadedCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade){
	return DW_DRAWING_SOFT_drawThickFadedCircle(gc, x, y, diameter, thickness, fade);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawThickFadedCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end){
	return DW_DRAWING_SOFT_drawThickFadedCircleArc(gc, x, y, diameter, startAngle, arcAngle, thickness, fade, start, end);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawThickFadedEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade){
	return DW_DRAWING_SOFT_drawThickFadedEllipse(gc, x, y, width, height, thickness, fade);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawThickLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness){
	return DW_DRAWING_SOFT_drawThickLine(gc, startX, startY, endX, endY, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawThickCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness){
	return DW_DRAWING_SOFT_drawThickCircle(gc, x, y, diameter, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawThickEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness){
	return DW_DRAWING_SOFT_drawThickEllipse(gc, x, y, width, height, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawThickCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness){
	return DW_DRAWING_SOFT_drawThickCircleArc(gc, x, y, diameter, startAngle, arcAngle, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawFlippedImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
#if !defined(LLUI_IMAGE_CUSTOM_FORMATS)
	return DW_DRAWING_SOFT_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
#else
	return UI_IMAGE_DRAWING_drawFlipped(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
#endif
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawRotatedImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
#if !defined(LLUI_IMAGE_CUSTOM_FORMATS)
	return DW_DRAWING_SOFT_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
#else
	return UI_IMAGE_DRAWING_drawRotatedNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
#endif
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawRotatedImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
#if !defined(LLUI_IMAGE_CUSTOM_FORMATS)
	return DW_DRAWING_SOFT_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
#else
	return UI_IMAGE_DRAWING_drawRotatedBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
#endif
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawScaledImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
#if !defined(LLUI_IMAGE_CUSTOM_FORMATS)
	return DW_DRAWING_SOFT_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
#else
	return UI_IMAGE_DRAWING_drawScaledNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
#endif
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_DEFAULT_drawScaledImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
#if !defined(LLUI_IMAGE_CUSTOM_FORMATS)
	return DW_DRAWING_SOFT_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
#else
	return UI_IMAGE_DRAWING_drawScaledBilinear(gc, img, x, y, factorX, factorY, alpha);
#endif
}

#if defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

/*
 * The VEE port supports several destination formats. All drawing functions use a
 * dedicated table to redirect to the right implementation. The VEE Port must implement
 * the functions UI_DRAWING_is_drawer_X() to identify the right drawer according to
 * the destination format.
 *
 * The "DEFAULT" functions (see above) are used as element "0" of the tables (== display
 * buffer format).
 */

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_writePixel(MICROUI_GraphicsContext* gc, jint x, jint y){
	return (*UI_DRAWER_writePixel[gc->drawer])(gc, x, y);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY){
	return (*UI_DRAWER_drawLine[gc->drawer])(gc, startX, startY, endX, endY);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawHorizontalLine(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y){
	return (*UI_DRAWER_drawHorizontalLine[gc->drawer])(gc, x1, x2, y);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawVerticalLine(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2){
	return (*UI_DRAWER_drawVerticalLine[gc->drawer])(gc, x, y1, y2);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	return (*UI_DRAWER_drawRectangle[gc->drawer])(gc, x1, y1, x2, y2);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_fillRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	return (*UI_DRAWER_fillRectangle[gc->drawer])(gc, x1, y1, x2, y2);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	return (*UI_DRAWER_drawRoundedRectangle[gc->drawer])(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_fillRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	return (*UI_DRAWER_fillRoundedRectangle[gc->drawer])(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	return (*UI_DRAWER_drawCircleArc[gc->drawer])(gc, x, y, diameter, startAngle, arcAngle);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	return (*UI_DRAWER_drawEllipseArc[gc->drawer])(gc, x, y, width, height, startAngle, arcAngle);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_fillCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	return (*UI_DRAWER_fillCircleArc[gc->drawer])(gc, x, y, diameter, startAngle, arcAngle);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_fillEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	return (*UI_DRAWER_fillEllipseArc[gc->drawer])(gc, x, y, width, height, startAngle, arcAngle);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	return (*UI_DRAWER_drawEllipse[gc->drawer])(gc, x, y, width, height);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_fillEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	return (*UI_DRAWER_fillEllipse[gc->drawer])(gc, x, y, width, height);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	return (*UI_DRAWER_drawCircle[gc->drawer])(gc, x, y, diameter);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_fillCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	return (*UI_DRAWER_fillCircle[gc->drawer])(gc, x, y, diameter);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return (*UI_DRAWER_drawImage[gc->drawer])(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_copyImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return (*UI_DRAWER_copyImage[gc->drawer])(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawRegion(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return (*UI_DRAWER_drawRegion[gc->drawer])(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawThickFadedPoint(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade){
	return (*UI_DRAWER_drawThickFadedPoint[gc->drawer])(gc, x, y, thickness, fade);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawThickFadedLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap){
	return (*UI_DRAWER_drawThickFadedLine[gc->drawer])(gc, startX, startY, endX, endY, thickness, fade, startCap, endCap);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawThickFadedCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade){
	return (*UI_DRAWER_drawThickFadedCircle[gc->drawer])(gc, x, y, diameter, thickness, fade);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawThickFadedCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end){
	return (*UI_DRAWER_drawThickFadedCircleArc[gc->drawer])(gc, x, y, diameter, startAngle, arcAngle, thickness, fade, start, end);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawThickFadedEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade){
	return (*UI_DRAWER_drawThickFadedEllipse[gc->drawer])(gc, x, y, width, height, thickness, fade);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawThickLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness){
	return (*UI_DRAWER_drawThickLine[gc->drawer])(gc, startX, startY, endX, endY, thickness);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawThickCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness){
	return (*UI_DRAWER_drawThickCircle[gc->drawer])(gc, x, y, diameter, thickness);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawThickEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness){
	return (*UI_DRAWER_drawThickEllipse[gc->drawer])(gc, x, y, width, height, thickness);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawThickCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness){
	return (*UI_DRAWER_drawThickCircleArc[gc->drawer])(gc, x, y, diameter, startAngle, arcAngle, thickness);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawFlippedImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return (*UI_DRAWER_drawFlippedImage[gc->drawer])(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawRotatedImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return (*UI_DRAWER_drawRotatedImageNearestNeighbor[gc->drawer])(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawRotatedImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return (*UI_DRAWER_drawRotatedImageBilinear[gc->drawer])(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawScaledImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return (*UI_DRAWER_drawScaledImageNearestNeighbor[gc->drawer])(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_drawScaledImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return (*UI_DRAWER_drawScaledImageBilinear[gc->drawer])(gc, img, x, y, factorX, factorY, alpha);
}

/*
 * The next functions are used as elements "1" of the tables. They call STUB functions and should be
 * overridden by the drawer that manages the format identified by the index "1".
 *
 * Only the functions UI_DRAWING_adjustNewImageCharacteristics_1() and UI_DRAWING_initializeNewImage_1()
 * are mandatory. If the drawing engine (for the format identified by the index "1") does not override these
 * functions, the application will not be able to open an image with the associated format (the default functions
 * are implemented as weak functions in case there is no drawing engine for this format).
 *
 * If no engine supports the format identified by the index "1", the application will not be able to open
 * an image with the associated format.
 */

BSP_DECLARE_WEAK_FCNT bool UI_DRAWING_is_drawer_1(jbyte image_format) {
	(void)image_format;
	// default behavior: format is not supported
	return false;
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT uint32_t UI_DRAWING_getNewImageStrideInBytes_1(jbyte image_format, uint32_t width, uint32_t height, uint32_t default_stride){
    (void)image_format;
    (void)width;
    (void)height;
	// does nothing and will throw an error if the application tries to open this kind of image.
	// see above.
	return default_stride;
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_adjustNewImageCharacteristics_1(jbyte image_format, uint32_t width, uint32_t height, uint32_t* data_size, uint32_t* data_alignment){
	(void)image_format;
	(void)width;
	(void)height;
	(void)data_size;
	(void)data_alignment;
	// does nothing and will throw an error if the application tries to open this kind of image.
	// see above.
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_initializeNewImage_1(MICROUI_Image* image){
	(void)image;
	// nothing to do
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_freeImageResources_1(MICROUI_Image* image){
	(void)image;
	// nothing to do
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_writePixel_1(MICROUI_GraphicsContext* gc, jint x, jint y){
	return UI_DRAWING_STUB_writePixel(gc, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawLine_1(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY){
	return UI_DRAWING_STUB_drawLine(gc, startX, startY, endX, endY);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawHorizontalLine_1(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y){
	return UI_DRAWING_STUB_drawHorizontalLine(gc, x1, x2, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawVerticalLine_1(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2){
	return UI_DRAWING_STUB_drawVerticalLine(gc, x, y1, y2);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRectangle_1(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	return UI_DRAWING_STUB_drawRectangle(gc, x1, y1, x2, y2);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillRectangle_1(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	return UI_DRAWING_STUB_fillRectangle(gc, x1, y1, x2, y2);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRoundedRectangle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	return UI_DRAWING_STUB_drawRoundedRectangle(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillRoundedRectangle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	return UI_DRAWING_STUB_fillRoundedRectangle(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawCircleArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_STUB_drawCircleArc(gc, x, y, diameter, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawEllipseArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_STUB_drawEllipseArc(gc, x, y, width, height, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillCircleArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_STUB_fillCircleArc(gc, x, y, diameter, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillEllipseArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_STUB_fillEllipseArc(gc, x, y, width, height, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawEllipse_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	return UI_DRAWING_STUB_drawEllipse(gc, x, y, width, height);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillEllipse_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	return UI_DRAWING_STUB_fillEllipse(gc, x, y, width, height);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawCircle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	return UI_DRAWING_STUB_drawCircle(gc, x, y, diameter);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillCircle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	return UI_DRAWING_STUB_fillCircle(gc, x, y, diameter);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawImage_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_IMAGE_DRAWING_draw(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_copyImage_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_IMAGE_DRAWING_copy(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRegion_1(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_IMAGE_DRAWING_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedPoint_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade){
	return UI_DRAWING_STUB_drawThickFadedPoint(gc, x, y, thickness, fade);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedLine_1(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap){
	return UI_DRAWING_STUB_drawThickFadedLine(gc, startX, startY, endX, endY, thickness, fade, startCap, endCap);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedCircle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade){
	return UI_DRAWING_STUB_drawThickFadedCircle(gc, x, y, diameter, thickness, fade);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedCircleArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end){
	return UI_DRAWING_STUB_drawThickFadedCircleArc(gc, x, y, diameter, startAngle, arcAngle, thickness, fade, start, end);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedEllipse_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade){
	return UI_DRAWING_STUB_drawThickFadedEllipse(gc, x, y, width, height, thickness, fade);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickLine_1(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness){
	return UI_DRAWING_STUB_drawThickLine(gc, startX, startY, endX, endY, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickCircle_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness){
	return UI_DRAWING_STUB_drawThickCircle(gc, x, y, diameter, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickEllipse_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness){
	return UI_DRAWING_STUB_drawThickEllipse(gc, x, y, width, height, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickCircleArc_1(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness){
	return UI_DRAWING_STUB_drawThickCircleArc(gc, x, y, diameter, startAngle, arcAngle, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawFlippedImage_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_IMAGE_DRAWING_drawFlipped(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRotatedImageNearestNeighbor_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_IMAGE_DRAWING_drawRotatedNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRotatedImageBilinear_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_IMAGE_DRAWING_drawRotatedBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawScaledImageNearestNeighbor_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_IMAGE_DRAWING_drawScaledNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawScaledImageBilinear_1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_IMAGE_DRAWING_drawScaledBilinear(gc, img, x, y, factorX, factorY, alpha);
}

#if (LLUI_GC_SUPPORTED_FORMATS > 2)

/*
 * The next functions are used as elements "2" of the tables. They call STUB functions and should be
 * overridden by the drawer that manages the format identified by the index "2".
 *
 * Only the functions UI_DRAWING_adjustNewImageCharacteristics_2() and UI_DRAWING_initializeNewImage_2()
 * are mandatory. If the drawing engine (for the format identified by the index "2") does not override these
 * functions, the application will not be able to open an image with the associated format (the default functions
 * are implemented as weak functions in case there is no drawing engine for this format).
 *
 * If no engine supports the format identified by the index "2", the application will not be able to open
 * an image with the associated format.
 */

BSP_DECLARE_WEAK_FCNT bool UI_DRAWING_is_drawer_2(jbyte image_format) {
	(void)image_format;
	// default behavior: format is not supported
	return false;
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT uint32_t UI_DRAWING_getNewImageStrideInBytes_2(jbyte image_format, uint32_t width, uint32_t height, uint32_t default_stride){
    (void)image_format;
    (void)width;
    (void)height;
	// does nothing and will throw an error if the application tries to open this kind of image.
	// see above.
	return default_stride;
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_adjustNewImageCharacteristics_2(jbyte image_format, uint32_t width, uint32_t height, uint32_t* data_size, uint32_t* data_alignment){
	(void)image_format;
	(void)width;
	(void)height;
	(void)data_size;
	(void)data_alignment;
	// does nothing and will throw an error if the application tries to open this kind of image.
	// see above.
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_initializeNewImage_2(MICROUI_Image* image){
	(void)image;
	// nothing to do
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_freeImageResources_2(MICROUI_Image* image){
	(void)image;
	// nothing to do
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_writePixel_2(MICROUI_GraphicsContext* gc, jint x, jint y){
	return UI_DRAWING_STUB_writePixel(gc, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawLine_2(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY){
	return UI_DRAWING_STUB_drawLine(gc, startX, startY, endX, endY);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawHorizontalLine_2(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y){
	return UI_DRAWING_STUB_drawHorizontalLine(gc, x1, x2, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawVerticalLine_2(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2){
	return UI_DRAWING_STUB_drawVerticalLine(gc, x, y1, y2);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRectangle_2(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	return UI_DRAWING_STUB_drawRectangle(gc, x1, y1, x2, y2);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillRectangle_2(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2){
	return UI_DRAWING_STUB_fillRectangle(gc, x1, y1, x2, y2);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRoundedRectangle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	return UI_DRAWING_STUB_drawRoundedRectangle(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillRoundedRectangle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight){
	return UI_DRAWING_STUB_fillRoundedRectangle(gc, x, y, width, height, cornerEllipseWidth, cornerEllipseHeight);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawCircleArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_STUB_drawCircleArc(gc, x, y, diameter, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawEllipseArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_STUB_drawEllipseArc(gc, x, y, width, height, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillCircleArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_STUB_fillCircleArc(gc, x, y, diameter, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillEllipseArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle){
	return UI_DRAWING_STUB_fillEllipseArc(gc, x, y, width, height, startAngle, arcAngle);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawEllipse_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	return UI_DRAWING_STUB_drawEllipse(gc, x, y, width, height);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillEllipse_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height){
	return UI_DRAWING_STUB_fillEllipse(gc, x, y, width, height);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawCircle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	return UI_DRAWING_STUB_drawCircle(gc, x, y, diameter);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_fillCircle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter){
	return UI_DRAWING_STUB_fillCircle(gc, x, y, diameter);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawImage_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_IMAGE_DRAWING_draw(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_copyImage_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_IMAGE_DRAWING_copy(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRegion_2(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_IMAGE_DRAWING_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedPoint_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint thickness, jint fade){
	return UI_DRAWING_STUB_drawThickFadedPoint(gc, x, y, thickness, fade);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedLine_2(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap){
	return UI_DRAWING_STUB_drawThickFadedLine(gc, startX, startY, endX, endY, thickness, fade, startCap, endCap);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedCircle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade){
	return UI_DRAWING_STUB_drawThickFadedCircle(gc, x, y, diameter, thickness, fade);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedCircleArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness, jint fade, DRAWING_Cap start, DRAWING_Cap end){
	return UI_DRAWING_STUB_drawThickFadedCircleArc(gc, x, y, diameter, startAngle, arcAngle, thickness, fade, start, end);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickFadedEllipse_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness, jint fade){
	return UI_DRAWING_STUB_drawThickFadedEllipse(gc, x, y, width, height, thickness, fade);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickLine_2(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness){
	return UI_DRAWING_STUB_drawThickLine(gc, startX, startY, endX, endY, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickCircle_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness){
	return UI_DRAWING_STUB_drawThickCircle(gc, x, y, diameter, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickEllipse_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint thickness){
	return UI_DRAWING_STUB_drawThickEllipse(gc, x, y, width, height, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawThickCircleArc_2(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle, jint thickness){
	return UI_DRAWING_STUB_drawThickCircleArc(gc, x, y, diameter, startAngle, arcAngle, thickness);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawFlippedImage_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_IMAGE_DRAWING_drawFlipped(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRotatedImageNearestNeighbor_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_IMAGE_DRAWING_drawRotatedNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawRotatedImageBilinear_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_IMAGE_DRAWING_drawRotatedBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawScaledImageNearestNeighbor_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_IMAGE_DRAWING_drawScaledNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_DRAWING_drawScaledImageBilinear_2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_IMAGE_DRAWING_drawScaledBilinear(gc, img, x, y, factorX, factorY, alpha);
}

#endif // (LLUI_GC_SUPPORTED_FORMATS > 2)

#else // #if defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

/*
 * The VEE port supports only one destination format: the display buffer format. The
 * application can only create immutable images or mutable images with the same format as
 * the display buffer. All drawing functions are redirected to the software implementation
 * by default. A third party implementation (often on a GPU) can replace each weak function
 * independently.
 *
 * The VEE Port can tune the new image characteristics to add a header before the pixel
 * array for instance.
 */

#endif // #if defined(LLUI_GC_SUPPORTED_FORMATS) && (LLUI_GC_SUPPORTED_FORMATS > 1)

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------
