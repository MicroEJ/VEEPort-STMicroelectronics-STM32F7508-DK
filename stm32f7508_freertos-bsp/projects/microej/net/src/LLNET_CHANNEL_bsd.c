/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_CHANNEL 3.0.0 implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 2.0.0
 * @date 17 June 2022
 */

#include "LLNET_CHANNEL_impl.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "sni.h"
#include "LLNET_configuration.h"
#include "async_select.h"
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

void LLNET_CHANNEL_IMPL_close(int32_t fd)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

	int32_t fd_errno;

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return;
    }

	if(llnet_close(fd) == -1){
		fd_errno = llnet_errno(fd);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
		LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X) close error (errno=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, fd_errno);
		return;
	}
	async_select_notify_closed_fd(fd);
}

void LLNET_CHANNEL_IMPL_initialize(void)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	int32_t res;
#ifdef LLNET_IGNORE_SIGPIPE
	// Ignore SIGPIPE signal that is sent when a connection is closed by the remote host.
	signal(SIGPIPE, SIG_IGN);
#endif
	llnet_init();
	res = async_select_init();
	if(res != 0){
		SNI_throwNativeIOException(J_EUNKNOWN, "init error");
	}
}


void LLNET_CHANNEL_IMPL_shutdown(int32_t fd)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return;
    }

	int32_t ret = llnet_shutdown(fd, SHUT_RDWR); //shutdown
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X) ret=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, ret, llnet_errno(fd));
}

void LLNET_CHANNEL_IMPL_bind(int32_t fd, int8_t* addr, int32_t length, int32_t port)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, port=%d, addrLength=%d,...)\n", __func__, SNI_getCurrentJavaThreadID(), fd, port, length);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return;
    }

	union llnet_sockaddr sockaddr = {0};
	int32_t sockaddr_sizeof = 0;
    int32_t fd_errno;


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
	if(length == sizeof(in_addr_t)){
		// Convert IPv4 into IPv6 and put the result directly in the in6_addr struct
		LLNET_map_ipv4_into_ipv6((in_addr_t*)addr, (struct in6_addr*)&sockaddr.in6.sin6_addr);
		// Update length and addr
		length = sizeof(struct in6_addr);
		addr = (int8_t*)&sockaddr.in6.sin6_addr;
		// Continue in the following if
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
			sockaddr.in6.sin6_scope_id=LLNET_getScopeForIp(ipAddress);
		} else {
			fd_errno = llnet_errno(fd);
			SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
			LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, inet_ntop failed, errno=%d, ...)\n", __func__, SNI_getCurrentJavaThreadID(), fd, fd_errno);
			return;
		}
		// Skip copy if in6_addr struct already contains the IPv6 address
		if((void*)addr != (void*)&sockaddr.in6.sin6_addr){
			memcpy((void*)&sockaddr.in6.sin6_addr, addr, sizeof(struct in6_addr));
		}
		sockaddr_sizeof = sizeof(struct sockaddr_in6);
	}
#endif

	if(sockaddr_sizeof == 0){
		SNI_throwNativeIOException(J_EINVAL, "invalid address length");
		return;
	}

	int32_t ret = llnet_bind(fd, &sockaddr.addr, sockaddr_sizeof);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, ret=%d, errno=%d, ...)\n", __func__, SNI_getCurrentJavaThreadID(), fd, ret, llnet_errno(fd));
	if(ret == -1){
		fd_errno = llnet_errno(fd);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
	}
}

int32_t LLNET_CHANNEL_IMPL_getOption(int32_t fd, int32_t option)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return SNI_IGNORED_RETURNED_VALUE;
    }

	int32_t optname = -1;
	int32_t level = SOL_SOCKET;
	int32_t optlen = sizeof(int32_t);
	int32_t optVal = 0;
	struct linger sock_linger = {0};
	void* p_optVal = &optVal;
	int32_t fd_errno;

	switch (option) {
		case LLNET_SOCKETOPTION_SO_KEEPALIVE:
			optname = SO_KEEPALIVE;
			break;
		case LLNET_SOCKETOPTION_SO_BROADCAST:
			optname = SO_BROADCAST;
			break;
		case LLNET_SOCKETOPTION_SO_SNDBUF :
			optname = SO_SNDBUF;
			break;
		case LLNET_SOCKETOPTION_SO_RCVBUF :
			optname = SO_RCVBUF;
			break;
		case LLNET_SOCKETOPTION_TCP_NODELAY:
			level = IPPROTO_TCP;
			optname = TCP_NODELAY;
			break;
		case LLNET_SOCKETOPTION_IP_TOS:
			level = IPPROTO_IP;
			optname = IP_TOS;
			break;
		case LLNET_SOCKETOPTION_SO_REUSEADDR:
			optname = SO_REUSEADDR;
			break;
		case LLNET_SOCKETOPTION_IP_MULTICAST_LOOP:
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
		case LLNET_SOCKETOPTION_IP_TTL:
			level = IPPROTO_IP;
			optname = IP_MULTICAST_TTL;
			break;
		case LLNET_SOCKETOPTION_SO_LINGER:
			p_optVal = &sock_linger;
			optlen = sizeof(sock_linger);
			optname = SO_LINGER;
			break;
		case LLNET_SOCKETOPTION_SO_OOBINLINE:
			optname = SO_OOBINLINE;
			break;
		default:
			//option not available
			SNI_throwNativeIOException(J_ENOPROTOOPT, "socket option not supported");
			return SNI_IGNORED_RETURNED_VALUE;
	}

	int32_t res = llnet_getsockopt(fd, level, optname, p_optVal, (socklen_t *)&optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d) res=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, res, llnet_errno(fd));
	if(res == -1){
		fd_errno = llnet_errno(fd);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
		return SNI_IGNORED_RETURNED_VALUE;
	}

	if (option == LLNET_SOCKETOPTION_SO_LINGER){
		if (sock_linger.l_onoff == 0) {
			return -1;
		} else {
			return sock_linger.l_linger;
		}
	}
	return optVal;
}

