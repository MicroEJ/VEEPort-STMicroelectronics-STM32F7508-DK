/*
 * C
 *
 * Copyright 2018-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_SSL_verifyCallback functions for mbedtls.
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 17 June 2022
 */

#ifndef LLNET_SSL_VERIFY_CALLBACK
#define LLNET_SSL_VERIFY_CALLBACK

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include "mbedtls/x509.h"

#ifdef __cplusplus
	extern "C" {
#endif

/*
 * Verify Callback context
*/
typedef struct
{
	mbedtls_ssl_config* conf;
	uint8_t             isUnTrustCA;
}cert_verify_ctx;


/*
 * Certificate verification callback
*/
int LLNET_SSL_VERIFY_verifyCallback(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags);


#ifdef __cplusplus
	}
#endif

#endif //LLNET_SSL_VERIFY_CALLBACK
