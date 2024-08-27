/*
 * C
 *
 * Copyright 2018-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>

#include "microej_allocator.h"


void* microej_malloc(size_t size)
{
    return malloc(size);
}

void* microej_calloc(size_t nmemb, size_t size)
{

    return calloc(nmemb, size);
}

void microej_free(void *ptr)
{
    free(ptr);
}

