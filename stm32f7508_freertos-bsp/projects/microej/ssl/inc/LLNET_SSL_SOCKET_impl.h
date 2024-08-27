/*
 * C
 *
 * Copyright 2015-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef __LLNET_SSL_SOCKET_IMPL__
#define __LLNET_SSL_SOCKET_IMPL__

/**
 * @file
 * @brief MicroEJ SSL low level API
 * @author MicroEJ Developer Team
 * @version 6.0.0
 * @date 26 August 2022
 */

#include <stdint.h>
#include <stdbool.h>

#include <intern/LLNET_SSL_SOCKET_impl.h>
#include <LLNET_SSL_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Initializes SSL/TLS stack components.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
void LLNET_SSL_SOCKET_IMPL_initialize(void);

/**
 * @brief Creates a new ssl socket using the given ssl context and the underlying
 * socket file descriptor <code>fd</code> for I/O operations.
 *
 * @param[in] contextID			The ssl context ID.
 * @param[in] fd				The underlying socket file descriptor used for I/O operations.
 * @param[in] hostname			The server hostname (null-terminated string). The hostname is <code>null</code> if the server does not have a valid domain name.
 * @param[in] hostnameLength	The length of the server hostname. (The length does not include the terminating null byte). The length is <code>0</code> if the server does not have a valid domain name.
 * @param[in] autoclose			<code>true</code> if the underlying TCP socket will be closed when this ssl socket is closed; false otherwise.
 * @param[in] useClientMode		Configures the socket to use client (or server) mode during the handshake (1 for client mode; 0 for server mode).
 * @param[in] needClientAuth	Configures the socket to require or not the client authentication. This option is only useful
 *	for sockets in the server mode. 1 if the client authentication is required; 0 otherwise.
 *
 * @return The new ssl socket ID on success.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 *
 * @warning hostname must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_SOCKET_IMPL_create(int32_t contextID, int32_t fd, uint8_t* hostname, int32_t hostnameLength, bool autoclose, uint8_t useClientMode, uint8_t needClientAuth);

/**
 * @brief Shuts down the SSL/TLS connection.
 *
 * This function will try to send a "close notify" alert to the peer and then wait for
 * the peer to send its "close notify" if <code>autoclose</code> is <code>false</code>.
 * Waiting for the peer to send its "close notify" will consist of reading until the received data is a "close notify" alert.
 *
 * If <code>autoclose</code> is <code>true</code>, there is no need to wait for the peer to send its "close notify"
 * since the underlying TCP connection will be closed right after this function call.
 *
 * @param[in] sslID					The ssl socket ID.
 * @param[in] fd					The associated underlying socket file descriptor.
 * @param[in] autoclose				If <code>true</code>, no need to wait for the peer to send its "close notify";
 *									otherwise we need to read until the received data is a "close notify" alert.
 * @param[in] absoluteJavaStartTime	The absolute start time of the call of this method from Java. This time is computed from the system time returned by <code>LLMJVM_IMPL_getCurrentTime(1)</code>.
 * @param[in] relativeTimeout		The relative timeout in milliseconds for each underlying read operation performed during the socket closure; or 0 if no timeout.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
void LLNET_SSL_SOCKET_IMPL_shutdown(int32_t sslID, int32_t fd, bool autoclose, int64_t absoluteJavaStartTime, int32_t relativeTimeout);

/**
 * @brief Releases the ssl socket identified by the given ssl ID.
 *
 * @param[in] sslID The ssl socket ID.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
void LLNET_SSL_SOCKET_IMPL_freeSSL(int32_t sslID);

/**
 * @brief Initiates the handshake in client mode.
 *
 * @param[in] sslID					The ssl socket ID.
 * @param[in] fd					The associated underlying socket file descriptor.
 * @param[in] absoluteJavaStartTime	The absolute start time of the call of this method from Java. This time is computed from the system time returned by <code>LLMJVM_IMPL_getCurrentTime(1)</code>.
 * @param[in] relativeTimeout		The relative timeout in milliseconds for each underlying read operation performed during the handshake; or 0 if no timeout.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
void LLNET_SSL_SOCKET_IMPL_initialClientHandShake(int32_t sslID, int32_t fd, int64_t absoluteJavaStartTime, int32_t relativeTimeout);

/**
 * @brief Initiates the handshake in server mode.
 *
 * @param[in] sslID					The ssl socket ID.
 * @param[in] fd					The associated underlying socket file descriptor.
 * @param[in] absoluteJavaStartTime	The absolute start time of the call of this method from Java. This time is computed from the system time returned by <code>LLMJVM_IMPL_getCurrentTime(1)</code>.
 * @param[in] relativeTimeout		The relative timeout in milliseconds for each underlying read operation performed during the handshake; or 0 if no timeout.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
void LLNET_SSL_SOCKET_IMPL_initialServerHandShake(int32_t sslID, int32_t fd, int64_t absoluteJavaStartTime, int32_t relativeTimeout);

/**
 * @brief Reads length bytes from the ssl socket internal read buffer into the specified buffer.
 *
 * @param[in]	sslID					The ssl socket ID.
 * @param[in]	fd						The associated underlying socket file descriptor.
 * @param[out]	buffer					The buffer into which the data is read.
 * @param[in]	off						The start offset in the buffer at which the data is written.
 * @param[in]	length					The maximum number of data to read.
 * @param[in] 	absoluteJavaStartTime 	The absolute start time of the call of this method from Java. This time is computed from the system time returned by <code>LLMJVM_IMPL_getCurrentTime(1)</code>.
 * @param[in] 	relativeTimeout 		The relative timeout in milliseconds or 0 if no timeout.
 *
 * @return 	The number of bytes read on success; J_EOF if there is no more data when the end of the stream has been reached.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 *
 * @warning buffer must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_SOCKET_IMPL_read(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int32_t relativeTimeout);

/**
 * @brief Returns the number of bytes available in the ssl socket internal read buffer.
 *
 * @param[in] sslID The ssl socket ID.
 *
 * @return 	The number of available bytes on success or a negative error code.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
int32_t LLNET_SSL_SOCKET_IMPL_available(int32_t sslID);

/**
 * @brief Writes length bytes from the specified buffer starting at the offset to the ssl socket output stream.
 * As a call to write operation may cause read operations (when a re-negotiation is requested for example),
 * the given relative timeout will be used for these read operations.
 *
 * @param[in] sslID					The ssl socket ID.
 * @param[in] fd					The associated underlying socket file descriptor.
 * @param[in] buffer				The buffer of data to write.
 * @param[in] offset				The offset in the data buffer.
 * @param[in] length				The number of data to write.
 * @param[in] absoluteJavaStartTime The absolute start time of the call if this method from Java. This time is computed from the system time returned by <code>LLMJVM_IMPL_getCurrentTime(1)</code>.
 * @param[in] relativeTimeout		The relative timeout in milliseconds for each underlying read operation performed during the write operation; or 0 if no timeout.
 *
 * @return 	The number of bytes written on success.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 *
 * @warning buffer must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_SOCKET_IMPL_write(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int32_t relativeTimeout);

#ifdef __cplusplus
	}
#endif

#endif //__LLNET_SSL_SOCKET_IMPL__
