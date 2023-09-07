/*
 * C
 *
 * Copyright 2014-2017 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#define LLNET_CHANNEL_IMPL_close 		Java_com_is2t_support_net_natives_ChannelNatives_closeNative
#define LLNET_CHANNEL_IMPL_initialize 	Java_com_is2t_support_net_natives_ChannelNatives_initialize
#define LLNET_CHANNEL_IMPL_setBlocking 	Java_com_is2t_support_net_natives_ChannelNatives_setBlocking
#define LLNET_CHANNEL_IMPL_shutdown 		Java_com_is2t_support_net_natives_ChannelNatives_shutdown
#define LLNET_CHANNEL_IMPL_bind 			Java_com_is2t_support_net_natives_ChannelNatives_bind
#define LLNET_CHANNEL_IMPL_getOption 	Java_com_is2t_support_net_natives_ChannelNatives_getOption__IIZ
#define LLNET_CHANNEL_IMPL_setOption 	Java_com_is2t_support_net_natives_ChannelNatives_setOption__IIIZ
#define LLNET_CHANNEL_IMPL_getOptionAsByteArray	Java_com_is2t_support_net_natives_ChannelNatives_getOption__II_3BIZ
#define LLNET_CHANNEL_IMPL_setOptionAsByteArray	Java_com_is2t_support_net_natives_ChannelNatives_setOption__II_3BIZ
#define LLNET_CHANNEL_IMPL_listen 		Java_com_is2t_support_net_natives_ChannelNatives_listen
