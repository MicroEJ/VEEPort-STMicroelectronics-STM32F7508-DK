/*
 * C
 *
 * Copyright 2020 IS2T. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break IS2T warranties on the whole library.
 */
#include <string.h>
#include "mbedtls_config.h"
#include "mbedtls/entropy_poll.h"
#include "ssl_utils.h"
#include "microej.h"
#include "lwip_util.h"
#include "lwip/sys.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @brief External function used to retrieve currentTime (defined in LLMJVM)
 */
extern int64_t LLMJVM_IMPL_getCurrentTime__Z(uint8_t system);

/* ---- Specific C Time function ---- */

time_t custom_mbedtls_time_sec(time_t *time)
{
	time_t currentTime = (time_t)(LLMJVM_IMPL_getCurrentTime__Z(MICROEJ_FALSE) / 1000);

	if (time != NULL)
	{
		*time = currentTime;
	}

	return currentTime;
}

#ifdef MBEDTLS_ENTROPY_HARDWARE_ALT

int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen)
{
	for (uint32_t index = 0; index < len/4; index++)
	{
		*olen += 4;
		memset(&(output[index * 4]), (int)lwip_getRandomNumber(), 4);
	}

	return 0;
}

#endif /*MBEDTLS_ENTROPY_HARDWARE_ALT*/

#ifdef __cplusplus
}
#endif
