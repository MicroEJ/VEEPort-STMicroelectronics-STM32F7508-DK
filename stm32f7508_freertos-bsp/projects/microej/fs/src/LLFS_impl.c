/*
 * C
 *
 * Copyright 2015-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLFS implementation with async worker.
 * @author MicroEJ Developer Team
 * @version 2.1.0
 * @date 17 June 2022
 */

/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "sni.h"
#include "LLFS_impl.h"
#include "fs_configuration.h"
#include "fs_helper.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Sanity check between the expected version of the configuration and the actual version of
 * the configuration.
 * If an error is raised here, it means that a new version of the CCO has been installed and
 * the configuration fs_configuration.h must be updated based on the one provided
 * by the new CCO version.
 */
#if FS_CONFIGURATION_VERSION != 1

	#error "Version of the configuration file fs_configuration.h is not compatible with this implementation."

#endif

#ifndef FS_CUSTOM_WORKER
/* Async worker task declaration ---------------------------------------------*/
MICROEJ_ASYNC_WORKER_worker_declare(fs_worker, FS_WORKER_JOB_COUNT, FS_worker_param_t, FS_WAITING_LIST_SIZE);
OSAL_task_stack_declare(fs_worker_stack, FS_WORKER_STACK_SIZE);
#endif

static MICROEJ_ASYNC_WORKER_job_t* LLFS_allocate_path_job(uint8_t* path, SNI_callback retry_function);
static int32_t LLFS_async_exec_path_job(uint8_t* path, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done);
static int32_t LLFS_async_exec_directory_job(int32_t directory_ID, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done);
static int32_t LLFS_IMPL_get_last_modified_on_done(uint8_t* path, LLFS_date_t* date);
static int32_t LLFS_IMPL_path_function_on_done(uint8_t* path);
static int64_t LLFS_IMPL_path64_function_on_done(uint8_t* path);
static int32_t LLFS_IMPL_create_on_done(uint8_t* path);
static int32_t LLFS_IMPL_read_directory_on_done(int32_t directory_ID, uint8_t* path);
static int32_t LLFS_IMPL_close_directory_on_done(int32_t directory_ID);
static int32_t LLFS_IMPL_rename_to_on_done(uint8_t* path, uint8_t* new_path);
static int64_t LLFS_IMPL_get_space_size_on_done(uint8_t* path, int32_t space_type);
static int32_t LLFS_IMPL_set_last_modified_on_done(uint8_t* path, LLFS_date_t* date);
static int32_t LLFS_IMPL_is_accessible_on_done(uint8_t* path, int32_t access);
static int32_t LLFS_IMPL_set_permission_on_done(uint8_t* path, int32_t access, int32_t enable, int32_t owner);
static int32_t LLFS_async_exec_path_result(void);

void LLFS_IMPL_initialize(void){
#ifndef FS_CUSTOM_WORKER
	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_initialize(&fs_worker, (uint8_t*)"MicroEJ FS", fs_worker_stack, FS_WORKER_PRIORITY);
	if(status == MICROEJ_ASYNC_WORKER_INVALID_ARGS){
		SNI_throwNativeException(status, "Invalid argument for FS async worker");
	}else if (status == MICROEJ_ASYNC_WORKER_ERROR){
		SNI_throwNativeException(status, "Error while initializing FS async worker");
	}
#endif

	llfs_init();
}

int32_t LLFS_IMPL_get_max_path_length(void){
	return FS_PATH_LENGTH;
}

int32_t LLFS_IMPL_get_last_modified(uint8_t* path, LLFS_date_t* date){
	(void)date;

	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_get_last_modified, LLFS_IMPL_get_last_modified_action, (SNI_callback)LLFS_IMPL_get_last_modified_on_done);
}

