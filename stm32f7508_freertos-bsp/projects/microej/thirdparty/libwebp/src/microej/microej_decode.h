// Copyright 2021-2022 MicroEJ Corp. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found with this software.

#ifndef WEBP_MICROEJ_MICROEJ_DECODE_H_
#define WEBP_MICROEJ_MICROEJ_DECODE_H_

#include "LLUI_DISPLAY_IMPL.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// MicroEJ WEBP decoder entry point

LLUI_DISPLAY_Status MICROEJ_DECODE_webp(uint8_t* addr, uint32_t length, MICROUI_ImageFormat expectedFormat, MICROUI_Image* data, bool* isFullyOpaque);

//------------------------------------------------------------------------------

#ifdef __cplusplus
}    // extern "C"
#endif

#endif  // WEBP_MICROEJ_MICROEJ_DECODE_H_
