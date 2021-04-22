/*
 * Copyright 2014-2017 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/**
 * @file
 * @brief Generic LLBSP implementation.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 11 April 2018
 */

#include <stdint.h>
#include <stdio.h>

#include "LLBSP_impl.h"

#ifdef __cplusplus
	extern "C" {
#endif

extern void SOAR_START(void);
extern void SOAR_END(void);

#ifdef MICROEJ_MULTIAPP
extern void _java_installed_features_text_start(void);
extern void _java_installed_features_text_size(void);
#endif // MICROEJ_MULTIAPP


/**
 * Checks if the given pointer is in a read only memory or not.
 */
uint8_t LLBSP_IMPL_isInReadOnlyMemory(void* ptr)
{
	uint32_t addr = (uint32_t)ptr;
	uint32_t soarStart = (uint32_t)(&SOAR_START);
	uint32_t soarEnd = (uint32_t)(&SOAR_END);
#ifdef MICROEJ_MULTIAPP
	uint32_t installedFeaturesStart = (uint32_t)(&_java_installed_features_text_start);
	uint32_t installedFeaturesEnd = installedFeaturesStart + (uint32_t)(&_java_installed_features_text_size);
#endif // MICROEJ_MULTIAPP
	return (uint8_t)(((addr >= soarStart) && (addr < soarEnd))
#ifdef MICROEJ_MULTIAPP
			|| ((addr >= installedFeaturesStart) && (addr < installedFeaturesEnd))
#endif // MICROEJ_MULTIAPPl
			);
}

/*
 * Writes the character <code>c</code>, cast to an unsigned char, to stdout stream.
 * This function is used by the default implementation of the Java <code>System.out</code>.
 */
void LLBSP_IMPL_putchar(int32_t c)
{
	putchar(c);
}

#ifdef __cplusplus
	}
#endif
