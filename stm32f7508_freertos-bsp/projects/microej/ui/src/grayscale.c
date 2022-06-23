/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#include <stdint.h>
#include <stdio.h>
#include "LLUI_DISPLAY.h"
#include "LLDISPLAY_configuration.h"

#define GRAYSCALE_ALGO_1	1	// (r + g + b)/3
#define GRAYSCALE_ALGO_2	2	// (r*0.299 + g*0.587 + b*0.144)
#define GRAYSCALE_ALGO_3	3	// (r*0.2126 + g*0.7152 + b*0.0722)

/* ======================================= */
#define GRAYSCALE_ALGO	GRAYSCALE_ALGO_3
/* ======================================= */

#define LCD_ID		0x00
#define RLE1_ID		0x01
#define ARGB8888_ID	0x02
#define RGB888_ID	0x03
#define RGB565_ID	0x04
#define ARGB1555_ID	0x05
#define ARGB4444_ID	0x06

#if GRAYSCALE_ALGO == GRAYSCALE_ALGO_2
#define RED_FACTOR 306		// 0.299 * 1024
#define GREEN_FACTOR 601	// 0.587 * 1024
#define BLUE_FACTOR 117		// 0.114 * 1024
#define SHIFT_FACTOR 10		// 1024
#endif

#if GRAYSCALE_ALGO == GRAYSCALE_ALGO_3
#define RED_FACTOR 3483			// 0.2126 * 16384
#define GREEN_FACTOR 11718	// 0.7152 * 16384
#define BLUE_FACTOR 1183		// 0.0722 * 16384
#define SHIFT_FACTOR 14			// 16384
#endif

#define RGB(r,g,b) (GRAYSCALE_ALGO == GRAYSCALE_ALGO_1 ? ((r) + (g) + (b)) / 3 : ((r) * RED_FACTOR + (g) * GREEN_FACTOR + (b) * BLUE_FACTOR) >> SHIFT_FACTOR)

#define ADDR_OFFSET(format) ((format) == ARGB8888_ID ? 4 : (format) == RGB888_ID ? 3 : 2)

#if defined( __ICCARM__ ) || defined(__GNUC__)
 #define __STATIC_INLINE  static inline
#endif // __ICCARM__ || __GNUC__

#if defined( __CC_ARM )
#define __STATIC_INLINE  static __inline
#endif // __CC_ARM

static uint8_t get_format(MICROUI_Image* gc)
{
	// get image format, TODO work item 14458
	uint8_t format = (MICROUI_ImageFormat)gc->format;

	// grayscale converter does not manage some image formats
	if (format == RLE1_ID || format > ARGB4444_ID)
	{
		// FIXME: return an error code in Java
		printf("invalid image format: %d\n", format);
		while(1);
	}

	// convert the LCD format in standard format
	if (format == LCD_ID)
	{
#if LLDISPLAY_BPP == 16
		format = RGB565_ID;
#elif LLDISPLAY_BPP == 24
		format = RGB888_ID;
#elif LLDISPLAY_BPP == 32
		format = ARGB8888_ID;
#else
		#error "Define 'LLDISPLAY_BPP' is required (16, 24 or 32)"
#endif
	}

	return format;
}

__STATIC_INLINE void read_8888(uint32_t addr, uint32_t* a, uint32_t* rgb)
{
	uint32_t pix = *((uint32_t*)addr);
	*a = pix & 0xff000000;
	*rgb = RGB((pix >> 16) & 0xff, (pix >> 8) & 0xff, pix & 0xff);
}

__STATIC_INLINE void read_888(uint32_t addr, uint32_t* a, uint32_t* rgb)
{
	*a = 0xff000000;
	*rgb = RGB(*((uint8_t*)(addr+2)), *((uint8_t*)(addr+1)), *((uint8_t*)(addr)));
}

__STATIC_INLINE void read_4444(uint32_t addr, uint32_t* a, uint32_t* rgb)
{
	uint32_t data = *((uint16_t*)addr);
	*a = (data & 0xf000) << 16;
	*rgb = RGB((data & 0x0f00) >> 4, data & 0x00f0, (data & 0x000f) << 4);
}

