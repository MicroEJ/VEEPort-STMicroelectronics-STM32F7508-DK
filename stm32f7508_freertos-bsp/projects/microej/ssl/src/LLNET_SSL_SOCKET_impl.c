/*
 * C
 *
 * Copyright 2018-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLNET_SSL_SOCKET implementation over mbedtls.
 * @author MicroEJ Developer Team
 * @version 3.0.0
 * @date 17 June 2022
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#define mbedtls_calloc    calloc
#define mbedtls_free      free
#endif

#include "mbedtls/ssl.h"
#include "mbedtls/net_sockets.h"
#if defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_CTR_DRBG_C)
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#endif
#include "LLNET_Common.h"
#include "LLNET_CHANNEL_impl.h"
#include "LLNET_SSL_mbedtls_configuration.h"
#include "LLNET_SSL_ERRORS.h"
#include "LLNET_SSL_utils_mbedtls.h"
#include "LLNET_SSL_SOCKET_impl.h"
#include "LLNET_SSL_CONSTANTS.h"
#include <stdio.h>

#ifdef __cplusplus
	extern "C" {
#endif

#if defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_CTR_DRBG_C)
/* Global variable for entropy in drbg context */
mbedtls_ctr_drbg_context ctr_drbg;
static mbedtls_entropy_context entropy;

/* personalization string for the drbg */
const char *DRBG_PERS = "mbed TLS microEJ client";
#endif

/* static functions */
static void LLNET_SSL_SOCKET_initialHandShake_callback(int32_t sslID, int32_t fd, int64_t absoluteJavaStartTime, int32_t relativeTimeout);
static void LLNET_SSL_SOCKET_initialHandShake(int32_t sslID, int32_t fd, int64_t absoluteJavaStartTime, int64_t absoluteTimeout, int32_t relativeTimeout);
static int32_t LLNET_SSL_SOCKET_read_callback(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int32_t relativeTimeout);
static int32_t LLNET_SSL_SOCKET_read(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int64_t absoluteTimeout, int32_t relativeTimeout, SNI_callback callback);
static int32_t LLNET_SSL_SOCKET_write_callback(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int32_t relativeTimeout);
static int32_t LLNET_SSL_SOCKET_write(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int64_t absoluteTimeout, int32_t relativeTimeout);
static void LLNET_SSL_SOCKET_close_callback(int32_t sslID, int32_t fd, uint8_t autoclose, int64_t absoluteJavaStartTime, int32_t relativeTimeout);

static void LLNET_SSL_SOCKET_initialHandShake_callback(int32_t sslID, int32_t fd, int64_t absoluteJavaStartTime, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d)\n", __func__, (int)sslID, (int)fd);
	uint32_t delta32 = 0;
	//get the time diff between the start time of the LL call and the time from which the current absolute timeout was computed.
	SNI_getCallbackArgs((void**)&delta32, NULL);
	//reconstruct the absolute timeout
	int64_t absoluteTimeout = (0 == relativeTimeout) ? 0 : absoluteJavaStartTime + delta32 + relativeTimeout;
	LLNET_SSL_SOCKET_initialHandShake(sslID, fd, absoluteJavaStartTime, absoluteTimeout, relativeTimeout);
}

static void LLNET_SSL_SOCKET_initialHandShake(int32_t sslID, int32_t fd, int64_t absoluteJavaStartTime, int64_t absoluteTimeout, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d)\n", __func__, (int)sslID, (int)fd);

	mbedtls_ssl_context* ssl_ctx = (mbedtls_ssl_context*)(sslID);
	if (NULL == ssl_ctx)
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return;
	}
	LLNET_SSL_utils_mbedtls_on_bio_start();
    int ret = mbedtls_ssl_handshake(ssl_ctx);

    if(0 != ret)
	{
    	//error
		uint32_t delta32 = 0;
		absoluteTimeout = LLNET_SSL_utils_mbedtls_update_next_bio_timeout(absoluteJavaStartTime, absoluteTimeout, relativeTimeout, &delta32);
		LLNET_SSL_utils_mbedtls_handle_IO_error(ssl_ctx, ret, true, fd, absoluteTimeout, (SNI_callback)LLNET_SSL_SOCKET_initialHandShake_callback, (void*)delta32);
	}

    LLNET_SSL_DEBUG_TRACE("%s HandShake (ssl=%d, fd=%d) ret=%d\n", __func__, (int)sslID, (int)fd, (int)ret);
}

