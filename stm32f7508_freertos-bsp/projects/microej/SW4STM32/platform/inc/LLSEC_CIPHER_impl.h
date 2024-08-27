/*
 * C
 *
 * Copyright 2017-2024 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_CIPHER_IMPL_H
#define LLSEC_CIPHER_IMPL_H

#include <intern/LLSEC_CIPHER_impl.h>
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
	ECB_MODE = 0,
	CBC_MODE = 1,
	CFB_MODE = 2,
	OFB_MODE = 3,
	PCBC_MODE = 4,
	CTR_MODE = 5,
	CTS_MODE = 6,
	GCM_MODE = 7
}LLSEC_CIPHER_mode;

typedef struct
{
	uint32_t block_size; // the block size (in bytes), or 0 if the underlying algorithm is not a block cipher
	uint32_t unit_bytes; // unit size (number of input bytes that can be processed at a time)
	LLSEC_CIPHER_mode cipher_mode; // one of the <code>*_MODE</code> values defined in {@link AbstractCipher}
} LLSEC_CIPHER_transformation_desc;

/**
 * @brief Gets for the given transformation the cipher description.
 * <p>
 * <code>transformation_desc</code> must be filled-in with:
 * <ul>
 * 	<li>[0-3]: native transformation ID</li>
 * 	<li>[4-7]: blockSize: the block size (in bytes), or 0 if the underlying algorithm is not a block cipher</li>
 * 	<li>[8-11]: unitBytes: unit size (number of input bytes that can be processed at a time)</li>
 * 	<li>[12-15]: cipherMode: one of the <code>*_MODE</code> values defined in {@link AbstractCipher}</li>
 * </ul>
 *
 * @param transformation_name[in]		Null terminated string that describes the transformation.
 * @param transformation_desc[out]		Description of the cipher.
 *
 * @return The transformation ID on success or -1 on error.
 *
 * @warning <code>transformation_name</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_CIPHER_IMPL_get_transformation_description(uint8_t* transformation_name, LLSEC_CIPHER_transformation_desc* transformation_desc);

/**
 * @brief Returns the number of bytes that are buffered internally inside the given cipher.
 *
 * @param[in] nativeTransformationId	The transformation ID.
 * @param[in] nativeId					The resource's native ID.
 *
 * @return The length of the buffer.
 */
int32_t LLSEC_CIPHER_IMPL_get_buffered_length(int32_t nativeTransformationId, int32_t nativeId);

/**
 * @brief Gets the initialization vector.
 *
 * @param[in] transformation_id			The transformation ID.
 * @param[in] native_id					The resource's native ID.
 * @param[out] iv						The initialization vector of the cipher.
 * @param[in] iv_length				The initialization vector size.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>iv</code> must not be used outside of the VM task or saved.
 */
void LLSEC_CIPHER_IMPL_get_IV(int32_t transformation_id, int32_t native_id, uint8_t* iv, int32_t iv_length);

/**
 * @brief Sets the initialization vector.
 *
 * @param[in] transformation_id			The transformation ID.
 * @param[in] native_id					The resource's native ID.
 * @param[in] iv						The initialization vector of the cipher.
 * @param[in] iv_length				The initialization vector size.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>iv</code> must not be used outside of the VM task or saved.
 */
void LLSEC_CIPHER_IMPL_set_IV(int32_t transformation_id, int32_t native_id, uint8_t* iv, int32_t iv_length);

/**
 * @brief Gets the length of the initialized vector.
 *
 * @param[in] transformation_id			The transformation ID.
 * @param[in] native_id					The resource's native ID.
 *
 * @return The length of the IV, -1 if there is no IV.
 */
int32_t LLSEC_CIPHER_IMPL_get_IV_length(int32_t transformation_id, int32_t native_id);

/**
 * @brief Initializes a Cipher resource.
 *
 * @param[in] tranformation_id			The transformation ID.
 * @param[in] is_decrypting				'1' for decrypting, '0' for encryting.
 * @param[in] key						The key.
 * @param[in] key_length				The key size.
 * @param[in] iv						The initialization vector.
 * @param[in] iv_length					The initialization vector size.
 *
 * @return The nativeId of the newly initialized resource.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>key</code> must not be used outside of the VM task or saved.
 * @warning <code>iv</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_CIPHER_IMPL_init(int32_t transformation_id, uint8_t is_decrypting, uint8_t* key, int32_t key_length, uint8_t* iv, int32_t iv_length);

/**
 * @brief Decrypts the message contained in <code>buffer</code>.
 *
 * @param[in] transformation_id			The transformation ID.
 * @param[in] native_id					The resource's native ID.
 * @param[in] buffer					The buffer containing the message to decrypt.
 * @param[in] buffer_offset				The buffer offset.
 * @param[in] buffer_length				The buffer length.
 * @param[out] output					The output buffer containing the plaintext message.
 * @param[out] output_offset			The output offset.
 *
 * @return The length of the buffer.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>buffer</code> must not be used outside of the VM task or saved.
 * @warning <code>output</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_CIPHER_IMPL_decrypt(int32_t transformation_id, int32_t native_id, uint8_t* buffer, int32_t buffer_offset, int32_t buffer_length, uint8_t* output, int32_t output_offset);

/**
 * @brief Encrypts the message contained in <code>buffer</code>.
 *
 * @param[in] transformation_id			The transformation ID.
 * @param[in] native_id					The resource's native ID.
 * @param[in] buffer					The buffer containing the plaintext message to encrypt.
 * @param[in] buffer_offset				The buffer offset.
 * @param[in] buffer_length				The buffer length.
 * @param[out] output					The output buffer containing the encrypted message.
 * @param[in] output_offset				The output offset.
 *
 * @return The length of the buffer.
 *
 * @note Throws NativeException on error.
 *
 * @warning <code>buffer</code> must not be used outside of the VM task or saved.
 * @warning <code>output</code> must not be used outside of the VM task or saved.
 */
int32_t LLSEC_CIPHER_IMPL_encrypt(int32_t transformation_id, int32_t native_id, uint8_t* buffer, int32_t buffer_offset, int32_t buffer_length, uint8_t* output, int32_t output_offset);


/**
 * @brief Closes the resource related to the native ID.
 *
 * @param[in] transformation_id			The transformation ID.
 * @param[in] native_id					The resource's native ID.
 *
 * @note Throws NativeException on error.
 */
void LLSEC_CIPHER_IMPL_close(int32_t transformation_id, int32_t native_id);

/**
 * @brief Gets the id of the native close function.
 * @param[in] transformation_id			The transformation ID.
 *
 * @return the id of the static native close function.
 *
 * @note Throws NativeException on error.
 */
int32_t LLSEC_CIPHER_IMPL_get_close_id(int32_t transformation_id);

#endif //LLSEC_CIPHER_IMPL_H
