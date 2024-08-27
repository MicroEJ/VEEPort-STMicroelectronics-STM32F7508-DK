/*
 * C
 *
 * Copyright 2018-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_SSL_verifyCallback implementation over mbedtls.
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 17 June 2022
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include "mbedtls/debug.h"
#include "LLNET_SSL_mbedtls_configuration.h"
#include "LLNET_SSL_CONTEXT_impl.h"
#include "LLNET_SSL_CONSTANTS.h"
#include "LLNET_SSL_ERRORS.h"
#include "LLNET_SSL_utils_mbedtls.h"
#include "LLNET_SSL_verifyCallback.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
	extern "C" {
#endif

#if MBEDTLS_DEBUG_LEVEL > 0
#define CERT_INFO_BUF_SIZE  1024
#endif

/**
 * Certificate verification callback for mbed TLS
 * Here we only use it to display information on each cert in the chain
 */
int LLNET_SSL_VERIFY_verifyCallback(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
#if MBEDTLS_DEBUG_LEVEL > 0
	/*
	 * If mbedtls debug level activated, print full informations on certificates
	 */
	char buf[CERT_INFO_BUF_SIZE] = {0};

	printf("\nVerifying certificate at depth %d:\n", depth);
	mbedtls_x509_crt_info(&buf[0], CERT_INFO_BUF_SIZE - 1, "  ", crt); //-1 to keep last byte for null-terminated character
	printf("%s", &buf[0]);

	if (0 == *flags)
		printf("No verification issue for this certificate\n");
	else
	{
		mbedtls_x509_crt_verify_info(&buf[0], CERT_INFO_BUF_SIZE, "  ! ", *flags);
		printf("%s\n", &buf[0]);
	}
#endif

	cert_verify_ctx* verify_ctx = (cert_verify_ctx*)data;

	/* If no certificate from peer is signed by on of the trust certificate,
	 * check if one of the trust certificate is identical to one of the peer certificate */
	if (MBEDTLS_X509_BADCERT_NOT_TRUSTED == ((*flags) & MBEDTLS_X509_BADCERT_NOT_TRUSTED))
	{
		LLNET_SSL_DEBUG_TRACE("%s(depth=%d) Certificate not trusted, clear error flag and lookup for identical certificate\n", __func__,depth);

		verify_ctx->isUnTrustCA = 1;

		/* Clear error flag*/
		*flags &= ~MBEDTLS_X509_BADCERT_NOT_TRUSTED;
	}

	if (0 != verify_ctx->isUnTrustCA)
	{
		/* Compare peer certificate with trusted certificates */

		mbedtls_x509_crt* trust_ca = NULL;

		/* Look for certificate in trust store that match peer certificate */
		for( trust_ca = verify_ctx->conf->ca_chain; trust_ca != NULL; trust_ca = trust_ca->next )
		{
			uint8_t raw_compare = -1;

			/* compare certificate raw content */
			if (trust_ca->raw.len == crt->raw.len)
			{
				raw_compare = memcmp(trust_ca->raw.p, crt->raw.p, trust_ca->raw.len);
			}

			if (0 == raw_compare)
			{
				LLNET_SSL_DEBUG_TRACE("%s(depth=%d) Found identical certificate in trust store\n", __func__, depth);
				verify_ctx->isUnTrustCA = 0;
			}
			else
				continue;

			break;

		}
	}

	/* if this is the last certificate of the peer chain, set back error flag */
	if ( (0 == depth) && (0 != verify_ctx->isUnTrustCA))
	{
		LLNET_SSL_DEBUG_TRACE("%s(depth=%d) No trusted nor identical certificate in trust store\n", __func__, depth);
		*flags |= MBEDTLS_X509_BADCERT_NOT_TRUSTED;

		/* Reset isUnTrustCA flag in verify context */
		verify_ctx->isUnTrustCA = 0;
	}

	return 0;
}

#ifdef __cplusplus
	}
#endif
