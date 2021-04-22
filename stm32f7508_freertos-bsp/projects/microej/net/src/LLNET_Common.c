/*
 * C
 *
 * Copyright 2016-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLNET_Common implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 1.1.1
 * @date 7 February 2020
 */

#include "LLNET_Common.h"
#include "async_select.h"
#include "async_select_cache.h"
#include <string.h>
#include "LLNET_CHANNEL_impl.h"
#include "LLNET_CONSTANTS.h"
#include "LLNET_ERRORS.h"
#include <sys/socket.h>
#include <stdint.h>
#if LLNET_AF & LLNET_AF_IPV6
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#endif

#ifndef USE_IOCTL_FOR_BLOCKING_OPTION
#include <fcntl.h>
#endif

/** @brief external function used to retrieve currentTime (same as MicroJvm) */
extern int64_t LLMJVM_IMPL_getCurrentTime__Z(uint8_t system);

int32_t asyncOperation(int32_t fd, SELECT_Operation operation, uint8_t retry){
	int32_t timeout;

	// the socket is not readable or writable.
	// A blocking request is added in the dispatch event queue.
	timeout = async_select_get_socket_timeout_from_cache(fd);
	if (timeout == -1) {
		timeout = LLNET_CHANNEL_IMPL_getOption(fd, CPNET_SO_TIMEOUT, 0);
	}

	//check the timeout
	if(timeout < 0){
		//error code
		return timeout;
	}
	if((timeout != 0) && retry){
		int64_t absolute_timeout = async_select_get_socket_absolute_timeout_from_cache(fd);

		//The socket has been configured with a timeout
		//AND there is no available data
		//AND this is the second read (retry==true)
		//THEN most likely the timeout has been reached during the first read
		//BUT there is a special case where more Java threads were waiting for a notification
		//on the same socket, and more Java threads were woken up by the async_select, but the
		//first Java thread which ran consumed the socket event, so we can end up in this case
		//without the full timeout being passed
		//SO check that the timeout really passed for the socket, otherwise add a new async_select
		//request with the remaining timeout
		if (absolute_timeout != -1) {
			int64_t current_time = LLMJVM_IMPL_getCurrentTime__Z(1);
			if (current_time >= absolute_timeout) {
				return J_ETIMEDOUT;
			} else {
				timeout = absolute_timeout - current_time;
			}
		} else {
			return J_EUNKNOWN;
		}
	}
	int32_t res = async_select(fd, operation, timeout, NULL);
	if(res == 0){
		// request added in the queue
		return J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT;
	}
	// requests queue limit reached
	return J_ASYNC_BLOCKING_REQUEST_QUEUE_LIMIT_REACHED;
}

/**
 * @brief Fills-in the given timeval struct with the given time in milliseconds.
 *
 * @param[in] time_ms time in milliseconds.
 * @param[in] time_timeval pointer to the timeval struct to fill-in.
 */
void time_ms_to_timeval(int64_t time_ms, struct timeval* time_timeval){
	if(time_ms >= 1000){
		time_timeval->tv_sec = time_ms / 1000;
		time_timeval->tv_usec = (time_ms % 1000) * 1000;
	}else{
		time_timeval->tv_sec = 0;
		time_timeval->tv_usec = time_ms * 1000;
	}
}


/**
 * @brief Sets the given socket in non-blocking mode or not.
 *
 * @param[in] fd socket file descriptor
 * @param[in] non_blocking true to enable non-blocking, false to enable blocking.
 *
 * @return 0 on success, a negative value on error.
 */
int32_t set_socket_non_blocking(int32_t fd, bool non_blocking){
#ifdef USE_IOCTL_FOR_BLOCKING_OPTION
	int32_t non_blocking_option = non_blocking==true ? 1 : 0;
	int32_t res = llnet_ioctl(fd, FIONBIO, &non_blocking_option);

	return res;
#else
	int32_t flags = llnet_fcntl(fd, F_GETFL, 0);
	if(flags == -1){
		return -1;
	}

	if(non_blocking == true){
		flags |= O_NONBLOCK;
	}
	else {
		flags &= ~O_NONBLOCK;
	}
	return llnet_fcntl(fd, F_SETFL, flags);
#endif
}

/**
 * @brief Checks if the given socket is in non-blocking mode or not.
 *
 * @param[in] fd socket file descriptor
 *
 * @return true if the socket is non blocking, false if the socket is blocking or an error occurs.
 */
bool is_socket_non_blocking(int32_t fd){
#ifdef USE_IOCTL_FOR_BLOCKING_OPTION
	return false;
#else
	int32_t flags = llnet_fcntl(fd, F_GETFL, 0);
	if(flags == -1){
		return false;
	}

	return (flags & O_NONBLOCK) != 0;
#endif
}

