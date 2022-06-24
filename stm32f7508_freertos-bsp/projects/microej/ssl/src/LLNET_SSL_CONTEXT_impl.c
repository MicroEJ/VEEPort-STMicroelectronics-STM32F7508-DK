/*
 * C
 *
 * Copyright 2018-2020 IS2T. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break IS2T warranties on the whole library.
 */
#include <LLNET_SSL_CONTEXT_impl.h>
#include <LLNET_SSL_CONSTANTS.h>
#include <net_ssl_errors.h>
#include <LLNET_SSL_utils_mbedtls.h>
#include "LLNET_SSL_verifyCallback.h"
#include "ssl_parse.h"
#include <stdlib.h>

#include "mbedtls/debug.h"
#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_internal.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/platform.h"

/* Check that the custom Mbedtls configuration file (MBEDTLS_CONFIG_FILE) is defined by the build definition*/
#ifndef MBEDTLS_CONFIG_FILE
#error "Custom configuration file (MBEDTLS_CONFIG_FILE) should be defined by the build definition"
#endif

#ifdef __cplusplus
	extern "C" {
#endif

/* ----------- external function and variables ----------- */
extern mbedtls_ctr_drbg_context ctr_drbg;

/* ----------- Private API  -----------*/
#if MBEDTLS_DEBUG_LEVEL > 0
/**
 * Debug callback for mbed TLS
 * Just prints on the USB serial port
 */
void my_debug(void *ctx, int level, const char *file, int line,
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

int32_t LLNET_SSL_CONTEXT_IMPL_createContext(int32_t protocol, uint8_t isClientContext, uint8_t retry){
	LLNET_SSL_DEBUG_TRACE("%s(protocol=%d, isClientContext=%d, retry=%d)\n", __func__, (int)protocol, isClientContext, retry);

	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)pvPortMalloc(sizeof(mbedtls_ssl_config));

	if (conf != NULL)
	{
		mbedtls_ssl_config_init(conf);

		int endpoint;
		if (isClientContext)
			endpoint = MBEDTLS_SSL_IS_CLIENT;
		else
			endpoint = MBEDTLS_SSL_IS_SERVER;

		int ret;
		if ((ret = mbedtls_ssl_config_defaults(conf,
						endpoint,
						MBEDTLS_SSL_TRANSPORT_STREAM,
						MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
#ifndef LLNET_SSL_DEBUG
			(void)ret;
#endif
			LLNET_SSL_DEBUG_MBEDTLS_TRACE("mbedtls_ssl_config_defaults", ret);
		}

		switch (protocol) {
#ifdef MBEDTLS_SSL_PROTO_SSL3
			case SSLv3_PROTOCOL:
				mbedtls_ssl_conf_min_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_0 );
				mbedtls_ssl_conf_max_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_0 );
				break;
#endif // MBEDTLS_SSL_PROTO_SSL3

#ifdef MBEDTLS_SSL_PROTO_TLS1
			case TLSv1_PROTOCOL:
				mbedtls_ssl_conf_min_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_1 );
				mbedtls_ssl_conf_max_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_1 );
				break;
#endif // MBEDTLS_SSL_PROTO_TLS1

#ifdef MBEDTLS_SSL_PROTO_TLS1_1
			case TLSv1_1_PROTOCOL:
				mbedtls_ssl_conf_min_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_2 );
				mbedtls_ssl_conf_max_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_2 );
				break;
#endif //MBEDTLS_SSL_PROTO_TLS1_1

#ifdef MBEDTLS_SSL_PROTO_TLS1_2
			case TLSv1_2_PROTOCOL:
				mbedtls_ssl_conf_min_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3 );
				mbedtls_ssl_conf_max_version(conf, MBEDTLS_SSL_MAJOR_VERSION_3, MBEDTLS_SSL_MINOR_VERSION_3 );
				break;
