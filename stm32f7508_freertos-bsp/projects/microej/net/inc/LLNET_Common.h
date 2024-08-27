/*
 * C
 *
 * Copyright 2016-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief Common LLNET macro and functions.
 * @author MicroEJ Developer Team
 * @version 2.0.0
 * @date 17 June 2022
 */

#ifndef  LLNET_COMMON_H
#define  LLNET_COMMON_H


#include <stdint.h>
#include <stdbool.h>
#include <sni.h>
#include "LLNET_configuration.h"
#include "async_select.h"
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>


#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Sanity check between the expected version of the configuration and the actual version of
 * the configuration.
 * If an error is raised here, it means that a new version of the CCO has been installed and
 * the configuration LLNET_configuration.h must be updated based on the one provided
 * by the new CCO version.
 */
#if LLNET_CONFIGURATION_VERSION != 3
	#error "Version of the configuration file LLNET_configuration.h is not compatible with this implementation."
#endif

// Define LLNET_DEBUG in LLNET_configuration.h to enable network debug trace
#ifdef LLNET_DEBUG
	#include <stdio.h>
	#define LLNET_DEBUG_TRACE 	printf
#else
	#define LLNET_DEBUG_TRACE(...) ((void) 0)
#endif

/**
 * @brief External function used to retrieve currentTime (defined in LLMJVM)
 */
extern int64_t LLMJVM_IMPL_getCurrentTime__Z(uint8_t system);
#define LLNET_current_time_ms()	LLMJVM_IMPL_getCurrentTime__Z(1) // 1 means that system time is required

union llnet_sockaddr{
	struct sockaddr addr;
#if LLNET_AF & LLNET_AF_IPV4
	struct sockaddr_in in;
#endif
#if LLNET_AF & LLNET_AF_IPV6
	struct sockaddr_in6 in6;
#endif
};

/**
 * @brief Handles the error code of a non-blocking I/O operation.
 * This function is called only when a non-blocking read/write/connect/accept operation results in an error.
 *
 * This function checks first the given absolute timeout and throws an SNI NativeIOException if the timeout is reached.
 *
 * <code>absolute_timeout_ms</code> is an absolute time in milliseconds computed from the system time returned by
 * <code>LLMJVM_IMPL_getCurrentTime(1)</code>. A timeout of zero is interpreted as an infinite timeout.
 *
 * If the error code is <code>EAGAIN</code>, <code>EWOULDBLOCK</code> or <code>EINPROGRESS</code>,
 * then an <code>async_select()</code> request is performed on the given <code>operation</code>; otherwise an SNI NativeIOException is thrown.
 *
 * @param[in] fd The file descriptor on which the error has occurred.
 * @param[in] fd_errno the error code to be handled.
 * @param[in] operation the async_select operation on which the async_select request will be performed.
 * @param[in] absolute_timeout_ms the absolute timeout in milliseconds or 0 if no timeout.
 * @param[in] callback the async_select SNI callback function.
 * @param[in] callback_suspend_arg the async_select SNI suspend callback argument.
 */
void LLNET_handle_blocking_operation_error(int32_t fd, int32_t fd_errno, select_operation operation, int64_t absolute_timeout_ms, SNI_callback callback, void* callback_suspend_arg);

/**
 * @brief Sets the given socket file descriptor in non-blocking mode.
 *
 * @param[in] fd socket file descriptor
 *
 * @return 0 on success, a negative value on error.
 */
int32_t LLNET_set_non_blocking(int32_t fd);

/**
 * @brief Convert a network error code into a java error code.
 *
 * @param[in] err the error code returned by a BSD-like function.
 *
 * @return an error code defined in LLNET_ERRORS.h.
 */
int32_t LLNET_map_to_java_exception(int32_t err);

/**
 * @brief get the error message describing the given error code.
 *
 * @param[in] error the error code returned by a BSD-like function.
 *
 * @return the error message describing the given error code.
 */
const char * LLNET_get_socket_error_msg(int32_t error);

#if LLNET_AF & LLNET_AF_IPV6

/**
 * @brief Determine the scope id for an IP address for IPV6
 *
 * @param[in] ip a pointer to a string containing the IP address
 *
 * @return the scope is for the IP address. Zero if not found
 */
uint32_t LLNET_getScopeForIp(const char *ip);

/**
 * @brief  Map the given IPv4 address into an IPv6 address.<p>
 * <code>ipv4_addr</code> and <code>ipv6_addr</code> pointers can reference the same memory address.
 *
 * @param[in] ipv4_addr the IPv4 to map
 * @param[out] ipv6_addr the destination IPv6
 */
void LLNET_map_ipv4_into_ipv6(in_addr_t* ipv4_addr, struct in6_addr* ipv6_addr);

#endif

#ifdef __cplusplus
	}
#endif

#endif // LLNET_COMMON_H
