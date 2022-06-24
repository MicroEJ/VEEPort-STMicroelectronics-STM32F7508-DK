// Copyright 2021-2022 MicroEJ Corp. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found with this software.
// -----------------------------------------------------------------------------
//
// Replace the allocator functions WebPSafe*: the original allocator is using
// malloc and calloc functions (see src/utils/utils.c). The MicroEJ allocator
// uses the Graphics Engine's heap allocator.

// get allocator prototypes
#include "src/utils/utils.h"

#include "LLUI_DISPLAY_IMPL.h"
#include "microui_heap.h"

// -----------------------------------------------------------------------------

void* WebPSafeMalloc(uint64_t nmemb, size_t size) {
  assert(nmemb * size > 0);
  return (void*)LLUI_DISPLAY_IMPL_image_heap_allocate((uint32_t)(nmemb * size));
}

void* WebPSafeCalloc(uint64_t nmemb, size_t size) {
  void* ptr;
  assert(nmemb * size > 0);
  ptr = (void*)LLUI_DISPLAY_IMPL_image_heap_allocate((uint32_t)(nmemb * size));
  if (!ptr) return NULL;
  memset(ptr, 0, (size_t)(nmemb * size));
  return ptr;
}

void WebPSafeFree(void* const ptr) {
  if (ptr != NULL) {
    LLUI_DISPLAY_IMPL_image_heap_free((uint8_t*)ptr);
  }
}

//------------------------------------------------------------------------------
