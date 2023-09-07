/* 
 * C
 * 
 * Copyright 2008-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
/* 
 * This file is auto-generated - DO NOT EDIT IT
 */
/* 
 * IceTea to C mapping support. Shall not be directly included by clients.
 */
#ifndef _BESTFIT_ALLOCATOR_intern
#define _BESTFIT_ALLOCATOR_intern 
typedef struct BESTFIT_ALLOCATOR BESTFIT_ALLOCATOR;
#include <stdint.h>
#ifdef __cplusplus
	extern "C" {
#endif
#define BESTFIT_ALLOCATOR_new com_is2t_allocator_BestFitAllocator___new
#define BESTFIT_ALLOCATOR_initialize __icetea__virtual__com_is2t_allocator_BestFitAllocator___initialize__II
#define BESTFIT_ALLOCATOR_allocate __icetea__virtual__com_is2t_allocator_BestFitAllocator___allocate__I
#define BESTFIT_ALLOCATOR_free __icetea__virtual__com_is2t_allocator_BestFitAllocator___free__LiceTea_lang_Ram_2
struct BESTFIT_ALLOCATOR
{
	int32_t __icetea__id__;
	int32_t _reserved0;
	int32_t _reserved1;
	int32_t _reserved2;
};
#ifdef __cplusplus
	}
#endif
#endif
