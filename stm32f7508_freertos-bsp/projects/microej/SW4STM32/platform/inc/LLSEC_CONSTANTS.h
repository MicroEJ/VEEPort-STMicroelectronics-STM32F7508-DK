/*
 * C
 *
 * Copyright 2014-2017 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef __LLSEC_CONSTANTS__
#define __LLSEC_CONSTANTS__

/**
 * @file
 * @brief MicroEJ Security low level API
 * @author MicroEJ Developer Team
 * @version 1.4.0
 * @date 30 January 2020
 */

#include <sni.h>

/**
 * PEM Certificate encoded format
 */
#define CERT_PEM_FORMAT (0)

/**
 * DER (ASN1) Certificate encoded format
 */
#define CERT_DER_FORMAT (1)

// comment this define to remove debug trace
//#define LLSEC_DEBUG


#ifdef LLSEC_DEBUG
#include <stdio.h>
#define LLSEC_DEBUG_DEBUG_TRACE 	printf
#define LLSEC_DEBUG_DEBUG_ASSERT(...) 	printf(__VA_ARGS__); while(1);
#else
#define LLSEC_DEBUG_DEBUG_TRACE(...) ((void) 0)
#define LLSEC_DEBUG_DEBUG_ASSERT(...) ((void) 0)
#endif

#endif //__LLSEC_CONSTANTS__
