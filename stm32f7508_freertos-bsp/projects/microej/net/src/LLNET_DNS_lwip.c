/*
 * C
 *
 * Copyright 2014-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLNET_DNS implementation over LWIP.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 25 November 2021
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sni.h>
#include <lwip/err.h>
#include <lwip/dns.h>

#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"
#include "LLNET_DNS_impl.h"
#include "LLNET_configuration.h"

#if LWIP_IPV6
#include "microej_pool.h"
#include "osal.h"
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#if LLNET_AF == LLNET_AF_DUAL
#if LWIP_IPV4 == 0 || LWIP_IPV6 == 0
#error "The network Adaptation Layer is configured to use both IPv4 and IPv6 addresses but LWIP stack does not have IPv4 and IPv6 features enabled. \
		Please enable the macros LWIP_IPV4 and LWIP_IPV6 in your LWIP configuration file to be able to use both IPv4 and IPv6 addresses."
#endif // LWIP_IPV4 == 0 || LWIP_IPV6 == 0

#else // LLNET_AF == LLNET_AF_DUAL

#if LLNET_AF == LLNET_AF_IPV4
#if LWIP_IPV4 == 0
#error "The network Adaptation Layer is configured to use IPv4 addresses but LWIP stack does not have IPv4 feature enabled. \
		Please enable the macro LWIP_IPV4 in your LWIP configuration file to be able to use IPv4 addresses."
#endif // LWIP_IPV4 == 0

#else // LLNET_AF == LLNET_AF_IPV4
#if LWIP_IPV6 == 0
#error "The network Adaptation Layer is configured to use IPv6 addresses but LWIP stack does not have IPv6 feature enabled. \
		Please enable the macro LWIP_IPV6 in your LWIP configuration file to be able to use IPv6 addresses."
#endif // LWIP_IPV6 == 0

#endif // LLNET_AF == LLNET_AF_DUAL
#endif // LLNET_AF == LLNET_AF_IPV4


#if LWIP_IPV6
/**
 * DNS pool item struct
 */
typedef struct dns_pool_item{
	int32_t java_thread_id; //the java thread id that reserved the item
	ip_addr_t ip; //the resolved DNS IP address
}dns_pool_item_t;

#define MAX_IP_ADDRESS_NUMBER_IN_POOL 5

POOL_declare(gst_pool_ipaddr_ctx, dns_pool_item_t, MAX_IP_ADDRESS_NUMBER_IN_POOL);

/**
 * @brief DNS mutex name.
 *
 */
#define LLNET_DNS_MUTEX_NAME	"LLNET_DNS_Mutex"
/**
 * @brief Mutex used for critical sections.
 */
static OSAL_mutex_handle_t llnet_dns_mutex;

/**
 * @brief Check whether the DNS mutex is created or not
 */
static bool llnet_dns_mutex_created = false;
static void LLNET_DNS_free_pool_item(dns_pool_item_t * dns_pool_item);
static bool LNET_DNS_pool_item_compare_to(void *item, void *characteristic);
static bool LNET_DNS_mutex_init(void);
static inline void LNET_DNS_mutex_lock(void);
static inline void LNET_DNS_mutex_unlock(void);
#endif // LWIP_IPV6

static int32_t LLNET_DNS_IMPL_getHostByNameAtCallback(int32_t index, uint8_t* hostname, int32_t hostnameLength, int8_t* address, int32_t addressLength);
static int32_t LLNET_DNS_copy_address(ip_addr_t* ipaddr, uint8_t* host, int32_t length);
static void LLNET_DNS_gethostbyname_lwip_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);

#if LWIP_IPV6
/**
 * @brief Free the the given DNS pool item
 */
static void LLNET_DNS_free_pool_item(dns_pool_item_t * dns_pool_item){
	LNET_DNS_mutex_lock();
	dns_pool_item->java_thread_id = -1; //reset the java_thread_id
	POOL_free_f(&gst_pool_ipaddr_ctx, dns_pool_item);
	LNET_DNS_mutex_unlock();
}

