/*
 * C
 *
 * Copyright 2019-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Prevent recursive inclusion */

#ifndef __UTIL_PRINT_H
#define __UTIL_PRINT_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

/* Public constant declaration */

/* public structure declaration */

/* public function declaration */
// weak function to implement
/** 
 * @fn void UTIL_print_initialize(void)
 * @brief This function is called before the tests which need to display some results. It can be used to initialize the hardware in charge of the display (serial line, screen,...).
 * 
 * @fn void UTIL_print_string(const char* string)
 * @brief  this function is called by test functions to display a string.
 * @param[in] const char* string  - the string to display.
 *
 * @fn void UTIL_print_integer(int integer)
 * @brief  this function is called by test functions to display a integer value.
 * @param[in] int integer - the value to display.
 *
 * @fn void UTIL_print_float(double value)
 * @brief this function is called by test functions to display a double value.
 * @param[in] double value - the doublye precision value to display.
 *
 * @fn void UTIL_print_longlong(long long value)
 *.@brief this function is called by test functions to display a 64-bit precision value.
 * @param[in] long long value - the 64-bit integer value to display.
 */
void UTIL_print_initialize(void);
void UTIL_print_string(const char* string);
void UTIL_print_integer(int integer);
void UTIL_print_float(double value);
void UTIL_print_longlong(long long value);

#ifdef __cplusplus
}
#endif

#endif
