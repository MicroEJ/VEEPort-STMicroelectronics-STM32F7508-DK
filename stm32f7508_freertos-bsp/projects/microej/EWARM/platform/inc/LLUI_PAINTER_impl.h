/* 
 * Copyright 2020-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLUI_PAINTER_IMPL
#define _LLUI_PAINTER_IMPL
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Provides native functions of MicroUI 3 library to perform some drawings.
 * Some structures and enumerations are available to identify the MicroUI 3 graphical
 * objects: GraphicsContext, Image, image format. etc.
 *
 * Implementation must use LLUI_DISPLAY.h functions to synchronize drawings, to identify
 * drawing limits (clip, flush area), to have more information on destination (MicroUI
 * Graphics Context) and optional source (MicroUI Image) etc.
 *
 * All native functions have a MICROUI_GraphicsContext* as the first parameter. This
 * identifies the destination target: the MicroUI GraphicsContext. This target is
 * retrieved in MicroEJ application calling the method gc.getSNIContext(). This method
 * returns a byte array which is directly mapped on the MICROUI_GraphicsContext
 * structure in MicroUI native drawing function declaration.
 *
 * For a MicroUI Image, MicroEJ application has to call image.getSNIContext() to get the
 * image byte array which is directly mapped on the MICROUI_Image structure in MicroUI
 * native drawing function declaration.
 *
 * A graphics context holds a clip and the drawer is not allowed to perform a drawing
 * outside this clip (otherwise the behavior is unknown). Note the bottom-right
 * coordinates might be smaller than top-left (in x and/or y) when the clip width
 * and/or height is null. The clip may be disabled (when the current drawing fits
 * the clip); this allows to reduce runtime. See LLUI_DISPLAY_isClipEnabled().
 * note: several clip functions are available in LLUI_DISPLAY.h to check if a drawing
 * fits the clip.
 *
 * The drawing function must update the next Display.flush() area (dirty area). If
 * not performed, the next call to Display.flush() will not call LLUI_DISPLAY_IMPL_flush()
 * function.
 *
 * The native function implementation pattern is:
 *
 * void _drawing_native_xxx(MICROUI_GraphicsContext* gc, ...)
 * {
 * 		// tell to the Graphics Engine if drawing can be performed
 * 		if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&_drawing_native_xxx))
 * 		{
 * 			DRAWING_Status status;
 *
 * 			// perform the drawings (respecting clip if not disabled)
 * 			[...]
 *
 *	 		// update new flush dirty area
 *			LLUI_DISPLAY_setDrawingLimits(gc, ...);
 *
 *			// set drawing status
 *			LLUI_DISPLAY_setDrawingStatus(DRAWING_DONE); // or DRAWING_RUNNING;
 * 		}
 * 		// else: refused drawing
 * }
 *
 * In this example, the drawer must respect the clip (if not disabled). However
 * for simple drawings (rectangular drawings), the graphics context clip can be
 * quickly checked in order to reduce the drawer rendering time:
 *
 * void _drawing_native_xxx(MICROUI_GraphicsContext* gc, ...)
 * {
 * 		// tell to the Graphics Engine if drawing can be performed
 * 		if (LLUI_DISPLAY_requestDrawing(gc, (SNI_callback)&_drawing_native_xxx))
 * 		{
 * 			DRAWING_Status status;
 *
 * 			if (LLUI_DISPLAY_isInClip(gc, ...))
 * 			{
 * 				// perform the drawings (respecting clip if not disabled)
 * 				[...]
 *
 *	 			status = DRAWING_DONE; // or DRAWING_RUNNING;
 *
 *	 			// update new flush dirty area
 *				LLUI_DISPLAY_setDrawingLimits(gc, ...);
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
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

/*
 * @brief MicroUI 3 is using SNI library to declare its native methods.
 */
#include "sni.h"

// --------------------------------------------------------------------------------
// Macros and Defines
// --------------------------------------------------------------------------------

/*
 * @brief Useful macros to concatenate easily some strings and defines.
 */
#define CONCAT_STRINGS(p, s) p ## s
#define CONCAT_DEFINES(p, s) CONCAT_STRINGS(p,s)

/*
 * MicroUI's native functions
 */
