/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief MicroEJ Security low level API
 * @author MicroEJ Developer Team
 * @version 1.5.0
 * @date 15 March 2024
 */

#include <LLSEC_SECRET_KEY_FACTORY_impl.h>
#include <LLSEC_configuration.h>
#include <LLSEC_mbedtls.h>
#include <string.h>
#include "mbedtls/platform.h"
#include "mbedtls/pkcs5.h"

typedef int32_t (*LLSEC_SECRET_KEY_FACTORY_get_key_data)(LLSEC_secret_key* secret_key, mbedtls_md_type_t md_type, uint8_t* password, int32_t password_length, uint8_t* salt, int32_t salt_length, int32_t iterations, int32_t key_length);
typedef void    (*LLSEC_SECRET_KEY_FACTORY_key_close)(void* native_id);

int32_t     LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_get_key_data(LLSEC_secret_key* secret_key, mbedtls_md_type_t md_type, uint8_t* password, int32_t password_length, uint8_t* salt, int32_t salt_length, int32_t iterations, int32_t key_length);
static void LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_key_close(void* native_id);
void        LLSEC_SECRET_KEY_FACTORY_mbedtls_free_secret_key(LLSEC_secret_key* secret_key);

typedef struct {
    const char*                           name;
    mbedtls_md_type_t                     md_type;
    LLSEC_SECRET_KEY_FACTORY_get_key_data get_key_data;
    LLSEC_SECRET_KEY_FACTORY_key_close    key_close;

} LLSEC_SECRET_KEY_FACTORY_IMPL_algorithm;

// cppcheck-suppress misra-c2012-8.9 // Define here for code readability even if it called once in this file.
static LLSEC_SECRET_KEY_FACTORY_IMPL_algorithm available_algorithms[5] = {
    {
        .name         = "PBKDF2WithHmacSHA1",
        .md_type      = MBEDTLS_MD_SHA1,
        .get_key_data = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_get_key_data,
        .key_close    = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_key_close
    },
    {
        .name         = "PBKDF2WithHmacSHA224",
        .md_type      = MBEDTLS_MD_SHA224,
        .get_key_data = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_get_key_data,
        .key_close    = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_key_close
    },
    {
        .name         = "PBKDF2WithHmacSHA256",
        .md_type      = MBEDTLS_MD_SHA256,
        .get_key_data = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_get_key_data,
        .key_close    = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_key_close
    },
    {
        .name         = "PBKDF2WithHmacSHA384",
        .md_type      = MBEDTLS_MD_SHA384,
        .get_key_data = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_get_key_data,
        .key_close    = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_key_close
    },
    {
        .name         = "PBKDF2WithHmacSHA512",
        .md_type      = MBEDTLS_MD_SHA512,
        .get_key_data = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_get_key_data,
        .key_close    = LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_key_close
    }
};

void LLSEC_SECRET_KEY_FACTORY_mbedtls_free_secret_key(LLSEC_secret_key* secret_key) {
    if(NULL != secret_key->key) {
        mbedtls_free(secret_key->key);
    }
    if(NULL != secret_key) {
        mbedtls_free(secret_key);
    }
}

int32_t LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_get_key_data(LLSEC_secret_key* secret_key, mbedtls_md_type_t md_type, uint8_t* password, int32_t password_length, uint8_t* salt, int32_t salt_length, int32_t iterations, int32_t key_length) {
    LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s \n", __func__);

    int32_t return_code = LLSEC_SUCCESS;
    mbedtls_md_context_t md_ctx;
    const mbedtls_md_info_t *md_info = NULL;

    /* Allocate resources */
    secret_key->key = mbedtls_calloc(1, key_length);
    if (NULL == secret_key->key) {
        (void)SNI_throwNativeException(LLSEC_ERROR, "mbedtls_calloc() failed");
        return_code = LLSEC_ERROR;
    }

    /* Initialize HMAC context */
    if (LLSEC_SUCCESS == return_code) {
        mbedtls_md_init(&md_ctx);
        md_info = mbedtls_md_info_from_type(md_type);
        if (NULL == md_info) {
            (void)SNI_throwNativeException(LLSEC_ERROR, "mbedtls_md_info_from_type() failed");
            return_code = LLSEC_ERROR;
        }
    }

    /* Setup HMAC context */
    if (LLSEC_SUCCESS == return_code) {
        int mbedtls_rc = mbedtls_md_setup(&md_ctx, md_info, 1);
        if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
            LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s mbedtls_md_setup() failed (rc = %d)\n", __func__, mbedtls_rc);
            (void)SNI_throwNativeException(LLSEC_ERROR, "mbedtls_md_setup() failed");
            return_code = LLSEC_ERROR;
        }
    }

    /* PKCS#5 PBKDF2 using HMAC */
    if (LLSEC_SUCCESS == return_code) {
        secret_key->key_length = key_length;
        int mbedtls_rc = mbedtls_pkcs5_pbkdf2_hmac((mbedtls_md_context_t *)&md_ctx,
                                                   (const unsigned char *)password,
                                                   (size_t)password_length,
                                                   (const unsigned char *)salt,
                                                   (size_t)salt_length,
                                                   (unsigned int)iterations,
                                                   (uint32_t)secret_key->key_length,
                                                   (unsigned char *)secret_key->key);
        if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
            LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s mbedtls_pkcs5_pbkdf2_hmac() failed (rc = %d)\n", __func__, mbedtls_rc);
            (void)SNI_throwNativeException(mbedtls_rc, "mbedtls_pkcs5_pbkdf2_hmac() failed");
            return_code = LLSEC_ERROR;
        }
    }

    /* Release HMAC context */
    mbedtls_md_free(&md_ctx);

    /* Register SNI close callback */
    if (LLSEC_SUCCESS == return_code) {
        if (SNI_OK != SNI_registerResource((void* )secret_key, (SNI_closeFunction)LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_key_close, NULL)) {
            (void)SNI_throwNativeException(LLSEC_ERROR, "Can't register SNI native resource");
            return_code = LLSEC_ERROR;
        }
    }

    /* Return key struct addr (native_id) */
    if (LLSEC_SUCCESS == return_code) {
        return_code = (int32_t)secret_key;
        LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s mbedtls_pkcs5_pbkdf2_hmac() success. (native_id = %d)\n", __func__, (int)return_code);
    } else {
        LLSEC_SECRET_KEY_FACTORY_mbedtls_free_secret_key(secret_key);
    }

    return return_code;
}

