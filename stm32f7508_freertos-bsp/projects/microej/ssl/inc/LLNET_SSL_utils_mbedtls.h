/*
 * C
 *
 * Copyright 2018-2019 IS2T. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break IS2T warranties on the whole library.
 */

#include <LLNET_SSL_CONSTANTS.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef LLNET_SSL_DEBUG
/* ---- Define the mbedtls debug level ---- */
#define MBEDTLS_DEBUG_LEVEL 2
 /* ---- Define the mbedtls debug printer ---- */
#define LLNET_SSL_DEBUG_MBEDTLS_TRACE 	   print_mbedtls_error
#else
#define LLNET_SSL_DEBUG_MBEDTLS_TRACE(...) ((void) 0)
#endif

/* ---- Specific net layer connection functions ---- */

/*
 * Net receive layer adaptation function for mbedtls .
 * @param ctx : Net context (pointer to the socket fd)
 * @param buf : buffer for receiving data
 * @param len : length to receive
 * @return -1 on error, number of received bytes otherwise
 */
int LLNET_SSL_utils_mbedtls_recv(void *ctx, unsigned char *buf, size_t len);

/*
 * Net send layer adaptation function for mbedtls .
 * @param ctx : Net context (pointer to the socket fd)
 * @param buf : buffer of data to send
 * @param len : length to send
 * @return -1 on error, number of sent bytes otherwise
 */
int LLNET_SSL_utils_mbedtls_send(void *ctx, const unsigned char *buf, size_t len);

/* ---- mbedtls custom function for error printing ---- */
void print_mbedtls_error(const char *name, int err);

#ifdef __cplusplus
}
#endif
