/*
 * C
 *
 * Copyright 2018-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief Asynchronous Worker implementation
 * @author MicroEJ Developer Team
 * @version 0.2.0
 * @date 11 December 2019
 */

#include "microej_async_worker.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#ifdef __cplusplus
	extern "C" {
#endif

// Entry point of the async worker task.
static void* MICROEJ_ASYNC_WORKER_loop(void* args);

// Generic method for MICROEJ_ASYNC_WORKER_async_exec and MICROEJ_ASYNC_WORKER_async_exec_no_wait
static MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_async_exec_intern(MICROEJ_ASYNC_WORKER_handle_t* async_worker, MICROEJ_ASYNC_WORKER_job_t* job, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done_callback, bool wait);

MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_initialize(MICROEJ_ASYNC_WORKER_handle_t* async_worker, uint8_t* name, OSAL_task_stack_t stack, int32_t priority){
	// Check configuration
	int32_t job_count = async_worker->job_count;
	if(job_count <= 0
	|| async_worker->waiting_threads_length <= 1 // compare with 1 because '+1' is added when declaring the array
	){
		return MICROEJ_ASYNC_WORKER_INVALID_ARGS;
	}

	// Init jobs
	MICROEJ_ASYNC_WORKER_job_t* jobs = async_worker->free_jobs;
	void* params = async_worker->params;
	int32_t params_sizeof = async_worker->params_sizeof;
	for(int i=0 ; i<job_count-1 ; i++){
		jobs[i]._intern.next_free_job = &jobs[i+1];
		jobs[i].params = params;
		params = ( void *) ( (int32_t)params + params_sizeof );
	}
	jobs[job_count-1]._intern.next_free_job = NULL;
	jobs[job_count-1].params = params;

	// Create queue
	OSAL_status_t res = OSAL_queue_create(name, async_worker->job_count, &async_worker->jobs_queue);
	if(res != OSAL_OK){
		return MICROEJ_ASYNC_WORKER_ERROR;
	}

	// Create mutex
	res = OSAL_mutex_create(name, &async_worker->mutex);
	if(res != OSAL_OK){
		return MICROEJ_ASYNC_WORKER_ERROR;
	}

	// Create task
	res = OSAL_task_create(MICROEJ_ASYNC_WORKER_loop, name, stack, priority, async_worker, &async_worker->task);
	if(res != OSAL_OK){
		return MICROEJ_ASYNC_WORKER_ERROR;
	}

	return MICROEJ_ASYNC_WORKER_OK;
}


MICROEJ_ASYNC_WORKER_job_t* MICROEJ_ASYNC_WORKER_allocate_job(MICROEJ_ASYNC_WORKER_handle_t* async_worker, SNI_callback sni_retry_callback){

	MICROEJ_ASYNC_WORKER_job_t* job = NULL;

	OSAL_mutex_take(&async_worker->mutex, OSAL_INFINITE_TIME);
	{
		job = async_worker->free_jobs;
		if(job != NULL){
			// Free job found: remove it from the free list
			async_worker->free_jobs = job->_intern.next_free_job;
			job->_intern.next_free_job = NULL;
		}
	}
	OSAL_mutex_give(&async_worker->mutex);

	if(job == NULL){
		// No free job available: wait for a free job.
		// Store the current thread id in the waiting list.
		// First check if there is a free element in the waiting list.
		int32_t free_waiting_thread_offset = async_worker->free_waiting_thread_offset;
		int32_t new_free_waiting_thread_offset = free_waiting_thread_offset + 1;
		if(new_free_waiting_thread_offset >= async_worker->waiting_threads_length){
			new_free_waiting_thread_offset = 0;
		}

		if(new_free_waiting_thread_offset == async_worker->waiting_thread_offset){
			// The waiting list is full.
			SNI_throwNativeIOException(-1, "MICROEJ_ASYNC_WORKER: thread cannot be suspended, waiting list is full.");
		}
		else {
			async_worker->free_waiting_thread_offset = (uint16_t)new_free_waiting_thread_offset;
			int32_t thread_id = SNI_getCurrentJavaThreadID();
			async_worker->waiting_threads[free_waiting_thread_offset] = thread_id;
			SNI_suspendCurrentJavaThreadWithCallback(0, sni_retry_callback, NULL);
		}
	}

	return job;
}


MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_free_job(MICROEJ_ASYNC_WORKER_handle_t* async_worker, MICROEJ_ASYNC_WORKER_job_t* job) {
	OSAL_mutex_take(&async_worker->mutex, OSAL_INFINITE_TIME);
	{
		job->_intern.next_free_job = async_worker->free_jobs;
		async_worker->free_jobs = job;

		int32_t waiting_thread_offset = async_worker->waiting_thread_offset;
		if(waiting_thread_offset != async_worker->free_waiting_thread_offset){
			// A thread was waiting for a free job: notify it
			int32_t thread_id = async_worker->waiting_threads[waiting_thread_offset];
			int32_t new_waiting_thread_offset = waiting_thread_offset + 1;
			if(new_waiting_thread_offset >= async_worker->waiting_threads_length){
				new_waiting_thread_offset = 0;
			}
			async_worker->waiting_thread_offset = new_waiting_thread_offset;
			SNI_resumeJavaThread(thread_id);
		}
	}
	OSAL_mutex_give(&async_worker->mutex);

	return MICROEJ_ASYNC_WORKER_OK;
}

MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_async_exec(MICROEJ_ASYNC_WORKER_handle_t* async_worker, MICROEJ_ASYNC_WORKER_job_t* job, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done_callback){
	return MICROEJ_ASYNC_WORKER_async_exec_intern(async_worker, job, action, on_done_callback, true);
}

MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_async_exec_no_wait(MICROEJ_ASYNC_WORKER_handle_t* async_worker, MICROEJ_ASYNC_WORKER_job_t* job, MICROEJ_ASYNC_WORKER_action_t action){
	return MICROEJ_ASYNC_WORKER_async_exec_intern(async_worker, job, action, NULL, false);
}

MICROEJ_ASYNC_WORKER_status_t MICROEJ_ASYNC_WORKER_async_exec_intern(MICROEJ_ASYNC_WORKER_handle_t* async_worker, MICROEJ_ASYNC_WORKER_job_t* job, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done_callback, bool wait){
	job->_intern.action = action;

	if(wait == true){
		job->_intern.thread_id = SNI_getCurrentJavaThreadID();
	}
	else {
		job->_intern.thread_id = SNI_ERROR;
	}

	OSAL_status_t res = OSAL_queue_post(&async_worker->jobs_queue, job);
	if(res == OSAL_OK){
		if(wait == true){
			SNI_suspendCurrentJavaThreadWithCallback(0, on_done_callback, job);
		}
		return MICROEJ_ASYNC_WORKER_OK;
	}
	else {
		SNI_throwNativeIOException(-1, "MICROEJ_ASYNC_WORKER: Internal error.");
		return MICROEJ_ASYNC_WORKER_ERROR;
	}
}


MICROEJ_ASYNC_WORKER_job_t* MICROEJ_ASYNC_WORKER_get_job_done(void){
	MICROEJ_ASYNC_WORKER_job_t* job = NULL;
	SNI_getCallbackArgs((void**)&job, NULL);
	return job;
}

static void* __attribute__((noreturn)) MICROEJ_ASYNC_WORKER_loop(void* args){
	MICROEJ_ASYNC_WORKER_handle_t* async_worker = (MICROEJ_ASYNC_WORKER_handle_t*) args;

	while(1){
		MICROEJ_ASYNC_WORKER_job_t* job;
		OSAL_status_t res = OSAL_queue_fetch(&async_worker->jobs_queue, (void**)&job, OSAL_INFINITE_TIME);

		if(res == OSAL_OK){
			// New job to execute
			job->_intern.action(job);
			if(job->_intern.thread_id != SNI_ERROR){
				SNI_resumeJavaThread(job->_intern.thread_id);
			}
			else {
				MICROEJ_ASYNC_WORKER_free_job(async_worker, job);
			}
		}
	}
}

#ifdef __cplusplus
	}
#endif