static void LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_key_close(void* native_id) {
    LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s (native_id = %p)\n", __func__, native_id);
    LLSEC_secret_key* secret_key = (LLSEC_secret_key*)native_id;

    /* Release resources */
    LLSEC_SECRET_KEY_FACTORY_mbedtls_free_secret_key(secret_key);

    /* Unregister SNI close callback */
    if (SNI_OK != SNI_unregisterResource((void*)native_id, (SNI_closeFunction)LLSEC_SECRET_KEY_FACTORY_PBKDF2_mbedtls_key_close)) {
        (void)SNI_throwNativeException(LLSEC_ERROR, "Can't unregister SNI native resource");
    }
}

/**
 * @brief Get the supported algorithm native ID.
 *
 * @param[in] algorithm_name        Null terminated string that describes the algorithm.
 *
 * @return The algorithm ID on success or -1 on error.
 */
int32_t LLSEC_SECRET_KEY_FACTORY_IMPL_get_algorithm(uint8_t* algorithm_name) {
    LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s \n", __func__);
    int32_t return_code = LLSEC_ERROR;
    int32_t nb_algorithms = sizeof(available_algorithms) / sizeof(LLSEC_SECRET_KEY_FACTORY_IMPL_algorithm);
    LLSEC_SECRET_KEY_FACTORY_IMPL_algorithm* algorithm = &available_algorithms[0];

    /* Check corresponding algorithm */
    while (--nb_algorithms >= 0) {
        if (strcmp((char*)algorithm_name, algorithm->name) == 0) {
            LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s Algorithm %s found\n", __func__, algorithm->name);
            break;
        }
        algorithm++;
    }

    if (0 <= nb_algorithms) {
        return_code = (int32_t)algorithm;
    }
    LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s Return handler = %d\n", __func__, (int)return_code);

    return return_code;
}

/**
 * @brief Generate a secret key from the encoded key format.
 *
 * @param[in] algorithm_id    algorithm pointer
 * @param[in] password        the password to encode, a null terminated '\0' byte array representation of the format in String
 * @param[in] password_length the length of password (in bytes)
 * @param[in] salt            salt
 * @param[in] salt_length     salt length (in bytes)
 * @param[in] iterations      number of iterations
 * @param[in] key_length      the length of encodedKey to generate (in bits)
 *
 * @return the pointer of the C structure holding the key data
 *
 * @throws NativeException on error
 */
int32_t LLSEC_SECRET_KEY_FACTORY_IMPL_get_key_data(int32_t algorithm_id, uint8_t* password, int32_t password_length, uint8_t* salt, int32_t salt_length, int32_t iterations, int32_t key_length) {
    LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s password length = %d, salt length = %d, key length = %d (handler = %d)\n", __func__, (int)password_length, (int)salt_length, (int)key_length, (int)algorithm_id);
    int32_t return_code = LLSEC_ERROR;

    /* Allocate secret key structure */
    LLSEC_secret_key* secret_key = (LLSEC_secret_key*)mbedtls_calloc(1, sizeof(LLSEC_secret_key));
    if (NULL == secret_key) {
        (void)SNI_throwNativeException(LLSEC_ERROR, "Can't allocate LLSEC_secret_key structure");
    } else {
        LLSEC_SECRET_KEY_FACTORY_IMPL_algorithm* algorithm = (LLSEC_SECRET_KEY_FACTORY_IMPL_algorithm*)algorithm_id;
        return_code = algorithm->get_key_data(secret_key, algorithm->md_type, password, password_length, salt, salt_length, iterations, key_length/8);
    }

    return return_code;
}

/**
 * Gets the id of the native close function.
 *
 * @param [in] nativeAlgorithmId    the algorithm ID
 *
 * @return the id of the static native close function
 * @throws NativeException on error
 */
int32_t LLSEC_SECRET_KEY_FACTORY_IMPL_get_close_id(int32_t algorithm_id) {
    LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE("%s (handler = %d)\n", __func__, (int)algorithm_id);

    LLSEC_SECRET_KEY_FACTORY_IMPL_algorithm* algorithm = (LLSEC_SECRET_KEY_FACTORY_IMPL_algorithm*)algorithm_id;
    // cppcheck-suppress misra-c2012-11.1 // Abstract data type for SNI usage
    return (int32_t)algorithm->key_close;
}