int32_t LLFS_IMPL_set_read_only(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_set_read_only, LLFS_IMPL_set_read_only_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

int32_t LLFS_IMPL_create(uint8_t* path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_IMPL_create);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return LLFS_NOK;
	}

	FS_path_operation_t* params = (FS_path_operation_t*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) != LLFS_OK){
		SNI_throwNativeIOException(LLFS_NOK, "Path name too long");
	}
	else{
		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_IMPL_create_action, (SNI_callback)LLFS_IMPL_create_on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

int32_t LLFS_IMPL_open_directory(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_open_directory, LLFS_IMPL_open_directory_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

int32_t LLFS_IMPL_read_directory(int32_t directory_ID, uint8_t* path){
	(void)path;

	return LLFS_async_exec_directory_job(directory_ID, (SNI_callback)LLFS_IMPL_read_directory, LLFS_IMPL_read_directory_action, (SNI_callback)LLFS_IMPL_read_directory_on_done);
}

int32_t LLFS_IMPL_close_directory(int32_t directory_ID){
	return LLFS_async_exec_directory_job(directory_ID, (SNI_callback)LLFS_IMPL_close_directory, LLFS_IMPL_close_directory_action, (SNI_callback)LLFS_IMPL_close_directory_on_done);
}

int32_t LLFS_IMPL_rename_to(uint8_t* path, uint8_t* new_path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_IMPL_rename_to);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending.
		return LLFS_NOK;
	}

	FS_rename_to_t* params = (FS_rename_to_t*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) == LLFS_OK && LLFS_set_path_param(new_path, (uint8_t*)&params->new_path) == LLFS_OK){
		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_IMPL_rename_to_action, (SNI_callback)LLFS_IMPL_rename_to_on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

int64_t LLFS_IMPL_get_length(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_get_length, LLFS_IMPL_get_length_action, (SNI_callback)LLFS_IMPL_path64_function_on_done);
}

int32_t LLFS_IMPL_exist(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_exist, LLFS_IMPL_exist_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

int64_t LLFS_IMPL_get_space_size(uint8_t* path, int32_t space_type){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_IMPL_get_space_size);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending.
		return LLFS_NOK;
	}

	FS_get_space_size* params = (FS_get_space_size*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) == LLFS_OK){
		params->space_type = space_type;

		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_IMPL_get_space_size_action, (SNI_callback)LLFS_IMPL_get_space_size_on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

int32_t LLFS_IMPL_make_directory(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_make_directory, LLFS_IMPL_make_directory_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

int32_t LLFS_IMPL_is_hidden(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_is_hidden, LLFS_IMPL_is_hidden_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

int32_t LLFS_IMPL_is_directory(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_is_directory, LLFS_IMPL_is_directory_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

int32_t LLFS_IMPL_is_file(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_is_file, LLFS_IMPL_is_file_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

int32_t LLFS_IMPL_set_last_modified(uint8_t* path, LLFS_date_t* date){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_IMPL_set_last_modified);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending.
		return LLFS_NOK;
	}

	FS_last_modified_t* params = (FS_last_modified_t*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) == LLFS_OK){
		params->date = *date;

		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_IMPL_set_last_modified_action, (SNI_callback)LLFS_IMPL_set_last_modified_on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

int32_t LLFS_IMPL_delete(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_delete, LLFS_IMPL_delete_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

int32_t LLFS_IMPL_is_accessible(uint8_t* path, int32_t access){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_IMPL_is_accessible);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending.
		return LLFS_NOK;
	}

	FS_is_accessible_t* params = (FS_is_accessible_t*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) == LLFS_OK){
		params->access = access;

		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_IMPL_is_accessible_action, (SNI_callback)LLFS_IMPL_is_accessible_on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

int32_t LLFS_IMPL_set_permission(uint8_t* path, int32_t access, int32_t enable, int32_t owner){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_IMPL_set_permission);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending.
		return LLFS_NOK;
	}

	FS_set_permission_t* params = (FS_set_permission_t*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) == LLFS_OK){
		params->access = access;
		params->enable = enable;
		params->owner = owner;

		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_IMPL_set_permission_action, (SNI_callback)LLFS_IMPL_set_permission_on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

int32_t LLFS_set_path_param(uint8_t* path, uint8_t* path_param){
	int32_t path_length = SNI_getArrayLength(path);
	if(path_length > FS_PATH_LENGTH){
		// Path too long for the buffer
		SNI_throwNativeException(LLFS_NOK, "Path length is too long");
		return LLFS_NOK;
	}

	memcpy(path_param, path, path_length);
	return LLFS_OK;
}

/**
 * @brief Allocates an async_worker job containing a generic file path buffer.
 *
 * @param[in] path absolute path of file.
 * @param[in] retry_function if the current Java thread has been suspended, this function is called when it is resumed.
 *
 * @return an allocated job object or NULL if the allocation fails.
 */
static MICROEJ_ASYNC_WORKER_job_t* LLFS_allocate_path_job(uint8_t* path, SNI_callback retry_function){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, retry_function);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending.
		return NULL;
	}

	FS_path_operation_t* params = (FS_path_operation_t*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) != LLFS_OK){
		// Path name too long
		MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
		return NULL;
	}

	return job;
}

