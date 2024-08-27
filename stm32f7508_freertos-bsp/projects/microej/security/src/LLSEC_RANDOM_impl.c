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
#include <LLSEC_RANDOM_impl.h>
#include <LLSEC_mbedtls.h>
#include "microej_time.h"
#include <LLSEC_configuration.h>

#include <sni.h>
#include <stdlib.h>
#include <string.h>

#include "mbedtls/platform.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"

static mbedtls_entropy_context entropy;
static mbedtls_ctr_drbg_context ctr_drbg;

// cppcheck-suppress misra-c2012-8.9 // Define here for code readability even if it called once in this file.
static int initialized = 0;

// cppcheck-suppress misra-c2012-8.9 // Define here for code readability even if it called once in this file.
static int32_t native_ids = 1;

/**
 * @brief Initializes a Random resource.
 *
 * @return The native ID of the resource.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_RANDOM_IMPL_init(void) {
    int32_t return_code = LLSEC_SUCCESS;
    LLSEC_RANDOM_DEBUG_TRACE("%s\n", __func__);
    const char* pers = llsec_gen_random_str_internal(8);
    int32_t native_id;

    if (0 == initialized) {
        mbedtls_ctr_drbg_init(&ctr_drbg);
        mbedtls_entropy_init(&entropy);
        int mbedtls_rc = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const uint8_t*)pers, 0);
        if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
            (void)SNI_throwNativeException(mbedtls_rc, "mbedtls_ctr_drbg_seed failed");
            LLSEC_RANDOM_IMPL_close(0);
            return_code = LLSEC_ERROR;
        }
        initialized = 1;
    }

    if (LLSEC_SUCCESS == return_code) {
        native_id = native_ids;
        native_ids++;
        // cppcheck-suppress misra-c2012-11.6 // Cast for matching SNI_registerResource function signature
        if (SNI_OK != SNI_registerResource((void*)native_id, (SNI_closeFunction)LLSEC_RANDOM_IMPL_close, NULL)) {
            (void)SNI_throwNativeException(LLSEC_ERROR, "Can't register SNI native resource");
            LLSEC_RANDOM_IMPL_close(native_id);
            return_code = LLSEC_ERROR;
        }
    }

    if(NULL != pers) {
        // cppcheck-suppress misra-c2012-11.8 // Cast for matching free function signature
        mbedtls_free((void*)pers);
    }

    if (LLSEC_SUCCESS == return_code) {
        return_code = native_id;
    }

    return return_code;
}

/**
 * @brief Closes the resource related to the nativeId.
 *
 * @param[in] native_id                    The native ID.
 *
 * @note Throws NativeException on error.
 */
void LLSEC_RANDOM_IMPL_close(int32_t native_id) {
    LLSEC_UNUSED_PARAM(native_id);
    LLSEC_RANDOM_DEBUG_TRACE("%s native_id:%d\n", __func__, (int)native_id);
    /*To suport multi-instance, do not free the ctr_drbg & entropy here!*/
    //mbedtls_ctr_drbg_free(&ctr_drbg);
    //mbedtls_entropy_free(&entropy);
}

/**
 * @brief Generates random bytes.
 *
 * @param[in]  native_id                   The native ID.
 * @param[out] rnd                         The buffer to fill with random bytes.
 * @param[in]  size                        The size of rnd.
 *
 * @note Throws NativeException on error.
 */
// cppcheck-suppress misra-c2012-8.7 // External API which is called also internally, cannot be made static
void LLSEC_RANDOM_IMPL_next_bytes(int32_t native_id, uint8_t* rnd, int32_t size) {
    LLSEC_UNUSED_PARAM(native_id);

    LLSEC_RANDOM_DEBUG_TRACE("%s rdn:0x%p, %d\n", __func__, rnd, (int)size);
    uint32_t bytes_left = size;
    while (bytes_left > (uint32_t)0) {
        int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;
        if (bytes_left > (uint32_t)MBEDTLS_CTR_DRBG_MAX_REQUEST) {
            mbedtls_rc = mbedtls_ctr_drbg_random(&ctr_drbg, rnd, MBEDTLS_CTR_DRBG_MAX_REQUEST);
            bytes_left -= (uint32_t)MBEDTLS_CTR_DRBG_MAX_REQUEST;
            // cppcheck-suppress misra-c2012-17.8 // Used as an output parameter
            rnd = &rnd[MBEDTLS_CTR_DRBG_MAX_REQUEST];
        } else {
            mbedtls_rc = mbedtls_ctr_drbg_random(&ctr_drbg, rnd, bytes_left);
            bytes_left = 0;
        }
        if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
            (void)SNI_throwNativeException(mbedtls_rc, "mbedtls_ctr_drbg_random failed");
        }
    }
}

