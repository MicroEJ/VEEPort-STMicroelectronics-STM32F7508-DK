/*
 * C
 *
 * Copyright 2017-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief Platform implementation specific macro.
 * @author MicroEJ Developer Team
 * @version 1.1.1
 * @date 7 February 2020
 */

#ifndef  LLNET_CONFIGURATION_H
#define  LLNET_CONFIGURATION_H

/**
 * @brief Compatibility sanity check value.
 * This define value is checked in the implementation to validate that the version of this configuration
 * is compatible with the implementation.
 *
 * This value must not be changed by the user of the CCO.
 * This value must be incremented by the implementor of the CCO when a configuration define is added, deleted or modified.
 */
#define LLNET_CONFIGURATION_VERSION (2)

/**
 * By default all the llnet_* functions are mapped on the BSD functions.
 * Documentation (signature, etc.) for each llnet_* function is available
 * in the mapped BSD function documentation.
 */
#define llnet_accept		accept
#define llnet_bind			bind
#define llnet_close			close
#define llnet_connect		connect
#define llnet_fcntl			fcntl
#define llnet_gethostname	gethostname
#define llnet_getpeername	getpeername
#define llnet_getsockname	getsockname
#define llnet_getsockopt	getsockopt
#define llnet_htonl			htonl
#define llnet_htons			htons
#define llnet_ioctl			ioctl
#define llnet_listen		listen
#define llnet_ntohs			ntohs
#define llnet_recv 			recv
#define llnet_recvfrom		recvfrom
#define llnet_send			send
#define llnet_sendto		sendto
#define llnet_setsockopt	setsockopt
#define llnet_socket		socket
#define llnet_shutdown		shutdown

/**
 * Define the maximum number of sockets that can be handled by the net module
 */
#define LLNET_MAX_SOCKETS MEMP_NUM_NETCONN

/**
 * Define the logical FD starting value
 */
#define LLNET_SOCKFD_START_IDX LWIP_SOCKET_OFFSET

/**
 * Don't modify the LLNET_AF_* constants.
 */
#define LLNET_AF_IPV4	(0x1)
#define LLNET_AF_IPV6	(0x2)
#define LLNET_AF_DUAL	((LLNET_AF_IPV4) | (LLNET_AF_IPV6))

/**
 *  Address family:
 *  - LLNET_AF_IPV4 for only IPv4
 *  - LLNET_AF_IPV6 for only IPv6
 *  - LLNET_AF_DUAL for both IPv4 and IPv6
 */
#define LLNET_AF (LLNET_AF_IPV4)

/**
 * Returns the errno value for the given file descriptor.
 * Given file descriptor may be -1 if no file descriptor is defined.
 */
#include <lwip/errno.h>
#define llnet_errno(fd) 	errno

/*
 * Initialize the network stack.
 * Returns 0 on success, -1 on error.
 * This method may block and must be called in another RTOS task.
 * See async_select task.
 * By default this macro does nothing.
 */
#include "lwip_util.h"
#define llnet_init    	llnet_lwip_init

/*
 * Returns true (bool) if we can call the services of the network stack without jeopardizing the system,
 * otherwise returns false (bool).
 * Returning true does not mean that a network interface is up.
 * By default this macro always return true.
 */
#define llnet_is_ready()	(true)

/** @brief Set this define if the system sends SIGPIPE signal when a connection is closed by the remote host. */
#if defined(__linux__) || defined(__QNXNTO__)
#define LLNET_IGNORE_SIGPIPE
#endif


/**
 * Enable network debug trace
 */
//#define LLNET_DEBUG

/**
 * If ioctl() cannot be used to implement the LLNET_STREAMSOCKETCHANNEL_IMPL_available() function,
 * define USE_MSG_PEEK_FOR_AVAILABLE and NET_EMBEDDED_AVAILABLE_BUFFER_SIZE.
 */
//#define USE_MSG_PEEK_FOR_AVAILABLE
//#define NET_EMBEDDED_AVAILABLE_BUFFER_SIZE (4096)

/**
 * Define USE_IOCTL_FOR_BLOCKING_OPTION to use ioctl() instead of fcntl() to configure socket
 * blocking or non-blocking mode.
 */
#define USE_IOCTL_FOR_BLOCKING_OPTION


#if LLNET_AF & LLNET_AF_IPV6
/**
 * Only one IPV6 interface is supported
 */
#define LLNET_IPV6_INTERFACE_NAME "st1"

#endif

#endif // LLNET_CONFIGURATION_H
