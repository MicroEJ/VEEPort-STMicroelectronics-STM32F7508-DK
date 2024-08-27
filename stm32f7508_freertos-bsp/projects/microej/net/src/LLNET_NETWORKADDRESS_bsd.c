/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_NETWORKADDRESS 3.0.0 implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 2.0.0
 * @date 17 June 2022
 */

#include "LLNET_NETWORKADDRESS_impl.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "sni.h"
#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"

#ifdef __cplusplus
	extern "C" {
#endif

#define LOCALHOST_NAME ("localhost")

int32_t LLNET_NETWORKADDRESS_IMPL_getLocalHostnameNative(uint8_t* localHostname, int32_t length)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
#ifndef LLNET_USE_DEFAULT_LOCALHOST_NAME
	jint ret;
	ret = llnet_gethostname((char*)localHostname, length);
	if(ret == 0){
		// From gethostname() man:
		// * If the null-terminated hostname is too large to fit, then the name is truncated, and no  error
		// * is  returned  (but  see  NOTES  below).  POSIX.1-2001 says that if such
		// * truncation occurs, then it is unspecified whether the  returned  buffer
		// * includes a terminating null byte.
		//
		// To avoid any issue, we set to 0 the last char.
		localHostname[length-1] = 0;
	}else{
#endif // LLNET_USE_DEFAULT_LOCALHOST_NAME
		strncpy((char *)localHostname, LOCALHOST_NAME, length);

		// From strncpy() man:
		// * Warning: If there is no null byte among the first n bytes
	    // * of src, the string placed in dest will not be null terminated.
		//
		// To avoid any issue, we set to 0 the last char.
		localHostname[length-1] = 0;
#ifndef LLNET_USE_DEFAULT_LOCALHOST_NAME
	}
#endif // LLNET_USE_DEFAULT_LOCALHOST_NAME
	return 0;
}

int32_t LLNET_NETWORKADDRESS_IMPL_lookupInaddrAny(int8_t* anyLocalAddr, int32_t length)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	// If IPv6 or IPv4+IPv6 configuration, then use IPv6. Otherwise (only IPv4 configuration) use IPv4.
#if LLNET_AF & LLNET_AF_IPV6
		if((uint32_t)length >= sizeof(struct in6_addr)){
			*((struct in6_addr*)anyLocalAddr) = in6addr_any;
			return sizeof(struct in6_addr);
		}
#else // only IPv4
		if((uint32_t)length >= sizeof(struct in_addr)){
			*((in_addr_t*)anyLocalAddr) = llnet_htonl(INADDR_ANY);
			return sizeof(in_addr_t);
		}
#endif
	SNI_throwNativeIOException(J_EINVAL, "wrong address length");
	return SNI_IGNORED_RETURNED_VALUE;
}

int32_t LLNET_NETWORKADDRESS_IMPL_loopbackAddress(int8_t* loopback, int32_t length){
	LLNET_DEBUG_TRACE("%s\n", __func__);
	// If IPv6 or IPv4+IPv6 configuration, then use IPv6. Otherwise (only IPv4 configuration) use IPv4.
#if LLNET_AF & LLNET_AF_IPV6
	if((uint32_t)length >= sizeof(struct in6_addr)){
		*((struct in6_addr*)loopback) = in6addr_loopback;
		return sizeof(struct in6_addr);
	}
#else // only IPv4
	if((uint32_t)length >= sizeof(struct in_addr)){
		*((in_addr_t*)loopback) = llnet_htonl(INADDR_LOOPBACK);
		return sizeof(in_addr_t);
	}
#endif
	SNI_throwNativeIOException(J_EINVAL, "wrong address length");
	return SNI_IGNORED_RETURNED_VALUE;
}

#ifdef __cplusplus
	}
#endif
