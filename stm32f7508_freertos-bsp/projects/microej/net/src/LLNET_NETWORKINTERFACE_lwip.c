/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLNET_NETWORKINTERFACE 2.1.0 implementation over LWIP.
 * @author MicroEJ Developer Team
 * @version 0.2.1
 * @date 3 August 2020
 */

#include <LLNET_NETWORKINTERFACE_impl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lwip/opt.h>
#include <lwip/netif.h>
#include <lwip/sockets.h>
#include "LLNET_CONSTANTS.h"
#include "LLNET_ERRORS.h"
#include "LLNET_Common.h"

#ifdef __cplusplus
	extern "C" {
#endif

static struct netif * getNetworkInterface(int8_t* name, int32_t length);

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterface(int32_t id, int8_t* nameReturned, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s(id=%d)\n", __func__, id);
	// retrieve network interface list global variable
	struct netif *pnetif_list = netif_list;
	// look at each network interface to find the one matching with 'idIf'
	while(pnetif_list != NULL){
		if(pnetif_list->num == id){
			// we need space for two letters and a number and the trailing 'NUL'.
			int32_t digits = 1;
			int32_t tmpNum = pnetif_list->num;
			while(tmpNum >= 10) {
				tmpNum /= 10;
				digits++;
			}
			int32_t requiredLen = 2 + digits + 1;
			if (length < requiredLen) {
				return J_EINVAL;
			}
			sprintf((char*)nameReturned, "%c%c%d", pnetif_list->name[0], pnetif_list->name[1], pnetif_list->num);
			return requiredLen;
		}
		// not found, look at the next interface
		pnetif_list = pnetif_list->next;
	}
	// error: no address found at the 'idAddr' index
	return 0;
}

// v6 is not supported yet, but here are the v4 and v6 constants 
// for the getVMInterfaceAddress protocol between Java and the native stacks

// ipv4 address info size (tag (1) + IP (4) + prefix (1) + hasBroadcast (1) + broadcast IP (4))
#define IPV4_ADDR_INFO_SIZE 11
// ipv6 address info size (tag (1) + IP (16) + prefix (1)
#define IPV6_ADDR_INFO_SIZE 18

// ipv4 address tag
#define IPV4_ADDR_TAG 4
// ipv6 address tag 
#define IPV6_ADDR_TAG 6

static int32_t _getMaskLen(struct netif *netif) 
{
	int32_t len = 0;
	u32_t mask = ip4_addr_get_u32(netif_ip4_netmask(netif));
	while (mask) {
		mask >>= 1;
		len++;
	}
	return len;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddress(int32_t idIf, int32_t idAddr, int8_t* addrInfo, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	// retrieve network interface list global variable
	struct netif *pnetif_list = netif_list;
	// look at each network interface to find the one matching with 'idIf'
	while(pnetif_list != NULL){
		if(pnetif_list->num == idIf){
			// interface found, return the configured ip address
			// 'idAddr' not take into account here because only one ip address
			// is configured per network interface
			addrInfo[0] = IPV4_ADDR_TAG;
			memcpy(&addrInfo[1], &pnetif_list->ip_addr, sizeof(ip_addr_t));
			// prefix can be from 0 to 128
			// so we encode it on 1 byte, from 0 to 0x80
			addrInfo[1 + sizeof(ip_addr_t)] = _getMaskLen(pnetif_list);
			// now the broadcast,
			if ((pnetif_list->flags & NETIF_FLAG_BROADCAST) == 0)
			{
				addrInfo[2 + sizeof(ip_addr_t)] = 0;
			}
			else
			{
				u32_t broadcast =  ip4_addr_get_u32(&pnetif_list->ip_addr) | ~ip4_addr_get_u32(netif_ip4_netmask(pnetif_list));
				int32_t pos = 2 + sizeof(ip_addr_t);
				addrInfo[pos++] = 1;
				addrInfo[pos++] = (broadcast) & 0xFF;				
				addrInfo[pos++] = (broadcast >> 8) & 0xFF;
				addrInfo[pos++] = (broadcast >> 16) & 0xFF;
				addrInfo[pos++] = (broadcast >> 24) & 0xFF;
			}
			return IPV4_ADDR_INFO_SIZE;
		}
		// not found, look at the next interface
		pnetif_list = pnetif_list->next;
	}
	// error: no address found at the 'idAddr' index
	return 0;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfaceAddressesCount(int32_t id, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	return 1; // only one address configuration per network interface
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getVMInterfacesCount(uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif_list = netif_list;
	int32_t ifCnt = 0;
	while(pnetif_list != NULL){
		++ifCnt;
		// get next interface, and verify it exists or not
		pnetif_list = pnetif_list->next;
	}
	return ifCnt;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_isLoopback(int8_t* name, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s for %s\n", __func__, (const char *)name);
	#if (defined(LWIP_NETIF_LOOPBACK) && (LWIP_NETIF_LOOPBACK != 0))
		return (strcmp("lo0", (const char*)name) == 0) ? 0 : 1; // 0 means true
	#else
		return 1; // 1 means false
	#endif
}

int32_t LLNET_NETWORKINTERFACE_IMPL_isPointToPoint(int8_t* name, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	return 1; // 0 means true
}

int32_t LLNET_NETWORKINTERFACE_IMPL_isUp(int8_t* name, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif = getNetworkInterface(name, length);
	if(pnetif != NULL){
		return netif_is_up(pnetif) ? 0 : 1; // 0 means true
	}
	return J_EUNKNOWN;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_supportsMulticast(int8_t* name, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif = getNetworkInterface(name, length);
	if(pnetif != NULL){
		return (pnetif->flags & NETIF_FLAG_BROADCAST) != 0 ? 0 : 1; // 0 means true
	}
	return J_EUNKNOWN;
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getHardwareAddress(int8_t* name, int32_t length, int8_t* hwAddr, int32_t hwAddrMaxLength, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif = getNetworkInterface(name, length);
	if(pnetif != NULL){
		memcpy(hwAddr, pnetif->hwaddr, pnetif->hwaddr_len);
		return pnetif->hwaddr_len;
	}
	// hardware address not found
	return 0;
}

static struct netif * getNetworkInterface(int8_t* name, int32_t length){
	if (name[length - 1] != '\0') {
		return NULL;
	}
	return netif_find((char*)name);
}

int32_t LLNET_NETWORKINTERFACE_IMPL_getMTU(int8_t* name, int32_t length, uint8_t retry)
{
	LLNET_DEBUG_TRACE("%s\n", __func__);
	struct netif *pnetif = getNetworkInterface(name, length);
	if(pnetif != NULL){
		if (pnetif->mtu == 0) {
			// -1 means unknown MTU value 
			return -1;
		}
		return pnetif->mtu;
	}
	// hardware address not found
	return J_EUNKNOWN;
}

#ifdef __cplusplus
	}
#endif
