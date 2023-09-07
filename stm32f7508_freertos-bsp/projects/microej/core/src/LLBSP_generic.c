/*
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
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

#ifdef MICROEJ_MULTIAPP
extern void _java_installed_features_text_start(void);
extern void _java_installed_features_text_size(void);
#endif // MICROEJ_MULTIAPP


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