static int32_t LLNET_SSL_SOCKET_read_callback(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d, offset=%d, length=%d)\n", __func__, (int)sslID, (int)fd, (int)off, (int)len);
	uint32_t delta32 = 0;
	//get the time diff between the start time of the LL call and the time from which the current absolute timeout was computed.
	SNI_getCallbackArgs((void**)&delta32, NULL);
	//reconstruct the absolute timeout
	int64_t absoluteTimeout = (0 == relativeTimeout) ? 0 : absoluteJavaStartTime + delta32 + relativeTimeout;
	return LLNET_SSL_SOCKET_read(sslID, fd, buf, off, len, absoluteJavaStartTime, absoluteTimeout, relativeTimeout, (SNI_callback)LLNET_SSL_SOCKET_read_callback);
}

static int32_t LLNET_SSL_SOCKET_read(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int64_t absoluteTimeout, int32_t relativeTimeout, SNI_callback callback)
{

	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d, offset=%d, length=%d)\n", __func__, (int)sslID, (int)fd, (int)off, (int)len);

	mbedtls_ssl_context* ssl = (mbedtls_ssl_context*)(sslID);
	LLNET_SSL_utils_mbedtls_on_bio_start();
	int32_t ret = mbedtls_ssl_read(ssl, (unsigned char *) buf + off, len);

	if(0 < ret)
	{
		//success
		return ret;
	}

	if(MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY == ret || 0 == ret)
	{
		//end-of-file
		return 0;
	}

	uint32_t delta32 = 0;
	absoluteTimeout = LLNET_SSL_utils_mbedtls_update_next_bio_timeout(absoluteJavaStartTime, absoluteTimeout, relativeTimeout, &delta32);
	LLNET_SSL_utils_mbedtls_handle_IO_error(ssl, ret, false, fd, absoluteTimeout, callback, (void*)delta32);
	return 0;
}

static int32_t LLNET_SSL_SOCKET_write_callback(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d, offset=%d, length=%d)\n", __func__, (int)sslID, (int)fd, (int)off, (int)len);
	uint32_t delta32 = 0;
	//get the time diff between the start time of the LL call and the time from which the current absolute timeout was computed.
	SNI_getCallbackArgs((void**)&delta32, NULL);
	//reconstruct the absolute timeout
	int64_t absoluteTimeout = (0 == relativeTimeout) ? 0 : absoluteJavaStartTime + delta32 + relativeTimeout;
	return LLNET_SSL_SOCKET_write(sslID, fd, buf, off, len, absoluteJavaStartTime, absoluteTimeout, relativeTimeout);

}

static int32_t LLNET_SSL_SOCKET_write(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int64_t absoluteTimeout, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d, offset=%d, length=%d)\n", __func__, (int)sslID, (int)fd, (int)off, (int)len);

	mbedtls_ssl_context* ssl = (mbedtls_ssl_context*)(sslID);
	LLNET_SSL_utils_mbedtls_on_bio_start();
	int ret = mbedtls_ssl_write(ssl, (const unsigned char *) buf + off, len);

	if(0 < ret)
	{
		//success
		return ret;
	}

	if(0 == ret)
	{
    	//should not happen
    	//0 byte written
    	SNI_throwNativeIOException(J_EUNKNOWN, "0 byte written");
    	return 0;
	}

	//error
	uint32_t delta32 = 0;
	absoluteTimeout = LLNET_SSL_utils_mbedtls_update_next_bio_timeout(absoluteJavaStartTime, absoluteTimeout, relativeTimeout, &delta32);
	LLNET_SSL_utils_mbedtls_handle_IO_error(ssl, ret, false, fd, absoluteTimeout, (SNI_callback)LLNET_SSL_SOCKET_write_callback, (void*)delta32);

	return 0;
}

static void LLNET_SSL_SOCKET_close_callback(int32_t sslID, int32_t fd, uint8_t autoclose, int64_t absoluteJavaStartTime, int32_t relativeTimeout)
{
	int8_t buffer[1];
	int32_t res;
	//get the time diff between the start time of the LL call and the time from which the current absolute timeout was computed.
	uint32_t delta32 = 0;
	SNI_getCallbackArgs((void**)&delta32, NULL);
	//reconstruct the absolute timeout
	int64_t absoluteTimeout = (0 == relativeTimeout) ? 0 : absoluteJavaStartTime + delta32 + relativeTimeout;
	do
	{
		res = LLNET_SSL_SOCKET_read(sslID, fd, buffer, 0, 1, absoluteJavaStartTime, absoluteTimeout, relativeTimeout, (SNI_callback)LLNET_SSL_SOCKET_close_callback);
		absoluteTimeout = (0 == relativeTimeout) ? 0 : LLNET_current_time_ms() + relativeTimeout;
	}
	while(res > 0);
}

