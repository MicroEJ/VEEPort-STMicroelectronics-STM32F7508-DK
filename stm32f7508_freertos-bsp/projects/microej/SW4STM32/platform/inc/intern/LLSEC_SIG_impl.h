/*
 * C
 *
 * Copyright 2017-2021 MicroEJ Corp. All rights reserved.
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

#define LLSEC_SIG_IMPL_get_algorithm_description		Java_com_is2t_support_security_signature_NativeSignatureSpi_nativeGetAlgorithmDescription
#define LLSEC_SIG_IMPL_get_algorithm_oid 				Java_com_is2t_support_security_signature_NativeSignatureSpi_getAlgOID
#define LLSEC_SIG_IMPL_verify							Java_com_is2t_support_security_signature_NativeSignatureSpi_nativeVerify
#define LLSEC_SIG_IMPL_sign							    Java_com_is2t_support_security_signature_NativeSignatureSpi_nativeSign
