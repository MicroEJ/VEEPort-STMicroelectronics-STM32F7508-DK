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

#define LLNET_CHANNEL_IMPL_close 		Java_com_is2t_support_net_natives_ChannelNatives_closeNative
#define LLNET_CHANNEL_IMPL_initialize 	Java_com_is2t_support_net_natives_ChannelNatives_initialize
#define LLNET_CHANNEL_IMPL_shutdown 		Java_com_is2t_support_net_natives_ChannelNatives_shutdown
#define LLNET_CHANNEL_IMPL_bind 			Java_com_is2t_support_net_natives_ChannelNatives_bind
#define LLNET_CHANNEL_IMPL_getOption 	Java_com_is2t_support_net_natives_ChannelNatives_getOption__II
#define LLNET_CHANNEL_IMPL_setOption 	Java_com_is2t_support_net_natives_ChannelNatives_setOption__III
#define LLNET_CHANNEL_IMPL_getOptionAsByteArray	Java_com_is2t_support_net_natives_ChannelNatives_getOption__II_3BI
#define LLNET_CHANNEL_IMPL_setOptionAsByteArray	Java_com_is2t_support_net_natives_ChannelNatives_setOption__II_3BI
#define LLNET_CHANNEL_IMPL_listen 		Java_com_is2t_support_net_natives_ChannelNatives_listen
