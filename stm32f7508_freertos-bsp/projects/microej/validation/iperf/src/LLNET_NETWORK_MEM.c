/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(__GNUC__)
#include <unistd.h>
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#if defined(__GNUC__)
	/* With GCC toolchain we need to relocate the implicit heap region at runtime */
	extern int __NETWORK_HEAP_Start__;
#elif defined(__ICCARM__)
	/* With IAR toolchain the heap region was declared in the linker file */
#else
	#error "NETWORK heap not initialized for this toolchain !"
#endif

void LLNET_NETWORK_HEAP_initialize(void) {
	/**
	 * To use the same lwipopts.h as the main application, need to implement custom memory allocator.
	 * Initialize (if needed) the dedicated network heap which is placed in SDRAM.
	 */

#if defined( __GNUC__ )
	/* With GCC toolchain we need to relocate the implicit heap region at runtime */
	sbrk((uint32_t)&__NETWORK_HEAP_Start__ - (uint32_t)sbrk(0));
#elif defined(__ICCARM__)
	/* With IAR toolchain the heap region was declared in the linker file */
#else
	#error "NETWORK heap not initialized for this toolchain !"
#endif
}

void* LLNET_NETWORK_HEAP_allocate(int32_t size) {
	return malloc(size);
}

void* LLNET_NETWORK_HEAP_calloc(int32_t n, int32_t size) {
	return calloc(n, size);
}

void LLNET_NETWORK_HEAP_free(void* block) {
	free(block);
}

#ifdef __cplusplus
	}
#endif
