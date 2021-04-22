/*
 * C
 *
 * Copyright 2014-2018 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include <LLNET_DNS_impl.h>

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include "LLNET_CONSTANTS.h"
#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"

int32_t LLNET_DNS_IMPL_getHostByAddr(int8_t* inOut, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	// FIXME no function to retrieve host name associated to an ip address
	return J_EHOSTUNKNOWN;
}

int32_t LLNET_DNS_IMPL_getHostByNameAt(int32_t index, int8_t* inOut, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct hostent *hret;
	hret = gethostbyname((char const *)inOut);

	if (hret == NULL){
		return J_EHOSTUNKNOWN;
	}
	if(hret->h_addrtype == AF_INET){
		int32_t addrLength = 4;
		// we wants the index-th address in the address list
		int8_t* addr = (int8_t*)(hret->h_addr_list[index]);
		if(addr != NULL){
			memcpy(inOut+offset, addr, addrLength);
			return addrLength;
		}

	}
	return J_EHOSTUNKNOWN;
}

int32_t LLNET_DNS_IMPL_getHostByNameCount(int8_t* hostname, int32_t offset, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct hostent * hret;
	hret = gethostbyname((char const *)hostname);
	if (hret == NULL){
		return J_EHOSTUNKNOWN;
	}

	int32_t counter = 0;
	int8_t** h_addr_list = (int8_t**)(hret->h_addr_list);
	// h_addr_list is null terminated by construction
	// (see struct hostent description in man 3 gethostbyname)
	// while the next pointer isn't this null pointer, we have at least
	// one address more to read
	while (h_addr_list[counter] != NULL) {
		++counter;
	}
	LLNET_DEBUG_TRACE("%s host count = %d\n", __func__,counter);
	return counter;
}
