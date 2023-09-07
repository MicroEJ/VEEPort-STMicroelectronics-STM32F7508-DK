/* 
 * C
 * 
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
/* 
 * This file is auto-generated - DO NOT EDIT IT
 */
/* 
 * IceTea to C mapping support. Shall not be directly included by clients.
 */
typedef struct LLKERNEL LLKERNEL;
#include <stdint.h>
#define LLKERNEL_CONCAT0(p,s) p ## s
#define LLKERNEL_CONCAT(p,s) LLKERNEL_CONCAT0(p,s)
#define LLKERNEL_IMPL_allocateWorkingBuffer LLKERNEL_CONCAT(LLKERNEL_IMPL,_allocateWorkingBuffer__I)
#define LLKERNEL_IMPL_freeWorkingBuffer LLKERNEL_CONCAT(LLKERNEL_IMPL,_freeWorkingBuffer__LiceTea_lang_Ram_2)
#define LLKERNEL_IMPL_allocateFeature LLKERNEL_CONCAT(LLKERNEL_IMPL,_allocateFeature__II)
#define LLKERNEL_IMPL_freeFeature LLKERNEL_CONCAT(LLKERNEL_IMPL,_freeFeature__I)
#define LLKERNEL_IMPL_getAllocatedFeaturesCount LLKERNEL_CONCAT(LLKERNEL_IMPL,_getAllocatedFeaturesCount)
#define LLKERNEL_IMPL_getFeatureHandle LLKERNEL_CONCAT(LLKERNEL_IMPL,_getFeatureHandle__I)
#define LLKERNEL_IMPL_getFeatureAddressRAM LLKERNEL_CONCAT(LLKERNEL_IMPL,_getFeatureAddressRAM__I)
#define LLKERNEL_IMPL_getFeatureAddressROM LLKERNEL_CONCAT(LLKERNEL_IMPL,_getFeatureAddressROM__I)
#define LLKERNEL_IMPL_copyToROM LLKERNEL_CONCAT(LLKERNEL_IMPL,_copyToROM__LiceTea_lang_Ram_2LiceTea_lang_Ram_2I)
#define LLKERNEL_IMPL_flushCopyToROM LLKERNEL_CONCAT(LLKERNEL_IMPL,_flushCopyToROM)
#define LLKERNEL_IMPL_onFeatureInitializationError LLKERNEL_CONCAT(LLKERNEL_IMPL,_onFeatureInitializationError__II)
struct LLKERNEL
{
	int32_t __icetea__id__;
};
LLKERNEL LLKERNEL_getSingleton_instance;

/* 
 * Get the unique instance of this type within the system.
 */
LLKERNEL* LLKERNEL_IMPL_getSingleton(void)
{
	return &LLKERNEL_getSingleton_instance;
}
