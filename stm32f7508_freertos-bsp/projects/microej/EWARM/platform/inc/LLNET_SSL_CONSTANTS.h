/*
 * C
 *
 * Copyright 2014-2019 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef __LLNET_SSL_ERROR_CONSTANTS__
#define __LLNET_SSL_ERROR_CONSTANTS__

/**
 * @file
 * @brief MicroEJ SSL low level API
 * @author MicroEJ Developer Team
 * @version 5.1.1
 * @date 7 May 2021
 */

#include <sni.h>


/**
 * SSL v3 protocol version
 */
#define SSLv3_PROTOCOL (0)

/**
 * TLS v1.0 protocol version
 */
#define TLSv1_PROTOCOL (1)

/**
 * TLS v1.1 protocol version
 */
#define TLSv1_1_PROTOCOL (2)

/**
 * TLS v1.2 protocol version
 */
#define TLSv1_2_PROTOCOL (3)

/**
 * DTLS v1 protocol version
 */
#define DTLSv1_PROTOCOL (4)

/**
 * DTLS v1.2 protocol version
 */
#define DTLSv1_2_PROTOCOL (5)

/**
 * TLS v1.3 protocol version
 */
#define TLSv1_3_PROTOCOL (6)

/**
 * PEM Certificate encoded format
 */
#define CERT_PEM_FORMAT (0)

/**
 * DER (ASN1) Certificate encoded format
 */
#define CERT_DER_FORMAT (1)

// comment this define to remove secure network debug trace
//#define LLNET_SSL_DEBUG


#ifdef LLNET_SSL_DEBUG
#include <stdio.h>
#define LLNET_SSL_DEBUG_TRACE 	printf
#define LLNET_SSL_DEBUG_ASSERT(...) 	printf(__VA_ARGS__); while(1);
#else
#define LLNET_SSL_DEBUG_TRACE(...) ((void) 0)
#define LLNET_SSL_DEBUG_ASSERT(...) ((void) 0)
#endif

#endif //__LLNET_SSL_ERROR_CONSTANTS__
