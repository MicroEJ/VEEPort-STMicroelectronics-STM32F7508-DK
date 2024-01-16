/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/*
 * @file
 * @brief Implementation of all drawing functions of ui_image_drawing.h.
 * @author MicroEJ Developer Team
 * @version 3.1.0
 * @see ui_image_drawing.h
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include <LLUI_DISPLAY.h>

#include "ui_image_drawing.h"
#include "ui_drawing_soft.h"
#include "dw_drawing_soft.h"
#include "ui_drawing_stub.h"
#include "bsp_util.h"

#if defined(LLUI_IMAGE_CUSTOM_FORMATS)

/*
 * VEE Port supports several images formats: standard (ARGB8888, A8, etc) and one or
 * several custom formats. The next functions and tables redirect the image drawing
 * to the right image manager (stub, soft or custom).
 *
 * VEE Port may support several destination formats: display format, one or several
 * standard formats and one or several custom formats. See _get_table_index().
 *
 * Note: The functions are called by ui_drawing.c. The use of GPU should be checked
 * in ui_drawing.c and not here.
 */

// --------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------

/*
 * @brief Indices in the tables  UI_IMAGE_DRAWING_xxx_custom[]
 */
#define TABLE_INDEX_STUB 0 // index to call functions stub
#define TABLE_INDEX_SOFT 1 // index to call functions in software
#define TABLE_INDEX_CUSTOM_OFFSET 2 // index to call VEE Port functions

#define GET_CUSTOM_IMAGE_INDEX(img) (((uint8_t)TABLE_INDEX_CUSTOM_OFFSET) + ((uint8_t)MICROUI_IMAGE_FORMAT_CUSTOM_0) - ((uint8_t)((img)->format)))

// --------------------------------------------------------------------------------
// Extern functions
// --------------------------------------------------------------------------------

/*
 * @brief Set of drawing functions according to the source image format.
 *
 * These functions must be declared in other H files.
 */

extern DRAWING_Status UI_IMAGE_DRAWING_draw_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_draw_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_draw_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_draw_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_draw_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_draw_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_draw_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_draw_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

extern DRAWING_Status UI_IMAGE_DRAWING_copy_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_IMAGE_DRAWING_copy_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_IMAGE_DRAWING_copy_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_IMAGE_DRAWING_copy_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_IMAGE_DRAWING_copy_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_IMAGE_DRAWING_copy_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_IMAGE_DRAWING_copy_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
extern DRAWING_Status UI_IMAGE_DRAWING_copy_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);

