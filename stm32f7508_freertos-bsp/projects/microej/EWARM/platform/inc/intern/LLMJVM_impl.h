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
typedef struct LLMJVM LLMJVM;
#include <stdint.h>
#define LLMJVM_CONCAT0(p,s) p ## s
#define LLMJVM_CONCAT(p,s) LLMJVM_CONCAT0(p,s)
#define LLMJVM_IMPL_initialize LLMJVM_CONCAT(LLMJVM_IMPL,_initialize)
#define LLMJVM_IMPL_vmTaskStarted LLMJVM_CONCAT(LLMJVM_IMPL,_vmTaskStarted)
#define LLMJVM_IMPL_scheduleRequest LLMJVM_CONCAT(LLMJVM_IMPL,_scheduleRequest__J)
#define LLMJVM_IMPL_idleVM LLMJVM_CONCAT(LLMJVM_IMPL,_idleVM)
#define LLMJVM_IMPL_wakeupVM LLMJVM_CONCAT(LLMJVM_IMPL,_wakeupVM)
#define LLMJVM_IMPL_ackWakeup LLMJVM_CONCAT(LLMJVM_IMPL,_ackWakeup)
#define LLMJVM_IMPL_getCurrentTaskID LLMJVM_CONCAT(LLMJVM_IMPL,_getCurrentTaskID)
#define LLMJVM_IMPL_shutdown LLMJVM_CONCAT(LLMJVM_IMPL,_shutdown)
#define LLMJVM_IMPL_setApplicationTime LLMJVM_CONCAT(LLMJVM_IMPL,_setApplicationTime__J)
#define LLMJVM_IMPL_getCurrentTime LLMJVM_CONCAT(LLMJVM_IMPL,_getCurrentTime__Z)
#define LLMJVM_IMPL_getTimeNanos LLMJVM_CONCAT(LLMJVM_IMPL,_getTimeNanos)
#define LLMJVM_schedule __icetea__virtual__com_is2t_microjvm_bsp_IGreenThreadMicroJvmScheduler___schedule
struct LLMJVM
{
	int32_t __icetea__id__;
};
LLMJVM LLMJVM_getSingleton_instance;

/* 
 * Get the unique instance of this type within the system.
 */
LLMJVM* LLMJVM_IMPL_getSingleton(void)
{
	return &LLMJVM_getSingleton_instance;
}
