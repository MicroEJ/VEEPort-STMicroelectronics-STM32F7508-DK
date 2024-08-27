/*
 * C
 *
 * Copyright 2017-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef __LWIP_UTIL_H
#define __LWIP_UTIL_H

/**
 * @file
 * @brief LLNET utility functions for LWIP.
 * @author MicroEJ Developer Team
 * @version 0.2.1
 * @date 3 August 2020
 */

#include <stdint.h>

uint32_t lwip_getRandomNumber(void);
int32_t llnet_lwip_init(void);
/**
 * Sets the lwIP network interface name in the stored list.
 *
 * @param id the id of the network interface to be stored.
 * @param netif_name the lwIP network interface name to be stored in the list.
 */
void set_lwip_netif_name(int32_t id, char *netif_name);

/**
 * Gets the lwIP network interface name from the stored list.
 *
 * @param id the id of the network interface to be stored.
 *
 * @return the lwIP network interface name from the stored list.
 */
char *get_lwip_netif_name(int32_t id);


/**
 * Gets the lwIP network interface address.
 *
 * @param name the buffer that store the interface name.
 *
 * @return the lwIP network interface address or null if no interface with the specified name.
 */
struct netif* getNetworkInterface(int8_t* name);
#endif // __LWIP_UTIL_H
