/*
 * C
 *
 * Copyright 2017-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef INTERN_TRACE_INTERN_H
#define INTERN_TRACE_INTERN_H

/**
 * @file
 * @brief MicroEJ Trace Library internal header
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 7 June 2017
 */

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

/*
 * If <code>TRACE_BAREMETAL</code>is defined, then the Trace library can be used in C
 * directly without the need to embed MicroEJ.
 */
#ifdef TRACE_BAREMETAL

#include "LLTRACE_impl.h"

// If we use the LL implementation directly, just replace the calls to
// TRACE_* with LLTRACE_IMPL_*

#define TRACE_start LLTRACE_IMPL_start
#define TRACE_stop LLTRACE_IMPL_stop
#define TRACE_is_started LLTRACE_IMPL_is_started
#define TRACE_declare_event_group LLTRACE_IMPL_declare_event_group
#define TRACE_record_event_void LLTRACE_IMPL_record_event_void
#define TRACE_record_event_u32 LLTRACE_IMPL_record_event_u32
#define TRACE_record_event_u32x2 LLTRACE_IMPL_record_event_u32x2
#define TRACE_record_event_u32x3 LLTRACE_IMPL_record_event_u32x3
#define TRACE_record_event_u32x4 LLTRACE_IMPL_record_event_u32x4
#define TRACE_record_event_u32x5 LLTRACE_IMPL_record_event_u32x5
#define TRACE_record_event_u32x6 LLTRACE_IMPL_record_event_u32x6
#define TRACE_record_event_u32x7 LLTRACE_IMPL_record_event_u32x7
#define TRACE_record_event_u32x8 LLTRACE_IMPL_record_event_u32x8
#define TRACE_record_event_u32x9 LLTRACE_IMPL_record_event_u32x9
#define TRACE_record_event_u32x10 LLTRACE_IMPL_record_event_u32x10
#define TRACE_record_event_end LLTRACE_IMPL_record_event_end
#define TRACE_record_event_end_u32 LLTRACE_IMPL_record_event_end_u32

#else

// If we are using the Trace library with MicroEJ, then map the functions
// definitions on the Java native methods.

#define TRACE_start Java_ej_trace_Tracer_startTrace
#define TRACE_stop Java_ej_trace_Tracer_stopTrace
#define TRACE_is_started Java_ej_trace_Tracer_isTraceStarted
#define TRACE_declare_event_group Java_ej_trace_Tracer_registerGroup
#define TRACE_record_event_void Java_ej_trace_Tracer_recordEventNative__II
#define TRACE_record_event_u32 Java_ej_trace_Tracer_recordEventNative__III
#define TRACE_record_event_u32x2 Java_ej_trace_Tracer_recordEventNative__IIII
#define TRACE_record_event_u32x3 Java_ej_trace_Tracer_recordEventNative__IIIII
#define TRACE_record_event_u32x4 Java_ej_trace_Tracer_recordEventNative__IIIIII
#define TRACE_record_event_u32x5 Java_ej_trace_Tracer_recordEventNative__IIIIIII
#define TRACE_record_event_u32x6 Java_ej_trace_Tracer_recordEventNative__IIIIIIII
#define TRACE_record_event_u32x7 Java_ej_trace_Tracer_recordEventNative__IIIIIIIII
#define TRACE_record_event_u32x8 Java_ej_trace_Tracer_recordEventNative__IIIIIIIIII
#define TRACE_record_event_u32x9 Java_ej_trace_Tracer_recordEventNative__IIIIIIIIIII
#define TRACE_record_event_u32x10 Java_ej_trace_Tracer_recordEventNative__IIIIIIIIIIII
#define TRACE_record_event_end Java_ej_trace_Tracer_recordEventEndNative__II
#define TRACE_record_event_end_u32 Java_ej_trace_Tracer_recordEventEndNative__III

#endif // TRACE_BAREMETAL

#ifdef __cplusplus
	}
#endif
#endif //INTERN_TRACE_INTERN_H