#endif // MBEDTLS_SSL_PROTO_TLS1_2
			default:
				break;
		}

		mbedtls_ssl_conf_rng(conf, mbedtls_ctr_drbg_random, &ctr_drbg);

		/* It is possible to disable authentication by passing
		 * MBEDTLS_SSL_VERIFY_NONE in the call to mbedtls_ssl_conf_authmode()
		 */
		mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_UNSET);

		/*
		 * Change certificate verification profile, allocate and initialize new profile
		 */
		mbedtls_x509_crt_profile* crt_profile = (mbedtls_x509_crt_profile*)pvPortMalloc(sizeof(mbedtls_x509_crt_profile));
		if (crt_profile == NULL)
			return J_CREATE_SSL_CONTEXT_ERROR;
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
		cert_verify_ctx* verify_ctx = (cert_verify_ctx*)pvPortMalloc(sizeof(cert_verify_ctx));
		if (verify_ctx == NULL)
			return J_CREATE_SSL_CONTEXT_ERROR;

        verify_ctx->conf = conf;
        verify_ctx->isUnTrustCA = 0;

		mbedtls_ssl_conf_verify(conf, LLNET_SSL_VERIFY_verifyCallback, (void*)verify_ctx);

#if MBEDTLS_DEBUG_LEVEL > 0
			mbedtls_ssl_conf_dbg(conf, my_debug, NULL);
#if defined(MBEDTLS_DEBUG_C)
			mbedtls_debug_set_threshold(MBEDTLS_DEBUG_LEVEL);
#endif
#endif

		LLNET_SSL_DEBUG_TRACE("%s(method=%d) return ctx=%p\n", __func__, protocol,conf);

		return (int32_t)conf;
	}

	return J_CREATE_SSL_CONTEXT_ERROR; //error
}

int32_t LLNET_SSL_CONTEXT_IMPL_addTrustedCert(int32_t contextID, uint8_t *cert, int32_t off, int32_t len, int32_t format, uint8_t retry){

	LLNET_SSL_DEBUG_TRACE("%s(context=%d, retry=%d)\n", __func__, (int)contextID, retry);

	int32_t ret = J_SSL_NO_ERROR;

	/* Check parameters */
	if (((uint8_t *)(cert + off) == NULL) || (len == 0) ||
		((format != CERT_DER_FORMAT) && (format != CERT_PEM_FORMAT)))
	{
		return J_BAD_FUNC_ARG;
	}

	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);
	mbedtls_x509_crt* cacert = NULL;
	if (conf->ca_chain == NULL)
	{
		cacert = (mbedtls_x509_crt*)pvPortMalloc(sizeof(mbedtls_x509_crt));
		mbedtls_x509_crt_init(cacert);
	}
	else
	{
		cacert = conf->ca_chain;
	}

	if (conf != NULL && cacert != NULL)
	{
		int ret;

		if (format == CERT_DER_FORMAT) {
			ret = mbedtls_x509_crt_parse_der(cacert, (const unsigned char *) (cert + off), len);
		} else {
			ret = microej_x509_crt_parse(cacert, cert, off, len);
		}

		if (ret != 0) {
			return NET_SSL_TranslateReturnCode(ret);
		}

		if (conf->ca_chain == NULL){
			mbedtls_ssl_conf_ca_chain(conf, cacert, NULL);
		}
	}
	else
	{
		ret = J_CREATE_SSL_CONTEXT_ERROR;
	}

    return ret;
}


int32_t LLNET_SSL_CONTEXT_IMPL_clearTrustStore(int32_t contextID, uint8_t retry){

	LLNET_SSL_DEBUG_TRACE("%s(context=%d, retry=%d)\n", __func__, (int)contextID, retry);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);

	if (conf != NULL)
	{
		if (conf->ca_chain != NULL)
		{
			void* chain_ptr = (void*)conf->ca_chain;
			mbedtls_ssl_conf_ca_chain(conf, NULL, NULL);
			mbedtls_x509_crt_free(chain_ptr);
			vPortFree(chain_ptr);
		}
	}

	return J_SSL_NO_ERROR;
}

