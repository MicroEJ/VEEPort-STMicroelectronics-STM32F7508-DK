/**
  * Copyright 2020 MicroEJ Corp. This file has been modified by MicroEJ Corp.
  */

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include "cmsis_os.h"

/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);      
void ethernetif_set_link(void const *argument);
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);
u32_t sys_now(void);
#endif
