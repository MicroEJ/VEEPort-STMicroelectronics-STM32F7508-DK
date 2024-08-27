/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLNET_MULTICASTSOCKETCHANNEL_IMPL
#define LLNET_MULTICASTSOCKETCHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 26 August 2022
 */

#include <stdint.h>

#include <intern/LLNET_MULTICASTSOCKETCHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Get the default time-to-live for multicast packets sent out on the channel.
 *
 * @param[in] nativeFD	The datagram socket file descriptor.
 * @param[in] ttl 		The TTL value to set.
 *
 * @return The default time-to-live value.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
int32_t LLNET_MULTICASTSOCKETCHANNEL_IMPL_getTimeToLive(int32_t nativeFD);

/**
 * @brief Set the default time-to-live for multicast packets sent out
 * on this channel in order to control the scope of the multicasts.
 * The ttl <B>must</B> be in the range <code>0 <= ttl <= 255</code>
 * Multicast packets sent with a TTL of <code>0</code> are not transmitted
 * on the network but may be delivered locally.
 *
 * @param[in] nativeFD	The datagram socket file descriptor.
 * @param[in] ttl		The TTL value to set.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 */
void LLNET_MULTICASTSOCKETCHANNEL_IMPL_setTimeToLive(int32_t nativeFD, int32_t ttl);

/**
 * @brief Joins or leaves a multicast group.

 * @param[in] fd				The datagram socket file descriptor.
 * @param[in] join				Join if '1', leave if '0'.
 * @param[in] mcastAddr			The multicast address to join.
 * @param[in] mcastAddrLength	The multicast address size.
 * @param[in] netIfAddr			The address of the interface that must join.
 * @param[in] netIfAddrLength	The interface address size (may be 0 if no interface is specified).
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_ERRORS.h header file for error codes.
 *
 * @warning mcastAddr must not be used outside of the VM task or saved.
 * @warning netIfAddr must not be used outside of the VM task or saved.
 */
void LLNET_MULTICASTSOCKETCHANNEL_IMPL_joinOrLeave(int32_t fd, uint8_t join, int8_t* mcastAddr, int32_t mcastAddrLength, int8_t* netIfAddr, int32_t netIfAddrLength);

#ifdef __cplusplus
	}
#endif

#endif // LLNET_MULTICASTSOCKETCHANNEL_IMPL
