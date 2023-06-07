/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef IPERF_CONFIGURATION_H
#define IPERF_CONFIGURATION_H

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Iperf has the following configurations:
 * 1 TCP Server
 * 2 TCP Client
 *   2.1 Tx only (default)
 *   2.2 Tx and Rx simultaneously
 *   2.3 Tx and Rx individually
 *
 * For simplicity, 2 test configurations are available, which cover all 4 iperf configurations:
 * - IPERF_TCP_CLIENT_SERVER
 *   => Starts first a TCP client default (2.1) and after finalization it starts a TCP server (1).
 *      Case (2.3) is exactly the same iperf configuration, the difference being that the TCP server is started implicitly
 *      by the lwiperf, rather than explicitly
 * - IPERF_TCP_CLIENT_TX_RX_SIMULTANEOUSLY
 *   => Starts only a TCP client (2.2) configuration, which starts internally a TCP server for simultaneous communication
 */
#define IPERF_TCP_CLIENT_SERVER
#undef IPERF_TCP_CLIENT_TX_RX_SIMULTANEOUSLY

/* Consistency check of the configuration */
#if (!defined(IPERF_TCP_CLIENT_SERVER) && !defined(IPERF_TCP_CLIENT_TX_RX_SIMULTANEOUSLY)) || \
	(defined(IPERF_TCP_CLIENT_SERVER) && defined(IPERF_TCP_CLIENT_TX_RX_SIMULTANEOUSLY))
	#error "Must define one iperf configuration!"
#endif

/** IP address of the remote TCP server, to be used for iperf client tests */
#define IPERF_REMOTE_TCP_SERVER_IP_ADDRESS ((char*)"192.168.137.1")

#ifdef __cplusplus
	}
#endif

#endif
