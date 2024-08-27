/*
 * C
 *
 * Copyright 2018-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_SSL_CONTEXT implementation over mbedtls.
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 17 June 2022
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc    calloc
#define mbedtls_free      free
#endif

#include "mbedtls/debug.h"
#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_internal.h"
#include "mbedtls/error.h"
#if defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_CTR_DRBG_C)
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#endif
#include "LLNET_SSL_mbedtls_configuration.h"
#include "LLNET_SSL_utils_mbedtls.h"
#include "LLNET_SSL_verifyCallback.h"
#include "LLNET_SSL_CONTEXT_impl.h"
#include "LLNET_SSL_CONSTANTS.h"
#include "LLNET_SSL_ERRORS.h"
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Sanity check between the expected version of the configuration and the actual version of
 * the configuration.
 * If an error is raised here, it means that a new version of the CCO has been installed and
 * the configuration LLNET_SSL_mbedtls_configuration.h must be updated based on the one provided
 * by the new CCO version.
 */
#if LLNET_SSL_MBEDTLS_CONFIGURATION_VERSION != 1
	#error "Version of the configuration file LLNET_SSL_mbedtls_configuration.h is not compatible with this implementation."
#endif

/* ----------- external function and variables ----------- */
extern int32_t LLNET_SSL_TranslateReturnCode(int32_t mbedtls_error);
#if defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_CTR_DRBG_C)
extern mbedtls_ctr_drbg_context ctr_drbg;
#endif

/* ----------- Definitions  -----------*/
#define PEM_END "-----END CERTIFICATE-----"

/* ----------- Private API  -----------*/
#if MBEDTLS_DEBUG_LEVEL > 0
/**
 * Debug callback for mbed TLS
 * Just prints on the USB serial port
 */
void microej_mbedtls_debug(void *ctx, int level, const char *file, int line,
					 const char *str)
{
	const char *p, *basename;
	(void) ctx;

	/* Extract basename from file */
	for(p = basename = file; *p != '\0'; p++) {
		if(*p == '/' || *p == '\\') {
			basename = p + 1;
		}
	}

	printf("%s:%04d: |%d| %s", basename, line, level, str);
}
#endif

/* ----------- API  -----------*/

int32_t LLNET_SSL_CONTEXT_IMPL_createContext(int32_t protocol, uint8_t isClientContext)
{
	LLNET_SSL_DEBUG_TRACE("%s(protocol=%d, isClientContext=%d\n", __func__, protocol, isClientContext);

	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)mbedtls_calloc(1, sizeof(mbedtls_ssl_config));
	if(NULL == conf)
	{
		SNI_throwNativeIOException(J_MEMORY_ERROR, "Not enough memory");
		return SNI_IGNORED_RETURNED_VALUE;
	}

	void* p_rng = NULL;
	mbedtls_ssl_config_init(conf);

	int endpoint;
	if (isClientContext)
	{
		endpoint = MBEDTLS_SSL_IS_CLIENT;
	}
	else
	{
		endpoint = MBEDTLS_SSL_IS_SERVER;
	}
	int ret;
	if ((ret = mbedtls_ssl_config_defaults(conf,
					endpoint,
					MBEDTLS_SSL_TRANSPORT_STREAM,
					MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
	{
		(void)ret;
		LLNET_SSL_DEBUG_MBEDTLS_TRACE("mbedtls_ssl_config_defaults", ret);
	}

	switch (protocol)
	{

		case SSLv3_PROTOCOL:
			mbedtls_ssl_conf_min_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_0 );
			mbedtls_ssl_conf_max_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_0 );
			break;
		case TLSv1_PROTOCOL:
			mbedtls_ssl_conf_min_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_1 );
			mbedtls_ssl_conf_max_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_1 );
			break;
		case TLSv1_1_PROTOCOL:
			mbedtls_ssl_conf_min_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_2 );
			mbedtls_ssl_conf_max_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_2 );
			break;
		case TLSv1_2_PROTOCOL:
			mbedtls_ssl_conf_min_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3 );
			mbedtls_ssl_conf_max_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3 );
			break;
		default:
			mbedtls_ssl_config_free(conf);
			mbedtls_free(conf);
			SNI_throwNativeIOException(J_VERSION_ERROR, "Unsupported protocol version");
			return SNI_IGNORED_RETURNED_VALUE;
	}

#if defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_CTR_DRBG_C)
		p_rng = &ctr_drbg;
