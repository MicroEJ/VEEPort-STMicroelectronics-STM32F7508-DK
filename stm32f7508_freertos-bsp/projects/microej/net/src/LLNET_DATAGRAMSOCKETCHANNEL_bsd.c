/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLNET_DATAGRAMSOCKETCHANNEL 2.1.0 implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 1.1.1
 * @date 7 February 2020
 */

#include <LLNET_DATAGRAMSOCKETCHANNEL_impl.h>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "LLNET_CONSTANTS.h"
#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"

#ifdef __cplusplus
	extern "C" {
#endif

int64_t DatagramSocketChannel_recvfrom(int32_t fd, int8_t* dst, int32_t dstOffset, int32_t dstLength, int8_t* hostPort, int32_t hostPortLength, uint8_t retry){
	LLNET_DEBUG_TRACE("%s(fd=0x%X, dstLength=%d, hostPortLength=%d, retry=%d)\n", __func__, fd, dstLength, hostPortLength, retry);

	union llnet_sockaddr sockaddr = {0};
	int32_t addrLen = sizeof(sockaddr);
	int32_t flags = MSG_WAITALL;
	int32_t ret = llnet_recvfrom(fd, dst+dstOffset, dstLength, flags, &sockaddr.addr, (socklen_t *)&addrLen);

	LLNET_DEBUG_TRACE("%s recvfrom() returned %d errno = %d\n",__func__,ret,errno);
	if (ret == -1) {
		LLNET_DEBUG_TRACE("%s returning %d\n", __func__, map_to_java_exception(llnet_errno(fd)));
		return map_to_java_exception(llnet_errno(fd));
	}
#if LLNET_AF & LLNET_AF_IPV4
	if (sockaddr.addr.sa_family == AF_INET) {
		if(hostPortLength < (sizeof(in_addr_t)+sizeof(int32_t))){
			return J_EINVAL;
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
		if(hostPortLength < (sizeof(struct in6_addr)+sizeof(int32_t))){
			LLNET_DEBUG_TRACE("%s returning J_EINVAL hostPortLength = %d\n", __func__,hostPortLength);
			return J_EINVAL;
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
	return J_EAFNOSUPPORT;
}

int32_t DatagramSocketChannel_sendto(int32_t fd, int8_t* src, int32_t srcoffset, int32_t srclength, int8_t* addr, int32_t addrlength, int32_t port, uint8_t retry){
	LLNET_DEBUG_TRACE("%s(fd=0x%X, ..., retry=%d)\n", __func__, fd, retry);
	union llnet_sockaddr sockaddr = {0};
	int sockaddr_sizeof = 0;
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
	if(addrlength == sizeof(in_addr_t) && SNI_getArrayLength(addr) >= sizeof(struct in6_addr)){
		// Convert IPv4 into IPv6 directly in the addr array
		map_ipv4_into_ipv6((in_addr_t*)addr, (struct in6_addr*)addr);

		// Now length of the address in addr is sizeof(struct in6_addr)
		addrlength = sizeof(struct in6_addr);
		// continue in the following if
	}
#endif

#if LLNET_AF & LLNET_AF_IPV6
	if(addrlength == sizeof(struct in6_addr))
	{
		sockaddr.in6.sin6_family = AF_INET6;
		memcpy((void*)&sockaddr.in6.sin6_addr, addr, sizeof(struct in6_addr));
		sockaddr.in6.sin6_port = llnet_htons(port);
		sockaddr_sizeof = sizeof(struct sockaddr_in6);
	}
#endif
	if(sockaddr_sizeof == 0){
		LLNET_DEBUG_TRACE("%s(fd=0x%X) invalid address type addrlength=%d\n", __func__, fd, addrlength);
		return J_EINVAL;
	}
	int32_t flags = 0;
	LLNET_DEBUG_TRACE("%s(fd=0x%X) calling sendto AddrSize: %d\n", __func__, fd, sockaddr_sizeof);

	int32_t ret = llnet_sendto(fd, src+srcoffset, srclength, flags,&sockaddr.addr, sockaddr_sizeof);

	LLNET_DEBUG_TRACE("%s(fd=0x%X) sendto result=%d errno=%d\n", __func__, fd, ret, llnet_errno(fd));

	if(ret == -1 && llnet_errno(fd) == EISCONN){
		//The datagram socket is connected.
		//According to BSD sendto specification, EISCONN can be set when trying to send a packet
		//on a connected stream or datagram socket if destination address is not null.
		//Retry to send the packet without specifying the destination address (set it to null).
		ret = llnet_sendto(fd, src+srcoffset, srclength, flags,(struct sockaddr*)NULL, 0);
	}
	if(ret == -1 && (llnet_errno(fd) == EAGAIN || llnet_errno(fd) == EWOULDBLOCK)){
		ret = 0;
	}
	if(ret == -1){
		return map_to_java_exception(llnet_errno(fd));
	}

	return ret;
}

int64_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_receive(int32_t fd, int8_t* dst, int32_t dstOffset, int32_t dstLength, int8_t* hostPort, int32_t hostPortLength, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s(fd=0x%X, ..., retry=%d hostPortLength = %d)\n", __func__, fd, retry, hostPortLength);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	// first, a select with zero timeout is done to check if the socket is readable or writable.
	// The select call is non blocking when the given timeout is equals to zero.
	// On success, the socket is ready to read or write data. A blocking call to recv function
	// can be done. It will not block the socket.
	int32_t selectRes = non_blocking_select(fd, SELECT_READ);

	if(selectRes == 0){
		return asyncOperation(fd, SELECT_READ, retry);
	}else{
		return DatagramSocketChannel_recvfrom(fd, dst, dstOffset, dstLength, hostPort, hostPortLength, retry);
	}

}

int32_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_send(int32_t fd, int8_t* src, int32_t srcoffset, int32_t srclength, int8_t* addr, int32_t addrlength, int32_t port, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s(fd=0x%X, ..., retry=%d addrlength = %d)\n", __func__, fd, retry, addrlength);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	// first, a select with zero timeout is done to check if the socket is readable or writable.
	// The select call is non blocking when the given timeout is equals to zero.
	// On success, the socket is ready to read or write data. A blocking call to recv function
	// can be done. It will not block the socket.
	int32_t selectRes = non_blocking_select(fd, SELECT_WRITE);

	if(selectRes == 0){
		return asyncOperation(fd, SELECT_WRITE, retry);
	}else{
		return DatagramSocketChannel_sendto(fd, src, srcoffset, srclength, addr, addrlength, port, retry);
	}

}

int32_t LLNET_DATAGRAMSOCKETCHANNEL_IMPL_disconnect(int32_t fd, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s(fd=0x%X)\n ", __func__, fd);

    if(llnet_is_ready() == false){
        return J_NETWORK_NOT_INITIALIZED;
    }

	struct sockaddr sockaddr = {0};
	sockaddr.sa_family = AF_UNSPEC;
	if(llnet_connect(fd, &sockaddr, sizeof(struct sockaddr)) == -1) {
		return map_to_java_exception(llnet_errno(fd));
	}
	return 0; //success
}

#ifdef __cplusplus
	}
#endif
