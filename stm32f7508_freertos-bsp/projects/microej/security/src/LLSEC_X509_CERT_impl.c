/*
 * C
 *
 * Copyright 2021-2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief MicroEJ Security low level API implementation for MbedTLS Library.
 * @author MicroEJ Developer Team
 * @version 1.5.0
 * @date 15 March 2024
 */

#include <LLSEC_X509_CERT_impl.h>
#include <LLSEC_configuration.h>
#include <sni.h>
#include <stdlib.h>
#include <string.h>

#include "LLSEC_mbedtls.h"
#include "mbedtls/platform.h"
#include "mbedtls/ssl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LLSEC_X509_UNKNOWN_FORMAT ((int)(-1))

static mbedtls_x509_crt* get_x509_certificate(int8_t* cert_data, int32_t len, int32_t* cert_format);
static int32_t LLSEC_X509_CERT_mbedtls_close_key(int32_t native_id);

static mbedtls_x509_crt* get_x509_certificate(int8_t* cert_data, int32_t len, int32_t* cert_format) {
    LLSEC_X509_DEBUG_TRACE("%s 00. cert_len:%d\n", __func__, (int)len);

    mbedtls_x509_crt* new_cert = NULL;
    int mbedtls_rc = LLSEC_MBEDTLS_SUCCESS;

    /* Allocate a new X509 certificate */
    new_cert = (mbedtls_x509_crt*)mbedtls_calloc(1, sizeof(mbedtls_x509_crt));
    if (NULL == new_cert) {
        if (NULL != cert_format) {
            *cert_format = J_MEMORY_ERROR;
        }
    }

    if (NULL != new_cert) {
        /* Initialize the X509 certificate */
        mbedtls_x509_crt_init(new_cert);

        /* Parse the X509 DER certificate */
        mbedtls_rc = mbedtls_x509_crt_parse_der(new_cert, (const uint8_t*)cert_data, len);
        if (LLSEC_MBEDTLS_SUCCESS == mbedtls_rc) {
            /* Encoded DER certificate */
            if (cert_format != NULL) {
                *cert_format = CERT_DER_FORMAT;
            }
        } else {
            LLSEC_X509_DEBUG_TRACE("%s. mbedtls_x509_crt_parse_der fail, return_code: %d\n", __func__, mbedtls_rc);
        }
    }

    if (NULL != new_cert) {
        /* Parse the X509 PEM certificate */
        mbedtls_x509_crt_init(new_cert);

        /* To avoid tmp_cert_data is not a string, which causes mbedtls_x509_crt_parse error */
        int8_t* tmp_cert_data = (int8_t*)mbedtls_calloc(1, len + 1);
        (void)memcpy(tmp_cert_data, cert_data, len);
        tmp_cert_data[len] = '\0';

        /* Parse the X509 PEM certificate */
        mbedtls_rc = mbedtls_x509_crt_parse(new_cert, (const uint8_t*)tmp_cert_data, len + 1);
        mbedtls_free(tmp_cert_data);
        if (LLSEC_MBEDTLS_SUCCESS == mbedtls_rc) {
            /* Encoded PEM certificate */
            if (cert_format != NULL) {
                *cert_format = CERT_PEM_FORMAT;
            }
        } else {
            LLSEC_X509_DEBUG_TRACE("%s. mbedtls_x509_crt_parse(PEM) fail, return_code: %d\n", __func__, mbedtls_rc);
        }
    }

    if ((NULL != new_cert) && (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc)) {
        /* Certificate is not PEM/DER */
        if (NULL != cert_format) {
            *cert_format = J_CERT_PARSE_ERROR;
        }
    }

    return new_cert;
}

static int32_t LLSEC_X509_CERT_mbedtls_close_key(int32_t native_id) {
    LLSEC_X509_DEBUG_TRACE("%s \n", __func__);
    int return_code = LLSEC_SUCCESS;
    LLSEC_pub_key* key = (LLSEC_pub_key*)native_id;

    if (TYPE_RSA == key->type) {
        mbedtls_rsa_free((mbedtls_rsa_context*)key->key);
    } else {
        mbedtls_ecdsa_free((mbedtls_ecdsa_context*)key->key);
    }

    mbedtls_free(key);
    return return_code;
}

int32_t LLSEC_X509_CERT_IMPL_parse(int8_t* cert, int32_t off, int32_t len) {
    LLSEC_X509_DEBUG_TRACE("%s(cert=%p, off=%d, len=%d)\n", __func__, cert, (int)off, (int)len);

    int32_t format = LLSEC_X509_UNKNOWN_FORMAT;
    int8_t* cert_data = &cert[off];

    mbedtls_x509_crt* tmp_cert = get_x509_certificate(cert_data, len, &format);

    /* Free the X509 certificate */
    if (NULL != tmp_cert) {
        mbedtls_x509_crt_free(tmp_cert);
        mbedtls_free(tmp_cert);
    }

    return format;
}

