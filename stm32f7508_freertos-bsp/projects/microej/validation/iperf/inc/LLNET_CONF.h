/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/*
 * Configuration header to be able to use MicroEJ specific code
 */

#ifndef LLNET_CONF_H
#define LLNET_CONF_H

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

/** Value returned in a configuration option is set. */
#define LLNET_CONF_TRUE		(1)

/** Value returned in a configuration option is not set. */
#define LLNET_CONF_FALSE 	(-1)

/**
 * Returns LLNET_CONF_TRUE if DHCP is enabled for automatic IP.
 * Returns LLNET_CONF_FALSE if a static IP Address has been defined.
 */
#define LLNET_CONF_is_ip_dhcp_enabled() (LLNET_CONF_TRUE)

/**
 * Returns the Device IP Address as a null terminated string (char*).
 */
#define LLNET_CONF_get_device_ip_address() ((char*)"0.0.0.0")

/**
 * Returns the Netmask as a null terminated string (char*).
 */
#define LLNET_CONF_get_netmask() ((char*)"0.0.0.0")

/**
 * Returns the Gateway IP Address as a null terminated string (char*).
 */
#define LLNET_CONF_get_gateway_ip_address() ((char*)"0.0.0.0")

/**
 * Returns LLNET_CONF_TRUE if DHCP is enabled for automatic DNS.
 * Returns LLNET_CONF_FALSE if an IP Address has been defined for the DNS.
 */
#define LLNET_CONF_is_dns_dhcp_enabled() (LLNET_CONF_TRUE)

/**
 * Returns the DNS IP Address as a null terminated string (char*).
 */
#define LLNET_CONF_get_dns_ip_address() ((char*)"0.0.0.0")

#ifdef __cplusplus
	}
#endif

#endif
