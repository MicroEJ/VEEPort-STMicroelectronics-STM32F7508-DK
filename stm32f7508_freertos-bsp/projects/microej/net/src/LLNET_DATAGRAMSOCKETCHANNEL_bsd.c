/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_DATAGRAMSOCKETCHANNEL 3.0.0 implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 2.0.0
 * @date 17 June 2022
 */


#include "LLNET_DATAGRAMSOCKETCHANNEL_impl.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "sni.h"
#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"

#ifdef __cplusplus
	extern "C" {
#endif


int64_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_receive(int32_t fd, int8_t* dst, int32_t dstOffset, int32_t dstLength, int8_t* hostPort, int32_t hostPortLength, int64_t absoluteTimeout)
{
	LLNET_DEBUG_TRACE("%s(fd=0x%X, absoluteTimeout=%lld, dstLength=%d, hostPortLength=%d)\n", __func__, fd, absoluteTimeout, dstLength, hostPortLength);
	if (llnet_is_ready() == false) {
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	union llnet_sockaddr sockaddr = {0};
	int32_t addrLen = sizeof(sockaddr);
	int32_t flags = MSG_WAITALL;
	int32_t ret;

	ret = llnet_recvfrom(fd, dst+dstOffset, dstLength, flags, &sockaddr.addr, (socklen_t *)&addrLen);

	LLNET_DEBUG_TRACE("%s recvfrom() returned %d errno = %d\n",__func__, ret, llnet_errno(fd));

	if(0 == ret){
		//EOF
		return -1;
	}

	if(0 > ret){
		//receive error
		LLNET_handle_blocking_operation_error(fd, llnet_errno(fd), SELECT_READ, absoluteTimeout, (SNI_callback)LLNET_DATAGRAMSOCKETCHANNEL_IMPL_receive, NULL);
		return SNI_IGNORED_RETURNED_VALUE;
	}

	//receive success
#if LLNET_AF & LLNET_AF_IPV4
	if (sockaddr.addr.sa_family == AF_INET) {
		if((uint32_t)hostPortLength < (sizeof(in_addr_t) + sizeof(int32_t))){
			SNI_throwNativeIOException(J_EINVAL, "wrong host port length");
			return SNI_IGNORED_RETURNED_VALUE;
		}
		// push host address in result buffer
		*((in_addr_t*)hostPort) = sockaddr.in.sin_addr.s_addr;
		hostPort += sizeof(in_addr_t);
		// push host port in result buffer
		*((int32_t*)hostPort) = llnet_ntohs(sockaddr.in.sin_port);
		// add data receive length in return value
		int64_t retValue = (((int64_t)ret) << 32l);
		// add host length in return value
		retValue |= sizeof(in_addr_t);
		LLNET_DEBUG_TRACE("%s returning %llx\n", __func__, retValue);
		return retValue;
	}
#endif
#if LLNET_AF & LLNET_AF_IPV6
	if (sockaddr.addr.sa_family == AF_INET6) {
		if((uint32_t)hostPortLength < (sizeof(struct in6_addr) + sizeof(int32_t))){
			LLNET_DEBUG_TRACE("%s returning J_EINVAL hostPortLength = %d\n", __func__,hostPortLength);
			SNI_throwNativeIOException(J_EINVAL, "wrong host port length");
			return SNI_IGNORED_RETURNED_VALUE;
		}
		// push host address in result buffer
		memcpy(hostPort, (void*)&sockaddr.in6.sin6_addr, sizeof(struct in6_addr));
		// push host port in result buffer
		hostPort +=  sizeof(struct in6_addr);
		*((int32_t*)hostPort) = llnet_ntohs(sockaddr.in6.sin6_port);
		LLNET_DEBUG_TRACE("%s sockaddr.in6.sin6_port = 0x%x\n", __func__, sockaddr.in6.sin6_port);
		LLNET_DEBUG_TRACE("%s llnet_ntohs(sockaddr.in6.sin6_port) = 0x%x\n", __func__, llnet_ntohs(sockaddr.in6.sin6_port));
		// add data receive length in return value
		int64_t retValue = (((int64_t)ret) << 32l);
		// add host length in return value
		retValue |= (sizeof(struct in6_addr));
		LLNET_DEBUG_TRACE("%s returning %llx\n", __func__, retValue);
		return retValue;
	}
#endif

	//unsupported address family
	SNI_throwNativeIOException(J_EAFNOSUPPORT, "unsupported address family");
	return SNI_IGNORED_RETURNED_VALUE;
}


void LLNET_DATAGRAMSOCKETCHANNEL_IMPL_send(int32_t fd, int8_t* src, int32_t srcoffset, int32_t srclength, int8_t* addr, int32_t addrlength, int32_t port)
{
	LLNET_DEBUG_TRACE("%s(fd=0x%X, srclength=%d, port=%d)\n", __func__, fd, srclength, port);
	union llnet_sockaddr sockaddr = {0};
	int sockaddr_sizeof = 0;
	int32_t ret;
	int32_t fd_errno;

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return;
    }

#if LLNET_AF == LLNET_AF_IPV4
	if(addrlength == sizeof(in_addr_t))
	{
		sockaddr.in.sin_family = AF_INET;
		sockaddr.in.sin_addr.s_addr = *((in_addr_t*)addr);
		sockaddr.in.sin_port = llnet_htons(port);
		sockaddr_sizeof = sizeof(struct sockaddr_in);
	}
#endif

#if LLNET_AF == LLNET_AF_DUAL
	if(addrlength == sizeof(in_addr_t)){
		// Convert IPv4 into IPv6 and put the result directly in the in6_addr struct
		LLNET_map_ipv4_into_ipv6((in_addr_t*)addr, (struct in6_addr*)&sockaddr.in6.sin6_addr);
		// Update length and addr
		addrlength = sizeof(struct in6_addr);
		addr = (int8_t*)&sockaddr.in6.sin6_addr;
		// Continue in the following if
	}
#endif

#if LLNET_AF & LLNET_AF_IPV6
	if(addrlength == sizeof(struct in6_addr))
	{
		sockaddr.in6.sin6_family = AF_INET6;
		// Skip copy if in6_addr struct already contains the IPv6 address
		if((void*)addr != (void*)&sockaddr.in6.sin6_addr){
			memcpy((void*)&sockaddr.in6.sin6_addr, addr, sizeof(struct in6_addr));
		}
		sockaddr.in6.sin6_port = llnet_htons(port);
		sockaddr_sizeof = sizeof(struct sockaddr_in6);
	}
#endif
	if(sockaddr_sizeof == 0){
		LLNET_DEBUG_TRACE("%s(fd=0x%X) invalid address type addrlength=%d\n", __func__, fd, addrlength);
		SNI_throwNativeIOException(J_EINVAL, "invalid address length");
		return;
	}
	LLNET_DEBUG_TRACE("%s(fd=0x%X) calling sendto AddrSize: %d\n", __func__, fd, sockaddr_sizeof);

	ret = llnet_sendto(fd, src+srcoffset, srclength, 0, &sockaddr.addr, sockaddr_sizeof);
	fd_errno = llnet_errno(fd);
	LLNET_DEBUG_TRACE("%s(fd=0x%X) sendto result=%d errno=%d\n", __func__, fd, ret, llnet_errno(fd));

	if(ret < 0 && fd_errno == EISCONN){
		//The datagram socket is connected.
		//According to BSD sendto specification, EISCONN can be set when trying to send a packet
		//on a connected stream or datagram socket if destination address is not null.
		//Retry to send the packet without specifying the destination address (set it to null).
		ret = llnet_sendto(fd, src+srcoffset, srclength, 0, (struct sockaddr*)NULL, 0);
	}

	if(ret == 0){
		SNI_throwNativeIOException(J_EUNKNOWN, "0 byte written");
		return;
	}

	if(0 > ret){
		LLNET_handle_blocking_operation_error(fd, llnet_errno(fd), SELECT_READ, 0, (SNI_callback)LLNET_DATAGRAMSOCKETCHANNEL_IMPL_send, NULL);
	}
}

void LLNET_DATAGRAMSOCKETCHANNEL_IMPL_disconnect(int32_t fd)
{
	LLNET_DEBUG_TRACE("%s(fd=0x%X)\n ", __func__, fd);

    if(llnet_is_ready() == false){
		SNI_throwNativeIOException(J_NETWORK_NOT_INITIALIZED, "network not initialized");
		return;
    }

	struct sockaddr sockaddr = {0};
	sockaddr.sa_family = AF_UNSPEC;
	if(llnet_connect(fd, &sockaddr, sizeof(struct sockaddr)) < 0) {
		int32_t fd_errno = llnet_errno(fd);
		SNI_throwNativeIOException(LLNET_map_to_java_exception(fd_errno), LLNET_get_socket_error_msg(fd_errno));
	}
}
#ifdef __cplusplus
	}
#endif