/**
 * @brief Prepare and send an execution job to async_worker, called either from
 * <code>LLFS_IMPL_get_last_modified</code>, <code>LLFS_IMPL_set_read_only</code>,
 * <code>LLFS_IMPL_open_directory</code>, <code>LLFS_IMPL_get_length</code>,
 * <code>LLFS_IMPL_exist</code>, <code>LLFS_IMPL_make_directory</code>,
 * <code>LLFS_IMPL_is_hidden</code>, <code>LLFS_IMPL_is_directory</code>,
 * <code>LLFS_IMPL_is_file</code> or <code>LLFS_IMPL_delete</code>.
 *
 * @param[in] path absolute path of file.
 * @param[in] retry_function if the current Java thread has been suspended, this function is called when it is resumed.
 * @param[in] action the function to execute asynchronously.
 * @param[in] on_done the <code>SNI_callback</code> called when the job is done.
 *
 * @return <code>SNI_IGNORED_RETURNED_VALUE</code> on success, else a negative error code.
 */
static int32_t LLFS_async_exec_path_job(uint8_t* path, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done){
	MICROEJ_ASYNC_WORKER_job_t* job = LLFS_allocate_path_job(path, retry_function);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending.
		return LLFS_NOK;
	}

	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, action, on_done);
	if(status != MICROEJ_ASYNC_WORKER_OK){
		// an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
		MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
		return LLFS_NOK;
	}
	else {
		// Wait for the action to be done
		return SNI_IGNORED_RETURNED_VALUE;//returned value not used
	}
}

/**
 * @brief Prepare and send an execution job to async_worker, called either from
 * <code>LLFS_IMPL_read_directory</code> or <code>LLFS_IMPL_close_directory</code>.
 *
 * @param[in] directory_ID the directory ID.
 * @param[in] retry_function if the current Java thread has been suspended, this function is called when it is resumed.
 * @param[in] action the function to execute asynchronously.
 * @param[in] on_done the <code>SNI_callback</code> called when the job is done.
 *
 * @return <code>SNI_IGNORED_RETURNED_VALUE</code> on success, else a negative error code.
 */
static int32_t LLFS_async_exec_directory_job(int32_t directory_ID, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, retry_function);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending.
		return LLFS_NOK;
	}

	FS_directory_operation_t* params = (FS_directory_operation_t*)job->params;
	params->directory_ID = directory_ID;

	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, action, on_done);

	if(status != MICROEJ_ASYNC_WORKER_OK){
		// an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
		MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
		return LLFS_NOK;
	}
	else {
		// Wait for the action to be done
		return LLFS_OK;//returned value not used
	}
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_get_last_modified</code> is done.
 *
 * @param[in] path absolute path of file.
 * @param[out] date the structure where the date is stored.
 *
 * @return <code>LLFS_IMPL_get_last_modified_action</code> function return code.
 */
