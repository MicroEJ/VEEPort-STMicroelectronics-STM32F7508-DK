/*
 * C
 *
 * Copyright 2017-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef  ASYNC_SELECT_CACHE_H
#define  ASYNC_SELECT_CACHE_H

#include <stdint.h>
#include <sni.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Initialize the socket timeout cache
 */
void async_select_init_socket_timeout_cache(void);

/**
 * @brief Get a socket timeout from the cache
 *
 * @param[in] fd the socket file descriptor
 *
 * @return socket timeout on success, a negative value on error.
 */
int32_t async_select_get_socket_timeout_from_cache(int32_t fd);

/**
 * @brief Get a socket absolute timeout from the cache (time when the async_select task should unblock the select)
 *
 * @param[in] fd the socket file descriptor
 *
 * @return socket absolute timeout on success, a negative value on error.
 */
int64_t async_select_get_socket_absolute_timeout_from_cache(int32_t fd);

/**
 * @brief Set the given socket's timeout in the cache
 *
 * @param[in] fd the socket file descriptor
 * @param[in] timeout socket timeout
 */
void async_select_set_socket_timeout_in_cache(int32_t fd, int32_t timeout);

/**
 * @brief Set the given socket's absolute timeout in the cache (time when the async_select task should unblock the select)
 *
 * @param[in] fd the socket file descriptor
 * @param[in] absolute_timeout socket absolute timeout
 */
void async_select_set_socket_absolute_timeout_in_cache(int32_t fd, int64_t absolute_timeout);

/**
 * @brief Remove a socket timeout from the cache
 *
 * @param[in] fd the socket file descriptor
 */
void async_select_remove_socket_timeout_from_cache(int32_t fd);

#ifdef __cplusplus
	}
#endif

#endif // ASYNC_SELECT_CACHE_H
