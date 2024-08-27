/*
 * C
 *
 * Copyright 2021-2023 MicroEJ Corp. All rights reserved.
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
#include <LLSEC_PUBLIC_KEY_impl.h>
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

/**
 * @brief Public pk context.
 * Received as input by the LLSEC_PUBLIC native functions, contains an initialized public key context
 * that will be used by the public pk context.
 */
static void* pub_pk_ctx;

static void *pub_ctx_alloc_func(void) {
    return pub_pk_ctx;
}

static void pub_ctx_free_func(void *ctx) {
    // nothing to do, context is received as input to the native function, so it must not be freed here
    LLSEC_UNUSED_PARAM(ctx);
}

/**
 * @brief get max size for encoded key.
 *
 * @param[in] native_id the C structure pointer holding the key data
 *
 * @return max encoded size for the public key
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PUBLIC_KEY_IMPL_get_encoded_max_size(int32_t native_id) {
    LLSEC_PUBLIC_KEY_DEBUG_TRACE("%s \n", __func__);

    LLSEC_pub_key* key = (LLSEC_pub_key*)native_id;

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
    info.ctx_alloc_func = pub_ctx_alloc_func;
    info.ctx_free_func = pub_ctx_free_func;

    pub_pk_ctx = (void*)key->key;

    mbedtls_rc = mbedtls_pk_setup(&pk, &info);
    if(LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
        (void)SNI_throwNativeException(mbedtls_rc, "Public key context setup failed");
    } else {
        char buf_local[LLSEC_PUBLIC_KEY_LOCAL_BUFFER_SIZE];
        /*
         * Write a public key to a SubjectPublicKeyInfo DER structure.
         * mbedtls_pk_write_pubkey_der() API will write data at the end of the buffer, not at the beginning, so instead of getting the encoded max size (mbedTLS v3.x has macros, mbedTLS v2.x doesn't), get the encoded fixed size.
         * LLSEC_PUBLIC_KEY_IMPL_get_encode() will then work straightforward with a fixed size and not a max size.
         */
        int length = mbedtls_pk_write_pubkey_der(&pk, (unsigned char*)(&buf_local), sizeof(buf_local));
        if (0 > length) {
            (void)SNI_throwNativeException(-length, "Encoded key max size get failed");
        } else {
            return_code = length;
        }
    }

    return return_code;
}

/**
 * @brief encode the public key.
 *
 * @param[in]  native_id                   the C structure pointer holding the key data
 * @param[out] output                      a byte array to hold the encoded key data
 * @param[in]  outputLength                the length of the output array
 *
 * @return the real size of the encoded key.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PUBLIC_KEY_IMPL_get_encode(int32_t native_id, uint8_t* output, int32_t outputLength) {
    LLSEC_PUBLIC_KEY_DEBUG_TRACE("%s \n", __func__);
    int return_code = LLSEC_ERROR;
    int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;

    LLSEC_pub_key* key = (LLSEC_pub_key*)native_id;
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
    info.ctx_alloc_func = pub_ctx_alloc_func;
    info.ctx_free_func = pub_ctx_free_func;

    pub_pk_ctx = (void*)key->key;

    mbedtls_rc = mbedtls_pk_setup(&pk, &info);
    if(LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
        (void)SNI_throwNativeException(mbedtls_rc, "Public key context setup failed");
    } else {
        /* Write a public key to a SubjectPublicKeyInfo DER structure */
        int length = mbedtls_pk_write_pubkey_der(&pk, output, outputLength);
        if (0 > length) {
            (void)SNI_throwNativeException(-length, "Public key encoding failed");
        } else {
            return_code = length;
        }
    }

    return return_code;
}

/**
 * @brief return the output size in bytes that an output buffer would need in order to hold the result of an encryption
 *        operation with this public key.
 *
 * @param[in] native_id the C structure pointer holding the key data
 *
 * @return the output size.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PUBLIC_KEY_IMPL_get_output_size(int32_t native_id) {
    LLSEC_PUBLIC_KEY_DEBUG_TRACE("%s \n", __func__);
    int return_code = LLSEC_ERROR;
    int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;

    LLSEC_pub_key* key = (LLSEC_pub_key*)native_id;
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
    info.ctx_alloc_func = pub_ctx_alloc_func;
    info.ctx_free_func = pub_ctx_free_func;

    pub_pk_ctx = (void*)key->key;

    mbedtls_rc = mbedtls_pk_setup(&pk, &info);
    if(LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
        (void)SNI_throwNativeException(mbedtls_rc, "Public key context setup failed");
    } else {
        return_code = mbedtls_pk_get_bitlen(&pk) / 8;
    }

    return return_code;
}
