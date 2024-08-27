/*
 * C
 *
 * Copyright 2017-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef  ASYNC_SELECT_CONFIGURATION_H
#define  ASYNC_SELECT_CONFIGURATION_H

/**
 * @file
 * @brief Asynchronous network select configuration.
 * @author MicroEJ Developer Team
 * @version 3.0.1
 * @date 13 October 2023
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
#define ASYNC_SELECT_CONFIGURATION_VERSION (4)


#define ASYNC_SELECT_TIMEOUT_CACHE_SIZE LLNET_MAX_SOCKETS
/*
 * Uncomment this define if you don't want to use the mode where a async_select thread that waits on select()
 * function is created to manage asynchronous operation.
 */
#define USE_ASYNC_SELECT_THREAD

#ifndef USE_ASYNC_SELECT_THREAD
#error "Since you don't choose async_select thread mode that calls select() to manage asynchronous operation, \
		please ensure that you add a call to the function async_select_update_notified_requests(...) \
		in the event callback system of your NET stack, to notify the waiting java threads when the file descriptor (fd) becomes ready for reading or writing operation. \
		For instance, in LWIP stack, you can call async_select_update_notified_requests(...) in the event_callback(...) function located in the sockets.c file.	\
		Remove this #error once you add the call to this method in your I/O event callback system."
#endif

/**
 * @brief Maximum number of asynchronous operation that can be requested at the same moment.
 */
#define MAX_NB_ASYNC_SELECT (16)

/**
 * @brief async_select task stack size in bytes.
 *
 * Requires: USE_ASYNC_SELECT_THREAD
 */
#define ASYNC_SELECT_TASK_STACK_SIZE (2048)

/**
 * @brief async_select task name.
 *
 * Requires: USE_ASYNC_SELECT_THREAD
 */
#define ASYNC_SELECT_TASK_NAME	((uint8_t*)"AsyncSelect")

/**
 * @brief async_select task priority.
 *
 * Requires: USE_ASYNC_SELECT_THREAD
 */
#define ASYNC_SELECT_TASK_PRIORITY	(12)

/**
 * @brief async_select mutex name.
 *
 * Requires: USE_ASYNC_SELECT_THREAD
 */
#define ASYNC_SELECT_MUTEX_NAME	((uint8_t*)"AsyncSelectMutex")

/**
 * @brief Timeout in milliseconds used when the async_select task cannot allocate a socket for notifications.
 *
 * In async_select task a socket is created to notify the task and unlock the select on demand. If this socket cannot
 * be created, then the async_select task polls for notification. This constant defines the wait time in milliseconds
 * between each poll.
 *
 * Requires: USE_ASYNC_SELECT_THREAD
 */
#define ASYNC_SELECT_POLLING_MODE_TIMEOUT_MS	(100)


/** @brief Set this define if a file descriptor close unblocks the select.
 *
 * Requires: USE_ASYNC_SELECT_THREAD
 */
#ifndef __linux__
#define ASYNC_SELECT_CLOSE_UNBLOCK_SELECT
#endif

/**
 * @brief On Linux, a close() operation does not unlock a select().
 * Instead of using a close on notify_fd_cache we will
 * use a pipe.
 *
 * Requires: USE_ASYNC_SELECT_THREAD
 */
#if defined(__linux__) || defined(__QNXNTO__)
#define ASYNC_SELECT_USE_PIPE_FOR_NOTIFICATION
#endif

/**
 * @brief On some systems, using a NULL pointer as the timeout parameter
 * for a select with infinite timeout does not work, so in that case use
 * a valid timeout parameter with maximum values as defined by the formula.
 * timeout_ms = (int32_t)(timeout->tv_usec/1000 + timeout->tv_sec*1000);
 */
//#define ASYNC_SELECT_USE_MAX_INFINITE_TIMEOUT

#ifdef ASYNC_SELECT_USE_MAX_INFINITE_TIMEOUT
#define ASYNC_SELECT_INFINITE_TIMEOUT_MS (0xFFFFFFFF)
#define ASYNC_SELECT_MAX_TV_SEC_VALUE    (ASYNC_SELECT_INFINITE_TIMEOUT_MS / 1000)
#define ASYNC_SELECT_MAX_TV_USEC_VALUE   ((ASYNC_SELECT_INFINITE_TIMEOUT_MS - ASYNC_SELECT_MAX_TV_SEC_VALUE * 1000) * 1000)
#endif // ASYNC_SELECT_USE_MAX_INFINITE_TIMEOUT

/**
 * @brief The inaddr used to bind the notify socket.
 */
#define ASYNC_SELECT_NOTIFY_SOCKET_BIND_INADDR  (INADDR_LOOPBACK)

/**
 * @brief The in6addr used to bind the notify socket.
 */
#define ASYNC_SELECT_NOTIFY_SOCKET_BIND_IN6ADDR (in6addr_loopback)


#ifdef __cplusplus
	}
#endif

#endif // ASYNC_SELECT_CONFIGURATION_H
