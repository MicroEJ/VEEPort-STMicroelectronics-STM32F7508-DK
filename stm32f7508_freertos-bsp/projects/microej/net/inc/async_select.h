/*
 * C
 *
 * Copyright 2017-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef  ASYNC_SELECT_H
#define  ASYNC_SELECT_H

/**
 * @file
 * @brief Asynchronous network select API
 * @author MicroEJ Developer Team
 * @version 3.0.1
 * @date 13 October 2023
 */

#include <stdint.h>
#include <sni.h>

#ifdef __cplusplus
	extern "C" {
#endif

/** @brief Select operations list. */
typedef enum
{
  SELECT_READ,
  SELECT_WRITE
}select_operation;


/**
 * @brief Executes asynchronously an I/0 operation on the given file descriptor.
 * This function creates an asynchronous select request for the given file descriptor and then suspends the execution
 * of the current Java thread using SNI_suspendCurrentJavaThreadWithCallback().
 * Once the file descriptor of the request will be ready for the given operation or the timeout is reached, the Java
 * thread is resumed and the given SNI callback is called.
 *
 * <code>absolute_timeout_ms</code> is an absolute time in milliseconds computed from the system time returned by
 * <code>LLMJVM_IMPL_getCurrentTime(1)</code>. A timeout of zero is interpreted as an infinite timeout.
 *
 * @param[in] fd the file descriptor.
 * @param[in] operation the operation (read or write) we want to monitor with the select().
 * @param[in] absolute_timeout_ms the absolute timeout in millisecond or 0 if no timeout.
 * @param[in] callback the SNI callback to call when the Java thread is resumed or timeout occurs.
 * @param[in] callback_suspend_arg the SNI suspend callback argument.
 *
 * @return 0 on success, -1 on failure.
 *
 * @note Throws NativeIOException on failure.
 *
 * @warning: This function needs to register the created asynchronous select request as a scoped native resource.
 * Since several scoped native resources cannot be registered in the same native context, the function <code>SNI_unregisterScopedResource()</code>
 * is called to unregister a potential existing scoped resource before registering the new one.
 * Make sure that no scoped resource is registered before calling this function otherwise it will be unregistered.
 * <code>SNI_getScopedResource()</code> can be called to check if there is an existing scoped resource.
 */
int32_t async_select(int32_t fd, select_operation operation, int64_t absolute_timeout_ms, SNI_callback callback, void* callback_suspend_arg);

/**
 * @brief Initialize the async_select component. This function must be called prior to any call of
 * async_select().
 *
 * @return 0 on success, -1 on failure.
 */
int32_t async_select_init(void);

/**
 * @brief Notifies the async_select task that a file descriptor has been closed.
 * On some systems the close of a file descriptor does not unblock the select that's
 * why we need to notify the async_select task.
 */
void async_select_notify_closed_fd(int32_t fd);

/**
 * @brief Notifies a new event on the file descriptor.
 * This function is called when the file descriptor becomes ready for read or write operation.
 * A file descriptor is ready when it is possible to perform the corresponding I/O operation without blocking.
 *
 * @param[in] fd The file descriptor.
 * @param[in] on_read true if the file descriptor is ready for "read" operation; false otherwise.
 * @param[in] on_write true if the file descriptor is ready for "write" operation; false otherwise.
 * @param[in] on_error true if an error has occurred on the file descriptor; In this case, read and write will not block and will result in an error.
 */
void async_select_update_notified_requests(int32_t fd, uint8_t on_read, uint8_t on_write, uint8_t on_error);

#ifdef __cplusplus
	}
#endif

#endif // ASYNC_SELECT_H
