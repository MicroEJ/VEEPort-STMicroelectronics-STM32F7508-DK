/*
 * C
 *
 * Copyright 2014-2021 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_MULTICASTSOCKETCHANNEL_IMPL
#define LLNET_MULTICASTSOCKETCHANNEL_IMPL

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#include <sni.h>
#include <intern/LLNET_MULTICASTSOCKETCHANNEL_impl.h>
#include <LLNET_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Get the default time-to-live for multicast packets sent out on the channel.
 * @param nativeFD the datagram socket file descriptor
 * @param ttl the TTL value to set
 * @param retry true when the previous call returned {@link NetErrors#J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return the default time-to-live value or negative error code
 * @see {@link NetErrors} for error codes
 */
int32_t LLNET_MULTICASTSOCKETCHANNEL_IMPL_getTimeToLive(int32_t nativeFD, uint8_t retry);

/**
 * Set the default time-to-live for multicast packets sent out
 * on this channel in order to control the scope of the multicasts.
 * The ttl <B>must</B> be in the range {@code  0 <= ttl <= 255}
 * Multicast packets sent with a TTL of {@code 0} are not transmitted
 * on the network but may be delivered locally.
 *
 * @param nativeFD the datagram socket file descriptor
 * @param ttl the TTL value to set
 * @param retry true when the previous call returned {@link NetErrors#J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return zero or negative error code
 * @see {@link NetErrors} for error codes
 */
int32_t LLNET_MULTICASTSOCKETCHANNEL_IMPL_setTimeToLive(int32_t nativeFD, int32_t ttl, uint8_t retry);

/**
 * Joins or leaves a multicast group.

 * @param fd the datagram socket file descriptor
 * @param join join if true, leave if false
 * @param mcastAddr is the multicast address to join
 * @param mcastAddrLength the multicast address size
 * @param netIfAddr is the address of the interface that must join
 * @param netIfAddrLength the interface address size (may be 0 if no interface is specified)
 * @param retry true when the previous call returned {@link NetErrors#J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT}
 * and the calling process repeats the call to this operation for its completion
 * @return zero or negative error code
 * @see {@link NetErrors} for error codes
 * @warning mcastAddr must not be used outside of the VM task or saved.
 * @warning netIfAddr must not be used outside of the VM task or saved.
 */
int32_t LLNET_MULTICASTSOCKETCHANNEL_IMPL_joinOrLeave(int32_t fd, uint8_t join, int8_t* mcastAddr, int32_t mcastAddrLength, int8_t* netIfAddr, int32_t netIfAddrLength, uint8_t retry);

#ifdef __cplusplus
	}
#endif

#endif