void LLNET_SSL_SOCKET_IMPL_initialize(void)
{
	LLNET_SSL_DEBUG_TRACE("%s()\n", __func__);

#if defined(MBEDTLS_ENTROPY_C) && defined(MBEDTLS_CTR_DRBG_C)
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    int ret;
    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                      (const unsigned char *) DRBG_PERS,
                      sizeof (DRBG_PERS))) != 0)
    {
    	LLNET_SSL_DEBUG_MBEDTLS_TRACE("mbedtls_crt_drbg_init", ret);
    	SNI_throwNativeIOException(LLNET_SSL_TranslateReturnCode(ret), "MbedTLS init failed");
    	return;
    }
#endif
}

int32_t LLNET_SSL_SOCKET_IMPL_create(int32_t contextID, int32_t fd, uint8_t* hostname, int32_t hostnameLength, bool autoclose, uint8_t useClientMode, uint8_t needClientAuth)
{
	LLNET_SSL_DEBUG_TRACE("%s(context=%d, fd=%d, autoclose=%d, useClientMode=%d, needClientAuth=%d)\n", __func__, (int)contextID, (int)fd, autoclose, useClientMode, needClientAuth);
	mbedtls_ssl_config* conf = (mbedtls_ssl_config*)(contextID);
	mbedtls_ssl_context* ssl_ctx = (mbedtls_ssl_context*)mbedtls_calloc(1, sizeof(mbedtls_ssl_context));

	if ( conf != NULL && ssl_ctx != NULL)
	{
		/* It is possible to disable authentication by passing
		 * MBEDTLS_SSL_VERIFY_NONE in the call to mbedtls_ssl_conf_authmode()
		 */
		if (useClientMode || needClientAuth)
		{
			mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_REQUIRED);
		}
		else
		{
			mbedtls_ssl_conf_authmode(conf, MBEDTLS_SSL_VERIFY_NONE);
		}

		mbedtls_ssl_init(ssl_ctx);

		if ((NULL != hostname) && (hostnameLength > 0))
		{
			mbedtls_ssl_set_hostname(ssl_ctx, (char*)hostname);
		}

		int ret = 0;
		if ((ret = mbedtls_ssl_setup(ssl_ctx, conf)) != 0)
		{
		    mbedtls_ssl_free(ssl_ctx);
		    mbedtls_free((void*)ssl_ctx);
			LLNET_SSL_DEBUG_MBEDTLS_TRACE("mbedtls_ssl_setup", ret);
			SNI_throwNativeIOException(LLNET_SSL_TranslateReturnCode(ret), "MbedTLS SSL context setup failed");
			return SNI_IGNORED_RETURNED_VALUE;
		}

		int* net_socket = (int*)mbedtls_calloc(1, sizeof(int));
		if (NULL != net_socket)
		{
			*(net_socket) = fd;
			mbedtls_ssl_set_bio(ssl_ctx, (void*)net_socket, LLNET_SSL_utils_mbedtls_send, LLNET_SSL_utils_mbedtls_recv, NULL );
		}
		else
		{
			mbedtls_ssl_free(ssl_ctx);
		    mbedtls_free((void*)ssl_ctx);
			SNI_throwNativeIOException(J_MEMORY_ERROR, "Not enough memory");
			return SNI_IGNORED_RETURNED_VALUE;
		}
	}
	else
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return SNI_IGNORED_RETURNED_VALUE;
	}

	return (int32_t)ssl_ctx;
}


void LLNET_SSL_SOCKET_IMPL_shutdown(int32_t sslID, int32_t fd, bool autoclose, int64_t absoluteJavaStartTime, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d, autoclose=%d)\n", __func__, (int)sslID, (int)fd, autoclose);

	mbedtls_ssl_context* ssl_ctx = (mbedtls_ssl_context*)(sslID);
	if (NULL == ssl_ctx)
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return;
	}

	//send close notify
	int ret = mbedtls_ssl_close_notify(ssl_ctx);
	if (ret < 0)
	{
		if (ret != MBEDTLS_ERR_SSL_WANT_READ &&
			ret != MBEDTLS_ERR_SSL_WANT_WRITE)
		{
			LLNET_SSL_DEBUG_MBEDTLS_TRACE("mbedtls_ssl_close_notify", ret);
		}
	}

	if(!autoclose)
	{
		//the close of the underlying socket is not requested
		//read close_notify alert to clear input stream
		int8_t buffer[1];
		int32_t res;
		int64_t absoluteTimeout = (0 == relativeTimeout) ? 0 : absoluteJavaStartTime + relativeTimeout;
		do
		{
			res = LLNET_SSL_SOCKET_read(sslID, fd, buffer, 0, 1, absoluteJavaStartTime, absoluteTimeout, relativeTimeout, (SNI_callback)LLNET_SSL_SOCKET_close_callback);
			absoluteTimeout = (0 == relativeTimeout) ? 0 : LLNET_current_time_ms() + relativeTimeout;
		}
		while(res > 0);
	}
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d, autoclose=%d) free SSL\n", __func__, (int)sslID, (int)fd, (int)autoclose);
}