/**
 * @brief Convert a network error code into a java error code.
 *
 * @param[in] err the error code returned by a BSD-like function.
 *
 * @return an error code defined in LLNET_ERRORS.h.
 */
int32_t map_to_java_exception(int32_t err){
	switch(err){
		case (EACCES):
			return J_EACCES;
		case (EBADF):
			return J_EBADF;
		case (EHOSTDOWN):
			return J_EHOSTDOWN;
		case (ENETDOWN):
			return J_ENETDOWN;
		case (ENETUNREACH):
			return J_ENETUNREACH;
		case (EADDRINUSE):
			return J_EADDRINUSE;
		case (EINVAL):
			return J_EINVAL;
		case (ECONNABORTED):
			return J_ECONNABORTED;
		case (ENOPROTOOPT):
			return J_ENOPROTOOPT;
		case (ENOTCONN):
			return J_ENOTCONN;
		case (EAFNOSUPPORT):
			return J_EAFNOSUPPORT;
		case (ECONNREFUSED):
			return J_ECONNREFUSED;
		case (EISCONN):
			return J_EISCONN;
		case (ECONNRESET):
			return J_ECONNRESET;
		case (EMSGSIZE):
			return J_EMSGSIZE;
		case (EPIPE):
			return J_EPIPE;
		case (ETIMEDOUT):
			return J_ETIMEDOUT;
		case (ENOBUFS):
		case (ENOSPC):
		case (ENOMEM):
			return J_ENOMEM;
		case (EHOSTUNREACH):
			return J_EHOSTUNREACH;
		default:
			return J_EUNKNOWN;
	}
}

#if LLNET_AF & LLNET_AF_IPV6

uint32_t getScopeForIp(const char *ip){
	struct ifaddrs *addrs;
	char ipAddress[NI_MAXHOST];
	uint32_t scope = 0;

	// walk over the list of all interface addresses
	getifaddrs(&addrs);
	for (struct ifaddrs *addr = addrs; addr; addr = addr->ifa_next) {
		if (addr->ifa_addr && addr->ifa_addr->sa_family == AF_INET6) { // only interested in ipv6 ones
			getnameinfo(addr->ifa_addr, sizeof(struct sockaddr_in6), ipAddress,
					sizeof(ipAddress), NULL, 0, NI_NUMERICHOST);
			// result actually contains the interface name, so strip it
			for (int i = 0; ipAddress[i]; i++) {
				if (ipAddress[i] == '%') {
					ipAddress[i] = '\0';
					break;
				}
			}
			// if the ip matches, convert the interface name to a scope index
			if (strcmp(ipAddress, ip) == 0) {
				scope = if_nametoindex(addr->ifa_name);
				break;
			}
		}
	}
	// If the scope wasn't set in the name search, set it to LLNET_IPV6_INTERFACE_NAME
	if(0 == scope) {
		scope = if_nametoindex(LLNET_IPV6_INTERFACE_NAME);
	}
	freeifaddrs(addrs);
	return scope;
}

/**
 * @brief  Map the given IPv4 address into an IPv6 address.<p>
 * <code>ipv4_addr</code> and <code>ipv6_addr</code> pointers can reference the same memory address.
 *
 * @param[in] ipv4_addr the IPv4 to map
 * @param[out] ipv6_addr the destination IPv6
 */
void map_ipv4_into_ipv6(in_addr_t* ipv4_addr, struct in6_addr* ipv6_addr){
	// Convert IPv4 into IPv6 directly in the addr array

	if(*(ipv4_addr) == llnet_htonl(INADDR_ANY)){
		// Use IPv6 wildcard address
		*ipv6_addr = in6addr_any;
	}
	else {
		uint8_t* ipv6_addr_u8 = (uint8_t*)ipv6_addr;
		// Destination offset of the IPv4 address: end of struct in6_addr
		int ipv4_dest_offset = sizeof(struct in6_addr) - sizeof(in_addr_t);
		// Copy the IPv4 address at the end of struct in6_addr
		memcpy(ipv6_addr_u8 + ipv4_dest_offset, ipv4_addr, sizeof(in_addr_t));

		// Add 0xFF:0xFF before
		ipv6_addr_u8[ipv4_dest_offset - 1] = 0xFF;
		ipv6_addr_u8[ipv4_dest_offset - 2] = 0xFF;

		// Set to 0 all the other bytes
		memset(ipv6_addr_u8, 0, ipv4_dest_offset - 2);
	}
}

#endif
