/*
 * C
 *
 * Copyright 2014-2018 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef LLNET_CFG_H
#define LLNET_CFG_H

#include <intern/LLNET_CONF.h>
#include <stdint.h>

/** Value returned in a configuration option is set. */
#define LLNET_CONF_TRUE		(1)
/** Value returned in a configuration option is not set. */
#define LLNET_CONF_FALSE 	(-1)

/**
 * Returns LLNET_CONF_TRUE if DHCP is enabled for automatic IP in the MicroEJ
 * Run configuration. Returns LLNET_CONF_FALSE if a static IP Address has been
 * defined.
 */
#define LLNET_CONF_is_ip_dhcp_enabled() ((int32_t)&LLNET_DHCP_CONF)

/**
 * Returns the Device IP Address defined in the MicroEJ Run configuration as
 * a null terminated string (char*).
 */
#define LLNET_CONF_get_device_ip_address() ((char*)&LLNET_DEVICE_IP_ADDRESS)

/**
 * Returns the Netmask defined in the MicroEJ Run configuration as
 * a null terminated string (char*).
 */
#define LLNET_CONF_get_netmask() ((char*)&LLNET_NETMASK)

/**
 * Returns the Gateway IP Address defined in the MicroEJ Run configuration as
 * a null terminated string (char*).
 */
#define LLNET_CONF_get_gateway_ip_address() ((char*)&LLNET_GATEWAY_IP_ADDRESS)

/**
 * Returns LLNET_CONF_TRUE if DHCP is enabled for automatic DNS in the MicroEJ
 * Run configuration. Returns LLNET_CONF_FALSE if an IP Address has been
 * defined for the DNS.
 */
#define LLNET_CONF_is_dns_dhcp_enabled() ((int32_t)&LLNET_DHCP_DNS_CONF)

/**
 * Returns the DNS IP Address defined in the MicroEJ Run configuration as
 * a null terminated string (char*).
 */
#define LLNET_CONF_get_dns_ip_address() ((char*)&LLNET_DNS_IP_ADDRESS)

/**
 * Returns LLNET_CONF_TRUE if the MAC address has been specified in the
 * MicroEJ Run configuration. Returns LLNET_CONF_FALSE if no MAC address
 * has been configured.
 */
#define LLNET_CONF_is_mac_address_specified() ((int32_t)&LLNET_MAC_CONF)

/**
 * Returns the MAC Address defined in the MicroEJ Run configuration as
 * a null terminated string (char*).
 */
#define LLNET_CONF_get_mac_address() ((char*)&LLNET_MAC_ADDRESS)


#endif // LLNET_CFG_H
