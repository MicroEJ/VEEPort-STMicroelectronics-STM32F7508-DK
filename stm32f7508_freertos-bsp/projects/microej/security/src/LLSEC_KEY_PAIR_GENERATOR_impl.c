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

#include <LLSEC_mbedtls.h>

#include <LLSEC_ERRORS.h>
#include <LLSEC_KEY_PAIR_GENERATOR_impl.h>
#include <LLSEC_configuration.h>
#include <sni.h>
#include <string.h>

#include "mbedtls/version.h"
#include "mbedtls/platform.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/dhm.h"
#include "mbedtls/ecdh.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/entropy.h"
#include "mbedtls/rsa.h"

typedef void (*LLSEC_KEY_PAIR_GENERATOR_close)(void* native_id);

//RSA
static int32_t LLSEC_KEY_PAIR_GENERATOR_RSA_mbedtls_generateKeyPair(int32_t rsa_Key_size, int32_t rsa_public_exponent);

//EC
static int32_t LLSEC_KEY_PAIR_GENERATOR_EC_mbedtls_generateKeyPair(uint8_t* ec_curve_stdname);

//common
static void LLSEC_KEY_PAIR_GENERATOR_mbedtls_close(void* native_id);

typedef struct {
    char* name;
    LLSEC_KEY_PAIR_GENERATOR_close close;
} LLSEC_KEY_PAIR_GENERATOR_algorithm;

// cppcheck-suppress misra-c2012-8.9 // Define here for code readability even if it called once in this file.
static LLSEC_KEY_PAIR_GENERATOR_algorithm supportedAlgorithms[2] = {
    {
        .name  = "RSA",
        .close = LLSEC_KEY_PAIR_GENERATOR_mbedtls_close
    },
    {
        .name  = "EC",
        .close = LLSEC_KEY_PAIR_GENERATOR_mbedtls_close
    }

};

static int32_t LLSEC_KEY_PAIR_GENERATOR_RSA_mbedtls_generateKeyPair(int32_t rsa_Key_size, int32_t rsa_public_exponent) {
    LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s\n", __func__);
    int return_code = LLSEC_SUCCESS;
    int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;
    mbedtls_rsa_context* ctx = mbedtls_calloc(1, sizeof(mbedtls_rsa_context)); //RSA key structure
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char* pers = llsec_gen_random_str_internal(8);
    LLSEC_priv_key* key = NULL;
    void* native_id = NULL;

    mbedtls_entropy_init(&entropy);   //init entropy structure
    mbedtls_ctr_drbg_init(&ctr_drbg); //Initial random structure

    /* init rsa structure: padding PKCS#1 v1.5 */
#if (MBEDTLS_VERSION_MAJOR == 2)
    mbedtls_rsa_init(ctx, MBEDTLS_RSA_PKCS_V15, MBEDTLS_MD_NONE);
#elif (MBEDTLS_VERSION_MAJOR == 3)
    mbedtls_rsa_init(ctx);
#else
    #error "Unsupported mbedTLS major version"
#endif

    if (LLSEC_SUCCESS == return_code) {
        /* update seed according personal string */
        mbedtls_rc = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t*)pers, strlen(pers));
        if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
            LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s mbedtls_ctr_drbg_seed (rc = %d)\n", __func__, mbedtls_rc);
            mbedtls_rsa_free(ctx);
            return_code = LLSEC_ERROR;
        }
    }

    if (LLSEC_SUCCESS == return_code) {
        /*Generate ras key pair*/
        (void)mbedtls_rsa_gen_key(ctx, mbedtls_ctr_drbg_random, //API of generating random
                                &ctr_drbg,                    //random structure
                                rsa_Key_size,                 //the size of public key
                                rsa_public_exponent);         //publick key exponent 0x01001

        key = (LLSEC_priv_key*)mbedtls_calloc(1, sizeof(LLSEC_priv_key));
        if (NULL == key) {
            LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s mbedtls_calloc error\n", __func__);
            mbedtls_rsa_free(ctx);
            return_code = LLSEC_ERROR;
        }
    }

    if (LLSEC_SUCCESS == return_code) {
        key->key = (char*)ctx;
        key->type = TYPE_RSA;

        // Register the key to be managed by SNI as a native resource.
        // the close callback when be called when the key is collected by the GC
        // The key is freed in the close callback
        native_id = (void*)key;
        if (SNI_OK != SNI_registerResource(native_id, LLSEC_KEY_PAIR_GENERATOR_mbedtls_close, NULL)) {
            mbedtls_rsa_free(ctx);
            mbedtls_free(key);
            return_code = LLSEC_ERROR;
        }
    }

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    // cppcheck-suppress misra-c2012-11.8 // Cast for matching free function signature
    mbedtls_free((void*)pers);

    if (LLSEC_SUCCESS == return_code) {
        // cppcheck-suppress misra-c2012-11.6 // Abstract data type for SNI usage
        return_code = (uint32_t)native_id;
    } else {
        mbedtls_free(ctx);
    }

    return return_code;
}