#endif
	mbedtls_ssl_conf_rng(conf, LLNET_SSL_utils_mbedtls_random, p_rng);

	/* It is possible to disable authentication by passing
	 * MBEDTLS_SSL_VERIFY_NONE in the call to mbedtls_ssl_conf_authmode()
	 */
	mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_UNSET);

	/*
	 * Change certificate verification profile, allocate and initialize new profile
	 */
	mbedtls_x509_crt_profile* crt_profile = (mbedtls_x509_crt_profile*)mbedtls_calloc(1, sizeof(mbedtls_x509_crt_profile));
	if (NULL == crt_profile)
	{
		mbedtls_ssl_config_free(conf);
		mbedtls_free(conf);
		SNI_throwNativeIOException(J_MEMORY_ERROR, "Not enough memory");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	/* Hashes from MD5 and above */
	crt_profile->allowed_mds = MBEDTLS_X509_ID_FLAG( MBEDTLS_MD_MD5 ) |
							   MBEDTLS_X509_ID_FLAG( MBEDTLS_MD_SHA1 ) |
							   MBEDTLS_X509_ID_FLAG( MBEDTLS_MD_RIPEMD160 ) |
							   MBEDTLS_X509_ID_FLAG( MBEDTLS_MD_SHA224 ) |
							   MBEDTLS_X509_ID_FLAG( MBEDTLS_MD_SHA256 ) |
							   MBEDTLS_X509_ID_FLAG( MBEDTLS_MD_SHA384 ) |
							   MBEDTLS_X509_ID_FLAG( MBEDTLS_MD_SHA512 );
	/* Any PK alg    */
	crt_profile->allowed_pks = 0xFFFFFFF;
	/* Any curve     */
	crt_profile->allowed_curves = 0xFFFFFFF;
	/* RSA min lenght */
	crt_profile->rsa_min_bitlen = 1024;

	mbedtls_ssl_conf_cert_profile(conf, crt_profile);

	/* Allocate and initialize certificate verify context*/
	cert_verify_ctx* verify_ctx = (cert_verify_ctx*)mbedtls_calloc(1, sizeof(cert_verify_ctx));
	if (NULL == verify_ctx)
	{
		mbedtls_ssl_conf_cert_profile(conf, NULL);
		mbedtls_free(crt_profile);
		mbedtls_ssl_config_free(conf);
		mbedtls_free(conf);
		SNI_throwNativeIOException(J_MEMORY_ERROR, "Not enough memory");
		return SNI_IGNORED_RETURNED_VALUE;
	}

	verify_ctx->conf = conf;
	verify_ctx->isUnTrustCA = 0;

	mbedtls_ssl_conf_verify(conf, LLNET_SSL_VERIFY_verifyCallback, (void*)verify_ctx);

#if MBEDTLS_DEBUG_LEVEL > 0
			mbedtls_ssl_conf_dbg(conf, microej_mbedtls_debug, NULL);
#if defined(MBEDTLS_DEBUG_C)
			mbedtls_debug_set_threshold(MBEDTLS_DEBUG_LEVEL);
#endif
#endif

	LLNET_SSL_DEBUG_TRACE("%s(method=%d) return ctx=%p\n", __func__, protocol,conf);
	return (int32_t)conf;
}

void LLNET_SSL_CONTEXT_IMPL_addTrustedCertificate(int32_t contextID, uint8_t *cert, int32_t len, int32_t format)
{

	LLNET_SSL_DEBUG_TRACE("%s(context=%d)\n", __func__, contextID);

	int32_t ret = J_SSL_NO_ERROR;

	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);
	mbedtls_x509_crt* cacert = NULL;

	/* Check parameters */
	if (NULL == conf || NULL == cert || 0 == len ||
		((CERT_DER_FORMAT != format) && (CERT_PEM_FORMAT != format)))
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return;
	}

	if (NULL == conf->ca_chain)
	{
		cacert = (mbedtls_x509_crt*)mbedtls_calloc(1, sizeof(mbedtls_x509_crt));
		if(NULL == cacert)
		{
			SNI_throwNativeIOException(J_MEMORY_ERROR, "Not enough memory");
			return;
		}
		mbedtls_x509_crt_init(cacert);
	}
	else
	{
		cacert = conf->ca_chain;
	}

	if (CERT_DER_FORMAT == format)
	{
		ret = mbedtls_x509_crt_parse_der(cacert, (const unsigned char *)cert, len);
		ret = LLNET_SSL_TranslateReturnCode(ret);
	}
	else
	{
		/* Find end of first certificate. */
		char* first_cert_end_ptr = strstr((char*) cert, PEM_END);
		if (NULL == first_cert_end_ptr) {
			ret = J_BAD_FUNC_ARG;
		} else {
			first_cert_end_ptr += sizeof(PEM_END);
			int first_cert_len = first_cert_end_ptr - (char*) cert;
			ret = LLNET_SSL_utils_mbedtls_x509_crt_parse(cacert, cert, 0, first_cert_len);
		}

	}

	if(J_SSL_NO_ERROR != ret)
	{
		mbedtls_x509_crt_free(cacert);
		mbedtls_free(cacert);
		SNI_throwNativeIOException(LLNET_SSL_TranslateReturnCode(ret), "Certificate parsing error");
		return;
	}

	if (NULL == conf->ca_chain)
	{
		mbedtls_ssl_conf_ca_chain(conf, cacert, NULL);
	}
}


