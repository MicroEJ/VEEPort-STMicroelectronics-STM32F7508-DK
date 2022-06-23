/*
 * C
 *
 * Copyright 2017-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef TRACE_H
#define TRACE_H

/**
 * @file
 * @brief MicroEJ Trace library API
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 1 June 2017
 */

#include <stdint.h>
#include <stdbool.h>
#include "intern/trace_intern.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Starts to record the events.
 * <p>
 * By default, the trace system is stopped.
 */
void TRACE_start(void);

/**
 * Stops to record the events. When the record system is stopped,
 * any call to the <code>TRACE_record_event_*</code> functions will not
 * record any events.
 * <p>
 * By default, the trace system is stopped.
 */
void TRACE_stop(void);

/**
 * Tests whether the trace system is started or not.
 *
 * @return <code>true</code> if the trace system is started,
 * 		   <code>false</code> otherwise.
 */
bool TRACE_is_started(void);

/**
 * Declare a new group of events with the given name. The created
 * group defines <code>nb_event_types</code> types of events. The
 * event IDs available for this group are from 0 to <code>nb_event_types</code>-1.
 *
 * @param group_name
 *            name that identifies the group. Must not be null.
 *
 * @param nb_event_types
 *            maximum number of event types available for the group.
 *
 * @return the ID of the created group or -1 on error.
 */
int32_t TRACE_declare_event_group(const char* group_name, int32_t nb_event_types);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 */
void TRACE_record_event_void(int32_t group_id, int32_t event_id);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 */
void TRACE_record_event_u32(int32_t group_id, int32_t event_id, uint32_t value0);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 *
 * @param value2
 *            custom value for the event.
 */
void TRACE_record_event_u32x2(int32_t group_id, int32_t event_id, uint32_t value1, uint32_t value2);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 *
 * @param value2
 *            custom value for the event.
 *
 * @param value3
 *            custom value for the event.
 */
void TRACE_record_event_u32x3(int32_t group_id, int32_t event_id, uint32_t value1, uint32_t value2, uint32_t value3);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 *
 * @param value2
 *            custom value for the event.
 *
 * @param value3
 *            custom value for the event.
 *
 * @param value4
 *            custom value for the event.
 */
void TRACE_record_event_u32x4(int32_t group_id, int32_t event_id, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 *
 * @param value2
 *            custom value for the event.
 *
 * @param value3
 *            custom value for the event.
 *
 * @param value4
 *            custom value for the event.
 *
 * @param value5
 *            custom value for the event.
 */
void TRACE_record_event_u32x5(int32_t group_id, int32_t event_id, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4, uint32_t value5);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 *
 * @param value2
 *            custom value for the event.
 *
 * @param value3
 *            custom value for the event.
 *
 * @param value4
 *            custom value for the event.
 *
 * @param value5
 *            custom value for the event.
 *
 * @param value6
 *            custom value for the event.
 */
void TRACE_record_event_u32x6(int32_t group_id, int32_t event_id, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4, uint32_t value5, uint32_t value6);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 *
 * @param value2
 *            custom value for the event.
 *
 * @param value3
 *            custom value for the event.
 *
 * @param value4
 *            custom value for the event.
 *
 * @param value5
 *            custom value for the event.
 *
 * @param value6
 *            custom value for the event.
 *
 * @param value7
 *            custom value for the event.
 */
void TRACE_record_event_u32x7(int32_t group_id, int32_t event_id, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4, uint32_t value5, uint32_t value6, uint32_t value7);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 *
 * @param value2
 *            custom value for the event.
 *
 * @param value3
 *            custom value for the event.
 *
 * @param value4
 *            custom value for the event.
 *
 * @param value5
 *            custom value for the event.
 *
 * @param value6
 *            custom value for the event.
 *
 * @param value7
 *            custom value for the event.
 *
 * @param value8
 *            custom value for the event.
 */
void TRACE_record_event_u32x8(int32_t group_id, int32_t event_id, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4, uint32_t value5, uint32_t value6, uint32_t value7, uint32_t value8);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 *
 * @param value2
 *            custom value for the event.
 *
 * @param value3
 *            custom value for the event.
 *
 * @param value4
 *            custom value for the event.
 *
 * @param value5
 *            custom value for the event.
 *
 * @param value6
 *            custom value for the event.
 *
 * @param value7
 *            custom value for the event.
 *
 * @param value8
 *            custom value for the event.
 *
 * @param value9
 *            custom value for the event.
 */
void TRACE_record_event_u32x9(int32_t group_id, int32_t event_id, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4, uint32_t value5, uint32_t value6, uint32_t value7, uint32_t value8, uint32_t value9);

/**
 * Record an event for the given group. The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event.
 *
 * @param value2
 *            custom value for the event.
 *
 * @param value3
 *            custom value for the event.
 *
 * @param value4
 *            custom value for the event.
 *
 * @param value5
 *            custom value for the event.
 *
 * @param value6
 *            custom value for the event.
 *
 * @param value7
 *            custom value for the event.
 *
 * @param value8
 *            custom value for the event.
 *
 * @param value9
 *            custom value for the event.
 *
 * @param value10
 *            custom value for the event.
 */
void TRACE_record_event_u32x10(int32_t group_id, int32_t event_id, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4, uint32_t value5, uint32_t value6, uint32_t value7, uint32_t value8, uint32_t value9, uint32_t value10);

/**
 * Record the end of the execution of an event for the given group. Call this function to
 * trace the duration of an event previously record with one of the <code>TRACE_record_event_*()<code>
 * functions. This function should not be called for event that has no duration.
 * <p>
 * For example, if you want to trace the execution of a function, you can call <code>TRACE_record_event_*()<code>
 * at the beginning of the function and <code>TRACE_record_event_end*()</code> at the end of the function.
 * <p>
 * The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 */
void TRACE_record_event_end(int32_t group_id, int32_t event_id);

/**
 * Record the end of the execution of an event for the given group. Call this function to
 * trace the duration of an event previously record with one of the <code>TRACE_record_event_()<code>
 * functions. This function should not be called for event that has no duration.
 * <p>
 * For example, if you want to trace the execution of a function, you can call <code>TRACE_record_event_*()<code>
 * at the beginning of the function and <code>TRACE_record_event_end*()</code> at the end of the function.
 * <p>
 * The given event ID must be valid for the specified
 * group (i.e. between 0 and <code>nb_event_types</code>-1 included where <code>nb_event_types</code>
 * is the value given to <code>TRACE_declare_event_group()</code>).
 * <p>
 * The record is done only if the trace system is started
 * (see <code>TRACE_start()</code>).
 *
 * @param group_id
 *            ID of the group returned by <code>TRACE_declare_event_group()</code>.
 *
 * @param event_id
 *            ID of the event.
 *
 * @param value1
 *            custom value for the event end.
 */
void TRACE_record_event_end_u32(int32_t group_id, int32_t event_id, uint32_t value1);

#ifdef __cplusplus
	}
#endif
#endif //TRACE_H
