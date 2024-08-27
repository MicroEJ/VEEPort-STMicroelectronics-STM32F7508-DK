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

#define LLNET_SSL_SOCKET_IMPL_initialize 		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_initialize
#define LLNET_SSL_SOCKET_IMPL_create 		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_create
#define LLNET_SSL_SOCKET_IMPL_shutdown 	Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_shutdown
#define LLNET_SSL_SOCKET_IMPL_freeSSL 	Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_freeSSL
#define LLNET_SSL_SOCKET_IMPL_initialClientHandShake		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_initialClientHandShake
#define LLNET_SSL_SOCKET_IMPL_initialServerHandShake		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_initialServerHandShake
#define LLNET_SSL_SOCKET_IMPL_read 	Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_read
#define LLNET_SSL_SOCKET_IMPL_available 	Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_available
#define LLNET_SSL_SOCKET_IMPL_write 		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_write
