/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLNET_SOCKETCHANNEL_IMPL
#define LLNET_SOCKETCHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 26 August 2022
 */

#include <stdint.h>

#include <intern/LLNET_SOCKETCHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Connects the socket associated with the file descriptor <code>fd</code> to the remote host address <code>addr</code> and <code>port</code>
 * with a specified absolute timeout <code>absoluteTimeout</code> value.
 *
 * The connection will then block until established, the given absolute timeout is reached, or an error occurs.
 * If the timeout expires, an SNI NativeIOException is raised with the error code <code>J_ETIMEDOUT</code>.
 *
 * <code>absoluteTimeout</code> is an absolute time in milliseconds computed from the system time returned by
 * <code>LLMJVM_IMPL_getCurrentTime(1)</code>. A timeout of zero is interpreted as an infinite timeout.
 *
 * @param[in] fd		The socket file descriptor.
 * @param[in] addr		The remote host address buffer.
 * @param[in] length	The remote host address size.
 * @param[in] port		The remote host port.
 * @param[in] absoluteTimeout	The absolute timeout in milliseconds or 0 if no timeout.
 *
 * @note Throws NativeIOException on error.
 *
 * @warning addr must not be used outside of the VM task or saved.
 */
void LLNET_SOCKETCHANNEL_IMPL_connect(int32_t fd, int8_t* addr, int32_t length, int32_t port, int64_t absoluteTimeout);

/**
 * @brief Returns the local port to which the socket associated with the file descriptor <code>fd</code> is bound.
 *
 * @param[in] fd The socket file descriptor.
 *
 * @return The local port to which the socket associated with the file descriptor <code>fd</code> is bound;
 * '-1' if the socket not bound.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalPort(int32_t fd);

/**
 * @brief Returns the remote host port to which the socket associated with the file descriptor <code>fd</code> is connected.
 *
 * @param[in] fd the socket file descriptor.
 *
 * @return The remote port to which the socket associated with the file descriptor <code>fd</code> is connected.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerPort(int32_t fd);

/**
 * @brief Gets the local address to which the socket associated with the file descriptor <code>fd</code> is bound.
 * When this methods returns, the <code>addr</code> buffer is filled with the local address.
 *
 * @param[in]	fd		The socket file descriptor.
 * @param[out]	addr	The buffer into which the local address will be stored.
 * @param[in]	length 	The buffer length.
 *
 * @return The local address size in bytes or '0' if the socket is not bound.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning addr must not be used outside of the VM task or saved.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalAddress(int32_t fd, int8_t* addr, int32_t length);

/**
 * @brief Gets the remote host address to which the socket associated with the file descriptor <code>fd</code> is connected.
 * When this methods returns, the <code>addr</code> buffer is filled with the remote host address.
 *
 * @param[in]	fd		The socket file descriptor.
 * @param[out]	addr	The buffer into which the remote host address will be stored.
 * @param[in]	length	The buffer length.
 *
 * @return The remote host address size in bytes or '0' if the socket is not bound.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning name must not be used outside of the VM task or saved.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerAddress(int32_t fd, int8_t* addr, int32_t length);

/**
 * @brief Creates a new socket and returns its associated file descriptor.
 *
 * @param[in] stream Set to '1' for streaming sockets; '0' for datagrams.
 *
 * @return The file descriptor of the new socket.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_socket(uint8_t stream);

/**
 * @brief Creates a new server socket, returning the native file descriptor.
 *
 * @return The file descriptor of the new server socket.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_serverSocket(void);

/**
 * @brief Creates a new multicast socket, returning the native file descriptor.
 *
 * @return The file descriptor of the new multicast socket.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_multicastSocket(void);

#ifdef __cplusplus
	}
#endif

#endif // LLNET_SOCKETCHANNEL_IMPL
