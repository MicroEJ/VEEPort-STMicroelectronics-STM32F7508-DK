/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sni.h>

#include <LLNET_SSL_ERRORS.h>
#include <LLNET_SSL_utils_mbedtls.h>
#include "LLNET_CONSTANTS.h"
#include <net_ssl_errors.h>

#include "mbedtls/x509_crt.h"

uint8_t * getStrFromArray(uint8_t * array, uint32_t offset, uint32_t * len) {
	uint8_t * p_str;

	p_str = (uint8_t *) array + offset;

	/* Check if private key is null terminated */
	if (p_str[*len - 1] != '\0') {
		int32_t trueLength = SNI_getArrayLength(array) - offset;

		/* Key should be one byte longer to store the null terminated key */
		(*len)++;

		/* Check if there is enough space in the byte array to insert a null character */
		if (trueLength > *len) {
			p_str[*len - 1] = '\0';
		} else {
			/*
			 * Duplicate the private key but with a null terminated character,
			 * /!\ this new string must be freed
			 */

			/*
			 * Unfortunately, strndup cannot be used directly because it would call libc malloc
			 * This allocator uses internal memory which might be too small.
			 * p_str = (uint8_t *) strndup((char *) array + offset, len-1);
			 */

			/* Try first internal memory because it is faster */
			p_str = pvPortMalloc(*len);

			if (NULL == p_str) {
				return NULL;
			}

#if defined( __GNUC__ )
			/* strlpy inserts the NULL character. see man page. */
			strlcpy((char *)p_str, (char *) array + offset, *len);
#else
			/* strlpy is not in any ISO standard, so use strncpy to copy (len - 1) bytes. */
			strncpy((char *)p_str, (char *) array + offset, *len - 1);

			/* add the NULL character. */
			p_str[*len - 1] = '\0';
#endif
		}
	}

	return (uint8_t *) p_str;
}

int microej_x509_crt_parse( mbedtls_x509_crt *cert, uint8_t * array, uint32_t offset, uint32_t len) {
	LLNET_SSL_DEBUG_TRACE("%s()\n", __func__);
	uint8_t * strCert = getStrFromArray(array, offset, &len);

	if (NULL == strCert) {
		return J_MEMORY_ERROR;
	}

	int ret = mbedtls_x509_crt_parse(cert, strCert, (uint32_t) len);

	// Free strCert if it has been allocated by getStrFromArray
	if (strCert != array + offset) {
		vPortFree(strCert);
	}

	return NET_SSL_TranslateReturnCode(ret);
}
