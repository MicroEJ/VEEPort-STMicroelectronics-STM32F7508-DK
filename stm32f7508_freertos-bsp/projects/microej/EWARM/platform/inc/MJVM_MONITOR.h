/*
 * C
 *
 * Copyright 2015-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef MJVM_MONITOR_H
#define MJVM_MONITOR_H

/**
 * @file
 * @brief MicroEJ MicroJvm virtual machine monitoring library API
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 7 June 2017
 */

#include <stdint.h>
#include "MJVM_MONITOR_types.h"

/**
 * Fills in  the int32_t array <code>threads_ids</code> with the ID of the alive Java threads.<p>
 * If the given array is not large enough to contains all the Java threads IDs, the
 * <code>threads_ids_length</code> first Java threads IDs are stored.
 * <p>
 * This function must be called within the virtual machine
 * task.
 *
 * @param threads_ids
 *            pointer to an int32_t array of <code>threads_ids_length</code> elements.
 * @param threads_ids_length
 *            length of <code>threads_ids</code> array.
 *
 * @return the number of alive Java threads or <code>MJVM_MONITOR_ERROR</code> if this function is not called within the
 * virtual machine task.
 */
int32_t MJVM_MONITOR_get_threads_ids(int32_t* threads_ids, int32_t threads_ids_length);

/**
 * Fills in the given <code>thread_info</code> structure and <code>name</code> array with the info of the thread
 * and the name of the thread.
 * <p>
 * Given <code>name</code> array may null. In this case, the name of the thread is not filled in. If given
 * <code>name</code> array is not large enough to copy the name of the thread, then the name is truncated to
 * <code>name_length</code>-1 characters (last character is '\0').
 * <p>
 * This function must be called within the virtual machine
 * task.
 *
 * @param thread_id
 *            ID of the thread.
 *
 * @param thread_info
 *            pointer to the structure to fill in.
 *
 * @param name
 *            pointer to a char array to fill in with the name of the thread. May be null.
 *
 * @param name_length
 *            length in bytes of the given <code>name</code> array.
 *
 * @return <code>MJVM_MONITOR_OK</code> on success,
 * 		   <code>MJVM_MONITOR_INVALID_THREAD_ID</code> if the given thread ID is invalid,
 * 		   <code>MJVM_MONITOR_INVALID_THREAD_INFO</code> if <code>thread_info</code> is null
 */
int32_t MJVM_MONITOR_get_thread_info(int32_t thread_id, MJVM_MONITOR_thread_info_t* thread_info, char* name, int32_t name_length);

/**
 * Prints the state of the MicroJvm virtual machine to the standard output stream.
 * For each Java thread, the Java stack trace, the name, the state and the priority are printed.
 */
void MJVM_MONITOR_dump_vm(void);

#endif /* MJVM_MONITOR_H */