/**
 * @brief Compare a pool item with the given characteristic.
 * @return true if the java thread id field of the item is equal to the characteristic; false otherwise.
 */
static bool LNET_DNS_pool_item_compare_to(void *item, void *characteristic){
	dns_pool_item_t * dns_pool_item = (dns_pool_item_t *)item;
	int32_t java_thread_id = (int32_t)characteristic;
	if(dns_pool_item->java_thread_id == java_thread_id){
		return true;
	}
	return false;
}

/**
 * @brief Enter critical section.
 */
static bool LNET_DNS_mutex_init(void){
	if(!llnet_dns_mutex_created){
		// init the mutex
		if(OSAL_OK == OSAL_mutex_create((uint8_t*)LLNET_DNS_MUTEX_NAME, &llnet_dns_mutex)){
			llnet_dns_mutex_created = true;
		}
	}
	return llnet_dns_mutex_created;
}
/**
 * @brief Enter critical section.
 */
static inline void LNET_DNS_mutex_lock(void){
	OSAL_mutex_take(&llnet_dns_mutex, OSAL_INFINITE_TIME);
}

/**
 * @brief Exit critical section.
 */
static inline void LNET_DNS_mutex_unlock(void){
	OSAL_mutex_give(&llnet_dns_mutex);
}
#endif // LWIP_IPV6

int32_t LLNET_DNS_IMPL_getHostByAddr(int8_t* address, int32_t addressLength, uint8_t* hostname, int32_t hostnameLength)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);

	/* no function to retrieve host name associated to an ip address */
	SNI_throwNativeIOException(J_EHOSTUNKNOWN, "Hostname resolution from address not supported");
	return SNI_IGNORED_RETURNED_VALUE;
}

