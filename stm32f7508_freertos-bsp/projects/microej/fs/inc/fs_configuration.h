/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef  FS_CONFIGURATION_H
#define  FS_CONFIGURATION_H

/**
 * @file
 * @brief LLFS configuration.
 * @author MicroEJ Developer Team
 * @version 2.1.0
 * @date 17 June 2022
 */

#include "microej_async_worker.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Compatibility sanity check value.
 * This define value is checked in the implementation to validate that the version of this configuration
 * is compatible with the implementation.
 *
 * This value must not be changed by the user of the CCO.
 * This value must be incremented by the implementor of the CCO when a configuration define is added, deleted or modified.
 */
#define FS_CONFIGURATION_VERSION (1)

/**
 * @brief Set this define to use a custom worker to handle FS asynchronous jobs.
 */
//#define FS_CUSTOM_WORKER

/**
 * @brief Define FS custom worker if required.
 */
#ifdef FS_CUSTOM_WORKER
extern MICROEJ_ASYNC_WORKER_handle_t my_custom_worker;
#define fs_worker my_custom_worker
#else
extern MICROEJ_ASYNC_WORKER_handle_t fs_worker;
#endif

/**
 * @brief Number of workers dedicated to the FS in async_worker.
 */
#define FS_WORKER_JOB_COUNT (4)

/**
 * @brief Size of the waiting list for FS jobs in async_worker.
 */
#define FS_WAITING_LIST_SIZE (16)

/**
 * @brief Size of the FS stack in bytes.
 */
#define FS_WORKER_STACK_SIZE (1024 * 2)

/**
 * @brief Initialization function for STM32 SD card.
 * 
 */
void LLFS_STM32_init_sdcard(void);

/**
 * @brief Use this macro to define the initialization function of the file system stack.
 * Called from LLFS_IMPL_initialize().
 */
#define llfs_init LLFS_STM32_init_sdcard

/**
 * @brief FS worker stack size must be calibrated, unless using a custom worker defined in another module.
 */
#if !defined(FS_CUSTOM_WORKER) && !defined(FS_WORKER_STACK_SIZE)
#error "FS_WORKER_STACK_SIZE not declared. Please uncomment the line above to enable macro declaration and put the right value according to the stack size calibration done for your environment"
#endif // FS_WORKER_STACK_SIZE

/**
 * @brief Priority of the FS workers.
 */
#define FS_WORKER_PRIORITY (6)

/**
 * @brief Maximum path length.
 */
#define FS_PATH_LENGTH (256)

/**
 * @brief Size of the IO buffer in bytes.
 */
#define FS_IO_BUFFER_SIZE (2048)

/**
 * @brief Copies a file path from an input buffer to another buffer that will be sent to
 * the async_worker job, checking against path size constraints.
 *
 * @param[in] path caller buffer where the file path is stored.
 * @param[in] path_param buffer where the file path will be copied if the path size constraints are solved.
 *
 * @return <code>LLFS_OK</code> on success, else a negative error code.
 */
int32_t LLFS_set_path_param(uint8_t* path, uint8_t* path_param);

/**
 * @brief Set this define to print debug traces.
 */
//#define LLFS_DEBUG

#ifdef LLFS_DEBUG
#include <stdio.h>
#define LLFS_DEBUG_TRACE printf("[DEBUG] ");printf
#else
#define LLFS_DEBUG_TRACE(...) ((void) 0)
#endif

#ifdef __cplusplus
	}
#endif

#endif // FS_CONFIGURATION_H
