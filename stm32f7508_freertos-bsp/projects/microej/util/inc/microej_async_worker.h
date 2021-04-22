/*
 * C
 *
 * Copyright 2018-2019 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef MICROEJ_ASYNC_WORKER_H
#define MICROEJ_ASYNC_WORKER_H

/**
 * @file
 * @brief Asynchronous Worker API.
 * 		This library helps writing SNI functions that must be executed asynchronously.
 *
 * 		The execution of an SNI function prevents the other Java thread to be scheduled. That's why blocking native functions
 * 		or long native functions should be executed asynchronously.
 * 		<p>
 * 		An async worker is declared statically using the <code>MICROEJ_ASYNC_WORKER_worker_declare()</code> macro and started with
 * 		the <code>MICROEJ_ASYNC_WORKER_initialize()</code> function.
 * 		Jobs are allocated using <code>MICROEJ_ASYNC_WORKER_allocate_job()</code> and scheduled with <code>MICROEJ_ASYNC_WORKER_async_exec()</code>.
 * 		<p>
 * 		Typical usage consists in declaring:
 * 		- for each SNI function, a structure that contains the parameters of the function,
 * 		- an union of all the previously declared structures,
 * 		- the async worker using <code>MICROEJ_ASYNC_WORKER_worker_declare()</code> macro.
 * 		<p>
 * 		For example, if we have to implement asynchronously the following two SNI functions :
 * 		@code
 * 		int foo(int i, int j);
 * 		void bar(int i);
 *		@endcode
 *
 *		<p>
 * 		Then we will declare the async worker as following:
 * 		@code
 *		// foo() parameters structure
 *		typedef struct {
 * 			int i;
 * 			int j;
 * 			int result;
 *		} foo_param_t;
 *
 *		// bar() parameters structure
 *		typedef struct {
 * 			int i;
 *		} bar_param_t;
 *
 *		// union of all the parameters structures
 *		typedef union {
 * 			foo_param_t foo;
 * 			bar_param_t bar;
 * 			...
 *		} my_worker_param_t;
 *
 *		// Declare the worker and the task that will execute it
 *		MICROEJ_ASYNC_WORKER_worker_declare(my_worker, MY_WORKER_JOB_COUNT, my_worker_param_t, MY_WORKER_WAITING_LIST_SIZE);
 *		OSAL_task_stack_declare(my_worker_stack, MY_WORKER_STACK_SIZE);
 *
 *		void initialize(){
 *			MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_initialize(&my_worker, "My Worker", my_worker_stack, MY_WORKER_PRIORITY);
 *			if(status != MICROEJ_ASYNC_WORKER_OK){
 *				...
 *			}
 *			...
 *		}
 *
 *		int foo(int i, int j){
 *			MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&my_worker, (SNI_callback)foo);
 *			if(job != NULL){
 *				foo_param_t* params = (foo_param_t*)job->params;
 *				params->i = i;
 *				params->j = j;
 *
 *				MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&my_worker, job, foo_action, (SNI_callback)foo_on_done);
 *				if(status != MICROEJ_ASYNC_WORKER_OK){
 *					// Error
 *					MICROEJ_ASYNC_WORKER_free_job(&my_worker, job);
 *				}
 *			}
 *			// If an error occurred, then an exception is pending,
 *			// otherwise the current thread is suspended.
 *			// In any case, the returned value is not used.
 *			return -1;
 *		}
 *
 *		void foo_action(MICROEJ_ASYNC_WORKER_job_t* job){
 *			foo_param_t* params = (foo_param_t*)job->params;
 *
 *			int i = params->i;
 *			int j = params->j;
 *
 *			...
 *
 *			params->result = result;
 *		}
 *
 *		int foo_on_done(int i, int j){
 *			MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
 *			foo_param_t* params = (foo_param_t*)job->params;
 *
 *			int result = params->result;
 *			MICROEJ_ASYNC_WORKER_free_job(&my_worker, job);
 *
 *			return result;
 *		}
 *
 *		...
 *		@endcode
 *
 *
 * @author MicroEJ Developer Team
 * @version 0.2.0
 * @date 11 December 2019
 */

#include <stdint.h>
#include "sni.h"
#include "osal.h"

