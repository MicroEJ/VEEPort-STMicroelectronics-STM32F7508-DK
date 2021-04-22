/*
 * C
 *
 * Copyright 2015-2019 MicroEJ Corp. All rights reserved.
 * For demonstration purpose only.
 * MicroEJ Corp. PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef __LLSEC_X509_CERT_IMPL__
#define __LLSEC_X509_CERT_IMPL__

/**
 * @file
 * @brief MicroEJ Security low level API
 * @author MicroEJ Developer Team
 * @version 1.4.0
 * @date 30 January 2020
 */

#include <intern/LLSEC_X509_CERT_impl.h>
#include <sni.h>
#include <LLSEC_CONSTANTS.h>
#include <LLSEC_ERRORS.h>
#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Parses the given certificate to validate it and returns its encoded format type.
 *
 * @param[in] cert							The certificate buffer.
 * @param[in] off							The offset in the buffer at which the certificate content started.
 * @param[in] len							The certificate content length.
 *
 * @return The certificate encoded format type (0 if PEM format or 1 if DER format); {@link J_CERT_PARSE_ERROR} if the certificate is not valid or an error occurs.
 *
 * @warning <code>cert</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_X509_CERT_IMPL_parse(int8_t* cert, int32_t off, int32_t len);


/**
 * @brief Gets a public key from a X509 certificate.
 *
 * @param[in] cert_data						The X509 certificate.
 * @param[in] cert_data_length				The certificate length.
 * @param[out] key							The public key extracted from the certificate.
 * @param[in] keyDataLength					The length of keyData buffer.
 *
 * @return The number of bytes copied into key.
 *
 * @warning <code>cert_data</code> must not be used outside of the VM task or saved.
 * @warning <code>key</code> must not be used outside of the VM task or saved.
 *
 * @throws NativeException on error.
 */
int32_t LLSEC_X509_CERT_IMPL_get_key(int8_t* cert_data, int32_t cert_data_length, uint8_t* key, int32_t key_length);

/**
 * @brief Verifies a certificate against a verification key.
 *
 * @param[in] cert_data						The X509 certificate.
 * @param[in] cert_data_length				The certificate length.
 * @param[in] key							The verification key.
 * @param[in] key_length					The key length.
 *
 * @return 0 if the verification succeeded, throws a NativeException otherwise.
 *
 * @throws NativeException on error.
 */
int32_t LLSEC_X509_CERT_IMPL_verify(int8_t* cert_data, int32_t cert_data_length, uint8_t* key, int32_t key_length);

/**
 * @brief Extracts the subject or issuer X500Principal from an X509Certificate.
 *
 * @param[in] cert_data						The certificate.
 * @param[in] cert_data_length				The certificate length.
 * @param[out] principal_data				The principal data extracted from the certificate.
 * @param[in] principal_data_length			The length of principal_data.
 * @param[in] get_issuer					'1' to get the issuer, '0' to get the subject.
 *
 * @return The number of bytes copied into principal_data.
 *
 * @throws NativeException on error.
 */
int32_t LLSEC_X509_CERT_IMPL_get_x500_principal_data(int8_t* cert_data, int32_t cert_data_length, uint8_t* principal_data, int32_t principal_data_length, uint8_t get_issuer);

#ifdef __cplusplus
	}
#endif

#endif //__LLSEC_X509_CERT_IMPL__