#define LLUI_PAINTER_IMPL_writePixel Java_ej_microui_display_PainterNatives_writePixel
#define LLUI_PAINTER_IMPL_drawLine Java_ej_microui_display_PainterNatives_drawLine
#define LLUI_PAINTER_IMPL_drawHorizontalLine Java_ej_microui_display_PainterNatives_drawHorizontalLine
#define LLUI_PAINTER_IMPL_drawVerticalLine Java_ej_microui_display_PainterNatives_drawVerticalLine
#define LLUI_PAINTER_IMPL_drawRectangle Java_ej_microui_display_PainterNatives_drawRectangle
#define LLUI_PAINTER_IMPL_fillRectangle Java_ej_microui_display_PainterNatives_fillRectangle
#define LLUI_PAINTER_IMPL_drawRoundedRectangle Java_ej_microui_display_PainterNatives_drawRoundedRectangle
#define LLUI_PAINTER_IMPL_fillRoundedRectangle Java_ej_microui_display_PainterNatives_fillRoundedRectangle
#define LLUI_PAINTER_IMPL_drawCircleArc Java_ej_microui_display_PainterNatives_drawCircleArc
#define LLUI_PAINTER_IMPL_drawEllipseArc Java_ej_microui_display_PainterNatives_drawEllipseArc
#define LLUI_PAINTER_IMPL_fillCircleArc Java_ej_microui_display_PainterNatives_fillCircleArc
#define LLUI_PAINTER_IMPL_fillEllipseArc Java_ej_microui_display_PainterNatives_fillEllipseArc
#define LLUI_PAINTER_IMPL_drawEllipse Java_ej_microui_display_PainterNatives_drawEllipse
#define LLUI_PAINTER_IMPL_fillEllipse Java_ej_microui_display_PainterNatives_fillEllipse
#define LLUI_PAINTER_IMPL_drawCircle Java_ej_microui_display_PainterNatives_drawCircle
#define LLUI_PAINTER_IMPL_fillCircle Java_ej_microui_display_PainterNatives_fillCircle
#define LLUI_PAINTER_IMPL_drawImage Java_ej_microui_display_PainterNatives_drawImage

// --------------------------------------------------------------------------------
// Typedefs and Structures
// --------------------------------------------------------------------------------

/*
 * @brief Enumerates all MicroUI Image RAW formats.
 */