int32_t LLNET_SSL_CONTEXT_IMPL_clearKeyStore(int32_t contextID, uint8_t retry){
	LLNET_SSL_DEBUG_TRACE("%s(context=%d, retry=%d)\n", __func__, (int)contextID, retry);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);

	/* Free the private key */
	if (conf != NULL)
	{
		if (conf->key_cert != NULL)
		{
			if (conf->key_cert->key != NULL)
			{
				mbedtls_pk_free(conf->key_cert->key);
				mbedtls_free(conf->key_cert->key);
			}
		}
	}

	return J_SSL_NO_ERROR;
}

int32_t LLNET_SSL_CONTEXT_IMPL_setCertificate(int32_t contextID, uint8_t* cert, int32_t offset, int32_t len, int32_t format, uint8_t retry){
	LLNET_SSL_DEBUG_TRACE("%s(context=%d, retry=%d)\n", __func__, (int)contextID, retry);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);
	mbedtls_x509_crt *clicert = NULL;
	int ret;

	/* Check parameters */
	if (((uint8_t *)((int32_t)cert + offset) == NULL) || (len == 0) ||
		((format != CERT_DER_FORMAT) && (format != CERT_PEM_FORMAT))) {
		return J_BAD_FUNC_ARG;
	}

	/* Allocate a new keycert if needed, otherwise free the existing certificate */
	if (conf->key_cert == NULL) {
		conf->key_cert = mbedtls_calloc( 1, sizeof( mbedtls_ssl_key_cert ) );
	} else {
		mbedtls_x509_crt_free(conf->key_cert->cert);
		mbedtls_free(conf->key_cert->cert);
	}

	/* The LLNET_SSL_* interface does not support multiple keycert (only one private key can be set) */
	conf->key_cert->next = NULL;

	/* Allocate a new certificate */
	clicert = mbedtls_calloc( 1, sizeof( mbedtls_x509_crt ) );

	/* Try to parse the certificate */
	mbedtls_x509_crt_init(clicert);
	if (format == CERT_DER_FORMAT) {
		ret = mbedtls_x509_crt_parse_der(clicert, (uint8_t *) ((int32_t)cert + offset), (uint32_t) len);
	} else {
			ret = microej_x509_crt_parse(clicert, cert, offset, len);
	}

	/* Check parse result */
	if (ret != 0) {
		/* Free the certificate */
		mbedtls_x509_crt_free(clicert);
		mbedtls_free(clicert);

		return J_CERT_PARSE_ERROR;
	}

	/* Link the certificate in the keycert */
	conf->key_cert->cert = clicert;

	return J_SSL_NO_ERROR;
}

int32_t LLNET_SSL_CONTEXT_IMPL_setPrivateKey(int32_t contextID, uint8_t* privateKey, int32_t offset, int32_t len, uint8_t* password,
		int32_t passwordOffset, int32_t passwordLen, uint8_t retry){
	LLNET_SSL_DEBUG_TRACE("%s(context=%d, retry=%d)\n", __func__, (int)contextID, retry);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);
	mbedtls_pk_context *key;
	uint8_t * pwd = NULL;

	/* Check parameters */
	if (((uint8_t *)((int32_t)privateKey + offset) == NULL) || (len == 0)) {
		return J_BAD_FUNC_ARG;
	}

	/* Check if the password has to be used for decryption */
	if (passwordLen > 0) {
		pwd = (uint8_t *) ((int32_t)password + passwordOffset);
	}

	/* Allocate a new keycert if needed, otherwise free the existing private key */
	if (conf->key_cert == NULL) {
		conf->key_cert = mbedtls_calloc( 1, sizeof( mbedtls_ssl_key_cert ) );
	} else {
		mbedtls_pk_free(conf->key_cert->key);
		mbedtls_free(conf->key_cert->key);
	}

	/* The LLNET_SSL_* interface does not support multiple keycert (only one private key can be set) */
	conf->key_cert->next = NULL;

	/* Allocate a new private key */
	key = mbedtls_calloc( 1, sizeof( mbedtls_pk_context ) );

	/* Try to parse the private key */
	mbedtls_pk_init(key);
	int mbed_err = mbedtls_pk_parse_key(key,
			(uint8_t *) ((int32_t)privateKey + offset), (uint32_t) len,
			pwd, (uint32_t) passwordLen);
	if (mbed_err != 0) {

		print_mbedtls_error(__func__, mbed_err);
		/* Free the private key */
		mbedtls_pk_free(key);
		mbedtls_free(key);

		return J_NO_PRIVATE_KEY;
	}

	/* Link the private key in the keycert */
	conf->key_cert->key = key;

	return J_SSL_NO_ERROR;
}