__STATIC_INLINE void read_1555(uint32_t addr, uint32_t* a, uint32_t* rgb)
{
	uint16_t data = *((uint16_t*)addr);
	*a = ((data & 0x8000) == 0 ? 0x00000000 : 0xff000000);
	*rgb = RGB((data & 0x7c00) >> 7, (data & 0x03e0) >> 2, (data & 0x001f) << 3);
}

__STATIC_INLINE void read_565(uint32_t addr, uint32_t* a, uint32_t* rgb)
{
	uint16_t data = *((uint16_t*)addr);
	*a = 0xff000000;
	*rgb = RGB((data & 0xf800) >> 8, (data & 0x07e0) >> 3, (data & 0x001f) << 3);
}

__STATIC_INLINE void draw_8888(uint32_t addr, uint32_t a, uint32_t rgb)
{
	*((uint32_t*)addr) = (a | (rgb << 16) | (rgb << 8) | rgb);
}

__STATIC_INLINE void draw_888(uint32_t addr, uint32_t a, uint32_t rgb)
{
	*((uint8_t*)(addr+2)) = rgb;
	*((uint8_t*)(addr+1)) = rgb;
	*((uint8_t*)(addr)) = rgb;
}

__STATIC_INLINE void draw_4444(uint32_t addr, uint32_t a, uint32_t rgb)
{
	rgb &= 0xf0;
	*((uint16_t*)addr) = ((a >> 16) & 0xf000) | (rgb << 4) | rgb | (rgb >> 4);
}

__STATIC_INLINE void draw_1555(uint32_t addr, uint32_t a, uint32_t rgb)
{
	*((uint16_t*)addr) = (a == 0 ? 0 : 0x8000) | ((rgb & 0xf8) << 7) | ((rgb & 0xf8) << 2) | (rgb >> 3);
}

__STATIC_INLINE void draw_565(uint32_t addr, uint32_t a, uint32_t rgb)
{
	*((uint16_t*)addr) = ((rgb & 0xf8) << 8) | ((rgb & 0xfc) << 3) | (rgb >> 3);
}

void Java_com_is2t_microui_util_Grayscale_convertToGrayScale (MICROUI_Image* src, MICROUI_Image* dest, uint32_t w, uint32_t h)
{
	uint32_t src_addr = (uint32_t)LLUI_DISPLAY_getBufferAddress(src);
	uint32_t dest_addr = (uint32_t)LLUI_DISPLAY_getBufferAddress(dest);

	uint8_t src_format = get_format(src);
	uint8_t dest_format = get_format(dest);

	uint8_t src_offset = ADDR_OFFSET(src_format);
	uint8_t dest_offset = ADDR_OFFSET(dest_format);

	uint32_t a = 0;
	uint32_t rgb = 0;

	for(int32_t i = w*h; --i>=0;)
	{
		switch(src_format)
		{
			case ARGB8888_ID:	read_8888(src_addr, &a, &rgb);	break;
			case RGB888_ID:		read_888(src_addr, &a, &rgb);	break;
			case RGB565_ID:		read_565(src_addr, &a, &rgb);	break;
			case ARGB1555_ID:	read_1555(src_addr, &a, &rgb);	break;
			case ARGB4444_ID:	read_4444(src_addr, &a, &rgb);	break;
		}

		switch(dest_format)
		{
			case ARGB8888_ID:	draw_8888(dest_addr, a, rgb);	break;
			case RGB888_ID:		draw_888(dest_addr, a, rgb);	break;
			case RGB565_ID:		draw_565(dest_addr, a, rgb);	break;
			case ARGB1555_ID:	draw_1555(dest_addr, a, rgb);	break;
			case ARGB4444_ID:	draw_4444(dest_addr, a, rgb);	break;
		}

		src_addr += src_offset;
		dest_addr += dest_offset;
	}
}