typedef enum
{
	/*
	 * @brief Defines an image with the same pixel representation and layout as
	 * the LCD memory.
	 */
	MICROUI_IMAGE_FORMAT_DISPLAY = 0x00,

	/*
	 * @brief Defines an image whose pixel format is ARGB8888.
	 */
	MICROUI_IMAGE_FORMAT_ARGB8888 = 0x02,

	/*
	 * @brief Defines an image whose pixel format is ARGB1555.
	 */
	MICROUI_IMAGE_FORMAT_ARGB1555 = 0x05,

	/*
	 * @brief Defines an image whose pixel format is ARGB4444.
	 */
	MICROUI_IMAGE_FORMAT_ARGB4444 = 0x06,

	/*
	 * @brief Defines an image whose pixel format is ARGB8888 pre-multiplied: each color component is multiplied by the
	 * opacity.
	 */
	MICROUI_IMAGE_FORMAT_ARGB8888_PRE = 0x18,

	/*
	 * @brief Defines an image whose pixel format is ARGB1555 pre-multiplied: each color component is multiplied by the
	 * opacity.
	 */
	MICROUI_IMAGE_FORMAT_ARGB1555_PRE = 0x19,

	/*
	 * @brief Defines an image whose pixel format is ARGB4444 pre-multiplied: each color component is multiplied by the
	 * opacity.
	 */
	MICROUI_IMAGE_FORMAT_ARGB4444_PRE = 0x1A,

	/*
	 * @brief Defines an image whose pixel format is RGB888.
	 */
	MICROUI_IMAGE_FORMAT_RGB888 = 0x03,

	/*
	 * @brief Defines an image whose pixel format is RGB565.
	 */
	MICROUI_IMAGE_FORMAT_RGB565 = 0x04,

	/*
	 * @brief Defines an image whose pixel format is Alpha1.
	 */
	MICROUI_IMAGE_FORMAT_A1 = 0x0C,

	/*
	 * @brief Defines an image whose pixel format is Alpha2.
	 */
	MICROUI_IMAGE_FORMAT_A2 = 0x0B,

	/*
	 * @brief Defines an image whose pixel format is Alpha4.
	 */
	MICROUI_IMAGE_FORMAT_A4 = 0x07,

	/*
	 * @brief Defines an image whose pixel format is Alpha8.
	 */
	MICROUI_IMAGE_FORMAT_A8 = 0x08,

	/*
	 * @brief Defines an image whose pixel format is Color1.
	 */
	MICROUI_IMAGE_FORMAT_C1 = 0x0F,

	/*
	 * @brief Defines an image whose pixel format is Color2.
	 */
	MICROUI_IMAGE_FORMAT_C2 = 0x0E,

	/*
	 * @brief Defines an image whose pixel format is Color4.
	 */
	MICROUI_IMAGE_FORMAT_C4 = 0x0D,

	/*
	 * @brief Defines an image whose pixel format is Alpha1-Color1.
	 */
	MICROUI_IMAGE_FORMAT_AC11 = 0x12,

	/*
	 * @brief Defines an image whose pixel format is Alpha2-Color2.
	 */
	MICROUI_IMAGE_FORMAT_AC22 = 0x11,

	/*
	 * @brief Defines an image whose pixel format is Alpha4-Color4.
	 */
	MICROUI_IMAGE_FORMAT_AC44 = 0x10,

	/*
	 * @brief Defines an image whose pixel format is a LUT entry on 8 bits and target
	 * an ARGB8888 color.
	 */
	MICROUI_IMAGE_FORMAT_LARGB8888 = 0x0A,

	/*
	 * @brief Defines an undefined format. Used by LLUI_DISPLAY_IMPL_decodeImage() to
	 * not specify a specific format.
	 */
	MICROUI_IMAGE_FORMAT_UNDEFINED = 0x80,

	/*
	 * @brief Defines the custom format 7.
	 */
	MICROUI_IMAGE_FORMAT_CUSTOM_7 = 0xF8,

	/*
	 * @brief Defines the custom format 6.
	 */
	MICROUI_IMAGE_FORMAT_CUSTOM_6 = 0xF9,

	/*
	 * @brief Defines the custom format 5.
	 */
	MICROUI_IMAGE_FORMAT_CUSTOM_5 = 0xFA,

	/*
	 * @brief Defines the custom format 4.
	 */
	MICROUI_IMAGE_FORMAT_CUSTOM_4 = 0xFB,

	/*
	 * @brief Defines the custom format 3.
	 */
	MICROUI_IMAGE_FORMAT_CUSTOM_3 = 0xFC,

	/*
	 * @brief Defines the custom format 2.
	 */
	MICROUI_IMAGE_FORMAT_CUSTOM_2 = 0xFD,

	/*
	 * @brief Defines the custom format 1.
	 */
	MICROUI_IMAGE_FORMAT_CUSTOM_1 = 0xFE,

	/*
	 * @brief Defines the custom format 0 (0xff).
	 */
	MICROUI_IMAGE_FORMAT_CUSTOM_0 = 0xFF,

} MICROUI_ImageFormat;

/*
 * @brief Represents a MicroUI Image.
 *
 * This structure is used by several drawing functions which use an image as source
 * image. It can be mapped on jbyte array given as parameter in some MicroUI natives.
 * This jbyte array is retrieved in MicroEJ application using the method Image.getData().
 *
 * Only the image size and format are available in this structure. Implementation
 * has to use some LLUI_DISPLAY.h functions to retrieve the image pixel's address and
 * some image characteristics.
 */
typedef struct
{
	/*
	 * @brief Graphics Engine reserved field.
	 */
	jint reserved0;

	/*
	 * @brief MicroUI Image width in pixels.
	 */
	jchar width;

	/*
	 * @brief MicroUI Image height in pixels.
	 */
	jchar height;

	/*
	 * @brief Graphics Engine reserved field.
	 */
	jchar reserved1;

	/*
	 * @brief MicroUI Image pixel representation.
	 *
	 * The format is one value from the MICROUI_ImageFormat enumeration.
	 */
	jbyte format;

	/*
	 * @brief Graphics Engine reserved field.
	 */
	jbyte reserved2;

} MICROUI_Image;

/*
 * @brief Represents a MicroUI Graphics Context.
 *
 * This structure is used by all drawing functions to target the destination. It
 * can be mapped on jbyte array given as parameter in MicroUI natives. This jbyte
 * array is retrieved in MicroEJ application using the method GraphicsContext.getData().
 *
 * Only the graphics context size, format, color and clip are available in this
 * structure. Implementation has to use some LLUI_DISPLAY.h functions to retrieve the
 * graphics context pixels' addresses and some graphics context characteristics.
 */
