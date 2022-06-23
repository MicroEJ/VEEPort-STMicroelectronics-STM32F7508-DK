/*
 * C
 *
 * Copyright 2017-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef LLMJVM_MONITOR_IMPL_H
#define LLMJVM_MONITOR_IMPL_H

/**
 * @file
 * @brief MicroEJ MicroJvm Monitoring low level API
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 1 June 2017
 */

#include <stdint.h>
#include <stdbool.h>
#include <MJVM_MONITOR_types.h>

#ifdef __cplusplus
	extern "C" {
#endif

/*
 * This function is called once during MicroJvm virtual machine startup.
 * It may be used to initialize specific data.
 * <p>
 * When <code>auto_start</code> is true, the monitoring traces must be generated now
 * (i.e. call the <code>TRACE_start()</code> function). If <code>auto_start</code>
 * is false, the monitoring traces must not be generated until the function
 * <code>TRACE_start()</code> is called. Even if <code>auto_start</code> is false,
 * the MicroJvm virtual machine will call the LLMJVM_MONITOR_IMPL_* functions.
 *
 * @param auto_start
 *            true to start monitoring now, false otherwise
 */
void LLMJVM_MONITOR_IMPL_initialize(bool auto_start);

/**
 * This function is called during MicroJvm virtual machine end.
 * It may be used to freed specific data.
 */
void LLMJVM_MONITOR_IMPL_on_shutdown(void);

/**
 * This function is called by the MicroJvm virtual machine when a new Java thread
 * is started.
 * <p>
 * Implementation can use the function <code>MJVM_MONITOR_get_thread_info()</code> to
 * retrieve more information about the created thread.
 * 
 * @param thread_id
 * 		ID of the created thread.
 */
void LLMJVM_MONITOR_IMPL_on_thread_create(int32_t thread_id);

/**
 * This function is called by the MicroJvm virtual machine when the priority or
 * the name of a Java thread is modified.
 * <p>
 * Implementation can use the function <code>MJVM_MONITOR_get_thread_info()</code> to
 * retrieve more information about the modified thread.
 * 
 * @param thread_id
 * 		ID of the modified thread.
 */
void LLMJVM_MONITOR_IMPL_on_thread_modified(int32_t thread_id);

/**
 * This function is called by the MicroJvm virtual machine when the state of a
 * Java thread is modified (e.g. when a thread is waiting, running, etc.).
 *
 * @param thread_id
 * 		ID of the modified thread.
 *
 * @param new_state
 * 		New state of the thread.
 */
void LLMJVM_MONITOR_IMPL_on_thread_state_changed(int32_t thread_id, MJVM_MONITOR_state_t new_state);

/**
 * This function is called by the MicroJvm virtual machine when it enter in idle
 * mode (i.e. all the Java threads are sleeping).
 */
void LLMJVM_MONITOR_IMPL_on_idle(void);

/**
 * This function is called by the MicroJvm virtual machine when a garbage collector
 * operation is started.
 *
 * @param current_thread_id
 * 		ID of the thread that raises the GC.
 */
void LLMJVM_MONITOR_IMPL_on_gc_start(int32_t current_thread_id);

/**
 * This function is called by the MicroJvm virtual machine when a garbage collector
 * operation is finished.
 *
 * @param current_thread_id
 * 		ID of the thread that raises the GC.
 */
void LLMJVM_MONITOR_IMPL_on_gc_stop(int32_t current_thread_id);

/**
 * This function is called by the MicroJvm virtual machine when a new Java object is
 * allocated.
 *
 * @param type
 * 		Class of the Java object.
 *
 * @param size
 * 		Size in bytes of the Java object.
 *
 * @param method
 * 		The Java method that allocates the object.
 *
 * @param instruction_address
 * 		Address of the Java instruction that allocates the object.
 *
 * @param total_memory
 * 		Total size of the Java heap in bytes.
 *
 * @param free_memory
 * 		Amount of the free memory in the Java heap in bytes.
 *
 * @param immortal
 * 		<code>true</code> is the allocated object is immortal,
 * 		<code>false</code> otherwise.
 */
void LLMJVM_MONITOR_IMPL_on_allocate(void* type, int32_t size, void* method, void* instruction_address, int32_t total_memory, int32_t free_memory, bool immortal);

/**
 * This function is called by the MicroJvm virtual machine when a Java exception
 * is thrown.
 *
 * @param exception_type
 * 		Class of the Java exception.
 *
 * @param throw_method
 * 		The Java method that throws the exception.
 *
 * @param throw_instruction_address
 * 		Address of the Java instruction that throws the exception.
 *
 * @param catch_method
 * 		The Java method that catches the exception.
 *
 * @param catch_instruction_address
 * 		Address of the Java instruction that catches the exception.
 *
 * @param message
 * 		Message of the exception.
 */
void LLMJVM_MONITOR_IMPL_on_exception(void* exception_type, void* throw_method, void* throw_instruction_address, void* catch_method, void* catch_instruction_address, const char* message);

#ifdef __cplusplus
	}
#endif
#endif /* LLMJVM_MONITOR_IMPL_H */
