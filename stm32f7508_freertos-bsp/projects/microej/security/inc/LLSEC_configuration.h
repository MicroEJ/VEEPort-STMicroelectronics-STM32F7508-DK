/*
 * C
 *
 * Copyright 2021-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
 
/**
 * @file
 * @brief Security natives configuration.
 * @author MicroEJ Developer Team
 * @version 1.5.0
 * @date 15 March 2024
 */

#ifndef LLSEC_CONFIGURATION_H
#define LLSEC_CONFIGURATION_H

#include "microej_allocator.h"

#define LLSEC_calloc                              microej_calloc
#define LLSEC_free                                microej_free

#define LLSEC_SUCCESS                             (0)
#define LLSEC_ERROR                               (-1)

#define LLSEC_MBEDTLS_SUCCESS                     (0)

#define LLSEC_UNUSED_PARAM(x)                     ((void)(x))

/*
* Used for private and public key generation
*/
#define LLSEC_PRIVATE_KEY_LOCAL_BUFFER_SIZE       (3072)
#define LLSEC_PUBLIC_KEY_LOCAL_BUFFER_SIZE        (3072)

/*
 * Debug traces activation
 */
#define LLSEC_DEBUG_TRACE_ENABLE                  (1)
#define LLSEC_DEBUG_TRACE_DISABLE                 (0)

#ifndef LLSEC_DEBUG_TRACE
// cppcheck-suppress misra-c2012-21.6 // Include only in debug
#include <stdio.h>
#define LLSEC_DEBUG_TRACE(...)                    (void)printf(__VA_ARGS__)
#endif
#ifndef LLSEC_ALL_DEBUG
#define LLSEC_ALL_DEBUG                           LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_CIPHER_DEBUG
#define LLSEC_CIPHER_DEBUG                        LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_DIGEST_DEBUG
#define LLSEC_DIGEST_DEBUG                        LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_KEY_FACTORY_DEBUG
#define LLSEC_KEY_FACTORY_DEBUG                   LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_KEY_PAIR_GENERATOR_DEBUG
#define LLSEC_KEY_PAIR_GENERATOR_DEBUG            LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_MAC_DEBUG
#define LLSEC_MAC_DEBUG                           LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_PRIVATE_KEY_DEBUG
#define LLSEC_PRIVATE_KEY_DEBUG                   LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_PUBLIC_KEY_DEBUG
#define LLSEC_PUBLIC_KEY_DEBUG                    LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_RANDOM_DEBUG
#define LLSEC_RANDOM_DEBUG                        LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_RSA_CIPHER_DEBUG
#define LLSEC_RSA_CIPHER_DEBUG                    LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_SECRET_KEY_FACTORY_DEBUG
#define LLSEC_SECRET_KEY_FACTORY_DEBUG            LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_SECRET_KEY_DEBUG
#define LLSEC_SECRET_KEY_DEBUG                    LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_SIG_DEBUG
#define LLSEC_SIG_DEBUG                           LLSEC_DEBUG_TRACE_DISABLE
#endif
#ifndef LLSEC_X509_DEBUG
#define LLSEC_X509_DEBUG                          LLSEC_DEBUG_TRACE_DISABLE
#endif

#if (LLSEC_CIPHER_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_CIPHER_DEBUG_TRACE(...)             LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_CIPHER_DEBUG_TRACE(...)             ((void)(0))
#endif

#if (LLSEC_DIGEST_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_DIGEST_DEBUG_TRACE(...)             LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_DIGEST_DEBUG_TRACE(...)             ((void)(0))
#endif

#if (LLSEC_KEY_FACTORY_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_KEY_FACTORY_DEBUG_TRACE(...)        LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_KEY_FACTORY_DEBUG_TRACE(...)        ((void)(0))
#endif

#if (LLSEC_KEY_PAIR_GENERATOR_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE(...) LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_KEY_PAIR_GENERATOR_DEBUG_TRACE(...) ((void)(0))
#endif

#if (LLSEC_MAC_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_MAC_DEBUG_TRACE(...)                LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_MAC_DEBUG_TRACE(...)                ((void)(0))
#endif

#if (LLSEC_PRIVATE_KEY_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_PRIVATE_KEY_DEBUG_TRACE(...)        LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_PRIVATE_KEY_DEBUG_TRACE(...)        ((void)(0))
#endif

#if (LLSEC_PUBLIC_KEY_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_PUBLIC_KEY_DEBUG_TRACE(...)         LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_PUBLIC_KEY_DEBUG_TRACE(...)         ((void)(0))
#endif

#if (LLSEC_RANDOM_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_RANDOM_DEBUG_TRACE(...)             LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_RANDOM_DEBUG_TRACE(...)             ((void)(0))
#endif

#if (LLSEC_RSA_CIPHER_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_RSA_CIPHER_DEBUG_TRACE(...)         LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_RSA_CIPHER_DEBUG_TRACE(...)         ((void)(0))
#endif

#if (LLSEC_SECRET_KEY_FACTORY_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE(...) LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_SECRET_KEY_FACTORY_DEBUG_TRACE(...) ((void)(0))
#endif

#if (LLSEC_SECRET_KEY_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_SECRET_KEY_DEBUG_TRACE(...)         LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_SECRET_KEY_DEBUG_TRACE(...)         ((void)(0))
#endif

#if (LLSEC_SIG_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_SIG_DEBUG_TRACE(...)                LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_SIG_DEBUG_TRACE(...)                ((void)(0))
#endif

#if (LLSEC_X509_DEBUG == LLSEC_DEBUG_TRACE_ENABLE || LLSEC_ALL_DEBUG == LLSEC_DEBUG_TRACE_ENABLE)
#define LLSEC_X509_DEBUG_TRACE(...)               LLSEC_DEBUG_TRACE(__VA_ARGS__)
#else
#define LLSEC_X509_DEBUG_TRACE(...)               ((void)(0))
#endif

#endif /* LLSEC_CONFIGURATION_H */
