/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/*
 * Dummy header to be able to resolve MicroEJ specific inclusions
 */

#ifndef SNI_H
#define SNI_H

#ifdef __cplusplus
	extern "C" {
#endif

// Pointer to the function to use for the SNI callback.
typedef void (*SNI_callback)(void);

#ifdef __cplusplus
	}
#endif

#endif