void LLNET_SSL_SOCKET_IMPL_freeSSL(int32_t sslID)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d)\n", __func__, (int)sslID);

	mbedtls_ssl_context* ssl_ctx = (mbedtls_ssl_context*)(sslID);
	if (NULL == ssl_ctx)
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return;
	}

	//free BIO
	if (ssl_ctx->p_bio)
	{
		mbedtls_free(ssl_ctx->p_bio);
	}

	//free SSL
    mbedtls_ssl_free(ssl_ctx);
    mbedtls_free((void*)ssl_ctx);
}

void LLNET_SSL_SOCKET_IMPL_initialClientHandShake(int32_t sslID, int32_t fd, int64_t absoluteJavaStartTime, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d)\n", __func__, (int)sslID, (int)fd);
	int64_t absoluteTimeout = (0 == relativeTimeout) ? 0 : absoluteJavaStartTime + relativeTimeout;
	LLNET_SSL_SOCKET_initialHandShake(sslID, fd, absoluteJavaStartTime, absoluteTimeout, relativeTimeout);
}

void LLNET_SSL_SOCKET_IMPL_initialServerHandShake(int32_t sslID, int32_t fd, int64_t absoluteJavaStartTime, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d)\n", __func__, (int)sslID, (int)fd);
	int64_t absoluteTimeout = (0 == relativeTimeout) ? 0 : absoluteJavaStartTime + relativeTimeout;
	LLNET_SSL_SOCKET_initialHandShake(sslID, fd, absoluteJavaStartTime, absoluteTimeout, relativeTimeout);
}

int32_t LLNET_SSL_SOCKET_IMPL_read(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d, offset=%d, length=%d)\n", __func__, (int)sslID, (int)fd, (int)off, (int)len);

	mbedtls_ssl_context* ssl_ctx = (mbedtls_ssl_context*)(sslID);
	if (NULL == ssl_ctx)
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	int64_t absoluteTimeout = (0 == relativeTimeout) ? 0 : absoluteJavaStartTime + relativeTimeout;
	return LLNET_SSL_SOCKET_read(sslID, fd, buf, off, len, absoluteJavaStartTime, absoluteTimeout, relativeTimeout, (SNI_callback)LLNET_SSL_SOCKET_read_callback);
}

int32_t LLNET_SSL_SOCKET_IMPL_write(int32_t sslID, int32_t fd, int8_t* buf, int32_t off, int32_t len, int64_t absoluteJavaStartTime, int32_t relativeTimeout)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d, fd=%d, offset=%d, length=%d)\n", __func__, (int)sslID, (int)fd, (int)off, (int)len);
	mbedtls_ssl_context* ssl_ctx = (mbedtls_ssl_context*)(sslID);
	if (NULL == ssl_ctx)
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return SNI_IGNORED_RETURNED_VALUE;
	}
	int64_t absoluteTimeout = (0 == relativeTimeout) ? 0 : absoluteJavaStartTime + relativeTimeout;
	return LLNET_SSL_SOCKET_write(sslID, fd, buf, off, len, absoluteJavaStartTime, absoluteTimeout, relativeTimeout);
}

int32_t LLNET_SSL_SOCKET_IMPL_available(int32_t sslID)
{
	LLNET_SSL_DEBUG_TRACE("%s(ssl=%d)\n", __func__, (int)sslID);

	mbedtls_ssl_context* ssl_ctx = (mbedtls_ssl_context*)(sslID);
	if (NULL == ssl_ctx)
	{
		SNI_throwNativeIOException(J_BAD_FUNC_ARG, "Invalid argument");
		return SNI_IGNORED_RETURNED_VALUE;
	}

    return mbedtls_ssl_get_bytes_avail(ssl_ctx);
}

#ifdef __cplusplus
	}
#endif