#ifdef __cplusplus
	extern "C" {
#endif

/** @brief Return codes list. */
typedef enum {
	MICROEJ_ASYNC_WORKER_OK,
	MICROEJ_ASYNC_WORKER_ERROR,
	MICROEJ_ASYNC_WORKER_INVALID_ARGS
} MICROEJ_ASYNC_WORKER_status_t;


/** @brief See <code>struct MICROEJ_ASYNC_WORKER_job</code>. */
typedef struct MICROEJ_ASYNC_WORKER_job MICROEJ_ASYNC_WORKER_job_t;

/** @brief Pointer to a function to call asynchronously. */
typedef void (*MICROEJ_ASYNC_WORKER_action_t)(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief A job to execute in a worker.
 *
 * Jobs are allocated using <code>MICROEJ_ASYNC_WORKER_allocate_job()</code>
 * and freed using <code>MICROEJ_ASYNC_WORKER_free_job()</code>.
 */
struct MICROEJ_ASYNC_WORKER_job{
	/**
	 * @brief Pointers to the parameters.
	 *
	 * This pointer is initialized by the system and references an union of type <code>_param_type</code> (value given to <code>MICROEJ_ASYNC_WORKER_worker_declare()</code>).
	 * This pointer field must not be modified but the content of the referenced union can be modified.
	 */
	void* params;
	/** @brief Structure internal data. Must not be modified. */
	struct {
		MICROEJ_ASYNC_WORKER_action_t action; // Pointer to the action to execute asynchronously.
		int32_t thread_id; // Id of the Java thread that is waiting for this job to complete ; SNI_ERROR if no thread is waiting.
		MICROEJ_ASYNC_WORKER_job_t* next_free_job; // Next in the free jobs linked list.
	} _intern;
};

/**
 * @brief An async worker.
 *
 * Workers are declared using the <code>MICROEJ_ASYNC_WORKER_worker_declare()</code> macro and
 * initialized with <code>MICROEJ_ASYNC_WORKER_initialize()</code>.
 * <p>
 * All the fields of this structure are internal data and must not be modified.
 */
typedef struct {
	int32_t job_count; // Maximum number of jobs.
	MICROEJ_ASYNC_WORKER_job_t* free_jobs; // Linked list of free jobs
	void* params; // Pointer to params array. Length of this array is job_count.
	int32_t params_sizeof; // Size of the params union
	int32_t waiting_threads_length; // Length of the waiting_threads array
	int32_t* waiting_threads; // Array of waiting threads (circular list)
	uint16_t waiting_thread_offset; // Offset of the first waiting thread. If equals to free_waiting_thread_offset: no waiting thread
	uint16_t free_waiting_thread_offset; // Offset of the first free slot in waiting_threads array
	OSAL_queue_handle_t jobs_queue; // Linked list of jobs
	OSAL_task_handle_t task; // The task that executes this worker.
	OSAL_mutex_handle_t mutex; // Mutex used for critical sections.
} MICROEJ_ASYNC_WORKER_handle_t;

/**
 * @brief Declares a worker named <code>_name</code>.
 *
 * This macro must be used outside of any function so the worker is declared as a global variable.
 *
 * @param _name name of the worker variable.
 * @param _job_count maximum number of jobs that can be allocated for this worker. Must be greater than 0.
 * @param _param_type type of the union of all the parameters structures
 * @param  _waiting_list_size Maximum Java thread that can be suspended on <code>MICROEJ_ASYNC_WORKER_allocate_job()</code> when no job is available. Must be greater than 0.
 */
#define MICROEJ_ASYNC_WORKER_worker_declare(_name, _job_count, _param_type, _waiting_list_size)\
	_param_type _name ## _params[_job_count];\
	MICROEJ_ASYNC_WORKER_job_t _name ## _jobs[_job_count];\
	int32_t _name ## _waiting_threads[_waiting_list_size+1];\
	MICROEJ_ASYNC_WORKER_handle_t _name = {\
		.job_count = _job_count,\
		.free_jobs = _name ## _jobs,\
		.params = _name ## _params,\
		.params_sizeof = sizeof(_param_type),\
		.waiting_threads_length = _waiting_list_size+1,\
		.waiting_threads = _name ## _waiting_threads,\
		.waiting_thread_offset = 0,\
		.free_waiting_thread_offset = 0\
	}


/**
 * @brief Initializes and starts a worker previously declared with <code>MICROEJ_ASYNC_WORKER_worker_declare()</code> macro.
 *
 * @param[in] async_worker the worker to initialize. Declared with <code>MICROEJ_ASYNC_WORKER_worker_declare()</code> macro.
 * @param[in] name worker name.
 * @param[in] stack worker task stack declared using <code>OSAL_task_stack_declare()</code> macro.
 * @param[in] priority worker task priority.
 *
 * @return MICROEJ_ASYNC_WORKER_INVALID_ARGS if given worker has not been correctly declared.
 * Returns MICROEJ_ASYNC_WORKER_ERROR if worker task or queue creation fails.
 * Returns MICROEJ_ASYNC_WORKER_OK on success.
 */
MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_initialize(MICROEJ_ASYNC_WORKER_handle_t* async_worker, uint8_t* name, OSAL_task_stack_t stack, int32_t priority);

/**
 * @brief Allocates a new job for the given worker.
 *
 * If a job is available, returns a reference to the job.
 * <p>
 * If there is no job available, the current Java thread is suspended, added to the waiting list and <code>NULL</code>
 * is returned. Then, when a job is available, the Java thread is resumed and the function
 * <code>sni_retry_callback</code> is called.
 * Usually the <code>sni_retry_callback</code> function argument is the current SNI function itself (i.e. the function
 * that is currently calling <code>MICROEJ_ASYNC_WORKER_allocate_job()</code>).
 * <p>
 * If the waiting list is full, an SNI exception is thrown using <code>SNI_throwNativeIOException()</code> and
 * <code>NULL</code> is returned. Size of the waiting list is defined when declaring the worker.
 * <p>
 * <code>params</code> field of the returned job is a pointer to a type <code>T</code> where <code>T</code> is
 * the value of the argument <code>_param_type</code> given to the <code>MICROEJ_ASYNC_WORKER_worker_declare()</code>
 * macro.
 * <p>
 * This function must be called within the virtual machine task.
 *
 * @param[in] async_worker the worker in which to allocate a job.
 * @param[in] sni_retry_callback if the current Java thread has been suspended, this function is called when it is resumed.
 *
 * @return A job on success or NULL if no job is available.
 */
MICROEJ_ASYNC_WORKER_job_t* MICROEJ_ASYNC_WORKER_allocate_job(MICROEJ_ASYNC_WORKER_handle_t* async_worker, SNI_callback sni_retry_callback);


/**
 * @brief Frees a job previously allocated with MICROEJ_ASYNC_WORKER_allocate_job().
 *
 * This function must be called when and only when the given job has been executed and the result retrieved. This is usually done
 * in the on_done_callback given to MICROEJ_ASYNC_WORKER_async_exec(). After calling this function the given job and its
 * parameters must not be used anymore.
 * <p>
 * This function must be called within the virtual machine task.
 *
 * @param[in] async_worker the worker used to allocate the given job.
 * @param[in] job the job to free. Must have been allocated with MICROEJ_ASYNC_WORKER_allocate_job().
 *
 * @return <code>MICROEJ_ASYNC_WORKER_OK</code> on success, otherwise returns the error status
 * <code>MICROEJ_ASYNC_WORKER_ERROR</code>.
 */
MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_free_job(MICROEJ_ASYNC_WORKER_handle_t* async_worker, MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Executes the given job asynchronously.
 *
 * This function does not block and returns immediately but it suspends the execution of the current Java thread
 * until the job is finished.
 * <p>
 * When the job is finished, the SNI callback <code>on_done_callback</code> is called before going back to Java.
 * If the job is not used anymore, the callback must released it explicitly by calling
 * <code>MICROEJ_ASYNC_WORKER_free_job()</code>.
 * <p>
 * If an error happens, an SNI exception is thrown using <code>SNI_throwNativeIOException()</code> and the error
 * status <code>MICROEJ_ASYNC_WORKER_ERROR</code> is returned. In this case, the SNI callback
 * <code>on_done_callback</code> is not called and the job must be released explicitly by calling
 * <code>MICROEJ_ASYNC_WORKER_free_job()</code>.
 * <p>
 * This function must be called within the virtual machine task.
 *
 * @param[in] async_worker the worker used to execute the given job. Must be the same than the one used to allocate the job.
 * @param[in] job the job to execute. Must have been allocated with <code>MICROEJ_ASYNC_WORKER_allocate_job()</code>.
 * @param[in] action the function to execute asynchronously.
 * @param[in] on_done_callback the <code>SNI_callback</code> called when the job is done.
 *
 * @return <code>MICROEJ_ASYNC_WORKER_OK</code> on success, otherwise returns the error status
 * <code>MICROEJ_ASYNC_WORKER_ERROR</code>.
 */
MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_async_exec(MICROEJ_ASYNC_WORKER_handle_t* async_worker, MICROEJ_ASYNC_WORKER_job_t* job, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done_callback);

/**
 * @brief Executes the given job asynchronously.
 *
 * This function does not block and returns immediately.
 * <p>
 * When the job is finished, the job is automatically released by the async worker thread.
 * <p>
 * If an error happens, an SNI exception is thrown using <code>SNI_throwNativeIOException()</code> and the error
 * status <code>MICROEJ_ASYNC_WORKER_ERROR</code> is returned. In this case, the job must be released explicitly
 * by calling <code>MICROEJ_ASYNC_WORKER_free_job()</code>.
 * <p>
 * This function must be called within the virtual machine task.
 *
 * @param[in] async_worker the worker used to execute the given job. Must be the same than the one used to allocate the job.
 * @param[in] job the job to execute. Must have been allocated with <code>MICROEJ_ASYNC_WORKER_allocate_job()</code>.
 * @param[in] action the function to execute asynchronously.
 *
 * @return <code>MICROEJ_ASYNC_WORKER_OK</code> on success, otherwise returns the error status
 * <code>MICROEJ_ASYNC_WORKER_ERROR</code>.
 */
MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_async_exec_no_wait(MICROEJ_ASYNC_WORKER_handle_t* async_worker, MICROEJ_ASYNC_WORKER_job_t* job, MICROEJ_ASYNC_WORKER_action_t action);

/**
 * @brief Returns the job that has been executed.
 *
 * This function must be called after the execution of a job in the function passed as <code>on_done_callback</code>
 * argument to <code>MICROEJ_ASYNC_WORKER_async_exec()</code>.
 *
 * @return the job that has been executed asynchronously or <code>NULL</code> if not called from an <code>on_done_callback</code>.
 */
MICROEJ_ASYNC_WORKER_job_t* MICROEJ_ASYNC_WORKER_get_job_done(void);

#ifdef __cplusplus
	}
#endif

#endif // MICROEJ_ASYNC_WORKER_H

