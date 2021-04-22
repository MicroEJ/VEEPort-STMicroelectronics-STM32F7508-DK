/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "BESTFIT_ALLOCATOR.h"
#include <sni.h>
#include <stdio.h>
#include <string.h>
#include "bsp_util.h"

#ifdef __cplusplus
	extern "C" {
#endif

// Define size to allocate for Network Heap
#define NETWORK_HEAP_SIZE 0x40000

// Declare Network Heap
uint8_t network_heap[NETWORK_HEAP_SIZE] __attribute__((section(".NetworkHeap")));

//NETWORK memory allocator
BESTFIT_ALLOCATOR NETWORK_allocatorInstance;
// NETWORK boolean to prevent a double initialization of the NETWORK Memory.
static int initialized = 0;

/* API -----------------------------------------------------------------------*/

void LLNET_NETWORK_HEAP_initialize(){
	if (! initialized) {
		BESTFIT_ALLOCATOR_new(&NETWORK_allocatorInstance);
		BESTFIT_ALLOCATOR_initialize(&NETWORK_allocatorInstance, (int32_t)(&network_heap[0]), (int32_t)(&network_heap[NETWORK_HEAP_SIZE]));
		initialized = 1;
		#ifdef ALLOCATOR_DEBUG
			printf("LLNET_NETWORK_HEAP_init %p - %p \n", (void *)(&network_heap[0]), (void *)(&network_heap[NETWORK_HEAP_SIZE]));
		#endif
	}
}

void* LLNET_NETWORK_HEAP_allocate(int32_t size){
    void* allocationStart = BESTFIT_ALLOCATOR_allocate(&NETWORK_allocatorInstance, size);
#ifdef ALLOCATOR_DEBUG
	printf("LLNET_NETWORK_HEAP_allocate %d %p \n", size, allocationStart);
#endif
    return (void*)allocationStart;
}

void* LLNET_NETWORK_HEAP_calloc(int32_t n, int32_t size){
    void* allocationStart = BESTFIT_ALLOCATOR_allocate(&NETWORK_allocatorInstance, size * n);

    if (allocationStart != NULL  )
    {
       memset (allocationStart, 0 , size * n);
    }

#ifdef ALLOCATOR_DEBUG
	printf("LLNET_NETWORK_HEAP_calloc %d %p \n", size * n, allocationStart);
#endif
    return (void*)allocationStart;
}


void LLNET_NETWORK_HEAP_free(void* block){
	if(block){
		BESTFIT_ALLOCATOR_free(&NETWORK_allocatorInstance, block);
	}
#ifdef ALLOCATOR_DEBUG
    printf("LLNET_NETWORK_HEAP_free %p \n", block);
#endif
}

#ifdef __cplusplus
	}
#endif
