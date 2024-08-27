/*
 * C
 *
 * Copyright 2015-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef __LLNET_SSL_CONTEXT_IMPL__
#define __LLNET_SSL_CONTEXT_IMPL__

/**
 * @file
 * @brief MicroEJ SSL low level API
 * @author MicroEJ Developer Team
 * @version 6.0.0
 * @date 26 August 2022
 */

#include <stdint.h>

#include <intern/LLNET_SSL_CONTEXT_impl.h>
#include <LLNET_SSL_CONSTANTS.h>
#include <LLNET_SSL_ERRORS.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Creates a new client ssl context based on the given SSL/TLS protocol version.
 *
 * @param[in] protocol			The SSL/TLS protocol version (0 for SSLv3, 1 for TLSv1, 2 for
 * 								TLSv1.1, 3 for TLSv1.2, 4 for DTLSv1.0, 5 for DTLS v1.2, and 6 for TLSv1.3).
 * @param[in] isClientContext	Controls whether the underlying context will be created to manage ssl client or ssl
 * 								server sockets. 1 for client sockets; 0 for server sockets.
 *
 * @return The new ssl context ID on success.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 * @see LLNET_SSL_CONSTANTS.h header file for SSL/TLS protocols.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_createContext(int32_t protocol, uint8_t isClientContext);

/**
 * @brief Releases the ssl context identified by the given contextID.
 *
 * @param[in] contextID	The ssl context ID.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
void LLNET_SSL_CONTEXT_IMPL_freeContext(int32_t contextID);

/**
 * @brief Adds the given certificate as trusted certificate to the ssl context.
 *
 * @param[in] contextID			The ssl context ID.
 * @param[in] certificate		The certificate to be added.
 * @param[in] certificateSize	The certificate size.
 * @param[in] format			The certificate encoded format (0 for PEM format and 1 for DER format).
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 *
 * @warning certificate must not be used outside of the VM task or saved.
 */
void LLNET_SSL_CONTEXT_IMPL_addTrustedCertificate(int32_t contextID, uint8_t *certificate, int32_t certificateSize, int32_t format);

/**
 * @brief Removes all trusted certificates from the ssl context.
 *
 * @param[in] contextID	The ssl context ID.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
void LLNET_SSL_CONTEXT_IMPL_clearTrustStore(int32_t contextID);

/**
 * @brief Removes all credentials (private key and associated certificates) from the ssl context.
 *
 * @param[in] contextID The ssl context ID.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
void LLNET_SSL_CONTEXT_IMPL_clearKeyStore(int32_t contextID);

/**
 * @brief Sets the certificate to be used in the ssl context.
 *
 * @param[in] contextID			The ssl context ID.
 * @param[in] certificate		The certificate of the private key.
 * @param[in] certificateSize	The certificate size.
 * @param[in] format			The certificate encoded format (0 for PEM format and 1 for DER format).
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 *
 * @warning certificate must not be used outside of the VM task or saved.
 */
void LLNET_SSL_CONTEXT_IMPL_setCertificate(int32_t contextID, uint8_t* certificate, int32_t certificateSize, int32_t format);

/**
 * @brief Sets the private key to be used in the ssl context.
 * The provided private key must be in DER-encoded encrypted PKCS#8 format.
 *
 * @param[in] contextID		The ssl context ID.
 * @param[in] privateKey	The private key (in DER-encoded encrypted PKCS#8 format).
 * @param[in] privateKeyLen	The length of private key in bytes.
 * @param[in] password		The password string to be used to recover the encrypted PKCS#8 private key. This password is a null-terminated string.
 * @param[in] passwordLen	The length of the password in bytes. The length does not include the terminating null byte.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 *
 * @warning privateKey must not be used outside of the VM task or saved.
 * @warning password must not be used outside of the VM task or saved.
 */
void LLNET_SSL_CONTEXT_IMPL_setPrivateKey(int32_t contextID, uint8_t* privateKey, int32_t privateKeyLen, uint8_t* password, int32_t passwordLen);

/**
 * @brief Initializes the certificates chain buffer and returns its total size.
 *
 * @param[in] contextID				The ssl context ID.
 * @param[in] nbChainCerts			The number of certificates in the chain.
 * @param[in] chainCertsTotalSize	The total size of certificates in the chain.
 *
 * @return The total size of the chain buffer on success or a negative error code.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_initChainBuffer(int32_t contextID, int32_t nbChainCerts, int32_t chainCertsTotalSize);

/**
 * @brief Adds the given certificate to the certificates's chain of the ssl context
 *
 * @param[in] contextID			The ssl context ID.
 * @param[in] certificate		The certificate to be added in the chain.
 * @param[in] certificateSize	The certificate size.
 * @param[in] format			The certificate encoded format (0 for PEM format and 1 for DER format).
 * @param[in] chainBufferSize	The chain buffer size.
 *
 * @note Throws NativeIOException on error.
 *
 * @see LLNET_SSL_ERRORS.h header file for error codes.
 *
 * @warning certificate must not be used outside of the VM task or saved.
 */
void LLNET_SSL_CONTEXT_IMPL_addChainCertificate(int32_t contextID, uint8_t* certificate, int32_t certificateSize, int32_t format, int32_t chainBufferSize);

#ifdef __cplusplus
	}
#endif

#endif //__LLNET_SSL_CONTEXT_IMPL__
