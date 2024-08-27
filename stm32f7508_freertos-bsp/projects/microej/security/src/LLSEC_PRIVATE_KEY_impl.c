/*
 * C
 *
 * Copyright 2021-2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief MicroEJ Security low level API implementation for MbedTLS Library.
 * @author MicroEJ Developer Team
 * @version 1.5.0
 * @date 15 March 2024
 */

#include <LLSEC_ERRORS.h>
#include <LLSEC_PRIVATE_KEY_impl.h>
#include <LLSEC_mbedtls.h>
#include <LLSEC_configuration.h>

#include <sni.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mbedtls/version.h"
#if (MBEDTLS_VERSION_MAJOR == 2)
#include "mbedtls/pk_internal.h"
#elif (MBEDTLS_VERSION_MAJOR == 3)
#include "pk_wrap.h"
#else
#error "Unsupported mbedTLS major version"
#endif
#include "mbedtls/platform.h"
#include "mbedtls/pk.h"
#include "mbedtls/asn1write.h"
#include "mbedtls/oid.h"

/**
 * @brief Private pk context.
 * Received as input by the LLSEC_PRIVATE native functions, contains an initialized private key context
 * that will be used by the private pk context.
 */
static void* priv_pk_ctx;

static void *priv_ctx_alloc_func(void) {
    return priv_pk_ctx;
}

static void priv_ctx_free_func(void *ctx) {
    // nothing to do, context is received as input to the native function, so it must not be freed here
    LLSEC_UNUSED_PARAM(ctx);
}

static int pk_write_key_pkcs8_der(mbedtls_pk_context *key, unsigned char *buf, size_t size) {
    mbedtls_pk_type_t pk_type;
    size_t par_len = 0;
    unsigned char *p;
    const char *oid;
    size_t oid_len;
    int len;
    int ret;

    /* Write the private key to a PKCS#1 or SEC1 DER structure */
    ret = mbedtls_pk_write_key_der(key, buf, size);
    if (ret < 0) {
        return ret;
    }

    len = ret;
    p = buf + size - len;

    /* Wrap PKCS#1 or SEC1 DER in OCTET STRING */
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_len(&p, buf, len));
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_tag(&p, buf, MBEDTLS_ASN1_OCTET_STRING));

    pk_type = mbedtls_pk_get_type(key);
    ret = mbedtls_oid_get_oid_by_pk_alg(pk_type, &oid, &oid_len);
    if (ret != 0) {
        return ret;
    }

    /* attributes */
    if (pk_type == MBEDTLS_PK_ECKEY) {
        const char *local_oid;
        size_t local_oid_len;

        // mbedtls_ecdsa_genkey uses fixed mbedtls_ecp_group_id
        ret = mbedtls_oid_get_oid_by_ec_grp(MBEDTLS_ECP_DP_SECP256R1, &local_oid, &local_oid_len);
        if (ret != 0) {
            return ret;
        }

        MBEDTLS_ASN1_CHK_ADD(par_len, mbedtls_asn1_write_oid(&p, buf, local_oid, local_oid_len));
    }

    /* privateKeyAlgorithm */
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_algorithm_identifier(&p, buf, oid, oid_len, par_len));

    /* version */
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_int(&p, buf, 0));

    /* sequence and length */
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_len(&p, buf, len));
    MBEDTLS_ASN1_CHK_ADD(len, mbedtls_asn1_write_tag(&p, buf, MBEDTLS_ASN1_CONSTRUCTED | MBEDTLS_ASN1_SEQUENCE));

    return len;
}

