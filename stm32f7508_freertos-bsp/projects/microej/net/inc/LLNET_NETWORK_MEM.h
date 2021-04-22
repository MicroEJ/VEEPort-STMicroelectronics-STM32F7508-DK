/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef __LLNET_NETWORK_HEAP_H__
#define __LLNET_NETWORK_HEAP_H__

#include "stdint.h"


/* Includes ------------------------------------------------------------------*/

/* API -----------------------------------------------------------------------*/

void LLNET_NETWORK_HEAP_initialize();

void* LLNET_NETWORK_HEAP_allocate(int32_t size);

void* LLNET_NETWORK_HEAP_calloc(int32_t n, int32_t size);

void LLNET_NETWORK_HEAP_free(void* block);

#endif