typedef struct
{
	/*
	 * @brief A graphics context targets a mutable image (size and format).
	 */
	MICROUI_Image image;

	/*
	 * @brief Current graphics context foreground color. This color must be used
	 * to render the drawing. The color format is 0xAARRGGBB (where alpha level
	 * is always 0xff == fully opaque).
	 */
	jint foreground_color;

	/*
	 * @brief Graphics Engine reserved field.
	 */
	jint reserved0;

	/*
	 * @brief Top-left X coordinate of current clip.
	 *
	 * Read-only. Call LLUI_DISPLAY_setClip to modify this value.
	 */
	jshort clip_x1;

	/*
	 * @brief Top-left Y coordinate of current clip.
	 *
	 * Read-only. Call LLUI_DISPLAY_setClip to modify this value.
	 */
	jshort clip_y1;

	/*
	 * @brief Bottom-right X coordinate of current clip.
	 *
	 * Read-only. Call LLUI_DISPLAY_setClip to modify this value.
	 */
	jshort clip_x2;

	/*
	 * @brief Bottom-right Y coordinate of current clip.
	 *
	 * Read-only. Call LLUI_DISPLAY_setClip to modify this value.
	 */
	jshort clip_y2;

	/*
	 * @brief Log flags from drawing operations.
	 */
	jint drawing_log_flags;

	/*
	 * @brief The drawing engine identifier. Useful to retrieve the engine able to
	 * draw in the GraphicsContext buffer. The "0" value indicates the default engine:
	 * the same engine used to draw in the display buffer.
	 * @see LLUI_DISPLAY_IMPL_getDrawerIdentifier()
	 */
	uint8_t drawer;

} MICROUI_GraphicsContext;

/*
 * @brief Drawing functions' return status.
 *
 * A function has to return DRAWING_DONE when the drawing has been fully done during
 * the function call (synchronous drawing). When the drawing is performed by an
 * asynchronous third party process (software or hardware), the function has to
 * return DRAWING_RUNNING to notify to the Graphics Engine the drawing is not fully
 * done yet.
 */
typedef enum {

	/*
	 * @brief Value to return when the drawing has been synchronously performed.
	 */
	DRAWING_DONE = 0,

	/*
	 * @brief Value to return when the drawing will be asynchronously performed.
	 */
	DRAWING_RUNNING = 1

} DRAWING_Status;

/*
 * @brief Flags describing incidents occurring during drawings.
 */
typedef enum DRAWING_Incident {

	/*
	 * @brief Value used when no incident occurred.
	 */
	DRAWING_SUCCESS = 0,

	/*
	 * @brief Flag stating that an error occurred during a drawing.
	 *
	 * This flag will cause an exception to be thrown when checking the flags in the application.
	 * No exception will be thrown if this flag is not set, although other flags will keep their state and be readable in the application.
	 * This flag is to be combined with other flags describing the error.
	 */
	DRAWING_LOG_ERROR = 1 << 31,

	/*
	 * @brief Flag stating that a drawing function is lacking an implementation.
	 */
	DRAWING_LOG_NOT_IMPLEMENTED = 1 << 0,

	/*
	 * @brief Flag stating that a function was called in a context that does not allow that operation.
	 */
	DRAWING_LOG_FORBIDDEN = 1 << 1,

	/*
	 * @brief Flag stating that the system ran out of memory while attempting to perform a drawing.
	 */
	DRAWING_LOG_OUT_OF_MEMORY = 1 << 2,

	/*
	 * @brief Flag stating that the clip area of a graphics context was modified by LLUI_DISPLAY_setClip or
	 * LLUI_DISPLAY_intersectClip.
	 *
	 * If this flag is set, the caller is responsible for saving the previous clip area and restore it afterwards.
	 *
	 * This flag merely warns the user that the clip values returned by <code>GraphicsContext.getClipX</code>,
	 * <code>GraphicsContext.getClipY</code>, <code>GraphicsContext.getClipWidth</code> and
	 * <code>GraphicsContext.getClipHeight</code> may not be identical to the clip values used in the low-level side. It
	 * is meant to be used as a debugging hint if a drawing seems incorrect.
	 *
	 * @see LLUI_DISPLAY_setClip
	 * @see LLUI_DISPLAY_intersectClip
	 */
	DRAWING_LOG_CLIP_MODIFIED = 1 << 3,

	/*
	 * @brief Flag stating that an undefined character was drawn.
	 *
	 * This happens when drawing a string that contains a character that is not included in the font used.
	 */
	DRAWING_LOG_MISSING_CHARACTER = 1 << 4,

	/*
	 * @brief Flag describing incidents occurring in a drawing library.
	 *
	 * Refer to the MicroUI implementation in the VEE port for more information about this incident.
	 */
	DRAWING_LOG_LIBRARY_INCIDENT = 1 << 29,

	/*
	 * @brief Flag describing incidents that do not match other values.
	 */
	DRAWING_LOG_UNKNOWN_INCIDENT = 1 << 30

} DRAWING_Incident;

