/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */


#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include <stdbool.h>

#ifdef __cplusplus
	extern "C" {
#endif

bool LWIP_NETWORK_apply_configuration(struct netif* interface, bool is_static, ip_addr_t ip, ip_addr_t netmask, ip_addr_t gw);

#ifdef __cplusplus
	}
#endif
