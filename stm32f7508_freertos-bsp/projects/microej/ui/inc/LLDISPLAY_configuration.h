/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLDISPLAY_DISCO_CONFIGURATION
#define _LLDISPLAY_DISCO_CONFIGURATION

/* Defines -------------------------------------------------------------------*/

/**
 * Configure the LLDISPLAY number of bits per pixel (BPP).
 *
 * There are 3 available BPP modes:
 * 	- 16:		16 BPP, in 5-6-5 mode
 * 	- 24:		24 BPP, in 8-8-8 mode
 * 	- 32:		32 BPP, in 8-8-8-8 mode
 *
 * Only one BPP can be used at any time. This define allows to select the wanted
 * implementation:
 * 	- '16':		use 16 BPP
 * 	- '24':		use 24 BPP
 * 	- '32':		use 32 BPP
 *
 * According the chosen mode, there are some actions which must be done together:
 *
 * (1)	In MicroEJ, select the wanted display stack during the platform creation. In the display
 * 		configuration properties file of the configuration project
 * 		"[platform configuration project]/display/display.properties",
 * 		specify the expected BPP value and format (RGB565 or RGB888 or ARGB8888)
 *
 * (2)	Recompile the platform.
 *
 * (3)	Launch again the Java application against the new platform.
 *
 * (4)	Configure the define LLDISPLAY_BPP (below) with the wanted mode.
 *
 * (5)	Update the declaration of DISPLAY_MEM_SIZE (LLUI_DISPLAY.c).
 *
 * (6)	Update the MPU region of DISPLAY_MEM_SIZE (if it exists, LLUI_DISPLAY.c).
 *
 * (7)	Recompile your BSP.
 */
#define LLDISPLAY_BPP DRAWING_DMA2D_BPP

#endif