// --------------------------------------------------------------------------------
// MicroUI 3 native functions that must be implemented
// --------------------------------------------------------------------------------

/*
 * @brief Draws a pixel at given position.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the pixel X coordinate.
 * @param[in] y the pixel Y coordinate.
 */
void LLUI_PAINTER_IMPL_writePixel(MICROUI_GraphicsContext* gc, jint x, jint y);

/*
 * @brief Draws a line at between points startX,startY (included) and endX,endY (included).
 * Note: startX may be higher than endX (and/or startX  may be higher than endY).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the first pixel line X coordinate.
 * @param[in] startY the first pixel line Y coordinate.
 * @param[in] endX the last pixel line X coordinate.
 * @param[in] endY the last pixel line Y coordinate.
 */
void LLUI_PAINTER_IMPL_drawLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY);

/*
 * @brief Draws a horizontal line at between points x,y (included) and x+length-1,y (included).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the first pixel line X coordinate.
 * @param[in] y the first pixel line Y coordinate.
 * @param[in] length the line size.
 */
void LLUI_PAINTER_IMPL_drawHorizontalLine(MICROUI_GraphicsContext* gc, jint x, jint y, jint length);

/*
 * @brief Draws a vertical line at between points x,y (included) and x,y+length-1 (included).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the first pixel line X coordinate.
 * @param[in] y the first pixel line Y coordinate.
 * @param[in] length the line size.
 */
void LLUI_PAINTER_IMPL_drawVerticalLine(MICROUI_GraphicsContext* gc, jint x, jint y, jint length);

/*
 * @brief Draws an orthogonal rectangle at from top-left point x,y (included) and bottom-right
 * point x+width-1,y+height-1 (included).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 */
void LLUI_PAINTER_IMPL_drawRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

/*
 * @brief Fills a rectangle at from top-left point x,y (included) and bottom-right
 * point x+width-1,y+height-1 (included).
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 */
void LLUI_PAINTER_IMPL_fillRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

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
void LLUI_PAINTER_IMPL_drawRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

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
void LLUI_PAINTER_IMPL_fillRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/*
 * @brief Draws a circular arc covering the square defined by top-left point x,y (included)
 * and bottom-right point x+diameter-1,y+diameter-1 (included).
 *
 * The arc is drawn from startAngle up to arcAngle degrees. The center of the arc is
 * defined as the center of the square whose origin is at (x,y) (upper-left corner)
 * and whose dimension is given by diameter.
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
 * @param[in] diameter  the diameter of the arc to draw.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 */
void LLUI_PAINTER_IMPL_drawCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);

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
void LLUI_PAINTER_IMPL_drawEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);

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
void LLUI_PAINTER_IMPL_fillCircleArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jfloat startAngle, jfloat arcAngle);

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
void LLUI_PAINTER_IMPL_fillEllipseArc(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jfloat startAngle, jfloat arcAngle);

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
 */
void LLUI_PAINTER_IMPL_drawEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

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
 */
void LLUI_PAINTER_IMPL_fillEllipse(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height);

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
 */
void LLUI_PAINTER_IMPL_drawCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);

/*
 * @brief Fills a circle covering the square defined by top-left point x,y (included)
 * and bottom-right point x+diameter-1,y+diameter-1 (included).
 *
 * If the given diameter is negative or zero, nothing is drawn.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the circle square size.
 */
void LLUI_PAINTER_IMPL_fillCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter);

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
 * When this value is lower than 0 or higher than 255, this value must be clamped.
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
 */
void LLUI_PAINTER_IMPL_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
