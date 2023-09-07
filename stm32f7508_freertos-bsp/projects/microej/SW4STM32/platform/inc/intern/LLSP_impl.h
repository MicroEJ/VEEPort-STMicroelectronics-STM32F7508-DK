/* 
 * C
 * 
 * Copyright 2011-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
/* 
 * This file is auto-generated - DO NOT EDIT IT
 */
/* 
 * IceTea to C mapping support. Shall not be directly included by clients.
 */
typedef struct LLSP LLSP;
#include <stdint.h>
#define LLSP_CONCAT0(p,s) p ## s
#define LLSP_CONCAT(p,s) LLSP_CONCAT0(p,s)
#define LLSP_IMPL_initialize LLSP_CONCAT(LLSP_IMPL,_initialize)
#define LLSP_IMPL_syncWriteBlockEnter LLSP_CONCAT(LLSP_IMPL,_syncWriteBlockEnter__II)
#define LLSP_IMPL_syncWriteBlockExit LLSP_CONCAT(LLSP_IMPL,_syncWriteBlockExit__II)
#define LLSP_IMPL_syncReadBlockEnter LLSP_CONCAT(LLSP_IMPL,_syncReadBlockEnter__II)
#define LLSP_IMPL_syncReadBlockExit LLSP_CONCAT(LLSP_IMPL,_syncReadBlockExit__II)
#define LLSP_IMPL_wait LLSP_CONCAT(LLSP_IMPL,_wait)
#define LLSP_IMPL_wakeup LLSP_CONCAT(LLSP_IMPL,_wakeup__I)
struct LLSP
{
	int32_t __icetea__id__;
};
LLSP LLSP_getSingleton_instance;

/* 
 * Get the unique instance of this type within the system.
 */
LLSP* LLSP_IMPL_getSingleton(void)
{
	return &LLSP_getSingleton_instance;
}
