/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_SOCKETCHANNEL 3.0.0 implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 2.0.0
 * @date 17 June 2022
 */


#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "LLNET_Common.h"
#if LLNET_AF & LLNET_AF_IPV6
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#endif
#include <sys/ioctl.h>
#include <unistd.h>

#include "LLNET_SOCKETCHANNEL_impl.h"
#include "LLNET_NETWORKADDRESS_impl.h"
#include "LLNET_ERRORS.h"
#include "sni.h"
#include "LLNET_configuration.h"

#ifdef __cplusplus
	extern "C" {
#endif

static void LLNET_SOCKETCHANNEL_connect_callback(int32_t fd, int8_t* addr, int32_t length, int32_t port, int64_t absoluteTimeout);
static int32_t LLNET_SOCKETCHANNEL_get_address(int32_t fd, int8_t* name, int32_t nameLength, uint8_t localAddress);
static int32_t LLNET_SOCKETCHANNEL_get_port(int32_t fd, uint8_t localPort);

static int32_t LLNET_SOCKETCHANNEL_get_port(int32_t fd, uint8_t localPort){
	union llnet_sockaddr sockaddr = {0};

	uint32_t addrlen = sizeof(sockaddr);
	int32_t ret = -1;
	if(localPort){
		ret = llnet_getsockname(fd, &sockaddr.addr, (socklen_t*)&addrlen);
	}else{
		ret = llnet_getpeername(fd, &sockaddr.addr, (socklen_t*)&addrlen);
	}

	if (ret == 0){
#if LLNET_AF & LLNET_AF_IPV4
		if(sockaddr.addr.sa_family == AF_INET) {
			return llnet_ntohs(sockaddr.in.sin_port);
		}
#endif
#if LLNET_AF & LLNET_AF_IPV6
		if(sockaddr.addr.sa_family == AF_INET6) {
			return llnet_ntohs(sockaddr.in6.sin6_port);
		}
#endif
		SNI_throwNativeIOException(J_EAFNOSUPPORT, "unsupported address family");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	int32_t fd_errno = llnet_errno(fd);
	SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
	return SNI_IGNORED_RETURNED_VALUE;
}

static int32_t LLNET_SOCKETCHANNEL_get_address(int32_t fd, int8_t* name, int32_t nameLength, uint8_t localAddress){

	union llnet_sockaddr sockaddr = {0};
	uint32_t addrlen = sizeof(sockaddr);
	int32_t ret = -1;
	if(localAddress){
		ret = llnet_getsockname(fd, &sockaddr.addr, (socklen_t*)&addrlen);
		LLNET_DEBUG_TRACE("getsockname(fd=0x%X) ret=%d errno=%d\n", fd,ret,llnet_errno(fd));
	}else{
		ret = llnet_getpeername(fd, &sockaddr.addr, (socklen_t*)&addrlen);
		LLNET_DEBUG_TRACE("getpeername(fd=0x%X) ret=%d errno=%d\n", fd,ret,llnet_errno(fd));
	}
	if (ret == 0){
#if LLNET_AF & LLNET_AF_IPV4
		if(sockaddr.addr.sa_family == AF_INET) {
			if(!localAddress && (sockaddr.in.sin_addr.s_addr == 0 || sockaddr.in.sin_port == llnet_htons(0))){
				//not connected
				SNI_throwNativeIOException(J_ENOTCONN, "not connected");
				return SNI_IGNORED_RETURNED_VALUE;
			}
			if((uint32_t)nameLength < sizeof(in_addr_t)){
				SNI_throwNativeIOException(J_EINVAL, "wrong name length");
				return SNI_IGNORED_RETURNED_VALUE;
			}
			*(in_addr_t*)name = sockaddr.in.sin_addr.s_addr;
			return sizeof(in_addr_t);
		}
#endif
#if LLNET_AF & LLNET_AF_IPV6
		if(sockaddr.addr.sa_family == AF_INET6) {
			if(!localAddress && (sockaddr.in6.sin6_addr.s6_addr == 0 || sockaddr.in6.sin6_port == llnet_htons(0))){
				//not connected
				SNI_throwNativeIOException(J_ENOTCONN, "not connected");
				return SNI_IGNORED_RETURNED_VALUE;
			}
			if((uint32_t)nameLength < sizeof(struct in6_addr)){
				SNI_throwNativeIOException(J_EINVAL, "wrong name length");
				return SNI_IGNORED_RETURNED_VALUE;
			}
			memcpy(name, (void *)&(sockaddr.in6.sin6_addr), sizeof(struct in6_addr));
			return sizeof(struct in6_addr);
		}
#endif
		SNI_throwNativeIOException(J_EAFNOSUPPORT, "unsupported address family");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	int32_t fd_errno = llnet_errno(fd);
	SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(llnet_errno(fd)));
	return SNI_IGNORED_RETURNED_VALUE;
}

static void LLNET_SOCKETCHANNEL_connect_callback(int32_t fd, int8_t* addr, int32_t length, int32_t port, int64_t absoluteTimeout)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, port=%d, absoluteTimeout=%d)\n", __func__, SNI_getCurrentJavaThreadID(), fd, port, absoluteTimeout);
	int32_t error_status = -1;
	int32_t error_status_size = sizeof(error_status);
	union llnet_sockaddr sockaddr = {0};
	int32_t selectRes;
	uint32_t addrlen = sizeof(sockaddr);
	struct timeval zero_timeout = {0};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	//We are in the connect callback and this means that either the connect is completed successfully (fd ready for write operation) or the timeout has expired.
	//To determine if the socket is connected:
	// 1- read SO_ERROR option to check if the socket has a non zero error status
	// 2- call llnet_getpeername() to check if the connected peer address can be retrieved
	// 3- perform a non-blocking select to check if the socket is ready for write operation
	if(0 == getsockopt(fd, SOL_SOCKET, SO_ERROR, &error_status, (socklen_t *)&error_status_size)
		&& 0 == error_status
		&& 0 == llnet_getpeername(fd, &sockaddr.addr, (socklen_t*) &addrlen)){

		selectRes = select(fd+1, NULL, &fds, NULL, &zero_timeout);
		if(0 < selectRes){
			// connection completed
			return;
		}
		if(0 == selectRes){
			//the select timeout expires => means the fd is not yet ready and the connection is still in progress
			//simulate an EAGAIN error to allow this callback to be called again until the connection succeeds or an error occurs.
			error_status = EAGAIN;
		}
	}
	//error
	LLNET_DEBUG_TRACE("[%s] error code=%d\n", __func__, 0==error_status? llnet_errno(fd): error_status);
	LLNET_handle_blocking_operation_error(fd, 0==error_status? llnet_errno(fd): error_status, SELECT_WRITE, absoluteTimeout, (SNI_callback)LLNET_SOCKETCHANNEL_connect_callback, NULL);

}


