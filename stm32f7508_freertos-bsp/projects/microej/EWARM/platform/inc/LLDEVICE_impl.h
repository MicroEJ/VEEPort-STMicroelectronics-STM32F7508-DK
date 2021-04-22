/*
 * C
 *
 * Copyright 2015-2017 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/* Includes ------------------------------------------------------------------*/

#ifndef LLDEVICE_IMPL
#define LLDEVICE_IMPL

/**
 * @file
 * @brief MicroEJ Device low level API
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 8 March 2017
 */

#include <sni.h>
#include <intern/LLDEVICE_impl.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Gets the platform architecture name.
 * @param buffer the buffer where to fill the architecture name (null terminated)
 * @param length the buffer size in bytes
 * @return 1 on success, 0 on error
 */
uint8_t LLDEVICE_IMPL_getArchitecture(uint8_t* buffer, int32_t length);

/**
 * Gets the unique device identifier relative to the architecture.
 * @param buffer the buffer where to fill the device identifier
 * @param length the buffer size in bytes
 * @return the number of bytes filled, 0 on error
 */
uint32_t LLDEVICE_IMPL_getId(uint8_t* buffer, int32_t length);

#ifdef __cplusplus
	}
#endif

#endif

