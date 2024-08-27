/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLNET_STREAMSOCKETCHANNEL_IMPL
#define LLNET_STREAMSOCKETCHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 26 August 2022
 */

#include <stdint.h>

#include <intern/LLNET_STREAMSOCKETCHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Reads <code>length</code> bytes from the socket associated with the file descriptor <code>fd</code>
 * into the specified destination buffer <code>dst</code>.
 *
 * The return number of bytes read could be smaller than the <code>length</code> bytes requested.
 *
 * The read operation will block until data is arrived, the given absolute timeout is reached, or an error occurs.
 * If the timeout expires, an SNI NativeIOException is raised with the error code <code>J_ETIMEDOUT</code>
 *
 * <code>absoluteTimeout</code> is an absolute time in milliseconds computed from the system time returned by
 * <code>LLMJVM_IMPL_getCurrentTime(1)</code>. A timeout of zero is interpreted as an infinite timeout.
 *
 * @param[in]	fd 					The socket file descriptor.
 * @param[out]	dst					The destination buffer into which the read data is stored.
 * @param[in]	offset				The start offset in the buffer at which the data is written.
 * @param[in]	length				The maximum number of data to read.
 * @param[in]	absoluteTimeout		The absolute timeout in milliseconds or 0 if no timeout.
 *
 * @return The number of bytes read; '-1' if there is no more data because the end of the stream has been reached.
 *
 * @note Throws NativeIOException on error.
 *
 * @warning dst must not be used outside of the VM task or saved.
 */
int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_read(int32_t fd, int8_t* dst, int32_t offset, int32_t length, int64_t absoluteTimeout);

/**
 * @brief Writes <code>length</code> bytes from the specified buffer <code>buf</code> starting at the <code>offset</code> to the output stream
 * of the socket associated with the file descriptor <code>fd</code>.
 * The write operation will block until all data have been sent or an error occurs.
 *
 * @param[in] fd		The socket file descriptor.
 * @param[in] buf		The buffer of data to write.
 * @param[in] offset	The offset in the data buffer.
 * @param[in] length	The number of data to write.
 *
 *
 * @note Throws NativeIOException on error.
 *
 * @warning buf must not be used outside of the VM task or saved.
 */
void LLNET_STREAMSOCKETCHANNEL_IMPL_write(int32_t fd, int8_t* buf, int32_t offset, int32_t length);

/**
 * @brief Accepts an incoming connection for the listening server socket associated with the file descriptor <code>fd</code>
 * and returns the file descriptor of the accepted socket.
 *
 * The accept operation will block until a new connection is attempted, the given absolute timeout is reached, or an error occurs.
 * If the timeout expires, an SNI NativeIOException is raised with the error code <code>J_ETIMEDOUT</code>
 *
 * <code>absoluteTimeout</code> is an absolute time in milliseconds computed from the system time returned by
 * <code>LLMJVM_IMPL_getCurrentTime(1)</code>. A timeout of zero is interpreted as an infinite timeout.
 *
 * @param[in] fd The socket 	The server socket file descriptor that listening for incoming connection.
 * @param[in] absoluteTimeout 	The absolute timeout in milliseconds or 0 if no timeout.
 * @return 						The file descriptor of the accepted socket.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_accept(int32_t fd, int64_t absoluteTimeout);

/**
 * @brief Returns an estimate of the number of bytes that can be read from the socket associated with the file descriptor <code>fd</code>
 * without blocking by the next invocation of a read operation.
 *
 * @param[in] fd The socket file descriptor.
 *
 * @return An estimate of the number of bytes that can be read from the socket associated with the file descriptor <code>fd</code>
 * without blocking by the next invocation of a read operation.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_available(int32_t fd);

#ifdef __cplusplus
	}
#endif

#endif // LLNET_STREAMSOCKETCHANNEL_IMPL
