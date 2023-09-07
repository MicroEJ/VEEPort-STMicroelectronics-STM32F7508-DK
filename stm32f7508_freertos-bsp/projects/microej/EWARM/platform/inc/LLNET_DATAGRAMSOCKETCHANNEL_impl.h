/*
 * C
 *
 * Copyright 2014-2021 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_DATAGRAMSOCKETCHANNEL_IMPL
#define LLNET_DATAGRAMSOCKETCHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#include <sni.h>
#include <intern/LLNET_DATAGRAMSOCKETCHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Receives data from the datagram socket associated with the file descriptor {@code fd}.
 * <p>When this method returns, the {@code remoteHost} buffer contains the remote host address
 * followed by the remote host port from which the data is received. Remote host port is an int32_t.
 * @param fd the datagram socket file descriptor
 * @param dst the destination buffer into which the received data will be stored
 * @param dstOffset the offset in the destination buffer
 * @param dstLength the maximum number of data to receive
 * @param remoteHost the buffer into which the remote host address and port from which the data is received will be stored
 * @param remoteHostLength the maximum number of data in the remote host buffer
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} error code and
 * the calling process repeats the call to this operation for its completion
 * @return long value on success, where the 32 most significant bits converted in integer is the number of data received (in bytes)
 * and the 32 least significant bits is the remote host address size (in bytes); or a negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning dst must not be used outside of the VM task or saved.
 */
int64_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_receive(int32_t fd, int8_t* dst, int32_t dstOffset, int32_t dstLength, int8_t* remoteHost, int32_t remoteHostLength, uint8_t retry);

/**
 * Sends data from the datagram socket associated with the file descriptor {@code fd}
 * to the remote host address {@code dstAddr} and {@code port}.
 * @param fd the datagram socket file descriptor
 * @param data the buffer of data to send
 * @param dataOffset the offset in the data buffer
 * @param dataLength the number of data to send
 * @param dstAddr the remote host IP address at which the data will be sent
 * @param dstAddrLength the remote host address size
 * @param dstPort the remote host port
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the number of bytes sent or negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 * @warning data must not be used outside of the VM task or saved.
 * @warning dstAddr must not be used outside of the VM task or saved.
 */
int32_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_send(int32_t fd, int8_t* data, int32_t dataOffset, int32_t dataLength, int8_t* dstAddr, int32_t dstAddrLength, int32_t dstPort, uint8_t retry);

/**
 * Disconnects the datagram socket associated with the file descriptor {@code fd}.
 * @param fd the datagram socket file descriptor
 * @param retry true when the previous call returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return 0 on success or negative error code
 * @see {@link LLNET_ERRORS} header file for error codes
 */
int32_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_disconnect(int32_t fd, uint8_t retry);

#ifdef __cplusplus
	}
#endif

#endif