int32_t LLNET_SSL_CONTEXT_IMPL_initChainBuffer(int32_t contextID, int32_t nbChainCerts, int32_t chainCertsTotalSize, uint8_t retry){
	LLNET_SSL_DEBUG_TRACE("%s(context=%d, nbChainCerts=%d, chainCertsTotalSize=%d, retry=%d)\n", __func__, (int)contextID, (int)nbChainCerts, (int)chainCertsTotalSize, retry);

	return J_SSL_NO_ERROR;
}

int32_t LLNET_SSL_CONTEXT_IMPL_addChainCertificate(int32_t contextID, uint8_t* cert, int32_t offset, int32_t len, int32_t format, int32_t chainBufferSize, uint8_t retry){
	LLNET_SSL_DEBUG_TRACE("%s(context=%d, retry=%d)\n", __func__, (int)contextID, retry);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);
	int ret;

	/* Check parameters */
	if (((uint8_t *)((int32_t)cert + offset) == NULL) || (len == 0) ||
		((format != CERT_DER_FORMAT) && (format != CERT_PEM_FORMAT))) {
		return J_BAD_FUNC_ARG;
	}

	/* Try to parse the certificate, adding it to the chained list of certificated from keycert */
	if (format == CERT_DER_FORMAT) {
		ret = mbedtls_x509_crt_parse_der(conf->key_cert->cert, (uint8_t *) ((int32_t)cert + offset), (uint32_t) len);
	} else {
		ret = microej_x509_crt_parse(conf->key_cert->cert, cert, offset, len);
	}

	/* Check parse result */
	if (ret != 0) {
		/* Free the certificate */
		mbedtls_x509_crt_free(conf->key_cert->cert);

		return J_CERT_PARSE_ERROR;
	}

	return J_SSL_NO_ERROR;
}

int32_t LLNET_SSL_CONTEXT_IMPL_closeContext(int32_t contextID, uint8_t retry){
	LLNET_SSL_DEBUG_TRACE("%s(context=%d, retry=%d)\n", __func__, (int)contextID, retry);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);

	if (conf != NULL)
	{
		if (conf->ca_chain != NULL)
		{
			void* chain_ptr = (void*)conf->ca_chain;
			mbedtls_ssl_conf_ca_chain(conf, NULL, NULL);
			mbedtls_x509_crt_free(chain_ptr);
			vPortFree(chain_ptr);
		}

		if (conf->p_vrfy != NULL)
		{
			void* vrfy_ptr = (void*)conf->p_vrfy;
			mbedtls_ssl_conf_verify(conf, NULL, NULL);
			vPortFree(vrfy_ptr);
		}

		if (conf->cert_profile != NULL)
		{
			void* profile_ptr = (void*)conf->cert_profile;
			mbedtls_ssl_conf_cert_profile(conf, NULL);
			vPortFree(profile_ptr);
		}

		if (conf->key_cert != NULL)
		{
			if (conf->key_cert->key != NULL)
			{
				mbedtls_pk_free(conf->key_cert->key);
				mbedtls_free(conf->key_cert->key);
			}

			if (conf->key_cert->cert != NULL)
			{
				mbedtls_x509_crt_free(conf->key_cert->cert);
				mbedtls_free(conf->key_cert->cert);
			}
		}

		mbedtls_ssl_config_free(conf);
		vPortFree((void*)conf);
	}

	return J_SSL_NO_ERROR;
}
#ifdef __cplusplus
	}
#endif
