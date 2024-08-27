/*
 * C
 *
 * Copyright 2021-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_KEY_PAIR_GENERATOR_IMPL_H
#define LLSEC_KEY_PAIR_GENERATOR_IMPL_H

#include <intern/LLSEC_KEY_PAIR_GENERATOR_impl.h>
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
 * @brief Gets for the given algorithm name the algorithm structure pointer if it's supported.
 *
 * @param[in] algorithm_name			Null terminated string that describes the algorithm.
 *
 * @return The algorithm ID (pointer) on success or -1 on error.
 */
int32_t LLSEC_KEY_PAIR_GENERATOR_IMPL_get_algorithm(uint8_t *algorithm_name);


/**
 * @brief generate key pair for given algorithm with given key size
 *
 * @param[in] algorithm_id algorithm pointer
 * @param[in] rsa_key_size the key size to be used to generate the RSA key pair
 * @param[in] rsa_public_exponent the public exponent to be used to generate the RSA key pair
 * @param[in] ec_curve_stdname elliptic curve standard name to generate the EC key pari
 *
 * @return the key native id
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_KEY_PAIR_GENERATOR_IMPL_generateKeyPair(int32_t algorithm_id, int32_t rsa_key_size, int32_t rsa_public_exponent, uint8_t* ec_curve_stdname);

/**
 * @brief Get the pointer for the close key pair method to be used as a close resource callback with SNI.
 *
 * @param[in] algorithm_id algorithm pointer
 *
 * @return the pointer for the close method.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_KEY_PAIR_GENERATOR_IMPL_get_close_id(int32_t algorithm_id);

#endif //LLSEC_KEY_PAIR_GENERATOR_IMPL_H
