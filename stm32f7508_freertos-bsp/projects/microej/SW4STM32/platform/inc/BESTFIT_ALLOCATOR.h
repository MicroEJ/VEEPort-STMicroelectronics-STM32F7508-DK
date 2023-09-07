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
/**
 * General purpose bestfit allocator, 32 bits oriented,
 * Allowed to work in a continuous interval of ram [startAddress, endAddress[
 */
#ifndef _BESTFIT_ALLOCATOR
#define _BESTFIT_ALLOCATOR 
#include <intern/BESTFIT_ALLOCATOR.h>
#include <stdint.h>
#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Default Constructor
 */
void BESTFIT_ALLOCATOR_new(BESTFIT_ALLOCATOR* env);

// --------------------------------------------------------------------------------
// -                      Functions provided by the library                       -
// --------------------------------------------------------------------------------

/**
 * Initializes this allocator to work within the given memory space.
 * This function must be called once and prior to any other calls.
 * The given address range must not overlap the address 0x80000000.
 * @param startAddress memory start address.
 * @param endAddress memory end address (excluded).
 */
void BESTFIT_ALLOCATOR_initialize(BESTFIT_ALLOCATOR* env, int32_t startAddress, int32_t endAddress);

/**
 * Allocates a new block.
 * @param size the block size in bytes. If size is lower than or equals to 0, returns an empty block.
 * @return the address of the allocated block or <code>null<code> if out of memory
 */
void* BESTFIT_ALLOCATOR_allocate(BESTFIT_ALLOCATOR* env, int32_t size);

/**
 * Releases a block that has been previously allocated using {@link BESTFIT_ALLOCATOR_allocate}.
 * @param the address of the block to release
 */
void BESTFIT_ALLOCATOR_free(BESTFIT_ALLOCATOR* env, void* block);

#ifdef __cplusplus
	}
#endif
#endif
