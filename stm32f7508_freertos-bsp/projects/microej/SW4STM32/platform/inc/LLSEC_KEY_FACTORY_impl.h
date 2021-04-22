/*
 * C
 *
 * Copyright 2018-2019 MicroEJ Corp. All rights reserved.
 * For demonstration purpose only.
 * MicroEJ Corp. PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef __LLSEC_KEY_FACTORY_IMPL__
#define __LLSEC_KEY_FACTORY_IMPL__

#include <intern/LLSEC_KEY_FACTORY_impl.h>
#include <sni.h>
#include <stdint.h>

/**
 * @file
 * @brief MicroEJ Security low level API
 * @author MicroEJ Developer Team
 * @version 1.4.0
 * @date 30 January 2020
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
 * @param[out] key_data					The private key in the native format.
 * @param[out] key_data_length			The private key length.
 * @param[in] encoded_key				The private key in its encoded form.
 * @param[in] encoded_key_length		The encoded key length.
 *
 * @note Throws NativeException on error.
 */
void LLSEC_KEY_FACTORY_IMPL_get_private_key_data(int32_t algorithm_id, uint8_t* format_name, uint8_t* key_data, int32_t key_data_length, uint8_t* encoded_key, int32_t encoded_key_length);

/**
 * @brief Gets the native private key size corresponding to the algorithm.
 *
 * @param[in] algorithm_id				The algorithm ID.
 * @param[in] format_name				Null terminated string that describes the key format.
 * @param[in] encoded_key				The private key in its encoded form.
 * @param[in] encoded_key_length		The encoded key length.
 *
 * @return The size of the private key.
 */
int32_t LLSEC_KEY_FACTORY_IMPL_get_private_key_length(int32_t algorithm_id, uint8_t* format_name, uint8_t* encoded_key, int32_t encoded_key_length);

#endif //__LLSEC_KEY_FACTORY_IMPL__
