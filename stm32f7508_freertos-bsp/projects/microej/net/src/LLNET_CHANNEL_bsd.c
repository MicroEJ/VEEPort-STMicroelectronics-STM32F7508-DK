/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLNET_CHANNEL 2.1.0 implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 1.1.1
 * @date 7 February 2020
 */

#include <LLNET_CHANNEL_impl.h>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "LLNET_CONSTANTS.h"
#include "async_select.h"
#include "async_select_cache.h"
#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"
#if LLNET_AF & LLNET_AF_IPV6
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#endif

#ifdef LLNET_IGNORE_SIGPIPE
#include <signal.h>
#endif

#ifdef __cplusplus
	extern "C" {
#endif

int32_t LLNET_CHANNEL_IMPL_close(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	if(llnet_close(fd) == -1){
		return map_to_java_exception(llnet_errno(fd));
	}

	async_select_remove_socket_timeout_from_cache(fd);
	async_select_notify_closed_fd(fd);

	return 0;
}

int32_t LLNET_CHANNEL_IMPL_initialize(void)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	int32_t res;
	
#ifdef LLNET_IGNORE_SIGPIPE
	// Ignore SIGPIPE signal that is sent when a connection is closed by the remote host.
	signal(SIGPIPE, SIG_IGN);
#endif

	async_select_init_socket_timeout_cache();

	res = async_select_init();
	if(res != 0){
		return J_EUNKNOWN;
	}

	return 0;

}

int32_t LLNET_CHANNEL_IMPL_setBlocking(int32_t fd, uint8_t blocking, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, blocking=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, blocking);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	int32_t res = set_socket_non_blocking(fd, blocking==0 ? true : false);
	if(res == 0){
		return 0;
	}
	return map_to_java_exception(llnet_errno(fd));
}

int32_t LLNET_CHANNEL_IMPL_shutdown(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);
	int32_t ret = llnet_shutdown(fd, SHUT_RD); //shutdown input stream
	(void)ret;
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X) ret=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, ret, llnet_errno(fd));
	return 0;
}

int32_t LLNET_CHANNEL_IMPL_bind(int32_t fd, int8_t* addr, int32_t length, int32_t port, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, port=%d, addrLength=%d,...)\n", __func__, SNI_getCurrentJavaThreadID(), fd, port, length);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

    union llnet_sockaddr sockaddr = {0};
    int sockaddr_sizeof = 0;


#if LLNET_AF == LLNET_AF_IPV4
    if(length == sizeof(in_addr_t))
    {
		sockaddr.in.sin_family = AF_INET;
		sockaddr.in.sin_port = llnet_htons(port);
		sockaddr.in.sin_addr.s_addr = *(in_addr_t*)addr;
		sockaddr_sizeof = sizeof(struct sockaddr_in);
    }
#endif

#if LLNET_AF == LLNET_AF_DUAL
	if(length == sizeof(in_addr_t) && SNI_getArrayLength(addr) >= sizeof(struct in6_addr)){
		// Convert IPv4 into IPv6 directly in the addr array
		map_ipv4_into_ipv6((in_addr_t*)addr, (struct in6_addr*)addr);

		// Now length of the address in addr is sizeof(struct in6_addr)
		length = sizeof(struct in6_addr);
		// continue in the following if
	}
#endif

#if LLNET_AF & LLNET_AF_IPV6
	if(length == sizeof(struct in6_addr))
	{
		char ipAddress[NI_MAXHOST];

		sockaddr.in6.sin6_family = AF_INET6;
		sockaddr.in6.sin6_port = llnet_htons(port);
		// Convert the incoming IP address to ASCII, lookup the interface and set the scope ID
		if(inet_ntop(AF_INET6, addr, ipAddress, NI_MAXHOST) != NULL) {
			sockaddr.in6.sin6_scope_id=getScopeForIp(ipAddress);
		} else {
			LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, inet_ntop failed, errno=%d, ...)\n", __func__, SNI_getCurrentJavaThreadID(), fd, errno);
			return J_EINVAL;
		}
		memcpy(&sockaddr.in6.sin6_addr, addr, sizeof(struct in6_addr));
		sockaddr_sizeof = sizeof(struct sockaddr_in6);
	}
