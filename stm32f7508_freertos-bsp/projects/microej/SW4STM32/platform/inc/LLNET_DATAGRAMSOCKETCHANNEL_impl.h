/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLNET_DATAGRAMSOCKETCHANNEL_IMPL
#define LLNET_DATAGRAMSOCKETCHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 26 August 2022
 */

#include <stdint.h>

#include <intern/LLNET_DATAGRAMSOCKETCHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Receives data from the datagram socket associated with the file descriptor <code>fd</code>.
 * When this method returns, the <code>remoteHost</code> buffer contains the remote host address
 * followed by the remote host port from which the data is received. Remote host port is an <code>int32_t</code>.
 *
 * The receive operation will block until data is arrived, the given absolute timeout is reached, or an error occurs.
 * If the timeout expires, an SNI NativeIOException is raised with the error code <code>J_ETIMEDOUT</code>.
 *
 * <code>absoluteTimeout</code> is an absolute time in milliseconds computed from the system time returned by
 * <code>LLMJVM_IMPL_getCurrentTime(1)</code>. A timeout of zero is interpreted as an infinite timeout.
 *
 * @param[in]	fd					The datagram socket file descriptor.
 * @param[out]	dst					The destination buffer into which the received data will be stored.
 * @param[in]	dstOffset			The offset in the destination buffer.
 * @param[in]	dstLength			The maximum number of data to receive.
 * @param[in]	remoteHost			The buffer into which the remote host address and port from which the data is received will be stored.
 * @param[in]	remoteHostLength	The maximum number of data in the remote host buffer.
 * @param[in]	absoluteTimeout		The absolute timeout in milliseconds or 0 if no timeout.
 *
 * @return long value on success, where the 32 most significant bits converted in integer is the number of data received (in bytes)
 * and the 32 least significant bits is the remote host address size (in bytes); or -1 when end-of-file is encountered.
 *
 * @note Throws SNI NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning dst must not be used outside of the VM task or saved.
 * @warning remoteHost must not be used outside of the VM task or saved.
 */
int64_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_receive(int32_t fd, int8_t* dst, int32_t dstOffset, int32_t dstLength, int8_t* remoteHost, int32_t remoteHostLength, int64_t absoluteTimeout);

/**
 * @brief Sends data from the datagram socket associated with the file descriptor <code>fd</code>
 * to the remote host address <code>dstAddr</code> and <code>port</code>.
 * The send operation will block until all data have been sent or an error occurs.
 *
 * @param[in] fd			The datagram socket file descriptor.
 * @param[in] data			The buffer of data to send.
 * @param[in] dataOffset	The offset in the data buffer.
 * @param[in] dataLength	The number of data to send.
 * @param[in] dstAddr		The remote host IP address at which the data will be sent.
 * @param[in] dstAddrLength	The remote host address size.
 * @param[in] dstPort		The remote host port.
 *
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning data must not be used outside of the VM task or saved.
 * @warning dstAddr must not be used outside of the VM task or saved.
 */
void LLNET_DATAGRAMSOCKETCHANNEL_IMPL_send(int32_t fd, int8_t* data, int32_t dataOffset, int32_t dataLength, int8_t* dstAddr, int32_t dstAddrLength, int32_t dstPort);

/**
 * @brief Disconnects the datagram socket associated with the file descriptor <code>fd</code>.
 *
 * @param[in] fd The datagram socket file descriptor.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
void LLNET_DATAGRAMSOCKETCHANNEL_IMPL_disconnect(int32_t fd);

#ifdef __cplusplus
	}
#endif

#endif // LLNET_DATAGRAMSOCKETCHANNEL_IMPL
