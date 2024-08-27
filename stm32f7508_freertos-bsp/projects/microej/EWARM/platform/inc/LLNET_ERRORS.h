/*
 * C
 *
 * Copyright 2014-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLNET_ERRORS_H
#define LLNET_ERRORS_H

/**
 * @file
 * @brief MicroEJ NET low level API
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 26 August 2022
 */

#define J_EACCES										(-2) /* @brief Permission denied. */
#define J_EBADF											(-3) /* @brief Bad socket file descriptor. */
#define J_EHOSTDOWN										(-4) /* @brief Host is down. */
#define J_ENETDOWN										(-5) /* @brief Network is down. */
#define J_ENETUNREACH									(-6) /* @brief Network is unreachable. */
#define J_EADDRINUSE									(-7) /* @brief Address already in use. */
#define J_ECONNABORTED									(-8) /* @brief Connection abort. */
#define J_EINVAL										(-9) /* @brief Invalid argument. */
#define J_ENOPROTOOPT									(-10) /* @brief Socket option not available. */
#define J_ENOTCONN										(-11) /* @brief Socket not connected. */
#define J_EAFNOSUPPORT									(-12) /* @brief Unsupported network address family. */
#define J_ECONNREFUSED									(-13) /* @brief Connection refused. */
#define J_EISCONN										(-14) /* @brief Socket already connected. */
#define J_ECONNRESET									(-15) /* @brief Connection reset by peer. */
#define J_EMSGSIZE										(-16) /* @brief Message size to be send is too long. */
#define J_EPIPE											(-17) /* @brief Broken pipe. */
#define J_ETIMEDOUT										(-18) /* @brief Connection timed out. */
#define J_ENOMEM										(-19) /* @brief Not enough free memory. */
#define J_EHOSTUNREACH									(-20) /* @brief No route to host. */
#define J_EHOSTUNKNOWN									(-21) /* @brief Unknown host. */
#define J_NET_NATIVE_NOT_IMPLEMENTED					(-23) /* @brief Native method not implemented. */
#define J_ASYNC_BLOCKING_REQUEST_QUEUE_LIMIT_REACHED	(-24) /* @brief Indicate the blocking request queue is full and a new request can not be added now. */
#define J_NETWORK_NOT_INITIALIZED 						(-25) /* @brief Network not initialized. */
#define J_EUNKNOWN 										(-255) /* @brief Unknown error. */

#endif // LLNET_ERRORS_H
