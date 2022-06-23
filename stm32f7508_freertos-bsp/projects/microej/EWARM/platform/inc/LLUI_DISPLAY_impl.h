/* 
 * Copyright 2011-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLUI_DISPLAY_IMPL
#define _LLUI_DISPLAY_IMPL
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Display module (MicroUI Graphical Engine) low level API. This file lists
 * a set of function to connect the Graphics User Interface (GUI). First functions
 * are mandatory; optional functions are listed in a second time.
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include "LLUI_DISPLAY.h"

// --------------------------------------------------------------------------------
// Typedef and Structure
// --------------------------------------------------------------------------------

/*
 * @brief Defines a binary semaphore used by the Graphics Engine.
 */
typedef void* LLUI_DISPLAY_binary_semaphore;

/*
 * @brief LLUI_DISPLAY_IMPL initialization data.
 *
 * Defines the element the implementation has to initialize when the Graphics Engine
 * is calling the function LLUI_DISPLAY_IMPL_initialize.
 */
typedef struct
{
	/*
	 * @brief Binary semaphore used by the Graphics Engine.
	 *
	 * This semaphore must be configured in a state such that the semaphore must
	 * first be 'given' before it can be 'taken'.
	 *
	 * This semaphore is reserved for the Graphics Engine. The LLUI_DISPLAY_IMPL.h
	 * implementation is not allowed to use this semaphore to synchronize the function
	 * LLUI_DISPLAY_IMPL_flush() with the display driver (or for any other synchronization
	 * actions). The implementation must create its own semaphores in addition with
	 * this dedicated Graphics Engine's semaphore.
	 */
	LLUI_DISPLAY_binary_semaphore* binary_semaphore_0;

	/*
	 * @brief Binary semaphore used by the Graphics Engine.
	 *
	 * This semaphore must be configured in a state such that the semaphore must
	 * first be 'given' before it can be 'taken'.
	 *
	 * For more information on this semaphore, refer to the binary_semaphore_0's documentation.
	 */
	LLUI_DISPLAY_binary_semaphore* binary_semaphore_1;

	/*
	 * @brief Back buffer address on startup.
	 *
	 * This buffer will be used by the Graphics Engine to draw application drawings.
	 * In COPY and DIRECT modes, this buffer address is used during all application
	 * runtime. In SWITCH mode, this buffer address will be internally updated by the
	 * Graphics Engine (see LLUI_DISPLAY_IMPL_flush()).
	 */
	uint8_t* back_buffer_address;

	/*
	 * @brief LCD width in pixels.
	 */
	uint32_t lcd_width;

	/*
	 * @brief LCD height in pixels.
	 */
	uint32_t lcd_height;

	/*
	 * @brief LCD buffer width in pixels.
	 *
	 * The display graphics buffer may require more memory than theoretical memory.
	 * Theoretical memory size is:
	 * 		lcd_width * lcd_height * bpp / 8.
	 *
	 * On some devices the memory width (in pixels) is higher than virtual width.
	 * In this way, the graphics buffer memory size is:
	 * 		memory_width * memory_height * bpp / 8.
	 *
	 * Keep this value to 0 when memory_width is equals to lcd_width. An error is
	 * thrown by the Graphics Engine when memory_width is smaller than lcd_width.
	 */
	uint32_t memory_width;

	/*
	 * @brief LCD buffer height in pixels.
	 *
	 * The display graphics buffer may require more memory than theoretical memory.
	 * Theoretical memory size is:
	 * 		lcd_width * lcd_height * bpp / 8.
	 *
	 * On some devices the memory height (in pixels) is higher than virtual height.
	 * In this way, the graphics buffer memory size is:
	 * 		memory_width * memory_height * bpp / 8.
	 *
	 * Keep this value to 0 when memory_height is equals to lcd_height. An error is
	 * thrown by the Graphics Engine when memory_height is smaller than lcd_height.
	 */
	uint32_t memory_height;

} LLUI_DISPLAY_SInitData;

// --------------------------------------------------------------------------------
// Functions that must be implemented
// --------------------------------------------------------------------------------

/*
 * @brief Initializes the display driver. Called by the Graphics Engine when
 * MicroEJ application is calling MicroUI.start().
 *
 * Given structure must be initialized with expected data. See LLUI_DISPLAY_SInitData
 * comments.
 *
 * @param[in] init_data the data to initialize.
 */
void LLUI_DISPLAY_IMPL_initialize(LLUI_DISPLAY_SInitData* init_data);

/*
 * @brief Takes the binary semaphore.
 *
 * @param[in] binary_semaphore the binary semaphore to take.
 */
void LLUI_DISPLAY_IMPL_binarySemaphoreTake(void* binary_semaphore);

