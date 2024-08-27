/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLNET_CHANNEL_IMPL
#define LLNET_CHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 26 August 2022
 */

#include <stdint.h>

#include <intern/LLNET_CHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Socket option constants
 */
#define LLNET_SOCKETOPTION_TCP_NODELAY			0x0001
#define LLNET_SOCKETOPTION_SO_BINDADDR			0x000F
#define LLNET_SOCKETOPTION_SO_REUSEADDR			0x04
#define LLNET_SOCKETOPTION_SO_BROADCAST			0x0020
#define LLNET_SOCKETOPTION_IP_MULTICAST_IF		0x10
#define LLNET_SOCKETOPTION_IP_MULTICAST_IF2		0x1f
#define LLNET_SOCKETOPTION_IP_MULTICAST_LOOP	0x12
#define LLNET_SOCKETOPTION_IP_TOS				0x3
#define LLNET_SOCKETOPTION_SO_LINGER			0x0080
#define LLNET_SOCKETOPTION_SO_SNDBUF			0x1001
#define LLNET_SOCKETOPTION_SO_RCVBUF			0x1002
#define LLNET_SOCKETOPTION_SO_KEEPALIVE			0x0008
#define LLNET_SOCKETOPTION_SO_OOBINLINE			0x1003

/**
 * Multicast socket option constants
 */
#define LLNET_SOCKETOPTION_IP_TTL				0x1E61

/**
 * @brief Initializes the TCP/IP stack components.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
void LLNET_CHANNEL_IMPL_initialize(void);

/**
 * @brief Returns option value for the socket associated with the file descriptor <code>fd</code>.
 *
 * @param[in] fd		The socket file descriptor.
 * @param[in] option	The socket option identifier (one of the <code>LLNET_SOCKETOPTION_*</code> constants value defined in this file).
 *
 * @return The option value for the socket associated with the file descriptor <code>fd</code>.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_CHANNEL_IMPL_getOption(int32_t fd, int32_t option);

/**
 * @brief Sets option value for the socket associated with the file descriptor <code>fd</code>.
 *
 * @param[in] fd		The socket file descriptor.
 * @param[in] option	The socket option identifier.
 * @param[in] value		The option value to set.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
void LLNET_CHANNEL_IMPL_setOption(int32_t fd, int32_t option, int32_t value);

/**
 * @brief Returns option value for the socket associated with the file descriptor <code>fd</code>.
 *
 * @param[in]	fd 			The socket file descriptor.
 * @param[in]	option 		The socket option identifier.
 * @param[out]	dst 		The destination byte array for the option value.
 * @param[in]	dstLength	The maximum length of the value the array can hold.
 *
 * @return The length of the option value for the socket associated with the file descriptor <code>fd</code>.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning dst must not be used outside of the VM task or saved.
 */
int32_t LLNET_CHANNEL_IMPL_getOptionAsByteArray(int32_t fd, int32_t option, int8_t* dst, int32_t dstLength);

/**
 * @brief Sets option value for the socket associated with the file descriptor <code>fd</code>.
 *
 * @param[in] fd			The socket file descriptor.
 * @param[in] option		The socket option identifier.
 * @param[in] value			The option value to set as a byte array.
 * @param[in] valueLength	The length of the value in the array.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning value must not be used outside of the VM task or saved.
 */
void LLNET_CHANNEL_IMPL_setOptionAsByteArray(int32_t fd, int32_t option, int8_t* value, int32_t valueLength);

/**
 * @brief Binds the socket associated with the file descriptor <code>fd</code> to the specified local address <code>addr</code> and <code>port</code>.
 *
 * @param[in] fd 		The native file descriptor.
 * @param[in] addr		The IP address array, in network byte order.
 * @param[in] length	The address size (4 bytes long for IPv4 addresses and 16 bytes long for IPv6 addresses).
 * @param[in] port		The port to bind to.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning addr must not be used outside of the VM task or saved.
 */
void LLNET_CHANNEL_IMPL_bind(int32_t fd, int8_t* addr, int32_t length, int32_t port);

/**
 * @brief Closes the socket associated with the file descriptor <code>fd</code>.
 *
 * @param[in] fd The socket file descriptor.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
void LLNET_CHANNEL_IMPL_close(int32_t fd);

/**
 * @brief Shuts down the socket associated with the file descriptor <code>fd</code>'s input/output streams.
 *
 * @param[in] fd The socket file descriptor.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
void LLNET_CHANNEL_IMPL_shutdown(int32_t fd);

/**
 * @brief Listens on the socket associated with the given file descriptor <code>fd</code> for incoming connections.
 *
 * @param[in] fd 		The socket file descriptor.
 * @param[in] backlog 	The listen backlog size.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
void LLNET_CHANNEL_IMPL_listen(int32_t fd, int32_t backlog);

#ifdef __cplusplus
	}
#endif

#endif // LLNET_CHANNEL_IMPL