void LLNET_CHANNEL_setOption_multicast_loopback(int32_t fd, int32_t value)
{
	int32_t optname = -1;
	int32_t level = SOL_SOCKET;
	int32_t res;
	int32_t fd_errno;
	int32_t optlen = sizeof(int32_t);
	void* p_optval = &value;

#if LLNET_AF & LLNET_AF_IPV4
	// Set the multicast option for IPv4
	optname = IP_MULTICAST_LOOP;
	level = IPPROTO_IP;
	res = llnet_setsockopt (fd, level, optname, p_optval, optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, value=%d), res=%d, errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, value, res, llnet_errno(fd));
	if (res == -1) {
		fd_errno = llnet_errno(fd);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
		return;
	}
#endif
#if LLNET_AF & LLNET_AF_IPV6
	// Set the multicast option for IPv6
	optname = IPV6_MULTICAST_LOOP;
	level = IPPROTO_IPV6;
	res = llnet_setsockopt (fd, level, optname, p_optval, optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, value=%d), res=%d, errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, value, res, llnet_errno(fd));
	if (res == -1) {
		fd_errno = llnet_errno(fd);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
	}
#endif
}

void LLNET_CHANNEL_IMPL_setOption(int32_t fd, int32_t option, int32_t value)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d, value=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, value);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return;
    }

	int32_t optname = -1;
	int32_t level = SOL_SOCKET;
	struct linger sock_linger;

	int32_t optlen = sizeof(int32_t);

	void* p_optval = &value;

	int32_t sock_type;
        uint32_t size;
	size = sizeof (uint32_t);
	int32_t fd_errno;

	switch (option) {
		case LLNET_SOCKETOPTION_SO_KEEPALIVE:
			optname = SO_KEEPALIVE;
			break;
		case LLNET_SOCKETOPTION_SO_BROADCAST:
			optname = SO_BROADCAST;
			break;
		case LLNET_SOCKETOPTION_SO_SNDBUF:
			optname = SO_SNDBUF;
			break;
		case LLNET_SOCKETOPTION_SO_RCVBUF:
			optname = SO_RCVBUF;
			break;
		case LLNET_SOCKETOPTION_TCP_NODELAY:
			level = IPPROTO_TCP;
			optname = TCP_NODELAY;
			break;
		case LLNET_SOCKETOPTION_IP_TOS:
			level = IPPROTO_IP;
			optname = IP_TOS;
			break;
		case LLNET_SOCKETOPTION_SO_REUSEADDR :
			optname = SO_REUSEADDR;
			break;
		case LLNET_SOCKETOPTION_IP_MULTICAST_LOOP:
			LLNET_CHANNEL_setOption_multicast_loopback(fd, value);
			return;
		case LLNET_SOCKETOPTION_IP_TTL:
			level = IPPROTO_IP;
			optname = IP_MULTICAST_TTL;
			break;
		case LLNET_SOCKETOPTION_SO_LINGER:
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
		case LLNET_SOCKETOPTION_SO_OOBINLINE:
			optname = SO_OOBINLINE;
			break;
		default:
			/* option not available */
			LLNET_DEBUG_TRACE("option not supported %d\n",option);
			SNI_throwNativeIOException(J_ENOPROTOOPT, "socket option not supported");
			return;
	}
	int32_t res = llnet_setsockopt (fd, level, optname, p_optval, optlen);
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d, value=%d), res=%d, errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, value, res, llnet_errno(fd));
	if (res == -1) {
		fd_errno = llnet_errno(fd);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
		return;
	}

	if(option == LLNET_SOCKETOPTION_SO_REUSEADDR){
		llnet_getsockopt(fd, level, SO_TYPE, (char *)&sock_type, &size);
		if(sock_type == SOCK_DGRAM){
			// Force enable/disable SO_REUSEPORT on UDP/Multicast sockets
			// to allow multiple multicast sockets to be bound to the same address
			llnet_setsockopt (fd, level, SO_REUSEPORT, p_optval, optlen);
		}
	}
}

