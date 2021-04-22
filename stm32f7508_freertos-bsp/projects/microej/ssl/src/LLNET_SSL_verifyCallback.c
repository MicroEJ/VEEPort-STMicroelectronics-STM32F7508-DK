/*
 * C
 *
 * Copyright 2018-2019 IS2T. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break IS2T warranties on the whole library.
 */
#include <LLNET_SSL_CONTEXT_impl.h>
#include <LLNET_SSL_CONSTANTS.h>
#include <LLNET_SSL_ERRORS.h>
#include <stdlib.h>
#include <string.h>

#include "LLNET_SSL_verifyCallback.h"

#include "mbedtls/x509.h"

#ifdef __cplusplus
	extern "C" {
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
	const uint32_t buf_size = 1024;
	char buf[buf_size];

	printf("\nVerifying certificate at depth %d:\n", depth);
	mbedtls_x509_crt_info(&buf[0], buf_size - 1, "  ", crt);
	printf("%s", &buf[0]);

	if (*flags == 0)
		printf("No verification issue for this certificate\n");
	else
	{
		mbedtls_x509_crt_verify_info(&buf[0], buf_size, "  ! ", *flags);
		printf("%s\n", &buf[0]);
	}
#endif

	cert_verify_ctx* verify_ctx = (cert_verify_ctx*)data;

	/* If no certificate from peer is signed by on of the trust certificate,
	 * check if one of the trust certificate is identical to one of the peer certificate */
	if ( ((*flags) & MBEDTLS_X509_BADCERT_NOT_TRUSTED) == MBEDTLS_X509_BADCERT_NOT_TRUSTED )
	{
		LLNET_SSL_DEBUG_TRACE("%s(depth=%d) Certificate not trusted, clear error flag and lookup for identical certificate\n", __func__,depth);

		verify_ctx->isUnTrustCA = 1;

		/* Clear error flag*/
		*flags &= ~MBEDTLS_X509_BADCERT_NOT_TRUSTED;
	}

	if (verify_ctx->isUnTrustCA != 0)
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

			if (raw_compare == 0)
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
	if ( (depth == 0) && (verify_ctx->isUnTrustCA != 0))
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
