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
 * Implementation header file. Shall only be included by client implementation C files.
 */
#include <stdint.h>
#include <intern/LLSP_impl.h>
// --------------------------------------------------------------------------------
// -                                  Constants                                   -
// --------------------------------------------------------------------------------

/**
 * Returned value when function call has succeeded.
 */
#define LLSP_OK (0)

/**
 * Returned value when current task has been interrupted during function call.
 */
#define LLSP_INTERRUPTED (1)

// --------------------------------------------------------------------------------
// -                      Functions that must be implemented                      -
// --------------------------------------------------------------------------------

/**
 * Initialize Shielded Plug synchronization data.
 * This function may be called several times. It is not called from a thread safe context so several threads
 * may call this method at the same time. The implementation may use synchronization mechanisms to ensure
 * that the initialization is made once.
 */
void LLSP_IMPL_initialize(void);

/**
 * This function is called before writing data in a block.
 * It must implement synchronization mechanisms to avoid multiple write access to the given block.
 */
void LLSP_IMPL_syncWriteBlockEnter(int32_t databaseID, int32_t blockID);

/**
 * This function is called after writing data in a block.
 * It must implement synchronization mechanisms to avoid multiple write access to the given block.
 */
void LLSP_IMPL_syncWriteBlockExit(int32_t databaseID, int32_t blockID);

/**
 * This function is called before reading data from a block.
 * It must implement synchronization mechanisms to avoid write access while reading the given block.
 */
void LLSP_IMPL_syncReadBlockEnter(int32_t databaseID, int32_t blockID);

/**
 * This function is called after reading data from a block.
 * It must implement synchronization mechanisms to avoid write access while reading the given block.
 */
void LLSP_IMPL_syncReadBlockExit(int32_t databaseID, int32_t blockID);

/**
 * Blocks the current task until {@link LLSP_wakeup} is called or the current task is interrupted.
 * @return {@link LLSP_OK} if the current task has been woken up, or {@link LLSP_INTERRUPTED} if the current task has been interrupted.
 */
int32_t LLSP_IMPL_wait(void);

/**
 * Wake up the specified given task (which is waiting in {@link LLSP_wait}).
 */
void LLSP_IMPL_wakeup(int32_t taskID);
