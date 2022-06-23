/* 
 * Copyright 2020-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLUI_DISPLAY
#define _LLUI_DISPLAY
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Provides some functions to use the graphics engine from the adaptation
 * layer (for the implementation of LLUI_DISPLAY_impl.h, LLUI_PAINTER_impl.h,
 * and LLDW_PAINTER_impl.h).
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

/*
 * @brief Requires ui_drawing.h typedefs.
 */
#include "ui_drawing.h"

// --------------------------------------------------------------------------------
// Typedefs and Structures
// --------------------------------------------------------------------------------

/*
 * @brief LLUI_DISPLAY error and status codes.
 *
 * These values are used by several functions. See each function comment to know
 * which are the available codes.
 */
typedef enum
{
	/*
	 * @brief Value when everything has been correctly executed.
	 */
	LLUI_DISPLAY_OK = 0,

	/*
	 * @brief Value when a function cannot be executed due to an out of memory error.
	 */
	LLUI_DISPLAY_OUT_OF_MEMORY = -2,

	/*
	 * @brief Value when an error has occurred or when a function is not able to
	 * implement the expected behavior.
	 */
	LLUI_DISPLAY_NOK = -9

} LLUI_DISPLAY_Status;

// --------------------------------------------------------------------------------
// Functions provided by the graphics engine
// --------------------------------------------------------------------------------

/*
 * @brief Returns true when the given MicroUI Image specified targets the LCD.
 *
 * @param[in] image the MicroUI Image to check.
 *
 * @return true when the image targets the LCD.
 */
bool LLUI_DISPLAY_isLCD(MICROUI_Image* image);

/*
 * @brief Returns true when the given MicroUI Image has been closed by the MicroEJ
 * application.
 *
 * @param[in] image the MicroUI Image.
 *
 * @return true when the image has been closed.
 */
bool LLUI_DISPLAY_isClosed(MICROUI_Image* image);

/*
 * @brief Adds a log when a drawing function starts.
 *
 * @param[in] log the log identifier.
 */
void LLUI_DISPLAY_logDrawingStart(uint32_t log);

/*
 * @brief Adds a log when a drawing function ends.
 *
 * @param[in] log the log identifier.
 */
void LLUI_DISPLAY_logDrawingEnd(uint32_t log);

/*
 * @brief Enables or disables the graphics context clip. Useful to ignore clip during
 * a drawing. The clip is automatically re-enabled when calling LLUI_DISPLAY_requestDrawing().
 *
 * @param[in] gc the MicroUI GraphicsContext where ignoring the clip.
 * @param[in] enable false to disable the clip.
 */
void LLUI_DISPLAY_configureClip(MICROUI_GraphicsContext* gc, bool enable);

/*
 * @brief Tells if the clip is enabled or not. This call should be performed at the
 * beginning of a drawing function in order to prevent to make some useless operations
 * when the clip is disabled.
 *
 * When it is disabled, that means the caller considers the drawing is fitting the
 * clip. In this case checking the clip bounds is useless.
 *
 * @param[in] gc the MicroUI GraphicsContext where checking the clip.
 *
 * @return true when the clip is enabled (the clip must be checked).
 */
bool LLUI_DISPLAY_isClipEnabled(MICROUI_GraphicsContext* gc);

/*
 * @brief Tells if given pixel fits the clip or not.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the pixel X coordinate.
 * @param[in] y the pixel Y coordinate.
 *
 * @return false when the point is outside the clip.
 */
bool LLUI_DISPLAY_isPixelInClip(MICROUI_GraphicsContext* gc, jint x, jint y);

/*
 * @brief Tells if given horizontal line fully fits the clip or not.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the first pixel line X coordinate.
 * @param[in] x2 the last pixel line X coordinate.
 * @param[in] y the both pixels line Y coordinate.
 *
 * @return false when the line is fully or partially outside the clip, true when
 * line fully fits the clip (clip can be disabled).
 */
bool LLUI_DISPLAY_isHorizontalLineInClip(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y);

