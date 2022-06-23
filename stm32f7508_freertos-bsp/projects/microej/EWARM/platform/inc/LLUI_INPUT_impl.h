/* 
 * Copyright 2012-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _LLUI_INPUT_IMPL
#define _LLUI_INPUT_IMPL
#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Input module (MicroUI Input Engine) low level API. This file lists a set
 * of API to connect the MicroUI input pump. All APIs are mandatory.
 */

#include "LLUI_INPUT.h"

// --------------------------------------------------------------------------------
// Functions that must be implemented
// --------------------------------------------------------------------------------

/**
 * @brief Called by the input engine at startup to initialize all input devices.
 */
void LLUI_INPUT_IMPL_initialize(void);

/**
 * @brief Called by the input engine at startup to get the initial state of the given state
 * machine. Only called if a <code>states</code> event generator is defined
 * in the static microui initializer xml file.
 *
 * A states is a group of state machines. At any time state machine keeps a state.
 * Each state machine has a 0-based ID: first state machine the ID 0, next state machine
 * the ID 1 etc.
 *
 * @param[in] stateMachinesID the state machines group ID.
 * @param[in] stateID the state machine ID in the state machines group.
 *
 * @return the initial state machine state.
 */
jint LLUI_INPUT_IMPL_getInitialStateValue(jint stateMachinesID, jint stateID);

/**
 * @brief Called by the input engine before entering in a critical section.
 *
 * During the critical section no event must be added to the input queue.
 * Therefore, the LLUI_INPUT_sendXXXEvent functions must not be called.
 * There are several ways to implement the enterCriticalSection method:
 *
 * - When all events are added to the input engine by OS task(s), the method must
 * ensure that the current task is not pre-empted by another task during the critical
 * section (use a binary semaphore)
 * - When some or all events are added to the input engine by interrupt routine(s),
 * the method has to disable all interrupts that could generate inputs, and, if
 * an interrupt is not currently being processed, ensure that the current task is
 * not pre-empted by another task during the critical section.
 *
 * Note that this function will never be called twice without an interleaving
 * call to <code>LLUI_INPUT_IMPL_leaveCriticalSection</code>. Therefore the
 * implementation does not need to support nested critical sections.
 */
void LLUI_INPUT_IMPL_enterCriticalSection(void);

/**
 * @brief Called by the input engine when exiting from a critical section.
 *
 * @see LLUI_INPUT_enterCriticalSection.
 */
void LLUI_INPUT_IMPL_leaveCriticalSection(void);

// --------------------------------------------------------------------------------
// Optional functions to implement
// --------------------------------------------------------------------------------

/**
 * @brief Initializes the MicroUI FIFO elements dumper.
 *
 * MicroUI FIFO contains some events and some optional events' data. MicroUI FIFO is not able to
 * interpret these events and data. The logger must save event (or data)'s metadata to
 * be able to interpret the event when dumping the FIFO.
 *
 * The default logger uses LLTRACE_IMPL_record_event_xxx logger.
 *
 * @param[in] length the MicroUI FIFO (queue) size.
 *
 * @see LLUI_INPUT_dump().
 */
void LLUI_INPUT_IMPL_log_queue_init(uint32_t length);

/**
 * @brief Called by the input engine when an event (or its data) cannot be added to the MicroUI
 * FIFO because the queue is full.
 *
 * The default logger uses LLTRACE_IMPL_record_event_xxx logger.
 *
 * @param[in] data the event or data that cannot be added to the queue.
 */
void LLUI_INPUT_IMPL_log_queue_full(uint32_t data);

/**
 * @brief Called by the input engine when an event (or its data) is added to the MicroUI
 * FIFO.
 *
 * The default logger uses LLTRACE_IMPL_record_event_xxx logger.
 *
 * @param[in] data the event or data added to the queue.
 * @param[in] index the event or data's index in queue.
 * @param[in] remaining_elements available number of remaining data to add in the queue
 * to store the full event.
 * @param[in] queue_length available number of events and data not executed by MicroUI pump.
 */
void LLUI_INPUT_IMPL_log_queue_add(uint32_t data, uint32_t index, uint32_t remaining_elements, uint32_t queue_length);

/**
 * @brief Called by the input engine when an event (or its data) replaces an older event (or data)
 * in the MicroUI FIFO.
 *
 * The default logger uses LLTRACE_IMPL_record_event_xxx logger.
 *
 * @param[in] old the replaced event or data.
 * @param[in] data the new event or data.
 * @param[in] index the event or data's index in queue.
 * @param[in] queue_length available number of events and data not executed by MicroUI pump.
 */
void LLUI_INPUT_IMPL_log_queue_replace(uint32_t old, uint32_t data, uint32_t index, uint32_t queue_length);

/**
 * @brief Called by the input engine when an event (or its data) is read by the MicroUI pump.
 *
 * The default logger uses LLTRACE_IMPL_record_event_xxx logger.
 *
 * @param[in] data the event or data read by pump.
 * @param[in] index the event or data's index in queue.
 */
void LLUI_INPUT_IMPL_log_queue_read(uint32_t data, uint32_t index);

/**
 * @brief Dumps a MicroUI FIFO element (event or data) or the dump status.
 *
 * The default logger does nothing and by consequence a call to LLUI_INPUT_dump() has no
 * effect.
 *
 * The dump status are:
 * - 0: start the dump: all next logs are the events or events' data already
		consumed ("past" events) and first log is the latest event or data
		stored in the queue.
 * - 1: next logs are the events or data not consumed yet ("future" events).
 * - 2: the dump will log the Java objects associated with the events that are not
 * 		consumed yet ("future" events); this feature is only available with the
 * 		MicroEJ Architecture 7.16 and higher.
 * - 3: end of dump.
 *
 * @param[in] log_type true when the data is an event or a data, false when the
 * data is a dump status.
 * @param[in] log the 32-bit event or data, or the dump status.
 * @param[in] index the event or data's index in queue (ignored when the data is a dump status).
 *
 * @see LLUI_INPUT_dump().
 */
void LLUI_INPUT_IMPL_log_dump(bool log_type, uint32_t log, uint32_t index);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
