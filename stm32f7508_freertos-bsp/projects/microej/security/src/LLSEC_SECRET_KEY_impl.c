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

#include <LLSEC_SECRET_KEY_impl.h>
#include <LLSEC_configuration.h>
#include <LLSEC_mbedtls.h>
#include <string.h>

/**
 * @brief return the max size of the encoded key.
 *
 * @param[in] native_id the C structure pointer holding the key data
 *
 * @return max encoded size for the secret key in DER format
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_SECRET_KEY_IMPL_get_encoded_max_size(int32_t native_id) {
    LLSEC_SECRET_KEY_DEBUG_TRACE("%s (native_id = %d)\n", __func__, (int)native_id);
    int32_t max_size = 0;

    LLSEC_secret_key* secret_key = (LLSEC_secret_key*)native_id;
    if(NULL != secret_key) {
        max_size = secret_key->key_length;
    }

    LLSEC_SECRET_KEY_DEBUG_TRACE("%s Return size = %d\n", __func__, (int)max_size);
    return max_size;
}

/**
 * @brief encode the secret key.
 *
 * @param[in]  native_id      the C structure pointer holding the key data
 * @param[out] output         a byte array to hold the encoded key data
 * @param[in]  output_length  the length of the output array (in bytes)
 *
 * @return the reel size of the encoded key (in bytes).
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_SECRET_KEY_IMPL_get_encoded(int32_t native_id, uint8_t* output, int32_t output_length) {
    LLSEC_SECRET_KEY_DEBUG_TRACE("%s (native_id = %d)\n", __func__, (int)native_id);

    LLSEC_secret_key* secret_key = (LLSEC_secret_key*)native_id;
    if(NULL != secret_key) {
        (void)memcpy(output, secret_key->key, output_length);
    }

    LLSEC_SECRET_KEY_DEBUG_TRACE("%s Return size = %d\n", __func__, (int)output_length);
    return output_length;
}