#endif

	if(sockaddr_sizeof == 0){
		return J_EINVAL;
	}

	int32_t ret = llnet_bind(fd, &sockaddr.addr, sockaddr_sizeof);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, ret=%d, errno=%d, ...)\n", __func__, SNI_getCurrentJavaThreadID(), fd, ret, llnet_errno(fd));
	if(ret == -1){
		return map_to_java_exception(llnet_errno(fd));
	}
	return 0;
}

int32_t LLNET_CHANNEL_IMPL_getOption(int32_t fd, int32_t option, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	int32_t optname = -1;
	int32_t level = SOL_SOCKET;
	int32_t optlen = sizeof(int32_t);
	int32_t optVal = 0;
	struct timeval sock_timeout;
	struct linger sock_linger;
	void* p_optVal = &optVal;

	if (option == CPNET_SO_TIMEOUT) {
		optVal = async_select_get_socket_timeout_from_cache(fd);
		if (optVal != -1) {
			return optVal;
		}
	}

	switch (option) {
		case CPNET_SO_KEEPALIVE:
			optname = SO_KEEPALIVE;
			break;
		case CPNET_SO_BROADCAST:
			optname = SO_BROADCAST;
			break;
		case CPNET_SO_TIMEOUT :
			p_optVal = &sock_timeout;
			optlen = sizeof(sock_timeout);
			optname = SO_RCVTIMEO;
			break;
		case CPNET_SO_SNDBUF :
			optname = SO_SNDBUF;
			break;
		case CPNET_SO_RCVBUF :
			optname = SO_RCVBUF;
			break;
		case CPNET_TCP_NODELAY:
			level = IPPROTO_TCP;
			optname = TCP_NODELAY;
			break;
		case CPNET_IP_TOS:
			level = IPPROTO_IP;
			optname = IP_TOS;
			break;
		case CPNET_SO_REUSEADDR:
			optname = SO_REUSEADDR;
			break;
		case CPNET_IP_MULTICAST_LOOP:
#if LLNET_AF & LLNET_AF_IPV4
			// IPv4 only and dual stack
			// In the case of dual stack, it is assumed that IP_MULTICAST_LOOP
			// and IPV6_MULTICAST_LOOP always match
			level = IPPROTO_IP;
			optname = IP_MULTICAST_LOOP;
#else // IPv6 only
			level = IPPROTO_IPV6;
			optname = IPV6_MULTICAST_LOOP;
#endif
			break;
		case CPNET_IP_TTL:
			level = IPPROTO_IP;
			optname = IP_MULTICAST_TTL;
			break;
		case CPNET_SO_LINGER:
			p_optVal = &sock_linger;
			optlen = sizeof(sock_linger);
			optname = SO_LINGER;
			break;
		case CPNET_SO_OOBINLINE:
			optname = SO_OOBINLINE;
			break;
		default:
			//option not available
			return J_ENOPROTOOPT;
	}

	int32_t res = llnet_getsockopt(fd, level, optname, p_optVal, (socklen_t *)&optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d) res=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, res, llnet_errno(fd));
	if(res == -1){
		return map_to_java_exception(llnet_errno(fd));
	}

	if (option == CPNET_SO_TIMEOUT) {
		async_select_set_socket_timeout_in_cache(fd, (sock_timeout.tv_usec / 1000) + (sock_timeout.tv_sec * 1000));
	}

	if(option == CPNET_SO_TIMEOUT){
		return (sock_timeout.tv_usec / 1000) + (sock_timeout.tv_sec * 1000);
	} else if (option == CPNET_SO_LINGER){
		if (sock_linger.l_onoff == 0) {
			return -1;
		} else {
			return sock_linger.l_linger;
		}
	}
	return optVal;
}

