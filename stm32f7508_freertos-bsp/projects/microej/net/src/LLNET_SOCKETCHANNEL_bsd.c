/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLNET_SOCKETCHANNEL 2.1.0 implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 1.1.1
 * @date 7 February 2020
 */

#include <LLNET_SOCKETCHANNEL_impl.h>

#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include "LLNET_Common.h"
#if LLNET_AF & LLNET_AF_IPV6
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#endif
#include <stdbool.h>
#include "LLNET_CONSTANTS.h"
#include "LLNET_NETWORKADDRESS_impl.h"
#include "LLNET_ERRORS.h"

#ifdef __cplusplus
	extern "C" {
#endif

// external function used to retrieve currentTime (same as MicroJvm)
extern int64_t LLMJVM_IMPL_getCurrentTime__Z(uint8_t system);
		
static int32_t SocketChanel_Address(int32_t fd, int8_t* name, int32_t nameLength, uint8_t localAddress);

static int32_t SocketChanel_Port(int32_t fd, uint8_t localPort);

int32_t LLNET_SOCKETCHANNEL_IMPL_connect(int32_t fd, int8_t* addr, int32_t length, int32_t port, int32_t timeout, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X, port=%d, timeout=%d)\n", __func__, SNI_getCurrentJavaThreadID(),fd, port, timeout);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	int32_t res;

	if(retry){
		union llnet_sockaddr sockaddr = {0};
		uint32_t addrlen = sizeof(sockaddr);

		//check if the socket is connected
		int32_t selectRes = non_blocking_select(fd, SELECT_WRITE);
		int32_t peerRes = llnet_getpeername(fd, &sockaddr.addr, (socklen_t*)&addrlen);
		int32_t socket_port = -1;
#if LLNET_AF & LLNET_AF_IPV4
		if (sockaddr.addr.sa_family == AF_INET) {
			socket_port = llnet_ntohs(sockaddr.in.sin_port);
		}
#endif
#if LLNET_AF & LLNET_AF_IPV6
		if (sockaddr.addr.sa_family == AF_INET6) {
			socket_port = llnet_ntohs(sockaddr.in6.sin6_port);
		}
#endif
		if(socket_port == -1) {
			return J_EINVAL;
		}

		if(selectRes > 0 && (peerRes == 0 && socket_port > 0)){
			//the socket is connected to a remote host
			return 1; //success
		}
		if(selectRes == 0 || (peerRes < 0 && llnet_errno(fd) == ENOTCONN)){
			//the socket is not connected or
			//a timeout expires during the first connect
			return J_ENOTCONN;
		}
		//other error
		return map_to_java_exception(llnet_errno(fd));
	}
	else { // retry == false

		union llnet_sockaddr sockaddr = {0};
		int sockaddr_sizeof = 0;

#if LLNET_AF == LLNET_AF_IPV4
		if(length == sizeof(in_addr_t)){
			sockaddr.in.sin_family = AF_INET;
			sockaddr.in.sin_port = llnet_htons(port);
			sockaddr.in.sin_addr.s_addr = *((in_addr_t*)addr);
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
		if(length == sizeof(struct in6_addr)){
			char ipAddress[NI_MAXHOST];
			sockaddr.in6.sin6_family = AF_INET6;
			sockaddr.in6.sin6_port = llnet_htons(port);
			// Convert the incoming IP address to ASCII, lookup the interface and set the scope ID
			if(inet_ntop(AF_INET6, addr, ipAddress, NI_MAXHOST) != NULL) {
				sockaddr.in6.sin6_scope_id=getScopeForIp(ipAddress);
			} else {
				LLNET_DEBUG_TRACE("%s[thread %d] inet_ntop failed, errno = %d\n", __func__, SNI_getCurrentJavaThreadID(), errno);
				return J_EINVAL;
			}
			memcpy((void*)&sockaddr.in6.sin6_addr, addr, sizeof(struct in6_addr));
			sockaddr_sizeof = sizeof(struct sockaddr_in6);
		}
#endif

		if(sockaddr_sizeof == 0){
			return J_EINVAL;
		}

		// Set nonblocking mode.
		bool was_non_blocking = is_socket_non_blocking(fd);
		if(was_non_blocking == false){
			res = set_socket_non_blocking(fd, true);
			if (res != 0) {
				return map_to_java_exception(llnet_errno(fd));
			}
		}
	
		int32_t connectRes = llnet_connect(fd, &sockaddr.addr, sockaddr_sizeof);
		int32_t connectErrno = llnet_errno(fd);

		// Set blocking mode.
		if(was_non_blocking == false){
			// The socket was blocking initially.
			res = set_socket_non_blocking(fd, false);
			if (res != 0) {
				return map_to_java_exception(llnet_errno(fd));
			}
		}

		if (connectRes == -1) {
			if(connectErrno == EINPROGRESS) {
				//The connection can not be completed immediately.

				if(was_non_blocking == true){
					// the socket is in non-blocking mode and the connection did not immediately succeed
					return 0;
				}

				// A blocking request on write operation is added in the dispatch event queue for completion.
				int32_t asyncSelectRes = async_select(fd, SELECT_WRITE, timeout, NULL);
				if(asyncSelectRes == 0){
					// request added in the queue
					LLNET_DEBUG_TRACE("Connect has blocked without result\n");
					return J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT;
				}
				// requests queue limit reached
				LLNET_DEBUG_TRACE("Blocking Request Queue Limit Reached\n");
				return J_ASYNC_BLOCKING_REQUEST_QUEUE_LIMIT_REACHED;
			}
			else {
				LLNET_DEBUG_TRACE("[ERROR] Connection failed (retry=%d, errno=%d)\n", retry, connectErrno);
				return map_to_java_exception(connectErrno);
			}
		}
		else {
			return 1;// connected
		}
	}
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalPort(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	return SocketChanel_Port(fd, JTRUE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getLocalAddress(int32_t fd, int8_t* name, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	return SocketChanel_Address(fd, name, length, JTRUE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerPort(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	return SocketChanel_Port(fd, JFALSE);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_getPeerAddress(int32_t fd, int8_t* name, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](fd=0x%X)\n", __func__, SNI_getCurrentJavaThreadID(), fd);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	return SocketChanel_Address(fd, name, length, JFALSE);
}


static int32_t SocketChanel_Port(int32_t fd, uint8_t localPort){
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
		return J_EUNKNOWN;
	}

	return map_to_java_exception(llnet_errno(fd));
}

static int32_t SocketChanel_Address(int32_t fd, int8_t* name, int32_t nameLength, uint8_t localAddress){

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
				return J_ENOTCONN;
			}
			if(nameLength < sizeof(in_addr_t)){
				return J_EINVAL;
			}
			*(in_addr_t*)name = sockaddr.in.sin_addr.s_addr;
			return sizeof(in_addr_t);
		}
#endif
#if LLNET_AF & LLNET_AF_IPV6
		if(sockaddr.addr.sa_family == AF_INET6) {
			if(!localAddress && (sockaddr.in6.sin6_addr.s6_addr == 0 || sockaddr.in6.sin6_port == llnet_htons(0))){
				//not connected
				return J_ENOTCONN;
			}
			if(nameLength < sizeof(struct in6_addr)){
				return J_EINVAL;
			}
			memcpy(name, (void *)&(sockaddr.in6.sin6_addr), sizeof(struct in6_addr));
			return sizeof(struct in6_addr);
		}
#endif
		return J_EUNKNOWN;
	}
	return map_to_java_exception(llnet_errno(fd));
}

int32_t LLNET_SOCKETCHANNEL_IMPL_socket(uint8_t stream, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s[thread %d](stream=%d)\n", __func__, SNI_getCurrentJavaThreadID(), stream);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	int32_t ret;
	int32_t sock_protocol = 0; //default value
	int domain;

// When IPv6 is available, always use IPv6 (even for IPv4 connections)
#if LLNET_AF & LLNET_AF_IPV6
	domain = AF_INET6;
#else // only IPv4
	domain = AF_INET;
#endif

	ret = llnet_socket(domain, stream ? SOCK_STREAM : SOCK_DGRAM, sock_protocol);
	if (ret == -1) {
		int32_t err = map_to_java_exception(llnet_errno(-1));
		return err;
	}

	int32_t fd = ret;

// If dual stack IPv4/IPv6
#if LLNET_AF == LLNET_AF_DUAL
    // Disable IPV6_V6ONLY to ensure dual-socket support
	int ipv6_only = 0;
	ret = llnet_setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ipv6_only, sizeof(int));
	if (ret == -1) {
		int32_t err = map_to_java_exception(llnet_errno(fd));
		close(fd);
		return err;
	}
#endif

	return fd;
}

int32_t LLNET_SOCKETCHANNEL_IMPL_serverSocket(uint8_t retry){
	return LLNET_SOCKETCHANNEL_IMPL_socket(JTRUE, retry);
}

int32_t LLNET_SOCKETCHANNEL_IMPL_multicastSocket(uint8_t retry){
	return LLNET_SOCKETCHANNEL_IMPL_socket(JFALSE, retry);
}

#ifdef __cplusplus
	}
#endif
