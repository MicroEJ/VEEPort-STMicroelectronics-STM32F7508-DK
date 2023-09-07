/*
 * C
 *
 * Copyright 2014-2021 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_SOCKETCHANNEL_IMPL
#define LLNET_SOCKETCHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#include <sni.h>
#include <intern/LLNET_SOCKETCHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Connects the socket associated with the file descriptor {@code fd} to the remote host address {@code addr} and {@code port}
 * with a specified {@code timeout} value.
 * <p>A timeout of zero is interpreted as an infinite timeout. The connection will then block until established or an error occurs.
 * <p>The timeout detection is done by the caller of this method so the implementation does not need to detect a timeout. In any
 * case, this method must block for more than the specified timeout (if timeout is different from 0).
 *
 * @param fd the socket file descriptor
 * @param addr the remote host address buffer
 * @param length the remote host address size
 * @param port the remote host port
 * @param timeout the timeout value in milliseconds
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 1 if the connection succeeded; 0 if the file descriptor is in non-blocking mode and the connection did not immediately succeed
 * or negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning addr must not be used outside of the VM task or saved.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_connect(int32_t fd, int8_t* addr, int32_t length, int32_t port, int32_t timeout, uint8_t retry);

/**
 * Returns the local port to which the socket associated with the file descriptor {@code fd} is bound.
 * @param fd the socket file descriptor
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the local port to which the socket associated with the file descriptor {@code fd} is bound;
 * -1 if the socket not bound or a negative error code.
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalPort(int32_t fd, uint8_t retry);

/**
 * Returns the remote host port to which the socket associated with the file descriptor {@code fd} is connected.
 * @param fd the socket file descriptor
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the remote port to which the socket associated with the file descriptor {@code fd} is connected;
 * or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerPort(int32_t fd, uint8_t retry);

/**
 * Gets the local address to which the socket associated with the file descriptor {@code fd} is bound.
 * When this methods returns, the {@code addr} buffer is filled with the local address.
 * @param fd the socket file descriptor
 * @param addr the buffer into which the local address will be stored
 * @param length the buffer length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the local address size in bytes, 0 if the socket is not bound or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning addr must not be used outside of the VM task or saved.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalAddress(int32_t fd, int8_t* addr, int32_t length, uint8_t retry);

/**
 * Gets the remote host address to which the socket associated with the file descriptor {@code fd} is connected.
 * When this methods returns, the {@code addr} buffer is filled with the remote host address.
 * @param fd the socket file descriptor
 * @param addr the buffer into which the remote host address will be stored
 * @param length the buffer length
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the remote host address size in bytes, 0 if the socket is not bound or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning name must not be used outside of the VM task or saved.
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerAddress(int32_t fd, int8_t* name, int32_t length, uint8_t retry);

/**
 * Creates a new socket and returns its associated file descriptor.
 * @param stream set to true for streaming sockets; false for datagrams.
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the file descriptor of the new socket or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_socket(uint8_t stream, uint8_t retry);

/**
 * Creates a new server socket, returning the native file descriptor.
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the file descriptor of the new server socket or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_serverSocket(uint8_t retry);

/**
 * Creates a new multicast socket, returning the native file descriptor.
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the file descriptor of the new multicast socket or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_SOCKETCHANNEL_IMPL_multicastSocket(uint8_t retry);

#ifdef __cplusplus
	}
#endif
#endif