/*
 * @brief Gives the binary semaphore.
 *
 * @param[in] binary_semaphore the binary semaphore to give.
 * @param[in] from_isr true when giving the semaphore from an interrupt.
 */
void LLUI_DISPLAY_IMPL_binarySemaphoreGive(void* binary_semaphore, bool from_isr);

/*
 * @brief Performs a flush.
 *
 * The content of given square in graphics buffer (back buffer) must be displayed
 * into LCD buffer (frame buffer). The rectangle specified by (xmin, ymin) to (xmax, ymax)
 * is the region which has changed during last draw and only this region should
 * be updated (dirty area).
 *
 * The implementation has to return the new graphics buffer address, depending on
 * the display flushing mode (see UI documentation):
 *
 * 		- In DIRECT mode, this action is useless because graphics and frame buffers
 * 		are same buffer. The buffer address to return must be equal to the given
 * 		buffer address.
 *
 * 		- In COPY mode, the content of given rectangle in back buffer must be copied
 * 		into LCD buffer. This function should be atomic and not block until the
 * 		buffer is copied. The implementation should start a DMA or another OS task
 * 		to perform the copy and return. The buffer address to return must be equal
 * 		to the given buffer address. At the end of the asynchronous copy, LLUI_DISPLAY
 * 		implementation has to call the function LLUI_DISPLAY_flushDone() to notify
 * 		to the Graphics Engine the back buffer is now free and can be reused to
 * 		perform the next drawing operations.
 *
 * 		- In SWITCH mode, the implementation has to set the new display buffer address
 * 		to the supplied address. The buffers are swapped: the existing display buffer
 * 		becomes the new back buffer and the supplied address becomes the new display
 * 		buffer. After the swap the implementation must copy the content of the new
 * 		display buffer into the new back buffer. This function should be atomic
 * 		and not block until the buffer is copied. The implementation should start
 * 		a DMA or another OS task to perform the copy and return. The buffer address
 * 		to return must be the old frame buffer address. At the end of the asynchronous
 * 		copy, LLUI_DISPLAY implementation has to call the function LLUI_DISPLAY_flushDone()
 * 		to notify to the Graphics Engine the new back buffer is now free and can
 * 		be reused to perform the next drawing operations.
 *
 * The new display buffer given as parameter can be also retrieved calling
 * LLUI_DISPLAY_getBufferAddress(&gc->image);
 *
 * @param[in] gc the graphics context to flush
 * @param[in] sourceAddr the address of graphics context buffer
 * @param[in] xmin the top-left X-coordinate of the dirty area.
 * @param[in] ymin the top-left Y-coordinate of the dirty area.
 * @param[in] xmax the bottom-right X-coordinate of the dirty area.
 * @param[in] ymax the bottom-right Y-coordinate of the dirty area.
 *
 * @return the new graphics buffer address
 */
uint8_t* LLUI_DISPLAY_IMPL_flush(MICROUI_GraphicsContext* gc, uint8_t* sourceAddr, uint32_t xmin, uint32_t ymin, uint32_t xmax, uint32_t ymax);

// --------------------------------------------------------------------------------
// Optional functions to implement
// --------------------------------------------------------------------------------

/*
 * @brief Initializes the MicroUI images heap. This heap is used to decode at runtime PNG
 * images, to store the MicroUI BufferedImage, etc.
 * <p>
 * The heap size is fixed by the MicroEJ application launcher. It is allocated in the
 * section <code>.bss.microui.display.imagesHeap</code>.
 * <p>
 * This function is not called by the Graphics Engine if the size is too small. By consequence,
 * all allocations will return an out of memory error.
 * <p>
 * The default implementation is using a best fit allocator:
 * <code>
 * 		BESTFIT_ALLOCATOR_new(&image_heap);
		BESTFIT_ALLOCATOR_initialize(&image_heap, (int32_t)heap_start, (int32_t)heap_limit);
 * </code>
 *
 * @param[in] heap_start bss section start address.
 * @param[in] heap_limit bss section end address + 1.
 */
void LLUI_DISPLAY_IMPL_image_heap_initialize(uint8_t* heap_start, uint8_t* heap_limit);

/*
 * @brief Allocates a block in the images heap.
 * <p>
 * The default implementation is using a best fit allocator:
 * <code>
 * 		return (uint8_t*)BESTFIT_ALLOCATOR_allocate(&image_heap, (int32_t)size);
 * </code>
 *
 * @param[in] size the block size.
 *
 * @return the block address or NULL in case of out of memory.
 */
uint8_t* LLUI_DISPLAY_IMPL_image_heap_allocate(uint32_t size);

