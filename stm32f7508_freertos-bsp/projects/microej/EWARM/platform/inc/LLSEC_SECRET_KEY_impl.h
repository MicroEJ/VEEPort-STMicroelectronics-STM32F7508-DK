/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_SECRET_KEY_IMPL_H
#define LLSEC_SECRET_KEY_IMPL_H

#include <intern/LLSEC_SECRET_KEY_impl.h>
#include <sni.h>
#include <stdint.h>


/**
 * @file
 * @brief MicroEJ Security low level API
 * @author MicroEJ Developer Team
 * @version 2.4.0
 * @date 16 February 2024
 */


/**
 * @brief return the max size of the encoded key.
 *
 * @param[in] native_id the C structure pointer holding the key data
 *
 * @return max encoded size for the secret key in DER format
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_SECRET_KEY_IMPL_get_encoded_max_size(int32_t native_id);

/**
 * @brief encode the secret key.
 *
 * @param[in] native_id the C structure pointer holding the key data
 * @param[out] output a byte array to hold the encoded key data
 * @param[in] output_length the length of the output array (in bytes)
 *
 * @return the real size of the encoded key.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_SECRET_KEY_IMPL_get_encoded(int32_t native_id, uint8_t* output, int32_t output_length);

#endif //LLSEC_SECRET_KEY_IMPL_H
