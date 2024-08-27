/*
 * C
 *
 * Copyright 2021-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_SSL mbedtls configuration file.
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 17 June 2022
 */

#ifndef LLNET_SSL_MBEDTLS_CONFIGURATION_H
#define LLNET_SSL_MBEDTLS_CONFIGURATION_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @brief Compatibility sanity check value.
 * This define value is checked in the implementation to validate that the version of this configuration
 * is compatible with the implementation.
 *
 * This value must not be changed by the user of the CCO.
 * This value must be incremented by the implementor of the CCO when a configuration define is added, deleted or modified.
 */
#define LLNET_SSL_MBEDTLS_CONFIGURATION_VERSION (1)

#if !defined(MBEDTLS_ENTROPY_C) || !defined(MBEDTLS_CTR_DRBG_C)
#error "Please set your custom random number generator function in the next #define by replacing my_custom_random_func with the appropriate one. Remove this #error when done."
/*
 * Custom random bytes generator function.
 * Used when the platform does not support entropy pool and CTR_DRBG AES-256 random bytes generator.
 *
 * Please set your custom random bytes generator function by replacing
 * my_custom_random_func with the appropriate one.
 *
 * The function must have the following prototype:
 *
 * int32_t my_custom_random_func(uint8_t *output, size_t size);
 *
 * The <code>output</code> parameter is the buffer to store the random bytes
 * The <code>size<code> parameter is the size of the random bytes.
 * Returns 0 on success, negative value on error.
 */
#define microej_custom_random_func my_custom_random_func
#endif


#ifdef __cplusplus
}
#endif

#endif //LLNET_SSL_MBEDTLS_CONFIGURATION_H