/**
 * @brief return the max size of the encoded key.
 *
 * @param[in] native_id the C structure pointer holding the key data
 *
 * @return max encoded size for the private key in DER format
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PRIVATE_KEY_IMPL_get_encoded_max_size(int32_t native_id) {
    LLSEC_PRIVATE_KEY_DEBUG_TRACE("%s \n", __func__);

    LLSEC_priv_key* key = (LLSEC_priv_key*)native_id;

    int return_code = LLSEC_ERROR;
    int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;

    mbedtls_pk_context pk;
    mbedtls_pk_type_t pk_type;

    if (TYPE_RSA == key->type) {
        pk_type = MBEDTLS_PK_RSA;
    } else {
        pk_type = MBEDTLS_PK_ECKEY;
    }

    mbedtls_pk_init(&pk);

    mbedtls_pk_info_t info;
    (void)memcpy(&info, mbedtls_pk_info_from_type(pk_type), sizeof(mbedtls_pk_info_t));
    info.ctx_alloc_func = priv_ctx_alloc_func;
    info.ctx_free_func = priv_ctx_free_func;

    priv_pk_ctx = (void*)key->key;

    mbedtls_rc = mbedtls_pk_setup(&pk, &info);
    if(LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
        (void)SNI_throwNativeException(mbedtls_rc, "Private key context setup failed");
    } else {
        char buf_local[LLSEC_PRIVATE_KEY_LOCAL_BUFFER_SIZE];
        /*
         * Write a private key to a PKCS#8 DER structure.
         * pk_write_key_pkcs8_der() API will write data at the end of the buffer, not at the beginning, so instead of getting the encoded max size (mbedTLS v3.x has macros, mbedTLS v2.x doesn't), get the encoded fixed size.
         * LLSEC_PRIVATE_KEY_IMPL_get_encode() will then work straightforward with a fixed size and not a max size.
         */
        int length = pk_write_key_pkcs8_der(&pk, (unsigned char*)(&buf_local), sizeof(buf_local));
        if (0 > length) {
            (void)SNI_throwNativeException(-length, "Encoded key max size get failed");
        } else {
            return_code = length;
        }
    }

    return return_code;
}

/**
 * @brief encode the private key into DER format.
 *
 * @param[in]  native_id                   the C structure pointer holding the key data
 * @param[out] output                      a byte array to hold the encoded key data
 * @pram[in]   outputLength                the length of the output array
 *
 * @return the reel size of the encoded key.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PRIVATE_KEY_IMPL_get_encode(int32_t native_id, uint8_t* output, int32_t outputLength) {
    LLSEC_PRIVATE_KEY_DEBUG_TRACE("%s \n", __func__);
    int return_code = LLSEC_ERROR;
    int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;

    LLSEC_priv_key* key = (LLSEC_priv_key*)native_id;
    mbedtls_pk_context pk;
    mbedtls_pk_type_t pk_type;

    if (TYPE_RSA == key->type) {
        pk_type = MBEDTLS_PK_RSA;
    } else {
        pk_type = MBEDTLS_PK_ECKEY;
    }

    mbedtls_pk_init(&pk);

    mbedtls_pk_info_t info;
    (void)memcpy(&info, mbedtls_pk_info_from_type(pk_type), sizeof(mbedtls_pk_info_t));
    info.ctx_alloc_func = priv_ctx_alloc_func;
    info.ctx_free_func = priv_ctx_free_func;

    priv_pk_ctx = (void*)key->key;

    mbedtls_rc = mbedtls_pk_setup(&pk, &info);
    if(LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
        (void)SNI_throwNativeException(mbedtls_rc, "Private key context setup failed");
    } else {
        /* Write a private key to a PKCS#8 DER structure */
        int length = pk_write_key_pkcs8_der(&pk, output, outputLength);
        if (0 > length) {
            (void)SNI_throwNativeException(-length, "Private key encoding failed");
        } else {
            return_code = length;
        }
    }

    return return_code;
}

/**
 * @brief return the output size in bytes that an output buffer would need in order to hold the result of a decryption
 *        operation with this private key.
 *
 * @param[in] native_id the C structure pointer holding the key data
 *
 * @return the output size.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PRIVATE_KEY_IMPL_get_output_size(int32_t native_id) {
    LLSEC_PRIVATE_KEY_DEBUG_TRACE("%s \n", __func__);
    int return_code = LLSEC_ERROR;
    int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;

    LLSEC_priv_key* key = (LLSEC_priv_key*)native_id;
    mbedtls_pk_context pk;
    mbedtls_pk_type_t pk_type;

    if (TYPE_RSA == key->type) {
        pk_type = MBEDTLS_PK_RSA;
    } else {
        pk_type = MBEDTLS_PK_ECKEY;
    }

    mbedtls_pk_init(&pk);

    mbedtls_pk_info_t info;
    (void)memcpy(&info, mbedtls_pk_info_from_type(pk_type), sizeof(mbedtls_pk_info_t));
    info.ctx_alloc_func = priv_ctx_alloc_func;
    info.ctx_free_func = priv_ctx_free_func;

    priv_pk_ctx = (void*)key->key;

    mbedtls_rc = mbedtls_pk_setup(&pk, &info);
    if(LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
        (void)SNI_throwNativeException(mbedtls_rc, "Private key context setup failed");
    } else {
        return_code = mbedtls_pk_get_bitlen(&pk) / 8;
    }

    return return_code;
}
