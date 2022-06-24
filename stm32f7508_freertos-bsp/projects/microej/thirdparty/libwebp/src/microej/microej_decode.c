// Copyright 2021-2022 MicroEJ Corp. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found with this software.

#include "src/microej/microej_decode.h"
#include "src/webp/decode.h"

// -----------------------------------------------------------------------------

LLUI_DISPLAY_Status MICROEJ_DECODE_webp(uint8_t* addr, uint32_t length, MICROUI_ImageFormat expectedFormat, MICROUI_Image* data, bool* isFullyOpaque) {

	WebPDecoderConfig config;
	WebPBitstreamFeatures* const bitstream = &config.input;
	LLUI_DISPLAY_Status ret;

	// reset config
	if (!WebPInitDecoderConfig(&config)) {
		ret = LLUI_DISPLAY_NOK;
	}
	else {

		// retrieve image information
		VP8StatusCode status = WebPGetFeatures(addr, length, bitstream);
		if (status != VP8_STATUS_OK) {
			ret = LLUI_DISPLAY_NOK;
		}
		else {

			WEBP_CSP_MODE format;
			switch(expectedFormat) {
			default:
			case MICROUI_IMAGE_FORMAT_ARGB8888:
			case MICROUI_IMAGE_FORMAT_ARGB1555:
			case MICROUI_IMAGE_FORMAT_ARGB4444: // XXX to check with MODE_RGBA_4444
			case MICROUI_IMAGE_FORMAT_RGB888:
			case MICROUI_IMAGE_FORMAT_A8:
				format = MODE_BGRA;
				expectedFormat = MICROUI_IMAGE_FORMAT_ARGB8888;
				*isFullyOpaque = !(bitstream->has_alpha);
				break;
				// not functional yet
				//	case MICROUI_IMAGE_FORMAT_RGB565:
				//		format = MODE_RGB_565;
				//		*isFullyOpaque = true;
				// break;
			}

			// fill image data
			data->width = bitstream->width;
			data->height = bitstream->height;
			data->format = expectedFormat;

			// allocate space in images heap
			if (!LLUI_DISPLAY_allocateImageBuffer(data, 0)) {
				ret = LLUI_DISPLAY_OUT_OF_MEMORY;
			}
			else {

				uint8_t* ret_addr;
				if (MODE_RGB_565 == format)	{
					// not functional yet
					ret_addr = WebPDecodeBGRInto(addr, length, LLUI_DISPLAY_getBufferAddress(data), LLUI_DISPLAY_getStrideInBytes(data) * data->height, LLUI_DISPLAY_getStrideInBytes(data));
				}
				else {
					ret_addr = WebPDecodeBGRAInto(addr, length, LLUI_DISPLAY_getBufferAddress(data), LLUI_DISPLAY_getStrideInBytes(data) * data->height, LLUI_DISPLAY_getStrideInBytes(data));
				}

				ret = ret_addr != LLUI_DISPLAY_getBufferAddress(data) ? LLUI_DISPLAY_NOK : LLUI_DISPLAY_OK;
			}
		}
	}
	return ret;
}

//------------------------------------------------------------------------------
