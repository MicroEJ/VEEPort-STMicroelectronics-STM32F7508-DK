/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 *
 */

#ifndef  FS_CONFIGURATION_H
#define  FS_CONFIGURATION_H

/**
 * @file
 * @brief LLFS configuration.
 * @author MicroEJ Developer Team
 * @version 1.1.0
 * @date 27 May 2020
 */

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
#if defined(__QNXNTO__)
	#define FS_WORKER_STACK_SIZE (1024*16) // stack requires more space on QNX
#else
	#define FS_WORKER_STACK_SIZE (1024*2)
#endif

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
	 * @brief Set this define to print debug traces.
	 */
//#define	LLFS_DEBUG
#ifdef __cplusplus
	}
#endif

#endif // FS_CONFIGURATION_H
