/*
 * C
 *
 * Copyright 2017-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_SIG_IMPL_H
#define LLSEC_SIG_IMPL_H

#include <intern/LLSEC_SIG_impl.h>
#include <sni.h>
#include <stdint.h>

/**
 * @file
 * @brief MicroEJ Security low level API
 * @author MicroEJ Developer Team
 * @version 2.4.0
 * @date 16 February 2024
 */

/**
 * @brief Gets for the given algorithm the message digest description.
 *
 * @param[in] algorithm_name				Null terminated string that describes the algorithm
 * @param[out] digest_algorithm_name		Null terminated string that describes the digest algorithm
 * @param[in] digest_algorithm_name_length	Length of digest_algorithm (in bytes)
 *
 * @return The algorithm ID on success or -1 on error.
 *
 * @warning <code>algorithm_name</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_SIG_IMPL_get_algorithm_description(uint8_t* algorithm_name, uint8_t* digest_algorithm_name, int32_t digest_algorithm_name_length);
/**
 * @brief Get algorithm OID "Object identifier".
 *
 * @param[in] algorithm_name the algorithm name;
 * @param[out] oid a pointer to an array holding a zero terminated string representation of the OID
 * @param[in] oid_length length of oid array
 *
 * @note Throws NativeException on error.
 */
void LLSEC_SIG_IMPL_get_algorithm_oid(uint8_t* algorithm_name, uint8_t* oid, int32_t oid_length);

/**
 * @brief Verifies a message.
 *
 * @param[in] algorithm_id					The algorithm ID.
 * @param[in] signature						The buffer containing the signature.
 * @param[in] signature_length				The signature length.
 * @param[in] public_key_id                 The public key pointer on C structure holding the key data.
 * @param[in] digest						The digest of the message to verify.
 * @param[in] digest_length					The digest length.
 *
 * @return JTRUE if the signature is valid, JFALSE otherwise.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>signature</code> must not be used outside of the VM task or saved.
 * @warning <code>key</code> must not be used outside of the VM task or saved.
 * @warning <code>digest</code> must not be used outside of the VM task or saved.
 *
 */
uint8_t LLSEC_SIG_IMPL_verify(int32_t algorithm_id, uint8_t* signature, int32_t signature_length, int32_t public_key_id, uint8_t* digest, int32_t digest_length);

/**
 * @brief Signs a message.
 *
 * @param[in]  algorithm_id					The algorithm ID.
 * @param[out] signature					The buffer containing the signature.
 * @param[in]  signature_length				The signature length.
 * @param[in]  private_key_id               The private key pointer on C structure holding the key data.
 * @param[in]  digest						The digest of the message to sign.
 * @param[in]  digest_length				The digest length.
 *
 * @return The length of the signature.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>signature</code> must not be used outside of the VM task or saved.
 * @warning <code>key</code> must not be used outside of the VM task or saved.
 * @warning <code>digest</code> must not be used outside of the VM task or saved.
 *
 */
int32_t LLSEC_SIG_IMPL_sign(int32_t algorithm_id, uint8_t* signature, int32_t signature_length, int32_t private_key_id, uint8_t* digest, int32_t digest_length);


#endif //LLSEC_SIG_IMPL_H
