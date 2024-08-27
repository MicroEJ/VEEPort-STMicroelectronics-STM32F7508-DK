/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_RSA_CIPHER_IMPL_H
#define LLSEC_RSA_CIPHER_IMPL_H

#include <intern/LLSEC_RSA_CIPHER_impl.h>
#include <sni.h>
#include <stdint.h>

/**
 * @file
 * @brief MicroEJ Security low level API
 * @author MicroEJ Developer Team
 * @version 2.4.0
 * @date 16 February 2024
 */

typedef enum
{
    PAD_PKCS1_TYPE = 0,
    PAD_OAEP_MGF1_TYPE = 1
}LLSEC_RSA_CIPHER_padding_type;

typedef enum
{
    OAEP_HASH_SHA_1_ALGORITHM = 0,
    OAEP_HASH_SHA_256_ALGORITHM = 1
}LLSEC_RSA_CIPHER_oaep_hash_algorithm;

typedef struct
{
    LLSEC_RSA_CIPHER_padding_type padding_type; // one of the <code>PAD_*</code> values defined in {@link AbstractRSACipherSpi}
    LLSEC_RSA_CIPHER_oaep_hash_algorithm oaep_hash_algorithm; // one of the <code>OAEP_HASH_*</code> values defined in {@link AbstractRSACipherSpi}
} LLSEC_RSA_CIPHER_transformation_desc;

/**
 * @brief Gets for the given transformation the RSA cipher description.
 * <p>
 * <code>transformation_desc</code> must be filled-in with:
 * <ul>
 *     <li>[0-3]: native transformation ID</li>
 *     <li>[4-7]: padding_type: one of the <code>PAD_*</code> values defined in {@link AbstractRSACipherSpi}</li>
 *     <li>[8-11]: oaep_hash_algorithm: one of the <code>OAEP_HASH_*</code> values defined in {@link AbstractRSACipherSpi}</li>
 * </ul>
 *
 * @param transformation_name[in]        Null terminated string that describes the transformation.
 * @param transformation_desc[out]        Description of the cipher.
 *
 * @return The transformation ID on success or -1 on error.
 *
 * @warning <code>transformation_name</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_get_transformation_description(uint8_t* transformation_name, LLSEC_RSA_CIPHER_transformation_desc* transformation_desc);

/**
 * @brief Initializes a RSA Cipher resource.
 *
 * @param[in] tranformation_id            The transformation ID.
 * @param[in] is_decrypting                '1' for decrypting, '0' for encryting.
 * @param[in] key_id                    The key id (either a {@link NativePublicKey} or a {@link NativePrivateKey}).
 * @param[in] padding_type                The RSA padding type.
 * @param[in] oaep_hash_algorithm        The hash algorithm for OAEP RSA padding type.
 *
 * @return The nativeId of the newly initialized resource.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_init(int32_t transformation_id, uint8_t is_decrypting, int32_t key_id, int32_t padding_type, int32_t oaep_hash_algorithm);

/**
 * @brief Decrypts the message contained in <code>buffer</code>.
 *
 * @param[in] transformation_id            The transformation ID.
 * @param[in] native_id                    The resource's native ID.
 * @param[in] buffer                    The buffer containing the message to decrypt.
 * @param[in] buffer_offset                The buffer offset.
 * @param[in] buffer_length                The buffer length.
 * @param[out] output                    The output buffer containing the plaintext message.
 * @param[out] output_offset            The output offset.
 *
 * @return The length of the buffer.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>buffer</code> must not be used outside of the VM task or saved.
 * @warning <code>output</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_decrypt(int32_t transformation_id, int32_t native_id, uint8_t* buffer, int32_t buffer_offset, int32_t buffer_length, uint8_t* output, int32_t output_offset);

/**
 * @brief Encrypts the message contained in <code>buffer</code>.
 *
 * @param[in] transformation_id            The transformation ID.
 * @param[in] native_id                    The resource's native ID.
 * @param[in] buffer                    The buffer containing the plaintext message to encrypt.
 * @param[in] buffer_offset                The buffer offset.
 * @param[in] buffer_length                The buffer length.
 * @param[out] output                    The output buffer containing the encrypted message.
 * @param[in] output_offset                The output offset.
 *
 * @return The length of the buffer.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>buffer</code> must not be used outside of the VM task or saved.
 * @warning <code>output</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_encrypt(int32_t transformation_id, int32_t native_id, uint8_t* buffer, int32_t buffer_offset, int32_t buffer_length, uint8_t* output, int32_t output_offset);

/**
 * @brief Closes the resource related to the native ID.
 *
 * @param[in] transformation_id            The transformation ID.
 * @param[in] native_id                    The resource's native ID.
 *
 * @note Throws NativeException on error.
 */
void LLSEC_RSA_CIPHER_IMPL_close(int32_t transformation_id, int32_t native_id);

/**
 * @brief Gets the id of the native close function.
 * @param[in] transformation_id            The transformation ID.
 *
 * @return the id of the static native close function.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_RSA_CIPHER_IMPL_get_close_id(int32_t transformation_id);

#endif //LLSEC_RSA_CIPHER_IMPL_H
