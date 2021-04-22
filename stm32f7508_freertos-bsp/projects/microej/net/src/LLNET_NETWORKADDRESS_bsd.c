/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLNET_NETWORKADDRESS 2.1.0 implementation over BSD-like API.
 * @author MicroEJ Developer Team
 * @version 1.1.1
 * @date 7 February 2020
 */

#include <LLNET_NETWORKADDRESS_impl.h>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "LLNET_CONSTANTS.h"
#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"

#ifdef __cplusplus
	extern "C" {
#endif

#define LOCALHOST_NAME ("localhost")

int32_t LLNET_NETWORKADDRESS_IMPL_getLocalHostnameNative(int8_t* result, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);

#if defined( __GNUC__ )
	/* strlpy inserts the NULL character. see man page. */
	strlcpy((char *)result, LOCALHOST_NAME, length);
#else
	/* strlpy is not in any ISO standard, so use strncpy to copy length bytes. */
	strncpy((char *)result, LOCALHOST_NAME, length);

	// From strncpy() man:
	// * Warning: If there is no null byte among the first n bytes
	// * of src, the string placed in dest will not be null terminated.
	//
	// To avoid any issue, we set to 0 the last char.
	result[length - 1] = 0;
#endif

	return 0;
}

int32_t LLNET_NETWORKADDRESS_IMPL_lookupInaddrAny(int8_t* result, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);

// If IPv6 or IPv4+IPv6 configuration, then use IPv6. Otherwise (only IPv4 configuration) use IPv4.
#if LLNET_AF & LLNET_AF_IPV6
	if(length >= sizeof(struct in6_addr)){
		*((struct in6_addr*)result) = in6addr_any;
		return sizeof(struct in6_addr);
	}
#else // only IPv4
	if(length >= sizeof(struct in_addr)){
		*((in_addr_t*)result) = llnet_htonl(INADDR_ANY);
		return sizeof(in_addr_t);
	}
#endif

	return J_EINVAL;
}

int32_t LLNET_NETWORKADDRESS_IMPL_loopbackAddress(int8_t* result, int32_t length, uint8_t retry){
	LLNET_DEBUG_TRACE("%s\n", __func__);

// If IPv6 or IPv4+IPv6 configuration, then use IPv6. Otherwise (only IPv4 configuration) use IPv4.
#if LLNET_AF & LLNET_AF_IPV6
	if(length >= sizeof(struct in6_addr)){
		*((struct in6_addr*)result) = in6addr_loopback;
		return sizeof(struct in6_addr);
	}
#else // only IPv4
	if(length >= sizeof(struct in_addr)){
		*((in_addr_t*)result) = llnet_htonl(INADDR_LOOPBACK);
		return sizeof(in_addr_t);
	}
#endif

	return J_EINVAL;
}

#ifdef __cplusplus
	}
#endif
