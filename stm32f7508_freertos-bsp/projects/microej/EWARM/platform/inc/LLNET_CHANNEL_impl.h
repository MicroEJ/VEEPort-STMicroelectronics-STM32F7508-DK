/*
 * C
 *
 * Copyright 2014-2021 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_CHANNEL_IMPL
#define LLNET_CHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#include <sni.h>
#include <intern/LLNET_CHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Initializes the TCP/IP stack components.
 * @return 0 on success or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_CHANNEL_IMPL_initialize(void);

/**
 * Returns option value for the socket associated with the file descriptor {@code fd}.
 * @param fd the socket file descriptor
 * @param option the socket option identifier (one of the CPNET_* constants value defined in <code>LLNET_CONSTANT.h</code>)
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the option value for the socket associated with the file descriptor {@code fd}
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_CHANNEL_IMPL_getOption(int32_t fd, int32_t option, uint8_t retry);

/**
 * Sets option value for the socket associated with the file descriptor {@code fd}.
 * @param fd the socket file descriptor
 * @param option the socket option identifier
 * @param value the option value to set
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 on success or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_CHANNEL_IMPL_setOption(int32_t fd, int32_t option, int32_t value, uint8_t retry);

/**
 * Returns option value for the socket associated with the file descriptor {@code fd}.
 *
 * @param fd the socket file descriptor
 * @param option the socket option identifier
 * @param dst the destination byte array for the option value
 * @param dstLength the maximum length of the value the array can hold
 * @param retry true when the previous call returned {@link NetErrors#J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the length of the option value for the socket associated with the file descriptor {@code fd} or a negative error code
 * @see {@link NetErrors} for error codes
 * @warning dst must not be used outside of the VM task or saved.
 */
int32_t LLNET_CHANNEL_IMPL_getOptionAsByteArray(int32_t fd, int32_t option, int8_t* dst, int32_t dstLength, uint8_t retry);

/**
 * Sets option value for the socket associated with the file descriptor {@code fd}.
 *
 * @param fd the socket file descriptor
 * @param option the socket option identifier
 * @param value the option value to set as a byte array
 * @param valueLength the length of the value in the array
 * @param retry true when the previous call returned {@link NetErrors#J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 on success or a negative error code
 * @see {@link NetErrors} for error codes
 * @warning value must not be used outside of the VM task or saved.
 */
int32_t LLNET_CHANNEL_IMPL_setOptionAsByteArray(int32_t fd, int32_t option, int8_t* value, int32_t valueLength, uint8_t retry);

/**
 * Binds the socket associated with the file descriptor {@code fd} to the specified local address {@code addr} and {@code port}.
 * @param fd the native file descriptor
 * @param addr the IP address array, in network byte order
 * @param length the address size (4 bytes long for IPv4 addresses and 16 bytes long for IPv6 addresses)
 * @param port the port to bind to
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 on success or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning addr must not be used outside of the VM task or saved.
 */
int32_t LLNET_CHANNEL_IMPL_bind(int32_t fd, int8_t* addr, int32_t length, int32_t port, uint8_t retry);

/**
 * Configures the socket associated with the file descriptor {@code fd} in blocking or non-blocking mode.
 * @param fd the socket file descriptor
 * @param blocking true for blocking mode, false for non-blocking mode
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 on success or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_CHANNEL_IMPL_setBlocking(int32_t fd, uint8_t blocking, uint8_t retry);

/**
 * Closes the socket associated with the file descriptor {@code fd}.
 * @param fd the socket file descriptor
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 on success or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_CHANNEL_IMPL_close(int32_t fd, uint8_t retry);

/**
 * Shuts down the socket associated with the file descriptor {@code fd}'s input/output streams.
 * @param fd the socket file descriptor
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 on success or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_CHANNEL_IMPL_shutdown(int32_t fd, uint8_t retry);

/**
 * Listens on the socket associated with the given file descriptor {@code fd} for incoming connections.
 * @param fd the socket file descriptor
 * @param backlog the listen backlog size
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 on success or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_CHANNEL_IMPL_listen(int32_t fd, int32_t backlog, uint8_t retry);

#ifdef __cplusplus
	}
#endif

#endif
