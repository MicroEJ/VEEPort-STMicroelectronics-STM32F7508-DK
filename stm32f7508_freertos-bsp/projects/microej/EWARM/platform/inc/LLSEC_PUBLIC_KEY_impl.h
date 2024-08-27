/*
 * C
 *
 * Copyright 2021-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_PUBLIC_KEY_IMPL_H
#define LLSEC_PUBLIC_KEY_IMPL_H

#include <intern/LLSEC_PUBLIC_KEY_impl.h>
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
 * @brief get max size for encoded key.
 *
 * @param[in] native_id the C structure pointer holding the key data
 *
 * @return max encoded size for the public key
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PUBLIC_KEY_IMPL_get_encoded_max_size(int32_t native_id);

/**
 * @brief encode the public key.
 *
 * @param[in] native_id the C structure pointer holding the key data
 * @param[out] output a byte array to hold the encoded key data
 * @param[in] outputLength the length of the output array
 *
 * @return the reel size of the encoded key.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PUBLIC_KEY_IMPL_get_encode(int32_t native_id, uint8_t* output, int32_t outputLength);

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
int32_t LLSEC_PUBLIC_KEY_IMPL_get_output_size(int32_t native_id);

#endif //LLSEC_PUBLIC_KEY_IMPL_H
