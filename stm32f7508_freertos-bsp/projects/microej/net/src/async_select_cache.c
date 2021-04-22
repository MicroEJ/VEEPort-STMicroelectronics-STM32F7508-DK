/*
 * C
 *
 * Copyright 2017-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#include "async_select_cache.h"
#include "LLNET_Common.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Socket timeout cache structure
 */
typedef struct async_select_sock_timeout {
	int32_t timeout;
	int64_t absolute_timeout;
} async_select_sock_timeout;

/**
 * @brief Cache of socket timeouts. Used to reserve LLNET_MAX_SOCKETS structures.
 */
static async_select_sock_timeout async_select_sockets_timeout_cache[LLNET_MAX_SOCKETS];

void async_select_init_socket_timeout_cache(void)
{
	for (int32_t i = 0; i < LLNET_MAX_SOCKETS; i++) {
		async_select_remove_socket_timeout_from_cache(i + LLNET_SOCKFD_START_IDX);
	}
}

int32_t async_select_get_socket_timeout_from_cache(int32_t fd)
{
	fd = fd - LLNET_SOCKFD_START_IDX;

	// check if the file descriptor is valid
	if ((fd >= 0) && (fd < LLNET_MAX_SOCKETS)) {
		return async_select_sockets_timeout_cache[fd].timeout;
	} else {
		LLNET_DEBUG_TRACE("async_select_get_socket_timeout_from_cache: invalid file descriptor(%d)!\n", fd);
		return -1;
	}
}

int64_t async_select_get_socket_absolute_timeout_from_cache(int32_t fd)
{
	fd = fd - LLNET_SOCKFD_START_IDX;

	// check if the file descriptor is valid
	if ((fd >= 0) && (fd < LLNET_MAX_SOCKETS)) {
		return async_select_sockets_timeout_cache[fd].absolute_timeout;
	} else {
		LLNET_DEBUG_TRACE("async_select_get_socket_absolute_timeout_from_cache: invalid file descriptor(%d)!\n", fd);
		return -1;
	}
}

void async_select_set_socket_timeout_in_cache(int32_t fd, int32_t timeout)
{
	fd = fd - LLNET_SOCKFD_START_IDX;

	// check if the file descriptor is valid
	if ((fd >= 0) && (fd < LLNET_MAX_SOCKETS)) {
		async_select_sockets_timeout_cache[fd].timeout = timeout;
	} else {
		LLNET_DEBUG_TRACE("async_select_set_socket_timeout_in_cache: invalid file descriptor(%d)!\n", fd);
	}
}

void async_select_set_socket_absolute_timeout_in_cache(int32_t fd, int64_t absolute_timeout)
{
	fd = fd - LLNET_SOCKFD_START_IDX;

	// check if the file descriptor is valid
	if ((fd >= 0) && (fd < LLNET_MAX_SOCKETS)) {
		async_select_sockets_timeout_cache[fd].absolute_timeout = absolute_timeout;
	} else {
		LLNET_DEBUG_TRACE("async_select_set_socket_absolute_timeout_in_cache: invalid file descriptor(%d)!\n", fd);
	}
}

void async_select_remove_socket_timeout_from_cache(int32_t fd)
{
	fd = fd - LLNET_SOCKFD_START_IDX;

	// check if the file descriptor is valid
	if ((fd >= 0) && (fd < LLNET_MAX_SOCKETS)) {
		async_select_sockets_timeout_cache[fd].timeout = -1;
		async_select_sockets_timeout_cache[fd].absolute_timeout = -1;
	} else {
		LLNET_DEBUG_TRACE("async_select_remove_socket_timeout_from_cache: invalid file descriptor(%d)!\n", fd);
	}
}

#ifdef __cplusplus
	}
#endif
