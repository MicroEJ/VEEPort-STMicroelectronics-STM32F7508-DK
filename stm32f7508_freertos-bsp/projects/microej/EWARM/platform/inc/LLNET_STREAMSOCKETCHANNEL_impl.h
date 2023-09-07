/*
 * C
 *
 * Copyright 2014-2021 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_STREAMSOCKETCHANNEL_IMPL
#define LLNET_STREAMSOCKETCHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#include <sni.h>
#include <intern/LLNET_STREAMSOCKETCHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Reads {@code length} bytes from the socket associated with the file descriptor {@code fd}
 * into the specified destination buffer {@code dst}.
 * @param fd the socket file descriptor
 * @param kind the kind of the socket (0 for streaming, 1 if datagrams)
 * @param dst the destination buffer into which the read data is stored
 * @param offset the start offset in the buffer at which the data is written
 * @param length the maximum number of data to read
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the number of bytes read; -1 if there is no more data because the end of the stream has been reached or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning dst must not be used outside of the VM task or saved.
 */
int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_readByteBufferNative(int32_t fd, int32_t kind, int8_t* dst, int32_t offset, int32_t length, uint8_t retry);

/**
 * Writes {@code length} bytes from the specified buffer {@code buf} starting at the {@code offset} to the output stream
 * of the socket associated with the file descriptor {@code fd}.
 * @param fd the socket file descriptor
 * @param kind the kind of the socket (0 for streaming, 1 if datagrams)
 * @param buf the buffer of data to write
 * @param offset the offset in the data buffer
 * @param length the number of data to write
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the number of bytes written or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning dst src not be used outside of the VM task or saved.
 */
int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_writeByteBufferNative(int32_t fd, int32_t kind, int8_t* src, int32_t offset, int32_t length, uint8_t retry);

/**
 * Accepts an incoming connection for the listening socket associated with the file descriptor {@code fd}
 * and returns the file descriptor of the accepted socket.
 * @param fd the socket file descriptor that listening for incoming connection
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the file descriptor of the accepted socket or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_accept(int32_t fd, uint8_t retry);

/**
 * Returns an estimate of the number of bytes that can be read from the socket associated with the file descriptor {@code fd}
 * without blocking by the next invocation of a read operation.
 * @param fd the socket file descriptor
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return an estimate of the number of bytes that can be read from the socket associated with the file descriptor {@code fd}
 * without blocking by the next invocation of a read operation or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_STREAMSOCKETCHANNEL_IMPL_available(int32_t fd, uint8_t retry);


#ifdef __cplusplus
	}
#endif
#endif
