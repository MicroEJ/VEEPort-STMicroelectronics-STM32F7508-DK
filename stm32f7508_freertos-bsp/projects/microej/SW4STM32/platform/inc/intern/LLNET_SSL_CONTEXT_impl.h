/*
 * C
 *
 * Copyright 2015-2017 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/**
 * @file
 * @brief MicroEJ SSL low level API
 * @author MicroEJ Developer Team
 * @version 5.1.1
 * @date 7 May 2021
 */

#define LLNET_SSL_CONTEXT_IMPL_createContext 		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_createContext
#define LLNET_SSL_CONTEXT_IMPL_closeContext 		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_closeContext
#define LLNET_SSL_CONTEXT_IMPL_addTrustedCert		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_addTrustedCert
#define LLNET_SSL_CONTEXT_IMPL_clearTrustStore		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_clearTrustStore
#define LLNET_SSL_CONTEXT_IMPL_clearKeyStore		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_clearKeyStore
#define LLNET_SSL_CONTEXT_IMPL_setCertificate		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_setCertificate
#define LLNET_SSL_CONTEXT_IMPL_setPrivateKey		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_setPrivateKey
#define LLNET_SSL_CONTEXT_IMPL_initChainBuffer	Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_initChainBuffer
#define LLNET_SSL_CONTEXT_IMPL_addChainCertificate		Java_com_is2t_support_net_ssl_natives_SSLContextSupportNatives_addChainCertificate

