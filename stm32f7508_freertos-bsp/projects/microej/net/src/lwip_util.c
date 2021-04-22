/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief lwip_util implementation over LWIP and FreeRTOS.
 * @author MicroEJ Developer Team
 * @version 0.2.1
 * @date 3 August 2020
 */

#include "lwip_util.h"

#include "LLNET_CONF.h"
#include "LLNET_Common.h"

#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include <netdb.h>
#include <stddef.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include "LLNET_NETWORK_MEM.h"

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rng.h"

// Sanity check. Synchronization has to be activated in LWIP configuration file.
#if (SYS_LIGHTWEIGHT_PROT != 1)
	#error LWIP synchronization not activated in lwipopts.h file. Please add "#define SYS_LIGHTWEIGHT_PROT	1" in lwipopts.h header file.
#endif

#define MAX_DHCP_TRIES  4
#define LWIP_LINK_TASK_PRIORITY 9
#define LWIP_DHCP_TASK_PRIORITY 8

#define LWIP_DHCP_POLLING_INTERVAL 250

/* DHCP process states */
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5

struct netif gnetif;
uint8_t DHCP_state;

static TaskHandle_t	link_task_handle;
static TaskHandle_t	dhcp_task_handle;

static uint8_t dhcp_sleeping = 1;

/* variable used to notify that DNS servers list has changed */
uint8_t dns_servers_list_updated = 1;

// RNG handle
static RNG_HandleTypeDef RngHandle;

static void ethernetif_static_ip_config(void);

/**
  * @brief  Reset the network interface ip, netmask and gateway addresses to zero.
  * @param  netif: the network interface
  * @retval None
  */
static void netif_addr_set_zero_ip4(struct netif* netif){
	ip_addr_set_zero_ip4(&netif->ip_addr);
	ip_addr_set_zero_ip4(&netif->netmask);
	ip_addr_set_zero_ip4(&netif->gw);
}

/**
  * @brief  This function is called when the network interface is disconnected.
  * @param  netif: the network interface
  * @retval None
  */
void netif_not_connected(struct netif *netif){
	netif_addr_set_zero_ip4(netif);
	LLNET_DEBUG_TRACE("[INFO] The network cable is not connected \n");
}

/**
  * @brief  Notify the User about the nework interface config status
  * @param  netif: the network interface
  * @retval None
  */
static void User_notification(struct netif *netif)
{
	int32_t dhcpConfEnabled = LLNET_CONF_is_ip_dhcp_enabled() == LLNET_CONF_TRUE;
	if (netif_is_up(netif))
	{
		if(dhcpConfEnabled)
		{
			/* Update DHCP state machine */
			DHCP_state = DHCP_START;
		}else{
			// launch static Network Interface configuration
			ethernetif_static_ip_config();
		}
	}
	else
	{
		netif_not_connected(netif);
		if(dhcpConfEnabled)
		{
			/* Update DHCP state machine */
			DHCP_state = DHCP_LINK_DOWN;
		}
	}

}

/**
  * @brief  DHCP Process
* @param  argument: network interface
  * @retval None
  */
static void DHCP_thread(void const * argument)
{
  struct netif *netif = (struct netif *) argument;
  uint32_t IPaddress;

  for (;;)
  {
  	// check if DHCP thread has to suspend
	if(dhcp_sleeping == 1){
		vTaskSuspend(NULL);
	}

    switch (DHCP_state)
    {
    case DHCP_START:
      {
        netif_addr_set_zero_ip4(netif);
        IPaddress = 0;
        dhcp_start(netif);
        DHCP_state = DHCP_WAIT_ADDRESS;

        LLNET_DEBUG_TRACE("[INFO] DHCP started\n");
      }
      break;

    case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = netif->ip_addr.addr;

        if (IPaddress!=0)
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;

          /* Stop DHCP */
#if LWIP_VERSION_MAJOR == 1
          dhcp_stop(netif);
#elif LWIP_VERSION_MAJOR == 2
          // LwIP version 2 onward clears existing IP address if dhcp is stopped.
          //dhcp_stop(netif);
#else
          #error "Invalid LWIP version (LWIP_VERSION_MAJOR)."
#endif
		  dhcp_sleeping = 1;

		  LLNET_DEBUG_TRACE("[INFO] DHCP address assigned: %s\n", inet_ntoa(IPaddress));

					// set static DNS configuration if required by user
					if(LLNET_CONF_is_dns_dhcp_enabled() != LLNET_CONF_TRUE)
					{
						ip_addr_t dnsaddr;
						if(DNS_MAX_SERVERS > 0)
						{
							char * static_dns_ip_addr = LLNET_CONF_get_dns_ip_address();
							if(static_dns_ip_addr != NULL)
							{
								dnsaddr.addr = inet_addr(static_dns_ip_addr);
								dns_setserver(0, &dnsaddr);
							}
						}
					}

					// notify DNS servers IP address updated
					dns_servers_list_updated = 1;
        }
        else
        {
#if LWIP_VERSION_MAJOR == 1

        	struct dhcp *dhcp = netif->dhcp;

#elif LWIP_VERSION_MAJOR == 2

			struct dhcp *dhcp = (struct dhcp*)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

#else

	#error "Invalid LWIP version (LWIP_VERSION_MAJOR)."

#endif

			/* DHCP timeout */
			if (dhcp->tries > MAX_DHCP_TRIES)
			{
				DHCP_state = DHCP_TIMEOUT;

				/* Stop DHCP */
				dhcp_stop(netif);
				dhcp_sleeping = 1;

				LLNET_DEBUG_TRACE("[INFO] DHCP timeout\n");
			}
        }
      }
      break;

    default: break;
    }

    /* wait 250 ms */
	TickType_t ticks = LWIP_DHCP_POLLING_INTERVAL / portTICK_PERIOD_MS;
    vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */
  }
}