int32_t LLNET_CHANNEL_setOption_multicast_loopback(int32_t fd, int32_t value)
{
	int32_t optname = -1;
	int32_t level = SOL_SOCKET;
	int32_t res;
	int optlen = sizeof(int32_t);
	void* p_optval = &value;

#if LLNET_AF & LLNET_AF_IPV4
	// Set the multicast option for IPv4
	optname = IP_MULTICAST_LOOP;
	level = IPPROTO_IP;
	res = llnet_setsockopt (fd, level, optname, p_optval, optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, value=%d), res=%d, errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, value, res, llnet_errno(fd));
	if (res == -1) {
		return map_to_java_exception(llnet_errno(fd));
	}
#endif
#if LLNET_AF & LLNET_AF_IPV6
	// Set the multicast option for IPv6
	optname = IPV6_MULTICAST_LOOP;
	level = IPPROTO_IPV6;
	res = llnet_setsockopt (fd, level, optname, p_optval, optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, value=%d), res=%d, errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, value, res, llnet_errno(fd));
	if (res == -1) {
		return map_to_java_exception(llnet_errno(fd));
	}
#endif
	return 0;
}

int32_t LLNET_CHANNEL_IMPL_setOption(int32_t fd, int32_t option, int32_t value, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d, value=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, value);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	int32_t optname = -1;
	int32_t level = SOL_SOCKET;
	struct timeval sock_timeout;
	struct linger sock_linger;

	int optlen = sizeof(int32_t);

	void* p_optval = &value;

	int sock_type, size;
	size = sizeof (int);

	switch (option) {
		case CPNET_SO_KEEPALIVE:
			optname = SO_KEEPALIVE;
			break;
		case CPNET_SO_BROADCAST:
			optname = SO_BROADCAST;
			break;
		case CPNET_SO_TIMEOUT:
			time_ms_to_timeval(value, &sock_timeout);
			p_optval = &sock_timeout;
			optlen = sizeof(sock_timeout);
			optname = SO_RCVTIMEO;
			break;
		case CPNET_SO_SNDBUF:
			optname = SO_SNDBUF;
			break;
		case CPNET_SO_RCVBUF:
			optname = SO_RCVBUF;
			break;
		case CPNET_TCP_NODELAY:
			level = IPPROTO_TCP;
			optname = TCP_NODELAY;
			break;
		case CPNET_IP_TOS:
			level = IPPROTO_IP;
			optname = IP_TOS;
			break;
		case CPNET_SO_REUSEADDR :
			optname = SO_REUSEADDR;
			break;
		case CPNET_IP_MULTICAST_LOOP:
			return LLNET_CHANNEL_setOption_multicast_loopback(fd, value);
		case CPNET_IP_TTL:
			level = IPPROTO_IP;
			optname = IP_MULTICAST_TTL;
			break;
		case CPNET_SO_LINGER:
			if (value == -1) {
				sock_linger.l_onoff = 0;
				sock_linger.l_linger = 0;
			} else if (value >= 0) {
				sock_linger.l_onoff = 1;
				sock_linger.l_linger = value;
			}
			p_optval = &sock_linger;
			optlen = sizeof(sock_linger);
			optname = SO_LINGER;
			break;
		case CPNET_SO_OOBINLINE:
			optname = SO_OOBINLINE;
			break;
		default:
			/* option not available */
			return J_ENOPROTOOPT;
	}
	int32_t res = llnet_setsockopt (fd, level, optname, p_optval, optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d, value=%d), res=%d, errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, value, res, llnet_errno(fd));
	if (res == -1) {
		return map_to_java_exception(llnet_errno(fd));
	}

	if (option == CPNET_SO_TIMEOUT) {
		async_select_set_socket_timeout_in_cache(fd, value);
	} else if(option == CPNET_SO_REUSEADDR){
		llnet_getsockopt(fd, level, SO_TYPE, (char *)&sock_type, (socklen_t *)&size);
		if(sock_type == SOCK_DGRAM){
			// Force enable/disable SO_REUSEPORT on UDP/Multicast sockets
			// to allow multiple multicast sockets to be bound to the same address
			llnet_setsockopt (fd, level, SO_REUSEPORT, p_optval, optlen);
		}
	}
	return 0;
}