void LLNET_SOCKETCHANNEL_IMPL_connect(int32_t fd, int8_t* addr, int32_t length, int32_t port, int64_t absoluteTimeout)
{

	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, port=%d, timeout=%d)\n", __func__, SNI_getCurrentJavaThreadID(),fd, port, absoluteTimeout);
	int32_t connectRes;
	union llnet_sockaddr sockaddr = {0};
	int sockaddr_sizeof = 0;

    if(llnet_is_ready() == false){
    	SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
    	return;
    }
#if LLNET_AF == LLNET_AF_IPV4
	if(length == sizeof(in_addr_t)){
		sockaddr.in.sin_family = AF_INET;
		sockaddr.in.sin_port = llnet_htons(port);
		sockaddr.in.sin_addr.s_addr = *((in_addr_t*)addr);
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
	if(length == sizeof(struct in6_addr)){
		char ipAddress[NI_MAXHOST];
		sockaddr.in6.sin6_family = AF_INET6;
		sockaddr.in6.sin6_port = llnet_htons(port);
		// Convert the incoming IP address to ASCII, lookup the interface and set the scope ID
		if(inet_ntop(AF_INET6, addr, ipAddress, NI_MAXHOST) != NULL) {
			sockaddr.in6.sin6_scope_id=LLNET_getScopeForIp(ipAddress);
		} else {
			int32_t fd_errno = llnet_errno(fd);
			LLNET_DEBUG_TRACE("%s[thread %d] inet_ntop failed, errno = %d\n", __func__, SNI_getCurrentJavaThreadID(), fd_errno);
			SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
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
		SNI_throwNativeIOException(J_EINVAL, "wrong address size");
		return;
	}

	connectRes = llnet_connect(fd, &sockaddr.addr, sockaddr_sizeof);

	if(connectRes < 0){
		//connect error
		LLNET_handle_blocking_operation_error(fd, llnet_errno(fd), SELECT_WRITE, absoluteTimeout, (SNI_callback)LLNET_SOCKETCHANNEL_connect_callback, NULL);
	}
	//else: successful connection
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalPort(int32_t fd)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return SNI_IGNORED_RETURNED_VALUE;
    }

	return LLNET_SOCKETCHANNEL_get_port(fd, JTRUE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalAddress(int32_t fd, int8_t* name, int32_t length)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return SNI_IGNORED_RETURNED_VALUE;
    }

	return LLNET_SOCKETCHANNEL_get_address(fd, name, length, JTRUE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerPort(int32_t fd)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return SNI_IGNORED_RETURNED_VALUE;
    }

	return LLNET_SOCKETCHANNEL_get_port(fd, JFALSE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerAddress(int32_t fd, int8_t* name, int32_t length)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return SNI_IGNORED_RETURNED_VALUE;
    }

	return LLNET_SOCKETCHANNEL_get_address(fd, name, length, JFALSE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_socket(uint8_t stream)
{
	LLNET_DEBUG_TRACE("%s[thread %d](stream=%d)\n", __func__, SNI_getCurrentJavaThreadID(), stream);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return SNI_IGNORED_RETURNED_VALUE;
    }

	int32_t ret;
	int32_t sock_protocol = 0; //default value
	int domain;
	int32_t fd_errno;

// When IPv6 is available, always use IPv6 (even for IPv4 connections)
#if LLNET_AF & LLNET_AF_IPV6
	domain = AF_INET6;
#else // only IPv4
	domain = AF_INET;
#endif

	ret = llnet_socket(domain, stream ? SOCK_STREAM : SOCK_DGRAM, sock_protocol);
	if (ret == -1) {
		fd_errno = llnet_errno(-1);
		LLNET_DEBUG_TRACE("%s: llnet_socket() errno=%d, err=%d\n", __func__, fd_errno, ret);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
		return SNI_IGNORED_RETURNED_VALUE;
	}

	int32_t fd = ret;
	ret = LLNET_set_non_blocking(fd);
	//set the socket in non blocking mode
	if(0 != ret){
		fd_errno = llnet_errno(fd);
		LLNET_DEBUG_TRACE("%s: set_non_blocking errno=%d, err=%d\n", __func__, fd_errno, ret);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
		close(fd);
		return SNI_IGNORED_RETURNED_VALUE;
	}

// If dual stack IPv4/IPv6
#if LLNET_AF == LLNET_AF_DUAL
    // Disable IPV6_V6ONLY to ensure dual-socket support
	int ipv6_only = 0;
	ret = llnet_setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6_only, sizeof(int));
	if (ret == -1) {
		fd_errno = llnet_errno(fd);
		LLNET_DEBUG_TRACE("%s: llnet_setsockopt() errno=%d, err=%d\n", __func__, fd_errno, ret);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
		close(fd);
		return SNI_IGNORED_RETURNED_VALUE;
	}
#endif

	return fd;
}

int32_t LLNET_SOCKETCHANNEL_IMPL_serverSocket(){
	return LLNET_SOCKETCHANNEL_IMPL_socket(JTRUE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_multicastSocket(){
	return LLNET_SOCKETCHANNEL_IMPL_socket(JFALSE);
}

#ifdef __cplusplus
	}
#endif
