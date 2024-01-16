/* 
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#if !defined UI_IMAGE_DRAWING_H
#define UI_IMAGE_DRAWING_H
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Provides image drawing functions called by ui_drawing.c.
 *
 * The default weak functions of ui_drawing.c call the ui_image_drawing.h functions to draw
 * the image. According to the kind of image (standard or custom), the implementation
 * of ui_image_drawing.h calls the right image manager functions.
 *
 * When the image is a standard image, the implementation calls the Graphics Engine
 * software algorithms.
 *
 * When the image is a custom image, the associated image manager is mandatory (the
 * Graphics Engine is not able to decode this custom image). The implementation
 * dispatches the image drawing to this image manager (the custom image manager is
 * retrieved thanks to the custom image format: MICROUI_IMAGE_FORMAT_CUSTOM_0 to
 * MICROUI_IMAGE_FORMAT_CUSTOM_7).
 *
 * Several cases:
 *
 * 1- The image manager is available and it is able to draw in the given Graphics
 * Context: it has to decode the image and draw into the Graphics Context.
 *
 * 2- The image manager is available and the given Graphics Context format is the
 * same format as the image: the image should be copied in the destination.
 *
 * 3- The image manager is available and it is not able to draw in the given Graphics
 * Context (unknown ouput format) but it is able to draw the image into the Graphics
 * Context by using the ui_drawing.h functions.
 *
 * 4- The image manager is available, it is not able to draw in the given Graphics
 * Context (unknown ouput format) and it is not able to call standard ui_drawing.h
 * functions to draw the image: it has to call the stub implementation.
 *
 * @author MicroEJ Developer Team
 * @version 3.1.0
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include "ui_drawing.h"

// --------------------------------------------------------------------------------
// API
// --------------------------------------------------------------------------------

/*
 * @brief Draws a region of an image.
 * @see UI_DRAWING_drawImage
 */
DRAWING_Status UI_IMAGE_DRAWING_draw(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

/*
 * @brief Copy a region of an image in another image with the same pixel format.
 * @see UI_DRAWING_copyImage
 */
DRAWING_Status UI_IMAGE_DRAWING_copy(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);

/*
 * @brief Draw a region of an image in the same image.
 * @see UI_DRAWING_drawRegion
 */
DRAWING_Status UI_IMAGE_DRAWING_drawRegion(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

/*
 * @brief Draws an image applying a flip.
 * @see UI_DRAWING_drawFlippedImage
 */
DRAWING_Status UI_IMAGE_DRAWING_drawFlipped(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);

/*
 * @brief Draws an image applying a free rotation (0 to 360 degrees).
 * @see UI_DRAWING_drawRotatedImageNearestNeighbor
 */
DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

/*
 * @brief Draws an image applying a free rotation (0 to 360 degrees).
 * @see UI_DRAWING_drawRotatedImageBilinear
 */
DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

/*
 * @brief Draws an image applying a scaling.
 * @see UI_DRAWING_drawScaledImageNearestNeighbor
 */
DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

/*
 * @brief Draws an image applying a scaling.
 * @see UI_DRAWING_drawScaledImageBilinear
 */
DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif // UI_IMAGE_DRAWING_H