/*
 * @brief Frees a block in images heap.
 * <p>
 * The default implementation is using a best fit allocator:
 * <code>
 * 		BESTFIT_ALLOCATOR_free(&image_heap, (void*)block);
 * </code>
 *
 * @param[in] block the block to free.
 */
void LLUI_DISPLAY_IMPL_image_heap_free(uint8_t* block);

/*
 * @brief Sets the new contrast. By default the weak function does nothing (feature
 * not supported).
 *
 * @param[in] contrast a value between 0 and 100.
 */
void LLUI_DISPLAY_IMPL_setContrast(uint32_t contrast);

/*
 * @brief Gets the current contrast. By default the weak function returns 0.
 *
 * @return a value between 0 and 100.
 */
uint32_t LLUI_DISPLAY_IMPL_getContrast(void);

/*
 * @brief Returns true when the display module can manage the display backlight,
 * false otherwise. By default the weak function returns false (feature not supported).
 *
 * @return true or false.
 */
bool LLUI_DISPLAY_IMPL_hasBacklight(void);

/*
 * @brief Sets the new backlight value. By default the weak function does nothing
 * (feature not supported).
 *
 * @param[in] backlight a value between 0 and 100.
 */
void LLUI_DISPLAY_IMPL_setBacklight(uint32_t backlight);

/*
 * @brief Gets the current backlight value. By default the weak function returns
 * 0.
 * @return a value between 0 and 100.
 */
uint32_t LLUI_DISPLAY_IMPL_getBacklight(void);

/*
 * @brief Asks if the display is a colored display or not.
 *
 * @return true when the display is not a grayscale display, false otherwise. By
 * default the weak function returns false when the BPP is lower than or equals
 * to 4 BPP and true otherwise.
 */
bool LLUI_DISPLAY_IMPL_isColor(void);

/*
 * @brief Returns the number of colors the display can display.
 *
 * Usually the number of colors is 1 << BPP (BPP without transparency bits). By default
 * the weak function returns a value which follows this rule.
 *
 * @return the number of colors the display can display.
 */
uint32_t LLUI_DISPLAY_IMPL_getNumberOfColors(void);

/*
 * @brief Asks if the display uses an underlying double buffer (either hardware
 * or software).
 *
 * This technique is useful to avoid flickering while the user is drawing.
 *
 * @return true if and only if a double buffer is used for the display false otherwise.
 * By default the weak function returns true.
 */
bool LLUI_DISPLAY_IMPL_isDoubleBuffered(void);

/*
 * @brief Converts the 32-bit ARGB color format (A-R-G-B) into the display color
 * format.
 *
 * This function is called only when the display is not a standard display: when
 * the pixel data does not match with one of these formats: MICROUI_IMAGE_FORMAT_ARGB8888,
 * MICROUI_IMAGE_FORMAT_RGB888, MICROUI_IMAGE_FORMAT_RGB565, MICROUI_IMAGE_FORMAT_ARGB1555
 * or MICROUI_IMAGE_FORMAT_ARGB4444. In case of this function is not implemented
 * whereas it is required, the result of pixel drawing is unknown.
 *
 * Note: the alpha level may be ignored if the display pixel representation does
 * not hold the alpha level information.
 *
 * The implementation should not directly call this function when performing a drawing.
 * It must call LLUI_DISPLAY_convertARGBColorToDisplayColor() instead in case of this
 * conversion is Graphics Engine built-in (standard display)
 *
 * @param[in] color the color to convert.
 *
 * @return the converted color.
 */
uint32_t LLUI_DISPLAY_IMPL_convertARGBColorToDisplayColor(uint32_t color);

/*
 * @brief Converts the display color format into a 32-bit ARGB color format (A-R-G-B).
 *
 * This function is called only when the display is not a standard display: when
 * the pixel data does not match with one of these formats: MICROUI_IMAGE_FORMAT_ARGB8888,
 * MICROUI_IMAGE_FORMAT_RGB888, MICROUI_IMAGE_FORMAT_RGB565, MICROUI_IMAGE_FORMAT_ARGB1555
 * or MICROUI_IMAGE_FORMAT_ARGB4444. In case of this function is not implemented
 * whereas it is required, the result of pixel drawing is unknown.
 *
 * Note: the alpha level may be ignored if the display pixel representation does
 * not hold the alpha level information. In this case, the returned alpha level is
 * 0xff (full opaque).
 *
 * @param[in] color the color to convert.
 *
 * @return the converted color.
 */
uint32_t LLUI_DISPLAY_IMPL_convertDisplayColorToARGBColor(uint32_t color);