/*
 * @brief Tells if given vertical line fully fits the clip or not.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the both pixels line X coordinate.
 * @param[in] y1 the first pixel line Y coordinate.
 * @param[in] y2 the last pixel line Y coordinate.
 *
 * @return false when the line is fully or partially outside the clip, true when
 * line fully fits the clip (clip can be disabled).
 */
bool LLUI_DISPLAY_isVerticalLineInClip(MICROUI_GraphicsContext* gc, jint y1, jint y2, jint x);

/*
 * @brief Tells if given rectangle fully fits the clip or not.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the top-left pixel X coordinate.
 * @param[in] y1 the top-left pixel Y coordinate.
 * @param[in] x2 the bottom-right pixel X coordinate.
 * @param[in] y2 the top-right pixel Y coordinate.
 *
 * @return false when the rectangle is fully or partially outside the clip, true when
 * rectangle fully fits the clip (clip can be disabled).
 */
bool LLUI_DISPLAY_isRectangleInClip(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2);

/*
 * @brief Tells if given region (from x,y to x+width-1,y+height-1) fully fits the clip
 * or not.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the region width.
 * @param[in] height the region height.
 *
 * @return false when the region is fully or partially outside the clip, true when
 * region fully fits the clip (clip can be disabled).
 */
bool LLUI_DISPLAY_isRegionInClip(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

/*
 * @brief Tells if given horizontal line fits the clip or not. If at least one pixel
 * fits the clip, the horizontal line size is cropped. The line may be partially
 * cropped even if the line is fully outside the clip. The line is not cropped when
 * the line fully fits the clip.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in/out] x1 pointer on the first pixel line X coordinate.
 * @param[in/out] x2 pointer on the last pixel line X coordinate.
 * @param[in] y the both pixels line Y coordinate.
 *
 * @return false when the line is fully outside the clip, true when at least
 * one pixel fits the clip.
 */
bool LLUI_DISPLAY_clipHorizontalLine(MICROUI_GraphicsContext* gc, jint* x1, jint* x2, jint y);

/*
 * @brief Tells if given vertical line fits the clip or not. If at least one pixel
 * fits the clip, the vertical line size is cropped. The line may be partially
 * cropped even if the line is fully outside the clip. The line is not cropped when
 * the line fully fits the clip.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the both pixels line X coordinate.
 * @param[in/out] y1 pointer on the first pixel line Y coordinate.
 * @param[in/out] y2 pointer on the last pixel line Y coordinate.
 *
 * @return false when the line is fully outside the clip, true when at least
 * one pixel fits the clip.
 */
bool LLUI_DISPLAY_clipVerticalLine(MICROUI_GraphicsContext* gc, jint* y1, jint* y2, jint x);

/*
 * @brief Tells if given rectangle fits the clip or not. If at least one pixel
 * fits the clip, the rectangle size is cropped. The rectangle may be partially
 * cropped even if the rectangle is fully outside the clip. The rectangle is not
 * cropped when the rectangle fully fits the clip.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in/out] x1 pointer on the top-left pixel X coordinate.
 * @param[in/out] y1 pointer on the top-left pixel Y coordinate.
 * @param[in/out] x2 pointer on the bottom-right pixel X coordinate.
 * @param[in/out] y2 pointer on the top-right pixel Y coordinate.
 *
 * @return false when the rectangle is fully outside the clip, true when at least
 * one pixel fits the clip.
 */
bool LLUI_DISPLAY_clipRectangle(MICROUI_GraphicsContext* gc, jint* x1, jint* y1, jint* x2, jint* y2);

/*
 * @brief Tells if given region (from x,y to x+width-1,y+height-1) fits the clip
 * or not. If at least one pixel fits the clip, the region size is cropped and the
 * destination point is increased. The region may be partially cropped even if the
 * region is fully outside the clip. The region is not cropped when the region fully
 * fits the clip.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in/out] x pointer on the top-left pixel X coordinate.
 * @param[in/out] y pointer on the top-left pixel Y coordinate.
 * @param[in/out] width pointer on the region width.
 * @param[in/out] height pointer on the region height.
 * @param[in/out] destX pointer on the top-left destination pixel X coordinate.
 * @param[in/out] destY pointer on the top-left destination pixel Y coordinate.
 *
 * @return false when the region is fully outside the clip, true when at least
 * one pixel fits the clip.
 */
bool LLUI_DISPLAY_clipRegion(MICROUI_GraphicsContext* gc, jint* x, jint* y, jint* width, jint* height, jint* destX, jint* destY);

/*
 * @brief Tells if source and destination share a region.
 *
 * Source and destination can share a region if and only if source and destination target the
 * same MicroUI Image. In that case, this function checks if there is an intersection of
 * source and destination regions. This function is useful when drawing an image on same image.
 *
 * @param[in] gc the MicroUI GraphicsContext target of destination.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] regionX the x coordinate of the upper-left corner of the region to check.
 * @param[in] regionY the x coordinate of the upper-left corner of the region to check.
 * @param[in] width the width of the region to check.
 * @param[in] height the height of the region to check.
 * @param[in] destX the x coordinate of the top-left point in the destination.
 * @param[in] destY the y coordinate of the top-left point in the destination.
 *
 * @return true when source and destination are same image and when destination region intersects source region.
 */
bool LLUI_DISPLAY_regionsOverlap(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint destX, jint destY);

/*
 * @brief Tells if the ellipsis is enabled or not. Returns 0 when ellipsis is disabled, a
 * positive value otherwise. This value is the maximum string width in pixels. If string width
 * is higher than this value, an ellipsis will be drawn to crop the string.
 *
 * @param[in] gc the MicroUI GraphicsContext where reading the ellipsis width.
 *
 * @return the ellipsis width or 0.
 */
uint32_t LLUI_DISPLAY_getEllipsisWidth(MICROUI_GraphicsContext* gc);

/*
 * @brief Requests a call to LLUI_DISPLAY_IMPL_flush(). The call of LLUI_DISPLAY_IMPL_flush()
 * is synchronized with the MicroEJ application drawing (see MicroUI event pump).
 *
 * @param[in] force true to force a call to LLUI_DISPLAY_IMPL_flush() even if the application
 * dirty area is null (the flush seems actually useless); false to not call LLUI_DISPLAY_IMPL_flush()
 * if the the application dirty area is null.
 *
 * @return true when request has been added, false when queue is full.
 */
bool LLUI_DISPLAY_requestFlush(bool force);

/*
 * @brief Requests a call to Displayable.render(). The call of Displayable.render()
 * is synchronized with the MicroEJ application drawing (see MicroUI event pump).
 *
 * @return true when request has been added, false when queue is full.
 */
bool LLUI_DISPLAY_requestRender(void);

/*
 * @brief Returns the buffer address of the given MicroUI Image. This buffer can
 * be located in runtime memory (RAM, SRAM, SDRAM etc.) or in read-only memory
 * (internal flash, NOR etc.).
 *
 * @param[in] image the MicroUI Image.
 *
 * @return the MicroUI Image pixels buffer address.
 */
uint8_t* LLUI_DISPLAY_getBufferAddress(MICROUI_Image* image);

/*
 * @brief Returns the MicroUI Image row stride in bytes. This value may be higher
 * than the formula "width * bpp / 8" when some padding bytes are available before
 * and/or after the row.
 *
 * @param[in] image the MicroUI Image.
 *
 * @return the MicroUI Image row stride in bytes.
 */
uint32_t LLUI_DISPLAY_getStrideInBytes(MICROUI_Image* image);

/*
 * @brief Returns the MicroUI Image row stride in pixels. This value may be higher
 * than "image width" when some padding pixels are available before and/or after
 * the row.
 *
 * @param[in] image the MicroUI Image.
 *
 * @return the MicroUI Image row stride in pixels.
 */
uint32_t LLUI_DISPLAY_getStrideInPixels(MICROUI_Image* image);

/*
 * @brief Returns the MicroUI Image LUT size when the image format is MICROUI_IMAGE_FORMAT_LARGB8888
 * or MICROUI_IMAGE_FORMAT_LRGB888. This LUT is located at the beginning of the
 * address returned by  LLUI_DISPLAY_getBufferAddress(). The indexed pixels are located
 * just after this LUT.
 *
 * @param[in] image the MicroUI Image.
 *
 * @return the MicroUI Image LUT size in bytes or 0 when the image format is not
 * MICROUI_IMAGE_FORMAT_LARGB8888 or MICROUI_IMAGE_FORMAT_LRGB888.
 */
uint32_t LLUI_DISPLAY_getLUTSize(MICROUI_Image* image);

/*
 * @brief Tells if the MicroUI Image is not fully opaque. An image may be fully
 * opaque even if its format contains an alpha level. This notion is useful to prevent
 * to use some blending algorithms when the image is fully opaque.
 *
 * @param[in] image the MicroUI Image.
 *
 * @return true when the image is not fully opaque.
 */
bool LLUI_DISPLAY_isTransparent(MICROUI_Image* image);

/*
 * @brief Updates the flush dirty area. This dirty area will be merged with the
 * current dirty area and given as parameter in LLUI_DISPLAY_IMPL_flush() function.
 *
 * This function is useless when the graphics context where the drawing has been performed
 * is not the display (in this case this call has no effect). Caller can check that calling
 * LLUI_DISPLAY_isLCD() before.
 *
 * This function can be called by the drawing native implementation or by the GPU callback
 * function. The graphics engine automatically checks if the current drawing concerns
 * the display or not.
 *
 * When this function is called by the drawing native implementation, this function must
 * be called before LLUI_DISPLAY_setDrawingStatus().
 *
 * When this function is called by the GPU callback function, this function must be
 * called before LLUI_DISPLAY_notifyAsynchronousDrawingEnd().
 *
 * @param[in] xmin the dirty area top-left X coordinate.
 * @param[in] ymin the dirty area top-left Y coordinate.
 * @param[in] xmax the dirty area bottom-right X coordinate.
 * @param[in] ymax the dirty area bottom-right Y coordinate.
 *
 * @return true when dirty area has been updated or false when the specified dirty
 * area is out of current clip or when the given graphics context does not target
 * the display.
 */
bool LLUI_DISPLAY_setDrawingLimits(jint xmin, jint ymin, jint xmax, jint ymax);

/*
 * @brief Converts the 32-bit ARGB color format (A-R-G-B) into the display color
 * format.
 *
 * Note: the alpha level may be ignored if the display pixel representation does
 * not hold the alpha level information.
 *
 * @param[in] color the color to convert.
 *
 * @return the converted color.
 */
uint32_t LLUI_DISPLAY_convertARGBColorToDisplayColor(uint32_t color);

/*
 * @brief Converts the display color format into a 32-bit ARGB color format (A-R-G-B).
 *
 * Note: the alpha level may be ignored if the display pixel representation does
 * not hold the alpha level information. In this case, the returned alpha level is
 * 0xff (full opaque).
 *
 * @param[in] color the color to convert.
 *
 * @return the converted color.
 */
uint32_t LLUI_DISPLAY_convertDisplayColorToARGBColor(uint32_t color);

/*
 * Returns the 32-bit ARGB color format (A-R-G-B) of a pixel of the image.
 *
 * @param[in] image the MicroUI Image.
 * @param[in] x the x coordinate of the pixel.
 * @param[in] y the y coordinate of the pixel.
 *
 * @return an ARGB8888 color or 0 if the pixel is out-of-bounds.
 */
uint32_t LLUI_DISPLAY_readPixel(MICROUI_Image* img, int32_t x, int32_t y);

/*
 * @brief Blends two colors applying a global alpha factor.
 *
 * @param[in] foreground the ARGB8888 foreground color.
 * @param[in] background the ARGB8888 background color.
 * @param[in] alpha the global alpha factor.
 *
 * @return an ARGB8888 color.
 */
uint32_t LLUI_DISPLAY_blend(uint32_t foreground, uint32_t background, uint32_t alpha);

/*
 * @brief Allocates a memory area in the images heap.
 *
 * On success, caller has to use the functions LLUI_DISPLAY_getBufferAddress() and
 * LLUI_DISPLAY_getStride*() to retrieve image buffer characteristics.
 *
 * @param[in] image the MicroUI Image.
 * @param[in] rowAlignmentInBytes @deprecated and not used: allocator will call
 * LLUI_DISPLAY_IMPL_getNewImageStrideInBytes() function instead.
 *
 * @return false when buffer cannot be allocated (out of memory)
 */
bool LLUI_DISPLAY_allocateImageBuffer(MICROUI_Image* img, uint8_t rowAlignmentInBytes);

/*
 * @brief Frees manually an image buffer. This action is useless when the image
 * has been allocated by a call to LLUI_DISPLAY_IMPL_decodeImage() because the allocated
 * image will be freed by the MicroEJ application thanks a call to Image.close().
 *
 * @param[in] image the MicroUI Image.
 */
void LLUI_DISPLAY_freeImageBuffer(MICROUI_Image* img);

/*
 * @brief Callback to call by LLUI_DISPLAY_IMPL.h implementation when the flush
 * (copy of drawings to LCD) is finished. See LLUI_DISPLAY_IMPL_flush() function.
 *
 * @param[in] from_isr true when this function is called from an interrupt context.
 */
void LLUI_DISPLAY_flushDone(bool from_isr);

/*
 * @brief Requests the graphics engine to start a drawing. This allows to suspend
 * another Java thread until the drawing is performed. In addition, this call may
 * suspend the current thread until the previous drawing is done. In this case, a
 * second call to the caller will be automatically performed.
 *
 * This function can only be called from a Java native function context.
 *
 * When returning true, a call to LLUI_DISPLAY_setDrawingStatus() is mandatory after
 * performing the drawing.
 *
 * Java native function pattern is:
 *
 * void _drawing_native_xxx(MICROUI_GraphicsContext* gc, ...)
 * {
 * 		// tell to graphics engine if drawing can be performed
 * 		if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&_drawing_native_xxx))
 * 		{
 * 			// perform the drawings (respecting clip if not disabled)
 *	 		DRAWING_Status status = _drawing_xxx(gc, ...);
 *
 *			// notify drawing status
 *			LLUI_DISPLAY_setDrawingStatus(status);
 * 		}
 * 		// else: refused drawing
 * }
 *
 * @param[in] gc the MicroUI GraphicsContext
 * @param[in] callback the Java native function (or another function with same signature
 * than Java native function) to (re)call if the thread must be suspended.
 *
 * @return true if the drawing can start or false when the drawing does not need
 * to be done for any reason.
 */
bool LLUI_DISPLAY_requestDrawing(MICROUI_GraphicsContext* gc, SNI_callback callback);

/*
 * @brief Notifies the graphics engine about the drawing status.
 *
 * This function must be called when the call to LLUI_DISPLAY_requestDrawing() has
 * returned true.
 *
 * After this call, the next call to LLUI_DISPLAY_setDrawingLimits() is ignored.
 *
 * @param[in] status drawing status: drawing is done (synchronous drawing): DRAWING_DONE
 * or drawing has been launched / is running (asynchronous drawing): DRAWING_RUNNING.
 */
void LLUI_DISPLAY_setDrawingStatus(DRAWING_Status status);

/*
 * @brief Callback to call by LLUI_DISPLAY_IMPL implementation when the asynchronous
 * drawing (launched just after the call to LLUI_DISPLAY_requestDrawing()) is finished.
 *
 * After this call, the next call to LLUI_DISPLAY_setDrawingLimits() is ignored.
 *
 * @param[in] from_isr true when this function is called from an interrupt context.
 */
void LLUI_DISPLAY_notifyAsynchronousDrawingEnd(bool from_isr);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