extern DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom0(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom1(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom2(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom3(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom4(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom5(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom6(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom7(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

extern DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);

extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);

extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
extern DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

// --------------------------------------------------------------------------------
// Typedef of drawing functions
// --------------------------------------------------------------------------------

typedef DRAWING_Status (* UI_IMAGE_DRAWING_draw_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
typedef DRAWING_Status (* UI_IMAGE_DRAWING_copy_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y);
typedef DRAWING_Status (* UI_IMAGE_DRAWING_drawRegion_t) (MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);
typedef DRAWING_Status (* UI_IMAGE_DRAWING_drawFlipped_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha);
typedef DRAWING_Status (* UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
typedef DRAWING_Status (* UI_IMAGE_DRAWING_drawRotatedBilinear_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha);
typedef DRAWING_Status (* UI_IMAGE_DRAWING_drawScaledNearestNeighbor_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);
typedef DRAWING_Status (* UI_IMAGE_DRAWING_drawScaledBilinear_t) (MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha);

// --------------------------------------------------------------------------------
// Tables according to the source image format.
// --------------------------------------------------------------------------------

static const UI_IMAGE_DRAWING_draw_t UI_IMAGE_DRAWING_draw_custom[] = {
		&UI_DRAWING_STUB_drawImage,
		&UI_DRAWING_SOFT_drawImage,
		&UI_IMAGE_DRAWING_draw_custom0,
		&UI_IMAGE_DRAWING_draw_custom1,
		&UI_IMAGE_DRAWING_draw_custom2,
		&UI_IMAGE_DRAWING_draw_custom3,
		&UI_IMAGE_DRAWING_draw_custom4,
		&UI_IMAGE_DRAWING_draw_custom5,
		&UI_IMAGE_DRAWING_draw_custom6,
		&UI_IMAGE_DRAWING_draw_custom7,
};

static const UI_IMAGE_DRAWING_copy_t UI_IMAGE_DRAWING_copy_custom[] = {
		UI_DRAWING_STUB_copyImage,
		UI_DRAWING_SOFT_copyImage,
		&UI_IMAGE_DRAWING_copy_custom0,
		&UI_IMAGE_DRAWING_copy_custom1,
		&UI_IMAGE_DRAWING_copy_custom2,
		&UI_IMAGE_DRAWING_copy_custom3,
		&UI_IMAGE_DRAWING_copy_custom4,
		&UI_IMAGE_DRAWING_copy_custom5,
		&UI_IMAGE_DRAWING_copy_custom6,
		&UI_IMAGE_DRAWING_copy_custom7,
};

static const UI_IMAGE_DRAWING_drawRegion_t UI_IMAGE_DRAWING_drawRegion_custom[] = {
		UI_DRAWING_STUB_drawRegion,
		UI_DRAWING_SOFT_drawRegion,
		&UI_IMAGE_DRAWING_drawRegion_custom0,
		&UI_IMAGE_DRAWING_drawRegion_custom1,
		&UI_IMAGE_DRAWING_drawRegion_custom2,
		&UI_IMAGE_DRAWING_drawRegion_custom3,
		&UI_IMAGE_DRAWING_drawRegion_custom4,
		&UI_IMAGE_DRAWING_drawRegion_custom5,
		&UI_IMAGE_DRAWING_drawRegion_custom6,
		&UI_IMAGE_DRAWING_drawRegion_custom7,
};

static const UI_IMAGE_DRAWING_drawFlipped_t UI_IMAGE_DRAWING_drawFlipped_custom[] = {
		UI_DRAWING_STUB_drawFlippedImage,
		DW_DRAWING_SOFT_drawFlippedImage,
		&UI_IMAGE_DRAWING_drawFlipped_custom0,
		&UI_IMAGE_DRAWING_drawFlipped_custom1,
		&UI_IMAGE_DRAWING_drawFlipped_custom2,
		&UI_IMAGE_DRAWING_drawFlipped_custom3,
		&UI_IMAGE_DRAWING_drawFlipped_custom4,
		&UI_IMAGE_DRAWING_drawFlipped_custom5,
		&UI_IMAGE_DRAWING_drawFlipped_custom6,
		&UI_IMAGE_DRAWING_drawFlipped_custom7,
};

static const UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_t UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom[] = {
		UI_DRAWING_STUB_drawRotatedImageNearestNeighbor,
		DW_DRAWING_SOFT_drawRotatedImageNearestNeighbor,
		&UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom0,
		&UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom1,
		&UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom2,
		&UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom3,
		&UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom4,
		&UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom5,
		&UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom6,
		&UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom7,
};

static const UI_IMAGE_DRAWING_drawRotatedBilinear_t UI_IMAGE_DRAWING_drawRotatedBilinear_custom[] = {
		UI_DRAWING_STUB_drawRotatedImageBilinear,
		DW_DRAWING_SOFT_drawRotatedImageBilinear,
		&UI_IMAGE_DRAWING_drawRotatedBilinear_custom0,
		&UI_IMAGE_DRAWING_drawRotatedBilinear_custom1,
		&UI_IMAGE_DRAWING_drawRotatedBilinear_custom2,
		&UI_IMAGE_DRAWING_drawRotatedBilinear_custom3,
		&UI_IMAGE_DRAWING_drawRotatedBilinear_custom4,
		&UI_IMAGE_DRAWING_drawRotatedBilinear_custom5,
		&UI_IMAGE_DRAWING_drawRotatedBilinear_custom6,
		&UI_IMAGE_DRAWING_drawRotatedBilinear_custom7,
};

static const UI_IMAGE_DRAWING_drawScaledNearestNeighbor_t UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom[] = {
		UI_DRAWING_STUB_drawScaledImageNearestNeighbor,
		DW_DRAWING_SOFT_drawScaledImageNearestNeighbor,
		&UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom0,
		&UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom1,
		&UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom2,
		&UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom3,
		&UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom4,
		&UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom5,
		&UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom6,
		&UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom7,
};

static const UI_IMAGE_DRAWING_drawScaledBilinear_t UI_IMAGE_DRAWING_drawScaledBilinear_custom[] = {
		UI_DRAWING_STUB_drawScaledImageBilinear,
		DW_DRAWING_SOFT_drawScaledImageBilinear,
		&UI_IMAGE_DRAWING_drawScaledBilinear_custom0,
		&UI_IMAGE_DRAWING_drawScaledBilinear_custom1,
		&UI_IMAGE_DRAWING_drawScaledBilinear_custom2,
		&UI_IMAGE_DRAWING_drawScaledBilinear_custom3,
		&UI_IMAGE_DRAWING_drawScaledBilinear_custom4,
		&UI_IMAGE_DRAWING_drawScaledBilinear_custom5,
		&UI_IMAGE_DRAWING_drawScaledBilinear_custom6,
		&UI_IMAGE_DRAWING_drawScaledBilinear_custom7,
};

// --------------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------------

static inline uint32_t _get_table_index(MICROUI_GraphicsContext* gc, MICROUI_Image* img) {
	uint32_t index;
	if (!LLUI_DISPLAY_isCustomFormat(img->format)) {
#if !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)
		(void)gc;
		// standard image in display GC -> can use soft algo
		index = TABLE_INDEX_SOFT;
#else
		index = LLUI_DISPLAY_isDisplayFormat(gc->image.format) ? TABLE_INDEX_SOFT : TABLE_INDEX_STUB;
#endif
	}
	else {
		// use the specific image manager to draw the custom image
		// (this manager must check the destination format)
		// cppcheck-suppress [misra-c2012-10.6] convert image format to an index
		index = GET_CUSTOM_IMAGE_INDEX(img);
	}
	return index;
}

// --------------------------------------------------------------------------------
// ui_image_drawing.h functions
// --------------------------------------------------------------------------------

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_draw(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return (*UI_IMAGE_DRAWING_draw_custom[_get_table_index(gc, img)])(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_copy(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return (*UI_IMAGE_DRAWING_copy_custom[_get_table_index(gc, img)])(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawRegion(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return (*UI_IMAGE_DRAWING_drawRegion_custom[_get_table_index(gc, &gc->image)])(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawFlipped(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return (*UI_IMAGE_DRAWING_drawFlipped_custom[_get_table_index(gc, img)])(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return (*UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom[_get_table_index(gc, img)])(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return (*UI_IMAGE_DRAWING_drawRotatedBilinear_custom[_get_table_index(gc, img)])(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return (*UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom[_get_table_index(gc, img)])(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return (*UI_IMAGE_DRAWING_drawScaledBilinear_custom[_get_table_index(gc, img)])(gc, img, x, y, factorX, factorY, alpha);
}

// --------------------------------------------------------------------------------
// Table weak functions
// --------------------------------------------------------------------------------

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_draw_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_draw_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_draw_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_draw_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_draw_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_draw_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_draw_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_draw_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_copy_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_DRAWING_STUB_copyImage(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_copy_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_DRAWING_STUB_copyImage(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_copy_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_DRAWING_STUB_copyImage(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_copy_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_DRAWING_STUB_copyImage(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_copy_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_DRAWING_STUB_copyImage(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_copy_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_DRAWING_STUB_copyImage(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_copy_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_DRAWING_STUB_copyImage(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_copy_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return UI_DRAWING_STUB_copyImage(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom0(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom1(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom2(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom3(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom4(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom5(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom6(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRegion_custom7(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return UI_DRAWING_STUB_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_DRAWING_STUB_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_DRAWING_STUB_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_DRAWING_STUB_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_DRAWING_STUB_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_DRAWING_STUB_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_DRAWING_STUB_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_DRAWING_STUB_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawFlipped_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return UI_DRAWING_STUB_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return UI_DRAWING_STUB_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom0(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom1(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom2(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom3(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom4(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom5(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom6(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear_custom7(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return UI_DRAWING_STUB_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
}

#else // #if defined(LLUI_IMAGE_CUSTOM_FORMATS)

/*
 * VEE Port supports only standard images formats: standard (ARGB8888, A8, etc). The
 * next functions redirect the image drawing to the software algorithms.
 *
 * VEE Port may support several destination formats: display format and one or several
 * standard formats.
 *
 * Note: The functions are called by ui_drawing.c. The use of GPU should be checked
 * in ui_drawing.c and not here.
 */

// --------------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------------

static inline bool _can_call_soft_algo(MICROUI_GraphicsContext* gc) {
#if !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)
	(void)gc;
	// standard image in display GC -> can use soft algo
	return true;
#else
	return LLUI_DISPLAY_isDisplayFormat(gc->image.format);
#endif
}

// --------------------------------------------------------------------------------
// ui_image_drawing.h functions
// --------------------------------------------------------------------------------

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_draw(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return _can_call_soft_algo(gc) ?
			UI_DRAWING_SOFT_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha)
			: UI_DRAWING_STUB_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_copy(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y){
	return _can_call_soft_algo(gc) ?
			UI_DRAWING_SOFT_copyImage(gc, img, regionX, regionY, width, height, x, y)
			: UI_DRAWING_STUB_copyImage(gc, img, regionX, regionY, width, height, x, y);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawRegion(MICROUI_GraphicsContext* gc, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha){
	return _can_call_soft_algo(gc) ?
			UI_DRAWING_SOFT_drawRegion(gc, regionX, regionY, width, height, x, y, alpha)
			: UI_DRAWING_STUB_drawRegion(gc, regionX, regionY, width, height, x, y, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawFlipped(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, DRAWING_Flip transformation, jint alpha){
	return _can_call_soft_algo(gc) ?
			DW_DRAWING_SOFT_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha)
			: UI_DRAWING_STUB_drawFlippedImage(gc, img, regionX, regionY, width, height, x, y, transformation, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawRotatedNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return _can_call_soft_algo(gc) ?
			DW_DRAWING_SOFT_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha)
			: UI_DRAWING_STUB_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawRotatedBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha){
	return _can_call_soft_algo(gc) ?
			DW_DRAWING_SOFT_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha)
			: UI_DRAWING_STUB_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawScaledNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return _can_call_soft_algo(gc) ?
			DW_DRAWING_SOFT_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha)
			: UI_DRAWING_STUB_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
}

// See the header file for the function documentation
DRAWING_Status UI_IMAGE_DRAWING_drawScaledBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha){
	return _can_call_soft_algo(gc) ?
			DW_DRAWING_SOFT_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha)
			: UI_DRAWING_STUB_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
}

#endif // #if defined(LLUI_IMAGE_CUSTOM_FORMATS)

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------
