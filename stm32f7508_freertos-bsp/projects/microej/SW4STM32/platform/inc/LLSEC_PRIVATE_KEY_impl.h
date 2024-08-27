/*
 * C
 *
 * Copyright 2021-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_PRIVATE_KEY_IMPL_H
#define LLSEC_PRIVATE_KEY_IMPL_H

#include <intern/LLSEC_PRIVATE_KEY_impl.h>
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
 * @return max encoded size for the private key in DER format
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PRIVATE_KEY_IMPL_get_encoded_max_size(int32_t native_id);

/**
 * @brief encode the private key into DER format.
 *
 * @param[in] native_id the C structure pointer holding the key data
 * @param[out] output a byte array to hold the encoded key data
 * @pram[in] outputLength the length of the output array
 *
 * @return the reel size of the encoded key.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_PRIVATE_KEY_IMPL_get_encode(int32_t native_id, uint8_t* output, int32_t outputLength);

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
int32_t LLSEC_PRIVATE_KEY_IMPL_get_output_size(int32_t native_id);

#endif //LLSEC_PRIVATE_KEY_IMPL_H