static int32_t LLFS_IMPL_get_last_modified_on_done(uint8_t* path, LLFS_date_t* date){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_last_modified_t* params = (FS_last_modified_t*)job->params;

	(void)path;

	int32_t result = params->result;
	if(result == LLFS_OK){
		*date = params->date;
	}
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_set_read_only</code>,
 * <code>LLFS_IMPL_open_directory</code>, <code>LLFS_IMPL_exist</code>,
 * <code>LLFS_IMPL_make_directory</code>, <code>LLFS_IMPL_is_hidden</code>,
 * <code>LLFS_IMPL_is_directory</code>, <code>LLFS_IMPL_is_file</code> or
 * <code>LLFS_IMPL_delete</code> is done.
 *
 * @param[in] path absolute path of file.
 *
 * @return <code>LLFS_IMPL_set_read_only_action</code>,
 * <code>LLFS_IMPL_open_directory_action</code>, <code>LLFS_IMPL_exist_action</code>,
 * <code>LLFS_IMPL_make_directory_action</code>, <code>LLFS_IMPL_is_hidden_action</code>,
 * <code>LLFS_IMPL_is_directory_action</code>, <code>LLFS_IMPL_is_file_action</code> or
 * <code>LLFS_IMPL_delete_action</code> function return code.
 */
static int32_t LLFS_IMPL_path_function_on_done(uint8_t* path){
	(void)path;

	return LLFS_async_exec_path_result();
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_get_length</code> is done.
 *
 * @param[in] path absolute path of file.
 *
 * @return <code>LLFS_IMPL_get_length_action</code> function return code.
 */
static int64_t LLFS_IMPL_path64_function_on_done(uint8_t* path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_path64_operation_t* params = (FS_path64_operation_t*)job->params;

	(void)path;

	int64_t result = params->result;
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return result;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_create</code> is done.
 *
 * @param[in] path absolute path of file.
 *
 * @return <code>LLFS_IMPL_create_action</code> function return code.
 */
static int32_t LLFS_IMPL_create_on_done(uint8_t* path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_create_t* params = (FS_create_t*)job->params;

	(void)path;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_read_directory</code> is done.
 *
 * @param[in] directory_ID the directory ID.
 * @param[out] path path where to store the next available file.
 *
 * @return <code>LLFS_IMPL_read_directory_action</code> function return code.
 */
static int32_t LLFS_IMPL_read_directory_on_done(int32_t directory_ID, uint8_t* path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_read_directory_t* params = (FS_read_directory_t*)job->params;

	(void)directory_ID;

	int32_t result = params->result;
	if(result == LLFS_OK){
		// Copy back the read path
		int32_t java_path_length = SNI_getArrayLength(path);
		int32_t path_length = strlen((char *)params->path)+1;//add 1 for the terminating null byte ('\0').
		if(java_path_length >= path_length){
			// Buffer large enough for the path
			memcpy(path, params->path, path_length);
		}
		else {
			// Path too long for the buffer
			result = LLFS_NOK;
		}
	}
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_close_directory</code> is done.
 *
 * @param[in] directory_ID the directory ID.
 *
 * @return <code>LLFS_IMPL_close_directory_action</code> function return code.
 */
static int32_t LLFS_IMPL_close_directory_on_done(int32_t directory_ID){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_close_directory_t* params = (FS_close_directory_t*)job->params;

	(void)directory_ID;

	int32_t result = params->result;
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return result;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_rename_to</code> is done.
 *
 * @param[in] path absolute path of file to modify.
 * @param[in] new_path new absolute path of file.
 *
 * @return <code>LLFS_IMPL_rename_to_action</code> function return code.
 */
static int32_t LLFS_IMPL_rename_to_on_done(uint8_t* path, uint8_t* new_path){
	(void)path;
	(void)new_path;

	return LLFS_async_exec_path_result();
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_get_space_size</code> is done.
 *
 * @param[in] path absolute path of file in the require partition.
 * @param[in] space_type space type.
 *
 * @return <code>LLFS_IMPL_get_space_size_action</code> function return code.
 */
static int64_t LLFS_IMPL_get_space_size_on_done(uint8_t* path, int32_t space_type){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_get_space_size* params = (FS_get_space_size*)job->params;

	(void)path;
	(void)space_type;

	int64_t result = params->result;
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return result;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_set_last_modified</code> is done.
 *
 * @param[in] path absolute path of file to modified.
 * @param[in] date the new date of the file.
 *
 * @return <code>LLFS_IMPL_set_last_modified_action</code> function return code.
 */
static int32_t LLFS_IMPL_set_last_modified_on_done(uint8_t* path, LLFS_date_t* date){
	(void)path;
	(void)date;

	return LLFS_async_exec_path_result();
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_is_accessible</code> is done.
 *
 * @param[in] path absolute path of file to modified.
 * @param[in] access access type to be checked.
 *
 * @return <code>LLFS_IMPL_is_accessible_action</code> function return code.
 */
static int32_t LLFS_IMPL_is_accessible_on_done(uint8_t* path, int32_t access){
	(void)path;
	(void)access;

	return LLFS_async_exec_path_result();
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by <code>LLFS_IMPL_set_permission</code> is done.
 *
 * @param[in] path absolute path of file to modified.
 * @param[in] access access type to set.
 * @param[in] enable permission enable or disable.
 * @param[in] owner permission owner.
 *
 * @return <code>LLFS_IMPL_set_permission_action</code> function return code.
 */
static int32_t LLFS_IMPL_set_permission_on_done(uint8_t* path, int32_t access, int32_t enable, int32_t owner){
	(void)path;
	(void)access;
	(void)enable;
	(void)owner;

	return LLFS_async_exec_path_result();
}

/**
 * @brief Unified handling for <code>SNI_callback</code>, called via
 * <code>LLFS_IMPL_path_function_on_done</code>, <code>LLFS_IMPL_rename_to_on_done</code>,
 * <code>LLFS_IMPL_set_last_modified_on_done</code>, <code>LLFS_IMPL_is_accessible_on_done</code> and
 * <code>LLFS_IMPL_set_permission_on_done</code> functions.
 *
 * @return @see function callers return code.
 */
static int32_t LLFS_async_exec_path_result(void){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_path_operation_t* params = (FS_path_operation_t*)job->params;

	int32_t result = params->result;
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return result;
}

#ifdef __cplusplus
	}
#endif