int32_t LLNET_CHANNEL_IMPL_getOptionAsByteArray(int32_t fd, int32_t option, int8_t* dst, int32_t dstLength, uint8_t retry)
{
	/* remove unused retry parameter warning*/
	(void)(retry);

	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d, dstLength=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, dstLength);

	switch (option) {
		case CPNET_IP_MULTICAST_IF:
		case CPNET_IP_MULTICAST_IF2:
			{
#if LLNET_AF & LLNET_AF_IPV4
				// Option valid only for IPv4
				if(dstLength >= sizeof(struct in_addr)){

					socklen_t option_length = sizeof(struct in_addr);

					int32_t res = llnet_getsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, dst, &option_length);
					LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d) res=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, res, llnet_errno(fd));
					if(res == -1){
						return map_to_java_exception(llnet_errno(fd));
					}

					return option_length;
				}
				else {
					return J_EINVAL;
				}
#else //Only IPv6
				// Not managed
				return J_ENOPROTOOPT;
#endif
			}
		default:
			//option not available
			return J_ENOPROTOOPT;
	}
}

int32_t LLNET_CHANNEL_IMPL_setOptionAsByteArray(int32_t fd, int32_t option, int8_t* value, int32_t valueLength, uint8_t retry)
{
	/* remove unused retry parameter warning*/
	(void)(retry);

	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d, valueLength=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, valueLength);

	switch (option) {
		case CPNET_IP_MULTICAST_IF:
		case CPNET_IP_MULTICAST_IF2:
			{
#if LLNET_AF & LLNET_AF_IPV4
				// Option valid only for IPv4
				if(valueLength == sizeof(struct in_addr)){
					int32_t res;
					res = llnet_setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, value, sizeof(struct in_addr));
					LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d) res=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, res, llnet_errno(fd));
					if(res == -1){
						return map_to_java_exception(llnet_errno(fd));
					}
					return 0;
				}
#endif
#if LLNET_AF & LLNET_AF_IPV6
				//Only IPv6
				if(valueLength == sizeof(struct in6_addr)) {
					// get interface name
					struct ifaddrs * ifaddrs = NULL;
					struct ifaddrs * i;
					if(getifaddrs(&ifaddrs) != 0) {
						return J_EUNKNOWN;
					}
					uint32_t ifindex;
					// Walk the interface address structure until we match the interface address
					for (i = ifaddrs; i != NULL; i = i->ifa_next) {
						if (AF_INET6 == i->ifa_addr->sa_family) {
							if(memcmp(value, ((struct sockaddr_in6*)i->ifa_addr)->sin6_addr.s6_addr, valueLength) == 0) {
								ifindex = if_nametoindex(i->ifa_name);
								break;
							}
						}
					}
					freeifaddrs(ifaddrs);

					int32_t res;
					res = llnet_setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, &ifindex, sizeof(ifindex));
					LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d) res=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, res, llnet_errno(fd));
					if(res == -1){
						return map_to_java_exception(llnet_errno(fd));
					}
					return 0;
				}
#endif
				return J_EINVAL;
			}
		default:
			/* option not available */
			return J_ENOPROTOOPT;
	}
}

int32_t LLNET_CHANNEL_IMPL_listen(int32_t fd, int32_t backlog, uint8_t retry)
{
	/* remove unused retry parameter warning*/
	(void)(retry);

	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, backlog=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, backlog);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	int32_t res = llnet_listen(fd, backlog);
	if(res == -1){
		return map_to_java_exception(llnet_errno(fd));
	}
	return 0;
}

#ifdef __cplusplus
	}
#endif
