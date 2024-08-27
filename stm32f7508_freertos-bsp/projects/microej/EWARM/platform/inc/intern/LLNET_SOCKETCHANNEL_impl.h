/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 26 August 2022
 */

#define LLNET_SOCKETCHANNEL_IMPL_connect 			Java_com_is2t_support_net_natives_SocketChannelNatives_connect
#define LLNET_SOCKETCHANNEL_IMPL_getLocalPort 		Java_com_is2t_support_net_natives_SocketChannelNatives_getLocalPort
#define LLNET_SOCKETCHANNEL_IMPL_getPeerAddress 	Java_com_is2t_support_net_natives_SocketChannelNatives_getPeerAddress
#define LLNET_SOCKETCHANNEL_IMPL_getPeerPort 		Java_com_is2t_support_net_natives_SocketChannelNatives_getPeerPort
#define LLNET_SOCKETCHANNEL_IMPL_getLocalAddress 	Java_com_is2t_support_net_natives_SocketChannelNatives_getLocalAddress
#define LLNET_SOCKETCHANNEL_IMPL_socket 			Java_com_is2t_support_net_natives_SocketChannelNatives_socket
#define LLNET_SOCKETCHANNEL_IMPL_serverSocket 		Java_com_is2t_support_net_natives_SocketChannelNatives_serverSocket
#define LLNET_SOCKETCHANNEL_IMPL_multicastSocket 	Java_com_is2t_support_net_natives_SocketChannelNatives_multicastSocket