int32_t LLNET_DNS_IMPL_getHostByNameAt(int32_t index, uint8_t* hostname, int32_t hostnameLength, int8_t* address, int32_t addressLength)
{
	LLNET_DEBUG_TRACE("%s (index=%d, hostname=%s, hostnameLength=%d)\n", __func__, index, (char *)hostname, hostnameLength);

	if(NULL == hostname){
		SNI_throwNativeIOException(J_EINVAL, "null hostname");
		return SNI_IGNORED_RETURNED_VALUE;
	}

	int32_t java_thread_id = SNI_getCurrentJavaThreadID(); // get the current java thread
#if LWIP_IPV6
	//try to initialize the DNS mutex
	if(!LNET_DNS_mutex_init()){
		SNI_throwNativeIOException(-1, "DNS getHostByNameAt cannot initialize the DNS mutex");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	//reverse a buffer from the pool
	dns_pool_item_t * dns_pool_item = NULL;
	POOL_status_t status = POOL_reserve_f(&gst_pool_ipaddr_ctx, (void **) &dns_pool_item);
	if (POOL_NO_ERROR != status){
		SNI_throwNativeIOException(J_ENOMEM, "cannot reserve buffer in DNS memory pool");
		return SNI_IGNORED_RETURNED_VALUE;
	}

	//register the reserved pool buffer as scoped resource
	if(SNI_OK != SNI_registerScopedResource((void*)dns_pool_item, (SNI_closeFunction) LLNET_DNS_free_pool_item, NULL)){
		//registration fail
		SNI_throwNativeIOException(-1, "DNS getHostByNameAt cannot register scoped resource");
		//free the allocated buffer
		POOL_free_f(&gst_pool_ipaddr_ctx, dns_pool_item);
		return SNI_IGNORED_RETURNED_VALUE;
	}

	dns_pool_item->java_thread_id = java_thread_id;
	ip_addr_t* ipaddr_ptr = &(dns_pool_item->ip);
#else
	//ipv4
	ip_addr_t ipaddr = {0};
	ip_addr_t* ipaddr_ptr = &ipaddr;
#endif
	//try to resolve the host name, registering a callback on success, failure or timeout
	err_t err = dns_gethostbyname((char *)hostname, ipaddr_ptr, LLNET_DNS_gethostbyname_lwip_callback, (void *)java_thread_id);
	if (ERR_OK == err)
	{
		//No need to free the registered scoped resource here.
		//It will be automatically closed and unregistered by the VM
		return LLNET_DNS_copy_address(ipaddr_ptr, (uint8_t*)address, addressLength);
	}
	else if (ERR_INPROGRESS == err)
	{
		// DNS resolve is in progress, suspend the current java thread
		if(SNI_OK != SNI_suspendCurrentJavaThreadWithCallback(0, (SNI_callback)&LLNET_DNS_IMPL_getHostByNameAtCallback, NULL)){
			//Suspend fails
			SNI_throwNativeIOException(-1, "DNS getHostByNameAt cannot suspend current java thread");
			//No need to free the registered scoped resource here.
			//It will be automatically closed and unregistered by the VM
		}
		return SNI_IGNORED_RETURNED_VALUE;
	}

	// An error occurred while resolving the host name
	SNI_throwNativeIOException(J_EHOSTUNKNOWN, "DNS resolution failed");
	//No need to free the registered scoped resource here.
	//It will be automatically closed and unregistered by the VM
	return SNI_IGNORED_RETURNED_VALUE;
}


int32_t LLNET_DNS_IMPL_getHostByNameCount(uint8_t* hostname, int32_t hostnameLength)
{
	LLNET_DEBUG_TRACE("%s (hostname=%s, hostnameLength=%d)\n", __func__, (char *)hostname, hostnameLength);
	// only one address per host name
	return 1;
}


/**
 * @brief asynchronous callback function used for dns_gethostbyname() to get the dns request result.
 * This callback is called only when ERR_INPROGRESS is returned for dns_gethostbyname().
 *
 * @param[in] name the hostname that was looked up.
 * @param[in] ipaddr the IP address of the hostname, or NULL if the name could not be found (or on any other error).
 * @param[in] callback_arg a user-specified callback argument passed to dns_gethostbyname
 */
static void LLNET_DNS_gethostbyname_lwip_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
		int32_t java_thread_id = (int32_t)callback_arg;
#if LWIP_IPV6
		//ipv6 is enabled and maybe ipv4 too (address might be ipv4 or ipv6)
		dns_pool_item_t * resolved_ip = NULL;
		if(NULL != ipaddr){
			LNET_DNS_mutex_lock();
			//retrieve the pool item reserved by the java thread
			POOL_status_t status = POOL_get_f(&gst_pool_ipaddr_ctx, (void **) &resolved_ip, (microej_pool_compare_functor_t)LNET_DNS_pool_item_compare_to, (void*)java_thread_id);
			if (POOL_NO_ERROR == status){
				memcpy((void*)&(resolved_ip->ip), (void*)ipaddr, sizeof(ip_addr_t));
			}
			LNET_DNS_mutex_unlock();
		}

#else //LWIP_IPV6
		//no ipv6 (only ipv4 address)
		uint32_t resolved_ip = 0;
		if(NULL != ipaddr){
			resolved_ip = ((ip4_addr_t*)ipaddr)->addr;
		}
#endif
	//this callback is called only when the java thread was suspended due to pending dns resolution,
	//so resume it now, regardless of the dns resolution status
	SNI_resumeJavaThreadWithArg(java_thread_id, (void*)resolved_ip);
}

