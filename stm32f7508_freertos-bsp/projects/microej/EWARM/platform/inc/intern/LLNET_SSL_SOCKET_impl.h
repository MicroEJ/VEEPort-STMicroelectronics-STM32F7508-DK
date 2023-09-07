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

#define LLNET_SSL_SOCKET_IMPL_initialize 		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_initialize
#define LLNET_SSL_SOCKET_IMPL_create 		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_create
#define LLNET_SSL_SOCKET_IMPL_close 	Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_close
#define LLNET_SSL_SOCKET_IMPL_freeSSL 	Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_freeSSL
#define LLNET_SSL_SOCKET_IMPL_initialClientHandShake		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_initialClientHandShake
#define LLNET_SSL_SOCKET_IMPL_initialServerHandShake		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_initialServerHandShake
#define LLNET_SSL_SOCKET_IMPL_read 	Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_read
#define LLNET_SSL_SOCKET_IMPL_available 	Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_available
#define LLNET_SSL_SOCKET_IMPL_write 		Java_com_is2t_support_net_ssl_natives_SSLSocketSupportNatives_write
