/* 
 * Copyright 2012-2020 MicroEJ Corp. All rights reserved.
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
 * @brief Called by the input stack at startup to initialize all input devices.
 */
void LLUI_INPUT_IMPL_initialize(void);

/**
 * @brief Called by the input stack at startup to get the initial state of the given state
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
 * @brief Called by the input stack when exiting from a critical section.
 *
 * @see LLUI_INPUT_enterCriticalSection.
 */
void LLUI_INPUT_IMPL_leaveCriticalSection(void);

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
