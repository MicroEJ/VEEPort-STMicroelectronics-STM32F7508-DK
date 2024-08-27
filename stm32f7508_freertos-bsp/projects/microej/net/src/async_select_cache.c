/*
 * C
 *
 * Copyright 2017-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief Socket timeout cache implementation
 * @author MicroEJ Developer Team
 * @version 2.3.1
 * @date 16 May 2022
 */

#include "async_select_cache.h"
#include "LLNET_Common.h"
#include "async_select_configuration.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Socket timeout cache structure
 */
typedef struct async_select_sock_timeout {
	int32_t fd;
	int32_t timeout;
	int64_t absolute_timeout;
} async_select_sock_timeout;

/**
 * @brief Cache of socket timeouts. Used to reserve ASYNC_SELECT_TIMEOUT_CACHE_SIZE structures.
 */
static async_select_sock_timeout async_select_sockets_timeout_cache[ASYNC_SELECT_TIMEOUT_CACHE_SIZE];

static int32_t async_select_get_socket_index(int32_t fd)
{
	int32_t index = -1;

	for (int32_t i = 0; i < ASYNC_SELECT_TIMEOUT_CACHE_SIZE; i++) {
		if (async_select_sockets_timeout_cache[i].fd == fd) {
			index = i;
			break;
		}
	}
	return index;
}

static int32_t async_select_add_socket_to_cache(int32_t fd)
{
	int32_t index = -1;
	for (int32_t i = 0; i < ASYNC_SELECT_TIMEOUT_CACHE_SIZE; i++) {
		if (async_select_sockets_timeout_cache[i].fd == -1) {
			async_select_sockets_timeout_cache[i].fd = fd;
			index = i;
			break;
		}
	}

	if (index == ASYNC_SELECT_TIMEOUT_CACHE_SIZE) {
		LLNET_DEBUG_TRACE("async_select_add_socket: too many file descriptors in cache!\n");
	}
	return index;
}

void async_select_remove_socket_timeout_from_cache(int32_t fd)
{
	int32_t idx = async_select_get_socket_index(fd);

	// check if the file descriptor is valid
	if (idx >= 0) {
		async_select_sockets_timeout_cache[idx].fd = -1;
		async_select_sockets_timeout_cache[idx].timeout = -1;
		async_select_sockets_timeout_cache[idx].absolute_timeout = -1;
	} else {
		LLNET_DEBUG_TRACE("async_select_remove_socket_timeout_from_cache: invalid file descriptor(%d)!\n", fd);
	}
}

void async_select_init_socket_timeout_cache(void)
{
	for (int32_t i = 0; i < ASYNC_SELECT_TIMEOUT_CACHE_SIZE; i++) {
		async_select_sockets_timeout_cache[i].fd = -1;
		async_select_sockets_timeout_cache[i].timeout = -1;
		async_select_sockets_timeout_cache[i].absolute_timeout = -1;
	}
}

int32_t async_select_get_socket_timeout_from_cache(int32_t fd)
{
	int32_t idx = async_select_get_socket_index(fd);

	// check if the file descriptor was found
	if (idx >= 0) {
		return async_select_sockets_timeout_cache[idx].timeout;
	} else {
		return -1;
	}
}

int64_t async_select_get_socket_absolute_timeout_from_cache(int32_t fd)
{
	int32_t idx = async_select_get_socket_index(fd);

	// check if the file descriptor was found
	if (idx >= 0) {
		return async_select_sockets_timeout_cache[idx].absolute_timeout;
	} else {
		return -1;
	}
}

void async_select_set_socket_timeout_in_cache(int32_t fd, int32_t timeout)
{
	int32_t idx = async_select_get_socket_index(fd);

	// check if the file descriptor was found
	if (idx < 0) {
		idx = async_select_add_socket_to_cache(fd);
	}

	if (idx >= 0) {
		async_select_sockets_timeout_cache[idx].timeout = timeout;
	}
}

int32_t async_select_set_socket_absolute_timeout_in_cache(int32_t fd, int64_t absolute_timeout)
{
	int32_t idx = async_select_get_socket_index(fd);

	// check if the file descriptor was found
	if (idx < 0) {
		idx = async_select_add_socket_to_cache(fd);
	}

	if (idx >= 0) {
		async_select_sockets_timeout_cache[idx].absolute_timeout = absolute_timeout;
	} else {
		return -1;
	}
	return 0;
}

#ifdef __cplusplus
	}
#endif
