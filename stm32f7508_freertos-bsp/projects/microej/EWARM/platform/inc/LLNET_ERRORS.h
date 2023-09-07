/*
 * C
 *
 * Copyright 2014-2017 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef __LLNET_ERRORS_H__
#define __LLNET_ERRORS_H__

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 2.1.2
 * @date 7 May 2021
 */

#include <sni.h>

#define J_EACCES (-2) /* Permission denied */

#define J_EBADF (-3) /* Bad socket file descriptor */

#define J_EHOSTDOWN (-4) /* Host is down */

#define J_ENETDOWN (-5) /* Network is down */

#define J_ENETUNREACH (-6) /* Network is unreachable */

#define J_EADDRINUSE (-7) /* Address already in use */

#define J_ECONNABORTED (-8) /* connection abort */

#define J_EINVAL (-9)	/* Invalid argument */

#define J_ENOPROTOOPT (-10) /* Socket option not available */

#define J_ENOTCONN (-11) /* Socket not connected */

#define J_EAFNOSUPPORT (-12) /* Unsupported network address family */

#define J_ECONNREFUSED (-13) /* Connection refused */

#define J_EISCONN (-14) /* Socket already connected */

#define J_ECONNRESET (-15) /* Connection reset by peer */

#define J_EMSGSIZE (-16)	/* Message size to be send is too long */

#define J_EPIPE (-17) /* Broken pipe */

#define J_ETIMEDOUT (-18) /* Connection timed out */

#define J_ENOMEM (-19) /* Not enough free memory */

#define J_EHOSTUNREACH (-20) /* No route to host */

#define J_EHOSTUNKNOWN (-21) /* Unknown host */

// Indicate the native code has been stopped (with a timeout or not) because result was not available
// When receiving this error code, the caller shall call the native method again,
// and is ensured that this next call will not block.
#define J_NET_NATIVE_CODE_BLOCKED_WITHOUT_RESULT (-22)

#define J_NET_NATIVE_NOT_IMPLEMENTED (-23) /* Native method not implemented */

// Indicate the blocking request queue is full and a new request can not be added now
#define J_ASYNC_BLOCKING_REQUEST_QUEUE_LIMIT_REACHED (-24)

#define J_NETWORK_NOT_INITIALIZED (-25) /* Network not initialized */

#define J_EUNKNOWN (-255) /* Unknown error */

#endif // __LLNET_ERRORS_H__

