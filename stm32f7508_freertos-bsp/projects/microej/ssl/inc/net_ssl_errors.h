/*
 * C
 *
 * Copyright 2020 IS2T. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break IS2T warranties on the whole library.
 */

#if !defined __NET_SSL_ERRORS_H__
# define __NET_SSL_ERRORS_H__

/*
 * This function translate a mbedtls error to a Java error
 *
 * @param mbedtls_error : mbedtls error
 *
 * RETURN:
 *  a J_ERROR: the error is known,
 *  J_UNKNOWN_ERROR: for unknow mbedtls error
 */
jint NET_SSL_TranslateReturnCode(int32_t mbedtls_error);

#endif // !defined __NET_SSL_ERRORS_H__
