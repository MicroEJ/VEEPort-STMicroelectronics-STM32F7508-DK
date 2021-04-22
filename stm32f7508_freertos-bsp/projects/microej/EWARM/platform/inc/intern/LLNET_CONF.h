/*
 * C
 *
 * Copyright 2014-2018 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_CONF_INTERN_H
#define LLNET_CONF_INTERN_H

// static IP configuration symbols
void* LLNET_DEVICE_IP_ADDRESS(void);
void* LLNET_NETMASK(void);
void* LLNET_GATEWAY_IP_ADDRESS(void);

// automatic IP configuration symbols
void* LLNET_DHCP_CONF(void);

// automatic DNS IP configuration symbols
void* LLNET_DHCP_DNS_CONF(void);

// DNS configurations symbols
void* LLNET_DNS_IP_ADDRESS(void);

// MAC configuration symbols
void* LLNET_MAC_ADDRESS(void);
void* LLNET_MAC_CONF(void);

#endif // LLNET_CONF_INTERN_H
