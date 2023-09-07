/*
 * C
 *
 * Copyright 2014-2017 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef error_constants
#define error_constants

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#include <sni.h>


/**
 * Socket option constants
 */
#define CPNET_SO_KEEPALIVE	0x8
#define CPNET_SO_LINGER     0x80
#define CPNET_SO_BINDADDR   0x0F
#define CPNET_IP_TTL        0x1E61
#define CPNET_SO_TIMEOUT	0x1006
#define CPNET_SO_SNDBUF		0x1001
#define CPNET_SO_RCVBUF		0x1002
#define CPNET_SO_REUSEADDR	0x04
#define CPNET_SO_OOBINLINE	0x1003
#define CPNET_TCP_NODELAY	0x01
#define CPNET_IP_TOS		0x03
#define CPNET_SO_BROADCAST 0x20
#define CPNET_IP_MULTICAST_IF 0x10
#define CPNET_IP_MULTICAST_IF2 0x1F
#define CPNET_IP_MULTICAST_LOOP 0x12

#endif
