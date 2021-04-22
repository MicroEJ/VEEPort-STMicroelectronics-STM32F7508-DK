/*
 * C
 *
 * Copyright 2018-2019 IS2T. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break IS2T warranties on the whole library.
 */
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <LLNET_Common.h>
#include <LLNET_SSL_utils_mbedtls.h>

#include "microej.h"

#include "mbedtls/error.h"
#include "mbedtls/net_sockets.h"

#include "lwip/sys.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* ---- Specific net layer connection functions ---- */

/**
 * Receive callback for mbed TLS
 */
int LLNET_SSL_utils_mbedtls_recv(void *ctx, unsigned char *buf, size_t len) {
	LLNET_SSL_DEBUG_TRACE("%s(ctx=%d, length=%d)\n", __func__, (int)ctx, (int)len);
    int recv_bytes = -1;

    int fd = *((int*)ctx);

    if( fd < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    int32_t selectRes = non_blocking_select( fd, SELECT_READ );

    if( selectRes == 0 ) {
    	LLNET_SSL_DEBUG_TRACE("%s RECV WANT READ(ctx=%d, length=%d)\n", __func__, (int)ctx, (int)len);
    	return( MBEDTLS_ERR_SSL_WANT_READ );
    } else {
    	recv_bytes = llnet_recv( fd, buf, len, 0 );
    	int netError = llnet_errno(fd);

    	LLNET_SSL_DEBUG_TRACE("%s RECV (ctx=%d, length=%d) bytes read=%d, netError=%d\n", __func__, (int)ctx, (int)len, (int)recv_bytes, (int)netError);

		if( recv_bytes < 0 )
		{
			if( netError == EPIPE || netError == ECONNRESET )
				return( MBEDTLS_ERR_NET_CONN_RESET );

			if( netError == EINTR )
				return( MBEDTLS_ERR_SSL_WANT_READ );

			return( MBEDTLS_ERR_NET_RECV_FAILED );
		}
    }

    return recv_bytes;
}

/**
 * Send callback for mbed TLS
 */
int LLNET_SSL_utils_mbedtls_send(void *ctx, const unsigned char *buf, size_t len) {
	LLNET_SSL_DEBUG_TRACE("%s(ctx=%d, length=%d)\n", __func__, (int)ctx, (int)len);
    int size_sent = -1;

    int fd = *((int*)ctx);
    if( fd < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    int32_t selectRes = non_blocking_select( fd, SELECT_WRITE );

    if( selectRes == 0 ) {
    	LLNET_SSL_DEBUG_TRACE("%s SEND WANT WRITE(ctx=%d, length=%d)\n", __func__, (int)ctx, (int)len);
    	return( MBEDTLS_ERR_SSL_WANT_WRITE );
    } else {
    	size_sent = llnet_send( fd, buf, len, 0 );
    	int netError = llnet_errno(fd);

    	LLNET_SSL_DEBUG_TRACE("%s SEND (ctx=%d, length=%d) bytes read=%d netError=%d\n", __func__, (int)ctx, (int)len, (int)size_sent, (int)netError);

		if( size_sent < 0 )
		{
			if( netError == EPIPE || netError == ECONNRESET )
				return( MBEDTLS_ERR_NET_CONN_RESET );

			if( netError == EINTR )
				return( MBEDTLS_ERR_SSL_WANT_WRITE );

			return( MBEDTLS_ERR_NET_SEND_FAILED );
		}
    }

	return size_sent;
}

/* ---- mbedtls custom function for error printing ---- */

/**
 * Helper for pretty-printing mbed TLS error codes
 */
 void print_mbedtls_error(const char *name, int err) {
    char buf[128];
    mbedtls_strerror(err, buf, sizeof (buf));
    printf("%s() failed: -0x%04x (%d): %s\r\n", name, -err, err, buf);
}

#ifdef __cplusplus
}
#endif
