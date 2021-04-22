/*
 * C
 *
 * Copyright 2018-2019 IS2T. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break IS2T warranties on the whole library.
 */
#ifndef LLNET_SSL_VERIFY_CALLBACK
#define LLNET_SSL_VERIFY_CALLBACK

#include "mbedtls/ssl.h"

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

#endif
