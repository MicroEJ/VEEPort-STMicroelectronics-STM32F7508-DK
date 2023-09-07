/*
 * C
 *
 * Copyright 2013-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Includes ------------------------------------------------------------------*/

#include "LLKERNEL_impl.h"
#include "BESTFIT_ALLOCATOR.h"
#include <stdio.h>
#include <string.h>
#include "bsp_util.h"

// Define size to allocate for KF working buffer
#define KERNEL_WORKING_BUFFER_SIZE 0x300000

// Declare MicroEJ KF working buffer
uint8_t kernel_working_buffer[KERNEL_WORKING_BUFFER_SIZE] __attribute__((section(".KfHeap")));

BESTFIT_ALLOCATOR allocatorInstance;
uint32_t KERNEL_allocationInitialized;

/* API -----------------------------------------------------------------------*/

void* LLKERNEL_IMPL_allocateWorkingBuffer(int32_t size) {
    if(KERNEL_allocationInitialized == 0){
		// lazy init
		BESTFIT_ALLOCATOR_new(&allocatorInstance);
		BESTFIT_ALLOCATOR_initialize(&allocatorInstance, (int32_t)(&kernel_working_buffer[0]), (int32_t)(&kernel_working_buffer[KERNEL_WORKING_BUFFER_SIZE]));
		KERNEL_allocationInitialized = 1;
	}

        void* allocationStart = BESTFIT_ALLOCATOR_allocate(&allocatorInstance, size);
#ifdef ALLOCATOR_DEBUG
	printf("LLKERNEL_IMPL_allocate %d %p \n", size, allocationStart);
#endif
        return (void*)allocationStart;
}

void LLKERNEL_IMPL_freeWorkingBuffer(void* chunk_address) {
   BESTFIT_ALLOCATOR_free(&allocatorInstance, chunk_address);
#ifdef ALLOCATOR_DEBUG
    printf("LLKERNEL_IMPL_free %p \n", block);
#endif
}


void BSP_UTIL_assert(int a){
	while (1);
}

int32_t LLKERNEL_IMPL_allocateFeature(int32_t size_ROM, int32_t size_RAM) {
   return 0;
}

int32_t LLKERNEL_IMPL_getAllocatedFeaturesCount(void) {
   return 0;
}

void LLKERNEL_IMPL_freeFeature(int32_t handle) {
   BSP_UTIL_assert(0);
}

int32_t LLKERNEL_IMPL_getFeatureHandle(int32_t allocation_index) {
   BSP_UTIL_assert(0);
   return 0;
}

void* LLKERNEL_IMPL_getFeatureAddressRAM(int32_t handle) {
   BSP_UTIL_assert(0);
   return 0;
}

void* LLKERNEL_IMPL_getFeatureAddressROM(int32_t handle) {
   BSP_UTIL_assert(0);
   return 0;
}

int32_t LLKERNEL_IMPL_copyToROM(void* dest_address_ROM, void* src_address, int32_t size) {
   BSP_UTIL_assert(0);
   return 0;
}

int32_t LLKERNEL_IMPL_flushCopyToROM(void) {
   BSP_UTIL_assert(0);
   return 0;
}

int32_t LLKERNEL_IMPL_onFeatureInitializationError(int32_t handle, int32_t error_code) {
   BSP_UTIL_assert(0);
   return 0;
}
