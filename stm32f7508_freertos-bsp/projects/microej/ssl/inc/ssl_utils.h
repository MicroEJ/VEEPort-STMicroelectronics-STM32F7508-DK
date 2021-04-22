/*
 * C
 *
 * Copyright 2020 IS2T. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break IS2T warranties on the whole library.
 */

#if !defined __SSL_UTILS_H__
# define __SSL_UTILS_H__

#include <time.h>

/* ---- Specific C Time function ---- */
/*
 * time function to get current JVM application time.
 * @param time : pointer to time_t struct that will be updated with the current time in seconds
 * @return the current time in seconds
 */
time_t custom_mbedtls_time_sec(time_t *time);

#endif // !defined __SSL_UTILS_H__