int32_t LLNET_CHANNEL_IMPL_getOptionAsByteArray(int32_t fd, int32_t option, int8_t* dst, int32_t dstLength)
{

	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d, dstLength=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, dstLength);

	int32_t fd_errno;
	switch (option) {
		case LLNET_SOCKETOPTION_IP_MULTICAST_IF:
		case LLNET_SOCKETOPTION_IP_MULTICAST_IF2:
			{
#if LLNET_AF & LLNET_AF_IPV4
				// Option valid only for IPv4
				if((uint32_t)dstLength >= sizeof(struct in_addr)){

					socklen_t option_length = sizeof(struct in_addr);

					int32_t res = llnet_getsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, dst, &option_length);
					LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d) res=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, res, llnet_errno(fd));
					if(res == -1){
						fd_errno = llnet_errno(fd);
						SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
						return SNI_IGNORED_RETURNED_VALUE;
					}

					return option_length;
				}
				else {
					SNI_throwNativeIOException(J_EINVAL, "wrong option length");
					return SNI_IGNORED_RETURNED_VALUE;
				}
#else //Only IPv6
				/* Remove unused parameters warning*/
				(void)fd;
				(void)dst;
				(void)dstLength
				// Not managed
				SNI_throwNativeIOException(J_ENOPROTOOPT, "socket option not supported for ipv6");
				return SNI_IGNORED_RETURNED_VALUE;
#endif
			}
		default:
			//option not available
			SNI_throwNativeIOException(J_ENOPROTOOPT, "socket option not supported");
			return SNI_IGNORED_RETURNED_VALUE;
	}
}

void LLNET_CHANNEL_IMPL_setOptionAsByteArray(int32_t fd, int32_t option, int8_t* value, int32_t valueLength)
{

	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d, valueLength=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, valueLength);

	int32_t fd_errno;

	switch (option) {
		case LLNET_SOCKETOPTION_IP_MULTICAST_IF:
		case LLNET_SOCKETOPTION_IP_MULTICAST_IF2:
			{
#if LLNET_AF & LLNET_AF_IPV4
				// Option valid only for IPv4
				if(valueLength == sizeof(struct in_addr)){
					int32_t res;
					res = llnet_setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, value, sizeof(struct in_addr));
					LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d) res=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, res, llnet_errno(fd));
					if(res == -1){
						fd_errno = llnet_errno(fd);
						SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
					}
					return;
				}
#endif
#if LLNET_AF & LLNET_AF_IPV6
				//Only IPv6
				if(valueLength == sizeof(struct in6_addr)) {
					// get interface name
					struct ifaddrs * ifaddrs = NULL;
					struct ifaddrs * i;
					if(getifaddrs(&ifaddrs) != 0) {
						fd_errno = llnet_errno(fd);
						SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
						return;
					}
					uint32_t ifindex = 0;
					// Walk the interface address structure until we match the interface address
					for (i = ifaddrs; i != NULL; i = i->ifa_next) {
						if (NULL != i->ifa_addr && AF_INET6 == i->ifa_addr->sa_family) {
							if(memcmp(value, ((struct sockaddr_in6*)i->ifa_addr)->sin6_addr.s6_addr, valueLength) == 0) {
								ifindex = if_nametoindex(i->ifa_name);
								break;
							}
						}
					}
					freeifaddrs(ifaddrs);
					if(ifindex == 0){
						SNI_throwNativeIOException(J_EINVAL, "No interface index found");
						return;
					}
					int32_t res = llnet_setsockopt(fd, IPPROTO_IPV6, IPV6_MULTICAST_IF, (void*)&ifindex, sizeof(ifindex));
					LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, option=%d) res=%d errno=%d\n", __func__, SNI_getCurrentJavaThreadID(), fd, option, res, llnet_errno(fd));
					if(res == -1){
						fd_errno = llnet_errno(fd);
						SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
					}
					return;
				}
#endif
				SNI_throwNativeIOException(J_EINVAL, "wrong option length");
				return;
			}
		default:
			/* option not available */
			SNI_throwNativeIOException(J_ENOPROTOOPT, "socket option not supported");
	}
}

void LLNET_CHANNEL_IMPL_listen(int32_t fd, int32_t backlog)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, backlog=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, backlog);

	int32_t fd_errno;

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return;
    }

	int32_t res = llnet_listen(fd, backlog);
	if(res == -1){
		fd_errno = llnet_errno(fd);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
		return;
	}
}

#ifdef __cplusplus
	}
#endif
