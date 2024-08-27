/*
 * C
 *
 * Copyright 2014-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLNET_NETWORKINTERFACE 3.0.0 implementation over LWIP.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 25 November 2021
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lwip/opt.h>
#include <lwip/netif.h>
#include "LLNET_NETWORKINTERFACE_impl.h"
#include "sni.h"
#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"

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

#if LWIP_IPV4
static int32_t LLNET_NETWORKINTERFACE_getMaskLen(struct netif *netif);

/**
 * @brief Gets the subnet mask length of the given network interface <code>netif</code>.
 *
 * @param[in] netif the network interface.
 *
 * @return the subnet mask length of the network interface.
 */
static int32_t LLNET_NETWORKINTERFACE_getMaskLen(struct netif *netif)
{
	int32_t len = 0;
	u32_t mask = ip4_addr_get_u32(netif_ip4_netmask(netif));
	while (mask) {
		mask >>= 1;
		len++;
	}
	return len;
}
#endif

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterface(int32_t index, uint8_t* ifnameBuffer, int32_t ifnameBufferLength)
{
	LLNET_DEBUG_TRACE("%s(id=%d)\n", __func__, index);
	// retrieve network interface list global variable
	struct netif *pnetif_list = netif_list;
	// look at each network interface to find the one matching with the index
	while(pnetif_list != NULL){
		if(pnetif_list->num == index){
			// we need space for two letters and a number and the trailing 'NUL'.
			int32_t digits = 1;
			int32_t tmpNum = pnetif_list->num;
			while(tmpNum >= 10) {
				tmpNum /= 10;
				digits++;
			}
			int32_t requiredLen = 2 + digits + 1;
			if (ifnameBufferLength < requiredLen) {
				SNI_throwNativeIOException(J_EINVAL, "interface name buffer too small");
				return SNI_IGNORED_RETURNED_VALUE;
			}
			sprintf((char*)ifnameBuffer, "%c%c%d", pnetif_list->name[0], pnetif_list->name[1], pnetif_list->num);
			return requiredLen;
		}
		// not found, look at the next interface
		pnetif_list = pnetif_list->next;
	}
	// error: no address found with the given index
	return 0;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress(int32_t idxIf, uint8_t* ifname, int32_t ifnameLength, int32_t idxAddr, int8_t* addrInfo, int32_t addrInfoLength)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	(void)ifnameLength;
	(void)idxIf;
	// retrieve the network interface
	struct netif * pnetif = netif_find((char*)ifname);
	if(pnetif == NULL){
		SNI_throwNativeIOException(J_EINVAL, "network interface not found");
		return SNI_IGNORED_RETURNED_VALUE;
	}
#if LWIP_IPV4
	if(IPV4_ADDR_INFO_SIZE > addrInfoLength){
		SNI_throwNativeIOException(J_EINVAL, "addrInfo buffer too small");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	// interface found, return the configured ip address
	// 'idxAddr' not take into account here because only one ip address
	// is configured per network interface
	addrInfo[0] = IPV4_ADDR_TAG;
	memcpy(&addrInfo[1], netif_ip4_addr(pnetif), sizeof(ip4_addr_t));
	// prefix can be from 0 to 128
	// so we encode it on 1 byte, from 0 to 0x80
	addrInfo[1 + sizeof(ip4_addr_t)] = LLNET_NETWORKINTERFACE_getMaskLen(pnetif);
	// now the broadcast,
	if ((pnetif->flags & NETIF_FLAG_BROADCAST) == 0)
	{
		addrInfo[2 + sizeof(ip4_addr_t)] = 0;
	}
	else
	{
		u32_t broadcast =  ip4_addr_get_u32(netif_ip4_addr(pnetif)) | ~ip4_addr_get_u32(netif_ip4_netmask(pnetif));
		int32_t pos = 2 + sizeof(ip4_addr_t);
		addrInfo[pos++] = 1;
		addrInfo[pos++] = (broadcast) & 0xFF;
		addrInfo[pos++] = (broadcast >> 8) & 0xFF;
		addrInfo[pos++] = (broadcast >> 16) & 0xFF;
		addrInfo[pos++] = (broadcast >> 24) & 0xFF;
	}
	return IPV4_ADDR_INFO_SIZE;
#else
	if(IPV6_ADDR_INFO_SIZE > addrInfoLength){
		SNI_throwNativeIOException(J_EINVAL, "addrInfo buffer too small");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	// only IPv6
	addrInfo[0] = IPV6_ADDR_TAG;
	// retrieve the link-local IPv6 address stored in the slot 0
	memcpy(&addrInfo[1], netif_ip6_addr(pnetif, 0), sizeof(ip6_addr_t));
	// Link-local prefix length is /64 (@see implementation of netif_create_ip6_linklocal_address())
	addrInfo[1 + sizeof(ip6_addr_t)] = 64;
	// no broadcast
	addrInfo[2 + sizeof(ip6_addr_t)] = 0;
	return IPV6_ADDR_INFO_SIZE;
#endif
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddressesCount(int32_t index, uint8_t* ifname, int32_t ifnameLength)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	(void) ifname;
	(void) ifnameLength;
	(void) index;
	return 1; // only one address configuration per network interface
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfacesCount(void)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif_list = netif_list;
	int32_t netif_count = 0;
	while(pnetif_list != NULL){
		++netif_count;
		// get next interface, and verify it exists or not
		pnetif_list = pnetif_list->next;
	}
	return netif_count;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_isLoopback(uint8_t* ifname, int32_t ifnameLength)
{
	LLNET_DEBUG_TRACE("%s for %s\n", __func__, (const char *)ifname);
	(void) ifnameLength;
	#if (defined(LWIP_NETIF_LOOPBACK) && (LWIP_NETIF_LOOPBACK != 0))
		return (strcmp("lo0", (const char*)ifname) == 0) ? 0 : 1; // 0 means true
	#else
		return 1; // 1 means false
	#endif
}

int32_t LLNET_NETWORKINTERFACE_IMPL_isPointToPoint(uint8_t* ifname, int32_t ifnameLength)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	(void) ifname;
	(void) ifnameLength;
	return 1; // 1 means false
}

int32_t LLNET_NETWORKINTERFACE_IMPL_isUp(uint8_t* ifname, int32_t ifnameLength)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	(void) ifnameLength;
	struct netif *pnetif = netif_find((char*)ifname);
	if(pnetif != NULL){
		return netif_is_up(pnetif) ? 0 : 1; // 0 means true
	}
	SNI_throwNativeIOException(J_EINVAL, "network interface not found");
	return SNI_IGNORED_RETURNED_VALUE;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_supportsMulticast(uint8_t* ifname, int32_t ifnameLength)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	(void) ifnameLength;
	struct netif *pnetif = netif_find((char*)ifname);
	if(pnetif != NULL){
		return (pnetif->flags & NETIF_FLAG_BROADCAST) != 0 ? 0 : 1; // 0 means true
	}
	SNI_throwNativeIOException(J_EINVAL, "network interface not found");
	return SNI_IGNORED_RETURNED_VALUE;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getHardwareAddress(uint8_t* ifname, int32_t ifnameLength, int8_t* hwAddr, int32_t hwAddrMaxLength)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	(void) ifnameLength;
	struct netif *pnetif = netif_find((char*)ifname);
	if(pnetif != NULL){
		if(pnetif->hwaddr_len > hwAddrMaxLength){
			SNI_throwNativeIOException(J_EINVAL, "hardware address buffer length too small");
			return SNI_IGNORED_RETURNED_VALUE;
		}
		memcpy(hwAddr, pnetif->hwaddr, pnetif->hwaddr_len);
		return pnetif->hwaddr_len;
	}
	// hardware address not found
	return 0;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getMTU(uint8_t* ifname, int32_t ifnameLength)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	(void) ifnameLength;
	struct netif *pnetif = netif_find((char*)ifname);
	if(pnetif != NULL){
		return pnetif->mtu;
	}
	// hardware address not found
	SNI_throwNativeIOException(J_EINVAL, "hardware address not found");
	return SNI_IGNORED_RETURNED_VALUE;
}

#ifdef __cplusplus
	}
#endif