/*
 * @brief Prepares the blending of two ARGB colors.
 *
 * This function is called only when the display is not a standard display: when
 * the pixel data does not match with one of these formats: MICROUI_IMAGE_FORMAT_ARGB8888,
 * MICROUI_IMAGE_FORMAT_RGB888, MICROUI_IMAGE_FORMAT_RGB565, MICROUI_IMAGE_FORMAT_ARGB1555
 * or MICROUI_IMAGE_FORMAT_ARGB4444.
 *
 * This is useful only when the LCD is a palletized LCD. This function is called
 * by framework when the MicroEJ application draws something which requires a blending
 * between the current foreground color and a specific background color (draw a string,
 * draw an anti-aliased line etc.).
 *
 * The implementation has to replace the ARGB colors by the indexes of these colors
 * in the LCD CLUT. The framework will use the intermediate values between these
 * two indexes instead of blending in software the ARGB colors. No post conversion
 * will be performed later.
 *
 * When the ARGB colors are not available in the CLUT or when the range specified
 * by the two ARGB colors is not expected by the CLUT, the implementation should
 * return false (feature not supported). In this way the blending will be done in
 * software and the result color will be converted later thanks a call to
 * convertRGBColorToDisplayColor().
 *
 * By default the weak function returns false (feature not supported / useless).
 *
 * @param[in/out] foreground pointer on the foreground ARGB color to convert.
 * @param[in/out] background pointer on the background ARGB color to convert.
 * @return true when the indexes have been found, false otherwise.
 */
bool LLUI_DISPLAY_IMPL_prepareBlendingOfIndexedColors(uint32_t* foreground, uint32_t* background);

/*
 * @brief Use an hardware image decoder to create a RAW image.
 *
 * The RAW image format is given by expectedFormat parameter. The decoder can choose
 * another RAW image output format when it is not able to apply the expected format.
 *
 * The implementation has to fill the given MICROUI_Image structure with the
 * image characteristics: its width, height, and the chosen RAW image format. The
 * others fields no need to be filled.
 *
 * To allocate the RAW image pixels area, the implementation has to call the function
 * LLUI_DISPLAY_allocateImageBuffer(), giving the filled MICROUI_Image as parameter.
 * This function will allocate the required memory area in the images heap.
 *
 * The implementation returns LLUI_DISPLAY_OK when the RAW image has been created.
 * It has to return LLUI_DISPLAY_NOK when the hardware decoder is not able to decode
 * the given image. If the function LLUI_DISPLAY_allocateImageBuffer() returns false
 * (means "out of memory"), the implementation has to return LLUI_DISPLAY_OUT_OF_MEMORY.
 *
 * @param[in] addr the source image address.
 * @param[in] length the source image size in bytes.
 * @param[in] expectedFormat the RAW image expected format (MICROUI_IMAGE_FORMAT_CUSTOM
 * means the implementation can choose the better output format).
 * @param[in/out] image the reserved MicroUI Image; implementation has to fill it
 * with the image characteristics.
 * @param[out] isFullyOpaque a boolean to notify to the  that the
 * decoded image is fully opaque or not; when fully opaque, the software algorithms
 * which use the image as source image are often faster (even if the image format
 * contains alpha levels).
 *
 * @return LLUI_DISPLAY_OK when decoding is successful.
 */
LLUI_DISPLAY_Status LLUI_DISPLAY_IMPL_decodeImage(uint8_t* addr, uint32_t length, MICROUI_ImageFormat expectedFormat, MICROUI_Image* image, bool* isFullyOpaque);

/*
 * @brief Returns the new image row stride in bytes.
 *
 * Some hardware accelerators require a specific row stride when manipulating images.
 * This function allows to fix the new runtime image size. A default stride is given;
 * this stride is calculated according the image format and platform characteristics.
 *
 * The retained row stride is the higher value between the default row stride and
 * the returned value. This value will be used to allocate the new image pixel buffer.
 *
 * This value can be retrieved later using the functions LLUI_DISPLAY_getStrideInBytes()
 * and LLUI_DISPLAY_getStrideInPixels().
 *
 * This function is only called when the number of bits-per-pixel is higher or equals
 * to 8. For lower values, default stride is always used.
 *
 * By default the weak function returns default row stride.
 *
 * @param[in] image_format the new RAW image format.
 * @param[in] image_width the new image width (in pixels).
 * @param[in] image_height the new image height (in pixels).
 * @param[in] default_stride the minimal row stride (in bytes)
 *
 * @return expected row stride (in bytes)
 */
uint32_t LLUI_DISPLAY_IMPL_getNewImageStrideInBytes(MICROUI_ImageFormat image_format, uint32_t image_width, uint32_t image_height, uint32_t default_stride);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