void LLNET_SSL_CONTEXT_IMPL_clearTrustStore(int32_t contextID){

	LLNET_SSL_DEBUG_TRACE("%s(context=%d)\n", __func__, contextID);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);

	if (NULL != conf)
	{
		if (NULL != conf->ca_chain)
		{
			void* chain_ptr = (void*)conf->ca_chain;
			mbedtls_ssl_conf_ca_chain(conf, NULL, NULL);
			mbedtls_x509_crt_free(chain_ptr);
			mbedtls_free(chain_ptr);
		}
	}
}

void LLNET_SSL_CONTEXT_IMPL_clearKeyStore(int32_t contextID){
	LLNET_SSL_DEBUG_TRACE("%s(context=%d)\n", __func__, contextID);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);

	/* Free the private key */
	if (NULL != conf)
	{
		if (NULL != conf->key_cert)
		{
			if (NULL != conf->key_cert->key)
			{
				mbedtls_pk_free(conf->key_cert->key);
				mbedtls_free(conf->key_cert->key);
			}
		}
	}
}

void LLNET_SSL_CONTEXT_IMPL_setCertificate(int32_t contextID, uint8_t* cert, int32_t len, int32_t format)
{
	LLNET_SSL_DEBUG_TRACE("%s(context=%d)\n", __func__, contextID);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);
	mbedtls_x509_crt *clicert;
	bool key_cert_allocated = false;
	int ret;

	/* Check parameters */
	if (NULL == conf || NULL == cert || 0 == len ||
		((CERT_DER_FORMAT != format) && (CERT_PEM_FORMAT != format)))
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return;
	}

	/* Allocate a new keycert if needed, otherwise free the existing certificate */
	if (NULL == conf->key_cert)
	{
		conf->key_cert = mbedtls_calloc( 1, sizeof(mbedtls_ssl_key_cert));
		if (NULL == conf->key_cert)
		{
			SNI_throwNativeIOException(J_MEMORY_ERROR, "Not enough memory");
			return;
		}
		key_cert_allocated = true;
	}
	else
	{
		mbedtls_x509_crt_free(conf->key_cert->cert);
		mbedtls_free(conf->key_cert->cert);
	}

	/* The LLNET_SSL_* interface does not support multiple keycert (only one private key can be set) */
	conf->key_cert->next = NULL;

	/* Allocate a new certificate */
	clicert = mbedtls_calloc( 1, sizeof(mbedtls_x509_crt));
	if (NULL == clicert)
	{
		if(key_cert_allocated)
		{
			mbedtls_free(conf->key_cert);
		}
		SNI_throwNativeIOException(J_MEMORY_ERROR, "Not enough memory");
		return;
	}

	/* Try to parse the certificate */
	mbedtls_x509_crt_init(clicert);
	if (CERT_DER_FORMAT == format)
	{
		ret = mbedtls_x509_crt_parse_der(clicert, (const unsigned char *)cert, len);
		ret = LLNET_SSL_TranslateReturnCode(ret);
	}
	else
	{
		ret = LLNET_SSL_utils_mbedtls_x509_crt_parse(clicert, cert, 0, len);
	}

	/* Check parse result */
	if (J_SSL_NO_ERROR != ret)
	{
		/* Free the certificate */
		mbedtls_x509_crt_free(clicert);
		mbedtls_free(clicert);
		if(key_cert_allocated)
		{
			mbedtls_free(conf->key_cert);
		}
		SNI_throwNativeIOException(LLNET_SSL_TranslateReturnCode(ret), "Certificate parsing error");
		return;
	}

	/* Link the certificate in the keycert */
	conf->key_cert->cert = clicert;
}

