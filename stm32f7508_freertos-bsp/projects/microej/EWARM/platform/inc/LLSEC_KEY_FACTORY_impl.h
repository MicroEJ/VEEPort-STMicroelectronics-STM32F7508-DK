/*
 * C
 *
 * Copyright 2018-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_KEY_FACTORY_IMPL_H
#define LLSEC_KEY_FACTORY_IMPL_H

#include <intern/LLSEC_KEY_FACTORY_impl.h>
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
 * @brief Gets for the given algorithm the key factory description.
 *
 * @param[in] algorithm_name			Null terminated string that describes the algorithm.
 *
 * @return The algorithm ID on success or -1 on error.
 */
int32_t LLSEC_KEY_FACTORY_IMPL_get_algorithm_description(uint8_t* algorithm_name);

/**
 * @brief Gets the native private key corresponding to the encoded data.
 *
 * @param[in] algorithm_id				The algorithm ID.
 * @param[in] format_name				Null terminated string that describes the key format.
 * @param[in] encoded_key				The private key in its encoded form.
 * @param[in] encoded_key_length		The encoded key length.
 *
 * @return pointer on the C structure holding the key information.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_KEY_FACTORY_IMPL_get_private_key_data(int32_t algorithm_id, uint8_t* format_name, uint8_t* encoded_key, int32_t encoded_key_length);


/**
 * @brief Get the pointer for the close private key method to be used as a close resource callback with SNI.
 *
 * @param[in] algorithm_id algorithm pointer
 *
 * @return the pointer for the close method.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_KEY_FACTORY_IMPL_get_private_key_close_id(int32_t algorithm_id);

/**
 * @brief Gets the native public key corresponding to the encoded data.
 *
 * @param[in] algorithm_id				The algorithm ID.
 * @param[in] format_name				Null terminated string that describes the key format.
 * @param[in] encoded_key				The public key in its encoded form.
 * @param[in] encoded_key_length		The encoded key length.
 *
 * @return pointer on the C structure holding the key information.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_KEY_FACTORY_IMPL_get_public_key_data(int32_t algorithm_id, uint8_t* format_name, uint8_t* encoded_key, int32_t encoded_key_length);

/**
 * @brief Get the pointer for the close public key method to be used as a close resource callback with SNI.
 *
 * @param[in] algorithm_id algorithm pointer
 *
 * @return the pointer for the close method.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_KEY_FACTORY_IMPL_get_public_key_close_id(int32_t algorithm_id);



#endif //LLSEC_KEY_FACTORY_IMPL_H