static int32_t LLSEC_KEY_PAIR_GENERATOR_EC_mbedtls_generateKeyPair(uint8_t* ec_curve_stdname) {
    LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s\n", __func__);
    LLSEC_UNUSED_PARAM(ec_curve_stdname);

    int return_code = LLSEC_SUCCESS;
    int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;
    mbedtls_ecdsa_context* ctx = mbedtls_calloc(1, sizeof(mbedtls_ecdsa_context));
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char* pers = llsec_gen_random_str_internal(8);
    LLSEC_priv_key* key = NULL;
    void* native_id = NULL;

    mbedtls_ecdsa_init(ctx);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    mbedtls_rc = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t*)pers, strlen(pers));
    if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
        LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s \n", __func__);
        mbedtls_ecdsa_free(ctx);
        return_code = LLSEC_ERROR;
    }

    if (LLSEC_SUCCESS == return_code) {
        /* Generate ecdsa Key pair */
        mbedtls_rc = mbedtls_ecdsa_genkey(ctx, MBEDTLS_ECP_DP_SECP256R1, mbedtls_ctr_drbg_random, &ctr_drbg);
        if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
            LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s \n", __func__);
            mbedtls_ecdsa_free(ctx);
            return_code = LLSEC_ERROR;
        }
    }

    if (LLSEC_SUCCESS == return_code) {
        key = (LLSEC_priv_key*)mbedtls_calloc(1, sizeof(LLSEC_priv_key));
        if (NULL == key) {
            LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s \n", __func__);
            mbedtls_ecdsa_free(ctx);
            return_code = LLSEC_ERROR;
        }
    }

    if (LLSEC_SUCCESS == return_code) {
        key->key = (char*)ctx;
        key->type = TYPE_ECDSA;

        // Register the key to be managed by SNI as a native resource.
        // the close callback when be called when the key is collected by the GC
        // The key is freed in the close callback
        native_id = (void*)key;
        if (SNI_OK != SNI_registerResource(native_id, LLSEC_KEY_PAIR_GENERATOR_mbedtls_close, NULL)) {
            (void)SNI_throwNativeException(LLSEC_ERROR, "Can't register SNI native resource");
            mbedtls_ecdsa_free(ctx);
            mbedtls_free(key);
            return_code = LLSEC_ERROR;
        }
    }

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    // cppcheck-suppress misra-c2012-11.8 // Cast for matching free function signature
    mbedtls_free((void*)pers);

    if (LLSEC_SUCCESS == return_code) {
        // cppcheck-suppress misra-c2012-11.6 // Abstract data type for SNI usage
        return_code = (uint32_t)native_id;
    } else {
        mbedtls_free(ctx);
    }

    return return_code;
}

static void LLSEC_KEY_PAIR_GENERATOR_mbedtls_close(void* native_id) {
    LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s \n", __func__);

    LLSEC_priv_key* key = (LLSEC_priv_key*)native_id;

    if (TYPE_RSA == key->type) {
        mbedtls_rsa_free((mbedtls_rsa_context*)key->key);
    } else {
        mbedtls_ecdsa_free((mbedtls_ecdsa_context*)key->key);
    }
    mbedtls_free(key);
}

int32_t LLSEC_KEY_PAIR_GENERATOR_IMPL_get_algorithm(uint8_t* algorithm_name) {
    int32_t return_code = LLSEC_ERROR;
    LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s \n", __func__);

    int32_t nb_algorithms = sizeof(supportedAlgorithms) / sizeof(LLSEC_KEY_PAIR_GENERATOR_algorithm);
    LLSEC_KEY_PAIR_GENERATOR_algorithm* algorithm = &supportedAlgorithms[0];

    while (--nb_algorithms >= 0) {
        if (strcmp((char*)algorithm_name, algorithm->name) == 0) {
            break;
        }
        algorithm++;
    }

    if (0 <= nb_algorithms) {
        return_code = (int32_t)algorithm;
    }
    return return_code;
}

int32_t LLSEC_KEY_PAIR_GENERATOR_IMPL_generateKeyPair(int32_t algorithm_id, int32_t rsa_key_size, int32_t rsa_public_exponent, uint8_t* ec_curve_stdname) {
    int32_t return_code = LLSEC_SUCCESS;
    LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE("%s \n", __func__);

    LLSEC_KEY_PAIR_GENERATOR_algorithm* algorithm = (LLSEC_KEY_PAIR_GENERATOR_algorithm*)algorithm_id;
    if (0 == strcmp(algorithm->name, "RSA")) {
        return_code = LLSEC_KEY_PAIR_GENERATOR_RSA_mbedtls_generateKeyPair(rsa_key_size, rsa_public_exponent);
    } else if (0 == strcmp(algorithm->name, "EC")) {
        return_code = LLSEC_KEY_PAIR_GENERATOR_EC_mbedtls_generateKeyPair(ec_curve_stdname);
    } else{
        // Algorithm not found error.
        // this should never happen because the algorithm_id is a valid algorithm at this level.
        (void)SNI_throwNativeException(LLSEC_ERROR, "Unsupported algorithm");
        return_code = LLSEC_ERROR;
    }
    return return_code;
}

int32_t LLSEC_KEY_PAIR_GENERATOR_IMPL_get_close_id(int32_t algorithm_id) {
    LLSEC_KEY_PAIR_GENERATOR_algorithm* algorithm = (LLSEC_KEY_PAIR_GENERATOR_algorithm*)algorithm_id;
    // cppcheck-suppress misra-c2012-11.1 // Abstract data type for SNI usage
    return (int32_t)algorithm->close;
}
