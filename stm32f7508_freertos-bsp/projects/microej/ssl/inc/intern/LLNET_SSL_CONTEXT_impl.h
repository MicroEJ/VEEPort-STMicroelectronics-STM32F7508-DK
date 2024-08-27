/*
 * C
 *
 * Copyright 2015-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief MicroEJ SSL low level API
 * @author MicroEJ Developer Team
 * @version 6.0.0
 * @date 26 August 2022
 */

#define LLNET_SSL_CONTEXT_IMPL_createContext 		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_createContext
#define LLNET_SSL_CONTEXT_IMPL_freeContext 		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_freeContext
#define LLNET_SSL_CONTEXT_IMPL_addTrustedCertificate		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_addTrustedCertificate
#define LLNET_SSL_CONTEXT_IMPL_clearTrustStore		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_clearTrustStore
#define LLNET_SSL_CONTEXT_IMPL_clearKeyStore		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_clearKeyStore
#define LLNET_SSL_CONTEXT_IMPL_setCertificate		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_setCertificate
#define LLNET_SSL_CONTEXT_IMPL_setPrivateKey		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_setPrivateKey
#define LLNET_SSL_CONTEXT_IMPL_initChainBuffer	Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_initChainBuffer
#define LLNET_SSL_CONTEXT_IMPL_addChainCertificate		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_addChainCertificate

