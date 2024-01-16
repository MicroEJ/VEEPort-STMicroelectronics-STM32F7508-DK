/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef UI_DRAWING_DMA2D_CONFIGURATION_H
#define UI_DRAWING_DMA2D_CONFIGURATION_H

/**
 * @file
 * @brief This file provides the configuration of ui_drawing_dma2d.c.
 *
 * How to use this library:
 *  - Set the define DRAWING_DMA2D_CACHE_MANAGEMENT to DRAWING_DMA2D_CACHE_MANAGEMENT_DISABLED
 *	  or DRAWING_DMA2D_CACHE_MANAGEMENT_ENABLED depending on the MPU configuration
 * 		 
 *
 * @author MicroEJ Developer Team
 * @version 4.0.0
 */

#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include <stdint.h>

// --------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------

/*
 * @brief Value to disable the cache management
 * @see DRAWING_DMA2D_CACHE_MANAGEMENT 
 */ 
#define DRAWING_DMA2D_CACHE_MANAGEMENT_DISABLED (0U)

/*
 * @brief Value to enable the cache management
 * @see DRAWING_DMA2D_CACHE_MANAGEMENT 
 */ 
#define DRAWING_DMA2D_CACHE_MANAGEMENT_ENABLED  (1U)

#if !defined (__DCACHE_PRESENT) || (__DCACHE_PRESENT == 0U)

/*
 * @brief This MCU does not have or does not use a cache. Cache management in
 * ui_drawing_dma2d.c is disabled.
 */
#define DRAWING_DMA2D_CACHE_MANAGEMENT (DRAWING_DMA2D_CACHE_MANAGEMENT_DISABLED)

#else // !defined (__DCACHE_PRESENT) || (__DCACHE_PRESENT == 0U)

/*
 * @brief This MCU has a cache. Cache management must be configured.
 *
 * To ensure the best SDRAM performance, the SDRAM section must be configured
 * as cacheable, write-through and shareable. As per the application notes below, 
 * this is required to maintain cache coherency. If the section is not defined as shareable, 
 * cache maintenance has to be performed, which adds overhead to all dma2d transfers: 
 * cache clean before the transfer and cache invalidate after the transfer. 
 */

/*
 * @brief Cache management requirements are detailed in the following documents:
 * https://www.st.com/resource/en/application_note/an4838-introduction-to-memory-protection-unit-management-on-stm32-mcus-stmicroelectronics.pdf
 * https://www.st.com/resource/en/application_note/an4839-level-1-cache-on-stm32f7-series-and-stm32h7-series-stmicroelectronics.pdf
 *
 * Cache management is not required in the following cases:
 *
 * - MCU without cache (e.g Cortex-M4)
 *
 * - Memory region containing display buffers is non-cacheable e.g.:
 *      MPU_InitStruct.BaseAddress = 0xC0000000;
 *      MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
 *      MPU_InitStruct.IsBufferable = MPU_ACCESS_NON_BUFFERABLE;
 *      MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
 *      MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
 *      MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
 *      MPU_InitStruct.Number = MPU_REGION_NUMBER4;
 *      HAL_MPU_ConfigRegion(&MPU_InitStruct);
 *
 * - Memory region containing display buffers is cacheable, write-through, no write allocate, shareable e.g.: 
 *      MPU_InitStruct.BaseAddress = 0xC0000000;
 *      MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
 *      MPU_InitStruct.IsBufferable = MPU_ACCESS_NON_BUFFERABLE;
 *      MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
 *      MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
 *      MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
 *      MPU_InitStruct.Number = MPU_REGION_NUMBER4;
 * 
 * As per application note AN4839, in order to maintain cache coherency, the memory region containing 
 * the display buffers and MicroUI images heap have to be configured as write-through and shareable,
 * otherwise cache maintenance operations have to be performed.
 */
#define DRAWING_DMA2D_CACHE_MANAGEMENT (DRAWING_DMA2D_CACHE_MANAGEMENT_DISABLED)

/*
 * @brief Cache management is required in the following cases:
 *
 * - MCU with cache (e.g. Cortex-M7) and the memory is configured as follows:
 *
 *      - Memory region containing display buffers configured as write-back e.g.:
 *          MPU_InitStruct.BaseAddress = 0xC0000000;
 *          MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
 *          MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
 *          MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
 *          MPU_InitStruct.IsShareable = MPU_ACCESS_NON_SHAREABLE;
 *          MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
 *          MPU_InitStruct.Number = MPU_REGION_NUMBER4;
 *
 *      - Memory region containing display buffers configured as write-through, non-shareable e.g.:
 *          MPU_InitStruct.BaseAddress = 0xC0000000;
 *          MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
 *          MPU_InitStruct.IsBufferable = MPU_ACCESS_NON_BUFFERABLE;
 *          MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
 *          MPU_InitStruct.IsShareable = MPU_ACCESS_NON_SHAREABLE;
 *          MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
 *          MPU_InitStruct.Number = MPU_REGION_NUMBER4;
 * 
 * Enabling cache management will reduce the performance of the UI, as each dma2d transfer requires 
 * cache clean before the transfer and cache invalidate after the transfer to maintain cache coherency.
 * 
 * Cache invalidation is performed in the DMA2D ISR, so for this reason the dma2d drawing code is placed 
 * in the ITCM section to minimize the impact on performance. 
 */
//#define DRAWING_DMA2D_CACHE_MANAGEMENT (DRAWING_DMA2D_CACHE_MANAGEMENT_ENABLED)

#endif // !defined (__DCACHE_PRESENT) || (__DCACHE_PRESENT == 0U)

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // UI_DRAWING_DMA2D_CONFIGURATION_H

