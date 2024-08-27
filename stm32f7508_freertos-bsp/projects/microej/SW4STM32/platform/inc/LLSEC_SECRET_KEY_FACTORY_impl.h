/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_SECRET_KEY_FACTORY_IMPL_H
#define LLSEC_SECRET_KEY_FACTORY_IMPL_H

#include <intern/LLSEC_SECRET_KEY_FACTORY_impl.h>
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
 * @brief Get the supported algorithm native ID.
 *
 * @param[in] algorithm_name Null terminated string that describes the algorithm.
 *
 * @return The algorithm ID on success or -1 on error.
 */
int32_t LLSEC_SECRET_KEY_FACTORY_IMPL_get_algorithm(uint8_t* algorithm_name);

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
int32_t LLSEC_SECRET_KEY_FACTORY_IMPL_get_key_data(int32_t algorithm_id, uint8_t* password, int32_t password_length, uint8_t* salt, int32_t salt_length, int32_t iterations, int32_t key_length);

/**
 * Gets the id of the native close function.
 *
 * @param[in] algorithm_id algorithm pointer
 *
 * @return the id of the static native close function
 * @throws NativeException on error
 */
int32_t LLSEC_SECRET_KEY_FACTORY_IMPL_get_close_id(int32_t algorithm_id);

#endif //LLSEC_SECRET_KEY_FACTORY_IMPL_H
