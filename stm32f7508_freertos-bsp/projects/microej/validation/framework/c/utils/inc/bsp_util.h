/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
/**
 * @file
 * @brief MicroEJ util
 * @author MicroEJ Developer Team
 * @version 1.3.0
 * @date 3 August 2017
 */

#ifndef BSP_UTIL_H
#define BSP_UTIL_H

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Exported defines ----------------------------------------------------------*/

/*
 * @brief defines that list the different byte ordered values
 */
#define BSP_BIG_ENDIAN		0
#define BSP_LITTLE_ENDIAN	1

/*
 * @brief MicroEJ buffer declaration. Used "BSP_DECLARE_BUFFER(XXX)" macro to
 * declare a MicroEJ buffer. It generates a "XXX_START" and "XXX_END"
 * constant value with the start address of the buffer in "XXX_START"
 * and the end address of the buffer in "XXX_END".
 */

#if defined( __GNUC__ )
#define BSP_DECLARE_BUFFER(NAME)				\
	void* __ ## NAME ## _Start__(void);		\
	void* __ ## NAME ## _End__(void);		\
	const uint32_t NAME ## _START = (uint32_t)&__ ## NAME ## _Start__;	\
	const uint32_t NAME ## _END = (uint32_t)&__ ## NAME ## _End__;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BSP_BYTE_ORDER		BSP_LITTLE_ENDIAN
#else
#define BSP_BYTE_ORDER		BSP_BIG_ENDIAN
#endif
#endif // __GNUC__

#if defined( __CC_ARM )
#define BSP_DECLARE_BASE(NAME)	Image$$ ## NAME ## $$ZI$$Base
#define BSP_DECLARE_LIMIT(NAME)	Image$$ ## NAME ## $$ZI$$Limit
#define BSP_DECLARE_BUFFER(NAME)				\
	extern uint32_t BSP_DECLARE_BASE(NAME);		\
	extern uint32_t BSP_DECLARE_LIMIT(NAME);	\
	const uint32_t NAME ## _START = (uint32_t)&BSP_DECLARE_BASE(NAME);	\
	const uint32_t NAME ## _END = (uint32_t)&BSP_DECLARE_LIMIT(NAME);

#if !defined(__BIG_ENDIAN)
#define BSP_BYTE_ORDER		BSP_LITTLE_ENDIAN
#else
#define BSP_BYTE_ORDER		BSP_BIG_ENDIAN
#endif
#endif // __CC_ARM

#if defined( __ICCARM__ )
#define BSP_DECLARE_BASE(NAME)	NAME ## $$Base
#define BSP_DECLARE_LIMIT(NAME)	NAME ## $$Limit
#define BSP_DECLARE_BUFFER(NAME)				\
	extern uint32_t BSP_DECLARE_BASE(NAME);		\
	extern uint32_t BSP_DECLARE_LIMIT(NAME);	\
	const uint32_t NAME ## _START = (uint32_t)&BSP_DECLARE_BASE(NAME);	\
	const uint32_t NAME ## _END = (uint32_t)&BSP_DECLARE_LIMIT(NAME);

#if __LITTLE_ENDIAN__ == 1
#define BSP_BYTE_ORDER		BSP_LITTLE_ENDIAN
#else
#define BSP_BYTE_ORDER		BSP_BIG_ENDIAN
#endif
#endif // __ICCARM__

/*
 * @brief MicroEJ weak function declaration. Used "BSP_DECLARE_WEAK_FCNT" macro to
 * declare a MicroEJ weak function as follow:
 * export my_return_type BSP_DECLARE_WEAK_FCNT my_function(my_params);
 */
 
#if defined( __CC_ARM ) || defined(__GNUC__)
#define BSP_DECLARE_WEAK_FCNT	__attribute__((weak))
#endif // __CC_ARM || __GNUC__

#if defined( __ICCARM__ )
#define BSP_DECLARE_WEAK_FCNT	__weak
#endif // __ICCARM__

/**
 * @brief Set this define to map "BSP_UTIL_trace" function on standard "printf"
 * function.
 */
#ifdef TRACE_USE_PRINTF
#include <stdio.h>
#define BSP_UTIL_trace printf
#endif

/* Exported functions ------------------------------------------------------- */

/**
 * @brief This function prints all strings. It uses the same arguments than
 * printf function. To map this function on printf function, set the global
 * define "#define TRACE_USE_PRINTF"
 */
int BSP_UTIL_trace(const char * , ...);

/**
 * @brief This function traces the called function.
 */
#define BSP_UTIL_trace_function() BSP_UTIL_trace("%s() %s:%d\n", (__func__), (__FILE__), (__LINE__))

/**
 * @brief This function prints the error code and an optional kind of error.
 * This kind of error is often the name of file which throws the error  (__FILE__).
 *
 * @param[in] kind the kind of error or NULL (0)
 * @param[in] error_code the error code
 */
#define BSP_UTIL_trace_error_code(kind, code) BSP_UTIL_trace("[ERROR] %d %s\n", (code), (kind))

/**
 * @brief This function checks the assert.
 *
 * @param[in] x the assert to check
 */
#define BSP_UTIL_assert(x) do{if(!(x)){BSP_UTIL_trace("assert %s:%d\n", __FILE__, __LINE__);while(1);}}while(0)

/**
 * @brief This function forces an assert.
 */
#define BSP_UTIL_assert_false() BSP_UTIL_assert(0)

/* EOF ---------------------------------------------------------------------- */

#endif // BSP_UTIL_H
