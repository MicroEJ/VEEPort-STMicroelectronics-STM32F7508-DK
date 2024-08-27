/*
 * C
 *
 * Copyright 2018-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief MicroEJ Security low level API
 * @author MicroEJ Developer Team
 * @version 2.4.0
 * @date 16 February 2024
 */

#define LLSEC_RANDOM_IMPL_init							Java_com_is2t_support_security_securerandom_NativeSecureRandomSpi_nativeInit
#define LLSEC_RANDOM_IMPL_close							Java_com_is2t_support_security_securerandom_NativeSecureRandomSpi_nativeClose
#define LLSEC_RANDOM_IMPL_next_bytes					Java_com_is2t_support_security_securerandom_NativeSecureRandomSpi_nativeNextBytes
#define LLSEC_RANDOM_IMPL_set_seed						Java_com_is2t_support_security_securerandom_NativeSecureRandomSpi_nativeSetSeed
#define LLSEC_RANDOM_IMPL_generate_seed					Java_com_is2t_support_security_securerandom_NativeSecureRandomSpi_nativeGenerateSeed
#define LLSEC_RANDOM_IMPL_get_close_id					Java_com_is2t_support_security_securerandom_NativeSecureRandomSpi_nativeGetCloseId
