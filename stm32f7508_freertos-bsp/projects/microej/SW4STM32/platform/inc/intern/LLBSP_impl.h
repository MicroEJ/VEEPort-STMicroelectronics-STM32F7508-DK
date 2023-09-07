/* 
 * C
 * 
 * Copyright 2011-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
/* 
 * This file is auto-generated - DO NOT EDIT IT
 */
/* 
 * IceTea to C mapping support. Shall not be directly included by clients.
 */
typedef struct LLBSP LLBSP;
#include <stdint.h>
#define LLBSP_CONCAT0(p,s) p ## s
#define LLBSP_CONCAT(p,s) LLBSP_CONCAT0(p,s)
#define LLBSP_IMPL_putchar LLBSP_CONCAT(LLBSP_IMPL,_putchar__I)
struct LLBSP
{
	int32_t __icetea__id__;
};
LLBSP LLBSP_getSingleton_instance;

/* 
 * Get the unique instance of this type within the system.
 */
LLBSP* LLBSP_IMPL_getSingleton(void)
{
	return &LLBSP_getSingleton_instance;
}