static int32_t LLNET_DNS_IMPL_getHostByNameAtCallback(int32_t index, uint8_t* hostname, int32_t hostnameLength, int8_t* address, int32_t addressLength)
{
#if LWIP_IPV6
	dns_pool_item_t * resolved_ip = NULL;
	SNI_getCallbackArgs(NULL, (void **)&resolved_ip);
	if(NULL != resolved_ip){
		//Success
		//Retrieve the host name previously resolved
		//No need to free the registered scoped resource here.
		//It will be automatically closed and unregistered by the VM
		return LLNET_DNS_copy_address(&(resolved_ip->ip), (uint8_t*)address, addressLength);
	}
#else
	ip_addr_t resolved_ip = {0};
	SNI_getCallbackArgs(NULL, (void **)&(resolved_ip.addr));
	if(0 != resolved_ip.addr){
		return LLNET_DNS_copy_address(&resolved_ip, (uint8_t*)address, addressLength);
	}
#endif
	// an error occurred while retrieving the ipaddr and timeout is exceeded
	SNI_throwNativeIOException(J_EHOSTUNKNOWN, "Unknown host");
	return SNI_IGNORED_RETURNED_VALUE;
}

/*
 * @brief copies the given IP address <code>ipaddr</code> to the <code>host</code> buffer.
 *
 * @param[in] ipaddr the IP address to be copied.
 * @param[out] host the buffer into which the host IP address will be copied.
 * @param[in] length the length of the host buffer.
 *
 * @return the IP address size: 4 for IPv4 address and 16 for IPv6 address.
 */
static int32_t LLNET_DNS_copy_address(ip_addr_t* ipaddr, uint8_t* host, int32_t length){
#if LWIP_IPV4 && LWIP_IPV6
	//both ipv4 and ipv6
	if(IP_IS_V4(ipaddr)){
		//ipv4 address
		if(length < 4){
			//length should be at least equal to ipv4 address size (4)
			SNI_throwNativeIOException(J_EINVAL, "invalid host buffer length");
			return SNI_IGNORED_RETURNED_VALUE;
		}
		// retrieve the host name resolution
		memcpy(host, ip_2_ip4(ipaddr), 4);
		LLNET_DEBUG_TRACE("%s (%hhu.%hhu.%hhu.%hhu)\n", __func__,
				host[0], host[1], host[2], host[3]);
		// success
		return 4; //4 for ipv6 address size
	}else{
		//ipv6 address
		if(length < 16){
			//length should be at least equal to ipv6 address size (16)
			SNI_throwNativeIOException(J_EINVAL, "invalid host buffer length");
			return SNI_IGNORED_RETURNED_VALUE;
		}
		//retrieve the host name resolution
		memcpy(host, ip_2_ip6(ipaddr), 16);
		LLNET_DEBUG_TRACE("%s (%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X)\n", __func__,
							host[0], host[1], host[2], host[3], host[4], host[5], host[6], host[7],
							host[8], host[9], host[10], host[11], host[12], host[13], host[14], host[15]);
		// success
		return 16; //16 for ipv6 address size
	}
#else // LWIP_IPV4 && LWIP_IPV6

#if LWIP_IPV4
	//only ipv4 address
	if(length < 4){
		//length should be at least equal to ipv4 address size (4)
		SNI_throwNativeIOException(J_EINVAL, "invalid host buffer length");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	//retrieve the host name resolution
	memcpy(host, ip_2_ip4(ipaddr), 4);
	LLNET_DEBUG_TRACE("%s (%hhu.%hhu.%hhu.%hhu)\n", __func__,
			host[0], host[1], host[2], host[3]);
	// success
	return 4; //4 for ipv6 address size

#else // LWIP_IPV4
	//only ipv6 address
	if(length < 16){
		//length should be at least equal to ipv6 address size (16)
		SNI_throwNativeIOException(J_EINVAL, "invalid host buffer length");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	// retrieve the host name resolution
	memcpy(host, ip_2_ip6(ipaddr), 16);
	LLNET_DEBUG_TRACE("%s (%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X)\n", __func__,
						host[0], host[1], host[2], host[3], host[4], host[5], host[6], host[7],
						host[8], host[9], host[10], host[11], host[12], host[13], host[14], host[15]);
	//success
	return 16; //16 for ipv6 address size

#endif // LWIP_IPV4
#endif // LWIP_IPV4 && LWIP_IPV6
}

#ifdef __cplusplus
	}
#endif