void LLNET_SSL_CONTEXT_IMPL_setPrivateKey(int32_t contextID, uint8_t* privateKey, int32_t len, uint8_t* password, int32_t passwordLen)
{
	LLNET_SSL_DEBUG_TRACE("%s(context=%d)\n", __func__, contextID);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);
	mbedtls_pk_context *key;
	bool key_cert_allocated = false;

	/* Check parameters */
	if (NULL == conf || NULL == privateKey || 0 >= len || NULL == password || 0 >= passwordLen)
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return;
	}

	/* Allocate a new keycert if needed, otherwise free the existing private key */
	if (NULL == conf->key_cert)
	{
		conf->key_cert = mbedtls_calloc( 1, sizeof(mbedtls_ssl_key_cert));
		if (NULL == conf->key_cert)
		{
			SNI_throwNativeIOException(J_MEMORY_ERROR, "Not enough memory");
			return;
		}
		key_cert_allocated = true;
	}
	else
	{
		mbedtls_pk_free(conf->key_cert->key);
		mbedtls_free(conf->key_cert->key);
	}

	/* The LLNET_SSL_* interface does not support multiple keycert (only one private key can be set) */
	conf->key_cert->next = NULL;

	/* Allocate a new private key */
	key = mbedtls_calloc( 1, sizeof(mbedtls_pk_context));
	if (NULL == key)
	{
		if(key_cert_allocated)
		{
			mbedtls_free(conf->key_cert);
		}
		SNI_throwNativeIOException(J_MEMORY_ERROR, "Not enough memory");
		return;
	}

	/* Try to parse the private key */
	mbedtls_pk_init(key);
	int ret = mbedtls_pk_parse_key(key, privateKey, (uint32_t)len, password, (uint32_t)passwordLen);

	if (0 != ret)
	{
		LLNET_SSL_DEBUG_MBEDTLS_TRACE(__func__, ret);
		/* Free the private key */
		mbedtls_pk_free(key);
		mbedtls_free(key);
		if(key_cert_allocated)
		{
			mbedtls_free(conf->key_cert);
		}
		SNI_throwNativeIOException(LLNET_SSL_TranslateReturnCode(ret), "Private key parsing error");
		return;
	}

	/* Link the private key in the keycert */
	conf->key_cert->key = key;
}

int32_t LLNET_SSL_CONTEXT_IMPL_initChainBuffer(int32_t contextID, int32_t nbChainCerts, int32_t chainCertsTotalSize)
{
	LLNET_SSL_DEBUG_TRACE("%s(context=%d, nbChainCerts=%d, chainCertsTotalSize=%d)\n", __func__, contextID, nbChainCerts, chainCertsTotalSize);
	return 0;
}

void LLNET_SSL_CONTEXT_IMPL_addChainCertificate(int32_t contextID, uint8_t* cert, int32_t len, int32_t format, int32_t chainBufferSize)
{
	LLNET_SSL_DEBUG_TRACE("%s(context=%d)\n", __func__, contextID);

	(void)chainBufferSize;
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);
	int ret;

	/* Check parameters */
	if (NULL == conf || NULL == cert || 0 == len ||
		((CERT_DER_FORMAT != format) && (CERT_PEM_FORMAT != format)))
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return;
	}

	/* Try to parse the certificate, adding it to the chained list of certificated from keycert */
	if (CERT_DER_FORMAT == format)
	{
		ret = mbedtls_x509_crt_parse_der(conf->key_cert->cert, cert, (uint32_t)len);
		ret = LLNET_SSL_TranslateReturnCode(ret);
	}
	else
	{
		ret = LLNET_SSL_utils_mbedtls_x509_crt_parse(conf->key_cert->cert, cert, 0, len);
	}

	/* Check parse result */
	if (J_SSL_NO_ERROR != ret)
	{
		SNI_throwNativeIOException(LLNET_SSL_TranslateReturnCode(ret), "Certificate parsing error");
	}
}

void LLNET_SSL_CONTEXT_IMPL_freeContext(int32_t contextID)
{
	LLNET_SSL_DEBUG_TRACE("%s(context=%d)\n", __func__, contextID);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);

	if (NULL != conf)
	{
		if (NULL != conf->ca_chain)
		{
			void* chain_ptr = (void*)conf->ca_chain;
			mbedtls_ssl_conf_ca_chain(conf, NULL, NULL);
			mbedtls_x509_crt_free(chain_ptr);
			mbedtls_free(chain_ptr);
		}

		if (NULL != conf->p_vrfy)
		{
			void* vrfy_ptr = (void*)conf->p_vrfy;
			mbedtls_ssl_conf_verify(conf, NULL, NULL);
			mbedtls_free(vrfy_ptr);
		}

		if (NULL != conf->cert_profile)
		{
			void* profile_ptr = (void*)conf->cert_profile;
			mbedtls_ssl_conf_cert_profile(conf, NULL);
			mbedtls_free(profile_ptr);
		}

		if (NULL != conf->key_cert)
		{
			if (NULL != conf->key_cert->key)
			{
				mbedtls_pk_free(conf->key_cert->key);
				mbedtls_free(conf->key_cert->key);
			}

			if (NULL != conf->key_cert->cert)
			{
				mbedtls_x509_crt_free(conf->key_cert->cert);
				mbedtls_free(conf->key_cert->cert);
			}
		}

		mbedtls_ssl_config_free(conf);
		mbedtls_free((void*)conf);
	}
}
#ifdef __cplusplus
	}
#endif
