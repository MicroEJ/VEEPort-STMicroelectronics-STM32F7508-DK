/*
 * C
 *
 * Copyright 2015-2023 MicroEJ Corp. All rights reserved.
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

#define LLSEC_X509_CERT_IMPL_parse						Java_com_is2t_support_security_natives_X509CertSupportNatives_parse
#define LLSEC_X509_CERT_IMPL_get_key					Java_com_is2t_support_security_natives_X509CertSupportNatives_getKeyData
#define LLSEC_X509_CERT_IMPL_verify						Java_com_is2t_support_security_natives_X509CertSupportNatives_verify
#define LLSEC_X509_CERT_IMPL_check_validity				Java_com_is2t_support_security_natives_X509CertSupportNatives_checkValidity
#define LLSEC_X509_CERT_IMPL_get_x500_principal_data	Java_com_is2t_support_security_natives_X509CertSupportNatives_getX500PrincipalData
#define LLSEC_X509_CERT_IMPL_get_close_key	            Java_com_is2t_support_security_natives_X509CertSupportNatives_nativeGetCloseKeyId
