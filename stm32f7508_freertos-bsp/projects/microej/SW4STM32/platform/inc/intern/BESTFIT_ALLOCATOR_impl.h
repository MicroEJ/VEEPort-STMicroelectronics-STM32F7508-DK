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
#include <stdint.h>
#define BESTFIT_ALLOCATOR_CONCAT0(p,s) p ## s
#define BESTFIT_ALLOCATOR_CONCAT(p,s) BESTFIT_ALLOCATOR_CONCAT0(p,s)
#define __icetea__nid__BESTFIT_ALLOCATOR___BESTFIT_ALLOCATOR_IMPL BESTFIT_ALLOCATOR_CONCAT(__icetea__nid__BESTFIT_ALLOCATOR___,BESTFIT_ALLOCATOR_IMPL)
#define BESTFIT_ALLOCATOR_IMPL_new BESTFIT_ALLOCATOR_CONCAT(BESTFIT_ALLOCATOR_IMPL,_new)
void __icetea__nid__BESTFIT_ALLOCATOR___BESTFIT_ALLOCATOR_IMPL(void);
void* com_is2t_allocator_BestFitAllocator____4init_5(void* env);

/**
 * Default Constructor
 */
void BESTFIT_ALLOCATOR_IMPL_new(BESTFIT_ALLOCATOR_IMPL* env)
{
	((BESTFIT_ALLOCATOR*)env)->__icetea__id__ = (int32_t)(&__icetea__nid__BESTFIT_ALLOCATOR___BESTFIT_ALLOCATOR_IMPL);
	com_is2t_allocator_BestFitAllocator____4init_5(env);
}
