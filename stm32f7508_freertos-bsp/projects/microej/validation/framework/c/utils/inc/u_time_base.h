/*
 * C
 *
 * Copyright 2019-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Prevent recursive inclusion */

#ifndef __UTIL_TIME_BASE_H
#define __UTIL_TIME_BASE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

/* Public constant declaration */

/* public structure declaration */

/* public function declaration */
// weak function to implement

/**
 * @fn void UTIL_TIME_BASE_initialize(void)
 * @brief This function is called by the SetUp functions of tests wich require some time references.
 *
 * @fn int64_t UTIL_TIME_BASE_getTime(void)
 * @brief This function provides a time reference for tests
 * @return a current time in microsecond (µs)
 */
void UTIL_TIME_BASE_initialize(void);
int64_t UTIL_TIME_BASE_getTime(void);

#ifdef __cplusplus
}
#endif

#endif