/**
 *
 * @param cert
 * @param certLen
 * @param keyData
 * @param keyDataLength inparameter. Contains the length of keyData.
 * @return the number of bytes copied into keyData
 *
 ** Warning: cert_data must not be used outside of the VM task or saved
 ** Warning: key must not be used outside of the VM task or saved
 *
 * @throws NativeException on error.
 */
int32_t LLSEC_X509_CERT_IMPL_get_x500_principal_data(int8_t* cert_data, int32_t cert_data_length, uint8_t* principal_data, int32_t principal_data_length, uint8_t get_issuer) {
    int32_t return_code = LLSEC_SUCCESS;
    LLSEC_X509_DEBUG_TRACE("%s(cert=%p, Cert_len=%d,prin_len=%d,get_issuer=%d)\n", __func__, cert_data, (int)cert_data_length, (int)principal_data_length, (int)get_issuer);

    mbedtls_x509_crt* x509 = get_x509_certificate(cert_data, cert_data_length, NULL);
    if (NULL == x509) {
        (void)SNI_throwNativeException(LLSEC_ERROR, "Bad x509 certificate");
        return_code = LLSEC_ERROR;
    }

    if(LLSEC_SUCCESS == return_code) {
        int len;
        char buf[256];
        if ((uint8_t) 0 != get_issuer) {
            len = mbedtls_x509_dn_gets(buf, sizeof(buf), &(x509->issuer));
        } else {
            len = mbedtls_x509_dn_gets(buf, sizeof(buf), &(x509->subject));
        }

        if (len > principal_data_length) {
            (void)SNI_throwNativeException(LLSEC_ERROR, "Principal data buffer is too small");
            return_code = LLSEC_ERROR;
        } else {
            (void)memcpy(principal_data, &buf[0], len);

            /* Free the X509 certificate */
            mbedtls_x509_crt_free(x509);
            mbedtls_free(x509);

            return_code = len;
        }
    }

    return return_code;
}

/**
 *
 * @param cert
 * @param certLen
 * @param keyData
 * @param keyDataLength inparameter. Contains the length of keyData.
 * @return the number of bytes copied into keyData
 *
 ** Warning: cert_data must not be used outside of the VM task or saved
 ** Warning: key must not be used outside of the VM task or saved
 *
 * @throws NativeException on error.
 */
int32_t LLSEC_X509_CERT_IMPL_get_key(int8_t* cert_data, int32_t cert_data_length) {
    int32_t return_code = LLSEC_SUCCESS;
    LLSEC_X509_DEBUG_TRACE("%s(cert=%p, len=%d)\n", __func__, cert_data, (int)cert_data_length);
    LLSEC_pub_key* pub_key = (LLSEC_pub_key*)mbedtls_calloc(1, sizeof(LLSEC_pub_key));
    mbedtls_x509_crt* x509 = NULL;
    void* native_id = NULL;

    if (NULL == pub_key) {
        (void)SNI_throwNativeException(LLSEC_ERROR, "Can't allocate LLSEC_pub_key structure");
        return_code = LLSEC_ERROR;
    }

    if(LLSEC_SUCCESS == return_code) {
        x509 = get_x509_certificate(cert_data, cert_data_length, NULL);
        if (NULL == x509) {
            (void)SNI_throwNativeException(LLSEC_ERROR, "Bad x509 certificate");
            return_code = LLSEC_ERROR;
        }
    }

    if(LLSEC_SUCCESS == return_code) {
        //Note:key TYPE: mbedtls_rsa_context or mbedtls_ecdsa_context
        if (MBEDTLS_PK_RSA == mbedtls_pk_get_type(&x509->pk)) {
            pub_key->type = TYPE_RSA;
            pub_key->key = (char*)mbedtls_pk_rsa(x509->pk);
        } else {
            pub_key->type = TYPE_ECDSA;
            pub_key->key = (char*)mbedtls_pk_ec(x509->pk);
        }

        if (NULL == pub_key->key) {
            (void)SNI_throwNativeException(LLSEC_ERROR, "Invalid public key from x509 certificate");
            return_code = LLSEC_ERROR;
        }
    }

    if(LLSEC_SUCCESS == return_code) {
        native_id = (void*)pub_key;
        if (SNI_OK != SNI_registerResource(native_id, (SNI_closeFunction)LLSEC_X509_CERT_mbedtls_close_key, NULL)) {
            (void)SNI_throwNativeException(LLSEC_ERROR, "Can't register SNI native resource");
            if (TYPE_RSA == pub_key->type) {
                mbedtls_rsa_free((mbedtls_rsa_context*)pub_key->key);
            } else {
                mbedtls_ecdsa_free((mbedtls_ecdsa_context*)pub_key->key);
            }
            return_code = LLSEC_ERROR;
        }
    }

    if(LLSEC_SUCCESS == return_code) {
        // cppcheck-suppress misra-c2012-11.6 // Abstract data type for SNI usage
        return_code = (uint32_t)native_id;
    } else {
        if(NULL != x509) {
            mbedtls_x509_crt_free(x509);
            mbedtls_free(x509);
        }
        if(NULL != pub_key) {
            mbedtls_free(pub_key);
        }
    }

    return return_code;
}

