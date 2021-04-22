/*
 * C
 *
 * Copyright 2017-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef  ASYNC_SELECT_CONFIGURATION_H
#define  ASYNC_SELECT_CONFIGURATION_H

/**
 * @file
 * @brief Asynchronous network select configuration.
 * @author MicroEJ Developer Team
 * @version 1.0.1
 * @date 19 February 2020
 */

#include <stdint.h>
#include <sni.h>

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
#define ASYNC_SELECT_CONFIGURATION_VERSION (2)

/**
 * @brief Maximum number of asynchronous select that can be done at the same moment.
 */
#define MAX_NB_ASYNC_SELECT (16)

/**
 * @brief async_select task stack size in bytes.
 */
#define ASYNC_SELECT_TASK_STACK_SIZE (2048)

/**
 * @brief async_select task name.
 */
#define ASYNC_SELECT_TASK_NAME	(uint8_t *)"AsyncSelect"

/**
 * @brief async_select task priority.
 */
#define ASYNC_SELECT_TASK_PRIORITY	(12)

/**
 * @brief async_select mutex name.
 */
#define ASYNC_SELECT_MUTEX_NAME	(uint8_t *)"AsyncSelectMutex"

/**
 * @brief Timeout in milliseconds used when the async_select task cannot allocate a socket for notifications.
 *
 * In async_select task a socket is created to notify the task and unlock the select on demand. If this socket cannot
 * be created, then the async_select task polls for notification. This constant defines the wait time in milliseconds
 * between each poll.
 */
#define ASYNC_SELECT_POLLING_MODE_TIMEOUT_MS	(100)


/** @brief Set this define if a file descriptor close unblocks the select. */
#ifndef __linux__
#define ASYNC_SELECT_CLOSE_UNBLOCK_SELECT
#endif

/**
 * @brief On Linux, a close() operation does not unlock a select().
 * Instead of using a close on notify_fd_cache we will
 * use a pipe.
 */
#if defined(__linux__) || defined(__QNXNTO__)
#define ASYNC_SELECT_USE_PIPE_FOR_NOTIFICATION
#endif

#ifdef __cplusplus
	}
#endif

#endif // ASYNC_SELECT_CONFIGURATION_H