/**
 * @brief  Setup the network interface
 * @param  None
 * @retval None
 */
static void Netif_Config(void)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	ip_addr_set_zero_ip4(&ipaddr);
	ip_addr_set_zero_ip4(&netmask);
	ip_addr_set_zero_ip4(&gw);


	/* Add the network interface */
#if NO_SYS
	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &netif_input);
#else
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
#endif

	/* Registers the default network interface */
	netif_set_default(&gnetif);

	if (netif_is_link_up(&gnetif))
	{
		/* When the netif is fully configured this function must be called */
		netif_set_up(&gnetif);
	}
	else
	{
		/* When the netif link is down this function must be called */
		netif_set_down(&gnetif);
	}

	/* Set the link callback function, this function is called on change of link status */
	netif_set_link_callback(&gnetif, ethernetif_update_config);

	/* Create the Ethernet link handler thread */
#if defined(__GNUC__)
	xTaskCreate((TaskFunction_t)ethernetif_set_link, "LWIP Link", configMINIMAL_STACK_SIZE * 5, &gnetif, LWIP_LINK_TASK_PRIORITY, &link_task_handle);
#else
	xTaskCreate((TaskFunction_t)ethernetif_set_link, "LWIP Link", configMINIMAL_STACK_SIZE * 2, &gnetif, LWIP_LINK_TASK_PRIORITY, &link_task_handle);
#endif
}


/*
 * Retrieve static IP configuration of the default network interface and set
 * IP parameters (Interface IP address, Netmask and Gateway IP address).
 */
static void ethernetif_static_ip_config()
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
	ip_addr_t dnsaddr;

	// static IP configuration. Retrieve IP settings from user properties.
	ipaddr.addr = inet_addr(LLNET_CONF_get_device_ip_address());
	netmask.addr = inet_addr(LLNET_CONF_get_netmask());
	gw.addr	= inet_addr(LLNET_CONF_get_gateway_ip_address());
	netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);
	LLNET_DEBUG_TRACE("[INFO] Static IP address assigned: %s\n", inet_ntoa(ipaddr.addr));

	// set static DNS Host IP address.
	if(DNS_MAX_SERVERS > 0)
	{
		char * static_dns_ip_addr = (char*)LLNET_CONF_get_dns_ip_address();
		if(static_dns_ip_addr != NULL)
		{
			dnsaddr.addr = inet_addr(static_dns_ip_addr);
			dns_setserver(0, &dnsaddr);
			// notify DNS servers IP address updated
			dns_servers_list_updated = 1;
		}
	}
}

/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
	int32_t dhcpConfEnabled = LLNET_CONF_is_ip_dhcp_enabled() == LLNET_CONF_TRUE;

	if(netif_is_link_up(netif))
	{
		LLNET_DEBUG_TRACE("[INFO] The network cable is now connected \n");

		if(dhcpConfEnabled)
		{
			/* Update DHCP state machine */
			DHCP_state = DHCP_START;

			netif_set_addr(netif, &ipaddr , &netmask, &gw);

			// resume DHCP thread
			dhcp_sleeping = 0;
			vTaskResume(dhcp_task_handle);
		}else{
			// launch static Network Interface configuration
			ethernetif_static_ip_config();
		}

		/* When the netif is fully configured this function must be called.*/
		netif_set_up(netif);
	}
	else
	{
		if(dhcpConfEnabled){
			/* Update DHCP state machine */
			DHCP_state = DHCP_LINK_DOWN;
		}

		/*  When the netif link is down this function must be called.*/
		netif_set_down(netif);

		netif_not_connected(netif);
	}
}


/**
 * Network initialization. Start network interfaces and configure it.
 * @return 0 if no error occurred, error code otherwise.
 */
int32_t llnet_lwip_init(void)
{
	int32_t dhcpConfEnabled =  LLNET_CONF_is_ip_dhcp_enabled() == LLNET_CONF_TRUE;

	LLNET_NETWORK_HEAP_initialize();

	/* Initialize the LwIP TCP/IP stack */
	tcpip_init(NULL, NULL);

	/* Configure the Network interface */
	Netif_Config();

	/* Notify user about the network interface config */
	User_notification(&gnetif);

	if(dhcpConfEnabled)
	{
		/* Start DHCPClient */
		dhcp_sleeping = 0;
#if defined(__GNUC__)
		xTaskCreate((TaskFunction_t)DHCP_thread, "DHCP", configMINIMAL_STACK_SIZE * 5, &gnetif, LWIP_DHCP_TASK_PRIORITY, &dhcp_task_handle);
#else
		xTaskCreate((TaskFunction_t)DHCP_thread, "DHCP", configMINIMAL_STACK_SIZE * 2, &gnetif, LWIP_DHCP_TASK_PRIORITY, &dhcp_task_handle);
#endif
	}

	return 0;
}

/**
 * Returns a 32 bit random number.
 */
uint32_t lwip_getRandomNumber(void) {
  uint32_t randomValue = 0;

  if (HAL_RNG_GetState(&RngHandle) == HAL_RNG_STATE_RESET) {
    // Enable Random Number Generator (RNG)
    __RNG_CLK_ENABLE();
    // Configure the RNG peripheral
    RngHandle.Instance = RNG;
    if (HAL_RNG_Init(&RngHandle) != HAL_OK) {
      printf("An error occurred during RNG initialization\n");
      while(1);
    }
  }

  if (HAL_RNG_GenerateRandomNumber(&RngHandle, &randomValue) != HAL_OK) {
    printf("An error occurred during RNG generation\n");
    while(1);
  }

  return randomValue;
}
