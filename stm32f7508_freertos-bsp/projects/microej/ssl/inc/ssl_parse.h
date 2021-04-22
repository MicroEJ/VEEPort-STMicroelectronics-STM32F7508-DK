/*
 * C
 *
 * Copyright 2020 IS2T. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break IS2T warranties on the whole library.
 */

#if !defined __SSL_PARSE_H__
# define __SSL_PARSE_H__

/*
 * This function gets a string from a java byte array
 * If the byte array contains a string that is not null terminated, this function will try to either:
 *  1. Insert a null character in the byte array,
 *  2. Allocate and duplicate the string in internal memory
 *  3. Allocate and duplicate the string in external memory
 *  4. return NULL
 *
 * @param array : Java array containing the string
 * @param offset : offset in the Java array where is located the string
 * @param len : length of the string
 *
 * @return
 *  - NULL:         if the string is not NULL terminated,
 *                  AND the NULL character could not be insterted,
 *                  AND the new string coult not be allocated.
 *
 *  - array+offset: if the string is already NULL terminated,
 *                  OR the NULL character could be insterted in the string.
 *
 *  - other value:  if the string has been duplicated and the NULL character appended,
 *                  this new string MUST be freed.
 *
 */
uint8_t * getStrFromArray(uint8_t * array, uint32_t offset, uint32_t len);

/*
 * This function parses a string certificate from a Java array
 *
 * @param cert : mbedtls_x509 certificate object, must be allocated by the caller
 * @param array : Java array containing the PEM certificate
 * @param offset : offset in the Java array where is located the PEM certificate
 * @param len : length of the PEM certificate
 *
 * RETURN:
 *  0: Success, certificate is successfully parsed,
 *  J_MEMORY_ERROR: failed getting string from the array,
 *  other values: failed parsing the certificate (see LLNET_SSL_ERRORS.h).
 */
int microej_x509_crt_parse( mbedtls_x509_crt *cert, uint8_t * array, uint32_t offset, uint32_t len);

#endif // !defined __SSL_PARSE_H__
