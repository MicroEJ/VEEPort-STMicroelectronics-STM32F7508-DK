/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/inet.h"
#include "lwip/apps/lwiperf.h"
#include "lwip_util.h"
#include "iperf.h"
#include "iperf_configuration.h"

#ifdef __cplusplus
	extern "C" {
#endif

/** Polling time to check for an assigned local IP address after LwIP initialization */
#define IP_ADDR_POLLING_TIME_MS (100)

/** Semaphore used to signal the end of an iperf test */
static SemaphoreHandle_t done_sem;

/* iperf report callback */
static void lwiperf_report(void *arg, enum lwiperf_report_type report_type,
		const ip_addr_t* local_addr, uint16_t local_port, const ip_addr_t* remote_addr, uint16_t remote_port,
		uint32_t bytes_transferred, uint32_t ms_duration, uint32_t bandwidth_kbitpsec) {
	switch (report_type) {
	case LWIPERF_TCP_DONE_SERVER:
	case LWIPERF_TCP_DONE_CLIENT:
		printf("[%3d] local %s port %d connected with %s port %d\n",
				(int)arg,
				inet_ntoa(local_addr->addr), local_port,
				inet_ntoa(remote_addr->addr), remote_port);
		printf("[ ID] Interval      Transfer     Bandwidth\n");
		printf("[%3d] 0.0-%.1f sec  %.1f MBytes  %.1f Mbits/sec\n",
				(int)arg,
				(float)ms_duration / 1000.0,
				(float)bytes_transferred / 1024.0 / 1024.0,
				(float)bandwidth_kbitpsec / 1000.0);
		break;
	default:
		printf("iperf aborted with report_type = %d\n", report_type);
		break;
	}
	xSemaphoreGive(done_sem);
}

void iperf_main(void) {
	ip_addr_t tcp_server = {0};
	uint32_t ip_address = 0;
	int id = 1;

	printf("Start iperf benchmark\n");

	done_sem = xSemaphoreCreateBinary();
	if (done_sem == NULL) {
		printf("Could not create RTOS semaphore, iperf will exit\n");
		return;
	}

	printf("Initialize LwIP\n");
	llnet_lwip_init();

	printf("Wait for IP address to be assigned\n");
	do {
		vTaskDelay(IP_ADDR_POLLING_TIME_MS / portTICK_PERIOD_MS);

		struct netif *netif = netif_list;
		while (netif != NULL) {
#if (defined(LWIP_NETIF_LOOPBACK) && (LWIP_NETIF_LOOPBACK != 0))
			if (strcmp("lo", (char*)netif->name) != 0) {
#endif
				ip_address = netif->ip_addr.addr;
#if (defined(LWIP_NETIF_LOOPBACK) && (LWIP_NETIF_LOOPBACK != 0))
			}
#endif
			netif = netif->next;
		}
	} while (ip_address == 0);
	printf("IP address assigned: %s\n", inet_ntoa(ip_address));

	tcp_server.addr = inet_addr(IPERF_REMOTE_TCP_SERVER_IP_ADDRESS);
	printf("Remote TCP server configuration used \"%s:%d\"\n", IPERF_REMOTE_TCP_SERVER_IP_ADDRESS, LWIPERF_TCP_PORT_DEFAULT);

#if defined(IPERF_TCP_CLIENT_SERVER)
	printf("Starting iperf TCP client test\n");
	lwiperf_start_tcp_client_default(&tcp_server, lwiperf_report, (void*)id);
	xSemaphoreTake(done_sem, portMAX_DELAY);

	printf("Starting iperf TCP server test\n");
	lwiperf_start_tcp_server_default(lwiperf_report, (void*)id);
	xSemaphoreTake(done_sem, portMAX_DELAY);
#elif defined(IPERF_TCP_CLIENT_TX_RX_SIMULTANEOUSLY)
	printf("Starting iperf TCP client test - Bidirectional test simultaneously\n");
	lwiperf_start_tcp_client(&tcp_server, LWIPERF_TCP_PORT_DEFAULT, LWIPERF_DUAL, lwiperf_report, (void*)id);
	xSemaphoreTake(done_sem, portMAX_DELAY);
	xSemaphoreTake(done_sem, portMAX_DELAY);
#else
	#error "Define an iperf configuration!"
#endif

	vSemaphoreDelete(done_sem);
	printf("End iperf benchmark\n");

	do {
		vTaskDelay(1);
	} while (1);
}

#ifdef __cplusplus
	}
#endif
