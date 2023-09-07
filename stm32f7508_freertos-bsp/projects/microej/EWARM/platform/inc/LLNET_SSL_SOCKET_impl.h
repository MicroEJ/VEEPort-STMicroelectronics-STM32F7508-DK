/*
 * C
 *
 * Copyright 2015-2021 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 * For demonstration purpose only.
 */
#ifndef __LLNET_SSL_SOCKET_IMPL__
#define __LLNET_SSL_SOCKET_IMPL__

/**
 * @file
 * @brief MicroEJ SSL low level API
 * @author MicroEJ Developer Team
 * @version 5.1.1
 * @date 7 May 2021
 */

#include <sni.h>
#include <intern/LLNET_SSL_SOCKET_impl.h>
#include <LLNET_SSL_CONSTANTS.h>
#include <LLNET_SSL_ERRORS.h>
#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Initializes SSL/TLS stack components
 * @return {@link J_SSL_NO_ERROR} on success
 */
int32_t LLNET_SSL_SOCKET_IMPL_initialize(void);

/**
 * Creates a new SSL Socket using the given SSL Context and the given underlying
 * Socket file descriptor {@code fd} for I/O operations
 * @param contextID the SSL Context ID
 * @param fd the underlying socket file descriptor used for I/O operations
 * @param hostname the server hostname
 * @param hostnameLen the length of the server hostname
 * @param autoclose true if the underlying socket will be closed when this SSL Socket is closed; false otherwise
 * @param useClientMode configures the socket to use client (or server) mode when handshaking. <code>true</code> if the socket should start its
 * handshaking in "client" mode; <code>false</code> if the socket should start its handshaking in "server" mode
 * @param needClientAuth configures the socket to require or not the client authentication. This option is only useful for sockets in the
 * server mode.<code>true</code> if the client authentication is required; <code>false</code> otherwise.
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return the new SSL Socket ID on success, {@link J_CREATE_SSL_SOCKET_ERROR} on error
 * @warning hostname must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_SOCKET_IMPL_create(int32_t contextID, int32_t fd, uint8_t* hostname, int32_t hostnameLen, uint8_t autoclose, uint8_t useClientMode, uint8_t needClientAuth, uint8_t retry);

/**
 * Sends close_notify alert and reads the received close_notify to clear the socket input stream if autoclose is set to false
 * and finally releases the SSL Socket identified by the given ssl ID.
 * @param sslID the SSL Socket ID
 * @param fd the associated underlying socket file descriptor
 * @param autoclose if true, no need to clear the input stream, the underlying socket will be closed;
 * otherwise read the received close_notify alert to clear the socket input stream.
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code
 * @see {@link LLNET_SSL_ERRORS} header file for error codes
 */
int32_t LLNET_SSL_SOCKET_IMPL_close(int32_t sslID, int32_t fd, uint8_t autoclose, uint8_t retry);

/**
 * Releases the SSL Socket identified by the given ssl ID.
 * @param sslID the SSL Socket ID
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code
 * @see {@link LLNET_SSL_ERRORS} header file for error codes
 */
int32_t LLNET_SSL_SOCKET_IMPL_freeSSL(int32_t sslID, uint8_t retry);

/**
 * Initiates the client handshaking
 * @param sslID the SSL Socket ID
 * @param fd the associated underlying socket file descriptor
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code
 * @see {@link LLNET_SSL_ERRORS} header file for error codes
 */
int32_t LLNET_SSL_SOCKET_IMPL_initialClientHandShake(int32_t sslID, int32_t fd, uint8_t retry);

/**
 * Initiates the server handshaking
 * @param sslID the SSL Socket ID
 * @param fd the associated underlying socket file descriptor
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code
 * @see {@link LLNET_SSL_ERRORS} header file for error codes
 */
int32_t LLNET_SSL_SOCKET_IMPL_initialServerHandShake(int32_t sslID, int32_t fd, uint8_t retry);

/**
 * Reads len bytes from the SSL Socket internal read buffer into the specified buffer {@code buf}
 * @param sslID the SSL Socket ID
 * @param fd the associated underlying socket file descriptor
 * @param buf the buffer into which the data is read
 * @param off the start offset in the buffer at which the data is written
 * @param len the maximum number of data to read
 * @param retry true if the calling process repeats the call to this operation for its completion because the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return the number of bytes read on success; {@link J_EOF} if there is no more data when the end of the stream
 * has been reached or a negative error code
 * @see {@link LLNET_SSL_ERRORS} header file for error codes
 * @warning buf must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_SOCKET_IMPL_read(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, uint8_t retry);

/**
 * Returns the number of bytes available in the SSL Socket internal read buffer
 * @param sslID the SSL Socket ID
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return the number of available bytes on success or a negative error code
 * @see {@link LLNET_SSL_ERRORS} header file for error codes
 */
int32_t LLNET_SSL_SOCKET_IMPL_available(int32_t sslID, uint8_t retry);

/**
 * Writes len bytes from the specified buffer {@code buf} starting at the offset to the SSL Socket output stream
 * @param sslID the SSL Socket ID
 * @param fd the associated underlying socket file descriptor
 * @param buf the buffer of data to write
 * @param off the offset in the data buffer
 * @param len the number of data to write
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return the number of bytes written on success or a negative error code
 * @see {@link LLNET_SSL_ERRORS} header file for error codes
 * @warning buf must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_SOCKET_IMPL_write(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, uint8_t retry);

#ifdef __cplusplus
	}
#endif

#endif //__LLNET_SSL_SOCKET_IMPL__
