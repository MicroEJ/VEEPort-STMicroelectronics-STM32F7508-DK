/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef __LLNET_SSL_ERROR_CONSTANTS__
#define __LLNET_SSL_ERROR_CONSTANTS__

/**
 * @file
 * @brief MicroEJ SSL low level API
 * @author MicroEJ Developer Team
 * @version 6.0.0
 * @date 26 August 2022
 */


#define SSLv3_PROTOCOL 		(0) /* @brief SSL v3 protocol version. */
#define TLSv1_PROTOCOL 		(1) /* @brief TLS v1.0 protocol version. */
#define TLSv1_1_PROTOCOL 	(2) /* @brief TLS v1.1 protocol version. */
#define TLSv1_2_PROTOCOL 	(3) /* @brief TLS v1.2 protocol version. */
#define DTLSv1_PROTOCOL 	(4) /* @brief DTLS v1.0 protocol version. */
#define DTLSv1_2_PROTOCOL 	(5) /* @brief DTLS v1.2 protocol version. */
#define TLSv1_3_PROTOCOL	(6) /* @brief TLS v1.3 protocol version. */

#define CERT_PEM_FORMAT 	(0) /* @brief PEM Certificate encoded format. */
#define CERT_DER_FORMAT 	(1) /* @brief DER (ASN1) Certificate encoded format */


// comment this define to remove secure network debug trace
//#define LLNET_SSL_DEBUG	/* @brief enable debug trace. */


#ifdef LLNET_SSL_DEBUG
#include <stdio.h>
#define LLNET_SSL_DEBUG_TRACE 	printf
#define LLNET_SSL_DEBUG_ASSERT(...) 	printf(__VA_ARGS__); while(1);
#else
#define LLNET_SSL_DEBUG_TRACE(...) ((void) 0)
#define LLNET_SSL_DEBUG_ASSERT(...) ((void) 0)
#endif

#endif //__LLNET_SSL_ERROR_CONSTANTS__