/**
 * @brief Sets the seed of the PRNG.
 *
 * @param[in] native_id                    The native ID.
 * @param[in] seed                         The array of bytes used as a seed.
 * @param[in] size                         The size of seed.
 *
 * @note Throws NativeException on error.
 */
void LLSEC_RANDOM_IMPL_set_seed(int32_t native_id, uint8_t* seed, int32_t size) {
    LLSEC_UNUSED_PARAM(native_id);

    LLSEC_RANDOM_DEBUG_TRACE("%s\n", __func__);
    LLSEC_RANDOM_DEBUG_TRACE("LLSEC_RANDOM_IMPL_set_seed, Seeding the random number generator\n");

    int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;

    /*
     * Since ctr_drbg is not freed (to support multi-instance), setting a new seed from application point of view can be done after mbedtls_ctr_drbg_random calls,
     * which will increase the reseed counter with every call, making the entropy nonce length higher than the maximum seed length (MBEDTLS_CTR_DRBG_MAX_SEED_INPUT),
     * thus faling to proper set a new seed.
     * So, need to reseed first to obtain appropriate entropy nonce length.
     */
    mbedtls_rc = mbedtls_ctr_drbg_reseed(&ctr_drbg, NULL, 0);
    if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
        (void)SNI_throwNativeException(mbedtls_rc, "mbedtls_ctr_drbg_reseed failed");
    }

    mbedtls_rc = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char*)seed, size);
    if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
        (void)SNI_throwNativeException(mbedtls_rc, "mbedtls_ctr_drbg_seed failed");
    }
}

/**
 * @brief Generates a new seed.
 *
 * @param[in]  native_id                   The native ID.
 * @param[out] seed                        The array to fill with the seed.
 * @param[in]  size                        The size of seed.
 *
 * @note Throws NativeException on error.
 */
void LLSEC_RANDOM_IMPL_generate_seed(int32_t native_id, uint8_t* seed, int32_t size) {
    LLSEC_RANDOM_DEBUG_TRACE("%s\n", __func__);
    LLSEC_RANDOM_IMPL_next_bytes(native_id, seed, size);
}

/**
 * @brief Gets the id of the native close function.
 *
 * @return the id of the static native close function.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_RANDOM_IMPL_get_close_id(void) {
    LLSEC_RANDOM_DEBUG_TRACE("%s\n", __func__);
    return (int32_t) LLSEC_RANDOM_IMPL_close;
}

/**
 * @brief generate random string function.
 * @param[in] length the string length
 *
 * @return pointer of random string.
 *
 * @note This function should only be used to provide a personalisation string
 *          when calling mbedtls_ctr_drbg_seed.
 */
// cppcheck-suppress misra-c2012-8.7 // External API which is called also internally, cannot be made static
char* llsec_gen_random_str_internal(int length) {
    char* return_code = NULL;
    char* str_ran;
    str_ran = (char*)mbedtls_calloc(1, length);
    if (NULL == str_ran) {
        LLSEC_RANDOM_DEBUG_TRACE("Random string malloc failed");
    } else {
        srand((unsigned int)microej_time_get_current_time(0));

        int idx;
        for (idx = 0; idx < (length - 1); idx++) {
            int flag = rand() % 3;
            switch (flag) {
                case 0:
                    // cppcheck-suppress misra-c2012-10.8 // Number in [0, 25] range
                    str_ran[idx] = 'A' + (uint8_t)(rand() % 26);
                    break;
                case 1:
                    // cppcheck-suppress misra-c2012-10.8 // Number in [0, 25] range
                    str_ran[idx] = 'a' + (uint8_t)(rand() % 26);
                    break;
                case 2:
                    // cppcheck-suppress misra-c2012-10.8 // Number in [0, 10] range
                    str_ran[idx] = '0' + (uint8_t)(rand() % 10);
                    break;
                default:
                    str_ran[idx] = 'x';
                    break;
            }
        }
        str_ran[length - 1] = '\0';
        return_code = str_ran;
    }

    return return_code;
}