int32_t LLSEC_X509_CERT_IMPL_verify(int8_t* cert_data, int32_t cert_data_length, int32_t public_key_id) {
    LLSEC_UNUSED_PARAM(public_key_id);

    LLSEC_X509_DEBUG_TRACE("%s \n", __func__);
    int return_code = LLSEC_SUCCESS;

    mbedtls_x509_crt* x509 = get_x509_certificate(cert_data, cert_data_length, NULL);
    if (NULL == x509) {
        (void)SNI_throwNativeException(LLSEC_ERROR, "Bad x509 certificate");
        return_code = LLSEC_ERROR;
    }

    if (LLSEC_SUCCESS == return_code) {
        uint32_t flags;
        int mbedtls_rc = mbedtls_x509_crt_verify(x509, NULL, NULL, NULL, &flags, NULL, NULL);
        if (LLSEC_MBEDTLS_SUCCESS != mbedtls_rc) {
            LLSEC_X509_DEBUG_TRACE("LLSEC_X509 > verify error");
            (void)SNI_throwNativeException(LLSEC_ERROR, "Error x509 verify failed");
            return_code = LLSEC_ERROR;
        }
    }

    if(NULL != x509) {
        mbedtls_x509_crt_free(x509);
        mbedtls_free(x509);
    }

    return return_code;
}

/**
 * @brief Checks that the certificate is currently valid, i.e. the current time is within the specified validity period.
 *
 * @param[in] cert_data						The X509 certificate.
 * @param[in] cert_data_length				The certificate length.
 *
 * @return {@link J_SEC_NO_ERROR} on success, {@link J_X509_CERT_EXPIRED_ERROR} if certificate expired,
 * {@link J_X509_CERT_NOT_YET_VALID_ERROR} if certificate is not yet valid.
 *
 * @throws NativeException on error.
 */
int32_t LLSEC_X509_CERT_IMPL_check_validity(int8_t* cert_data, int32_t cert_data_length) {
    LLSEC_X509_DEBUG_TRACE("%s \n", __func__);
    int return_code = LLSEC_SUCCESS;

    mbedtls_x509_crt* x509 = get_x509_certificate(cert_data, cert_data_length, NULL);
    if (NULL == x509) {
        (void)SNI_throwNativeException(LLSEC_ERROR, "Bad x509 certificate");
        return_code = LLSEC_ERROR;
    }

    if (LLSEC_SUCCESS == return_code) {
        int mbedtls_is_past = mbedtls_x509_time_is_past(&x509->valid_to);
        int mbedtls_is_future = mbedtls_x509_time_is_future(&x509->valid_from);
        if (LLSEC_MBEDTLS_SUCCESS != mbedtls_is_past) {
            LLSEC_X509_DEBUG_TRACE("LLSEC_X509 time is past");
            return_code = J_X509_CERT_EXPIRED_ERROR;
        } else if (LLSEC_MBEDTLS_SUCCESS != mbedtls_is_future) {
            LLSEC_X509_DEBUG_TRACE("LLSEC_X509 time is future");
            return_code = J_X509_CERT_NOT_YET_VALID_ERROR;
        } else {
            return_code = J_SEC_NO_ERROR;
        }
    }

    if(NULL != x509) {
        mbedtls_x509_crt_free(x509);
        mbedtls_free(x509);
    }

    return return_code;
}

/**
 * @brief Get the pointer for the close key  method to be used as a close resource callback with SNI.
 *
 * @return the pointer for the close method.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_X509_CERT_IMPL_get_close_key(void) {
    LLSEC_X509_DEBUG_TRACE("%s \n", __func__);
    return (int32_t)LLSEC_X509_CERT_mbedtls_close_key;
}
