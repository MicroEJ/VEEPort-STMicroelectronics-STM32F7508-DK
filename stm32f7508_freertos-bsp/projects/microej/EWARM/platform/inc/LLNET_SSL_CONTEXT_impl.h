/*
 * C
 *
 * Copyright 2015-2021 IS2T. All rights reserved.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 * For demonstration purpose only.
 */
#ifndef __LLNET_SSL_CONTEXT_IMPL__
#define __LLNET_SSL_CONTEXT_IMPL__

/**
 * @file
 * @brief MicroEJ SSL low level API
 * @author MicroEJ Developer Team
 * @version 5.1.1
 * @date 7 May 2021
 */

#include <sni.h>
#include <intern/LLNET_SSL_CONTEXT_impl.h>
#include <LLNET_SSL_CONSTANTS.h>
#include <LLNET_SSL_ERRORS.h>
#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Creates a new client SSL Context based on the given SSL/TLS protocol version.
 * @param protocol the SSL/TLS protocol version (0 for SSLv3, 1 for TLSv1, 2 for TLSv1.1 and 3 for TLSv1.2).
 * @param isClientContext controls whether the underlying context will be created to manage ssl client or ssl server sockets. 
 * <code>true</code> for client sockets; <code>false</code> for server sockets.
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call.
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return the new SSL Context ID on success, {@link J_CREATE_SSL_CONTEXT_ERROR} on error (for example if not enough space to create the context).
 */
int32_t LLNET_SSL_CONTEXT_IMPL_createContext(int32_t protocol, uint8_t isClientContext, uint8_t retry);

/**
 * Releases the SSL Context identified by the given contextID.
 * @param contextID the SSL context ID.
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code.
 * @see {@link LLNET_SSL_ERRORS} header file for error codes.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_closeContext(int32_t contextID, uint8_t retry);

/**
 * Adds the given certificate as trusted certificate into the given SSL context.
 * @param contextID the SSL Context ID.
 * @param cert the certificate buffer.
 * @param off the offset in the buffer at which the certificate content started.
 * @param len the certificate content length.
 * @param format the certificate encoded format type (0 for PEM format and 1 for DER format).
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code.
 * @see {@link LLNET_SSL_ERRORS} header file for error codes.
 * @warning cert must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_addTrustedCert(int32_t contextID, uint8_t *cert, int32_t off, int32_t len, int32_t format, uint8_t retry);

/**
 * Removes all trusted certificates from the given SSL context.
 * @param contextID the SSL Context ID.
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code.
 * @see {@link LLNET_SSL_ERRORS} header file for error codes.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_clearTrustStore(int32_t contextID, uint8_t retry);

/**
 * Removes all credentials (private key and associated certificates) from the given ssl context.
 * @param contextID the ssl context ID.
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code.
 * @see {@link LLNET_SSL_ERRORS} header file for error codes.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_clearKeyStore(int32_t contextID, uint8_t retry);

/**
 * Sets the certificate of the context private key.
 * @param contextID the ssl context ID.
 * @param cert the certificate of the context private key.
 * @param offset the offset in the buffer at which the certificate content started.
 * @param len the certificate content length.
 * @param format the certificate encoded format type (0 for PEM format and 1 for DER format).
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code.
 * @see {@link LLNET_SSL_ERRORS} header file for error codes.
 * @warning cert must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_setCertificate(int32_t contextID, uint8_t* cert, int32_t offset, int32_t len, int32_t format, uint8_t retry);

/**
 * Sets the context private key.
 * @param contextID the ssl context ID.
 * @param privateKey the context private key.
 * @param offset the offset in the buffer at which the private key content started.
 * @param len the private key content length.
 * @param password the password to recover the private key if it is encrypted.
 * @param passwordOffset the offset in the password buffer at which the password content started.
 * @param passwordLen the password length.
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code.
 * @see {@link LLNET_SSL_ERRORS} header file for error codes.
 * @warning privateKey must not be used outside of the VM task or saved.
 * @warning password must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_setPrivateKey(int32_t contextID, uint8_t* privateKey, int32_t offset, int32_t len, uint8_t* password,
		int32_t passwordOffset, int32_t passwordLen, uint8_t retry);

/**
 * Initializes the chain buffer and returns its total size.
 * @param contextID the ssl context ID.
 * @param nbChainCerts the number of certificates in the chain.
 * @param chainCertsTotalSize the total size of certificates in the chain.
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return the total size of the chain buffer on success or a negative error code.
 * @see {@link LLNET_SSL_ERRORS} header file for error codes.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_initChainBuffer(int32_t contextID, int32_t nbChainCerts, int32_t chainCertsTotalSize, uint8_t retry);

/**
 * Adds the given certificate to the certificate chain buffer associated with the context private key.
 * @param contextID the ssl context ID.
 * @param cert the certificate of the context private key.
 * @param offset the offset in the buffer at which the certificate content started
 * @param len the certificate content length
 * @param format the certificate encoded format type (0 for PEM format and 1 for DER format).
 * @param chainBufferSize the chain buffer size.
 * @param retry true if the calling process repeats the call to this operation for its completion when the previous call
 * has returned {@link J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT} to indicate that the operation was not completed.
 * @return {@link J_SSL_NO_ERROR} on success or a negative error code.
 * @see {@link LLNET_SSL_ERRORS} header file for error codes.
 * @warning cert must not be used outside of the VM task or saved.
 */
int32_t LLNET_SSL_CONTEXT_IMPL_addChainCertificate(int32_t contextID, uint8_t* cert, int32_t offset, int32_t len, int32_t format, int32_t chainBufferSize, uint8_t retry);

#ifdef __cplusplus
	}
#endif

#endif //__LLNET_SSL_CONTEXT_IMPL__
