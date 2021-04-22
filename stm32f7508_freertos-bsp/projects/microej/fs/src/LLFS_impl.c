/* 
 * C
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLFS implementation with async worker.
 * @author MicroEJ Developer Team
 * @version 1.1.0
 * @date 27 May 2020
 */

/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "LLFS_impl.h"
#include "sni.h"
#include "microej_async_worker.h"
#include "fs_helper.h"
#include "fs_configuration.h"

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

//TODO: init errorcode and error message in caller

MICROEJ_ASYNC_WORKER_worker_declare(fs_worker, FS_WORKER_JOB_COUNT, FS_worker_param_t, FS_WAITING_LIST_SIZE);
OSAL_task_stack_declare(fs_worker_stack, FS_WORKER_STACK_SIZE);

int32_t LLFS_set_path_param(uint8_t* path, uint8_t* path_param){
	int32_t path_length = SNI_getArrayLength(path);
	if(path_length > FS_PATH_LENGTH){
		// Path too long for the buffer
		//TODO: remove next line, review if/else structure
		//SNI_throwNativeException(LLFS_NOK, "path length is too long");
		return LLFS_NOK;
	}

	memcpy(path_param, path, path_length);
	return LLFS_OK;
}

static MICROEJ_ASYNC_WORKER_job_t* LLFS_allocate_path_job(uint8_t* path, SNI_callback sni_retry_callback){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, sni_retry_callback);
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

static int32_t LLFS_async_exec_path_job(uint8_t* path, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done){

	MICROEJ_ASYNC_WORKER_job_t* job = LLFS_allocate_path_job(path, retry_function);
	if(job == NULL){
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
		return LLFS_OK;//returned value not used
	}
}

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

static int32_t LLFS_async_exec_path_result(){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_path_operation_t* params = (FS_path_operation_t*)job->params;

	int32_t result = params->result;
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return result;
}

void LLFS_IMPL_initialize(void)
{
	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_initialize(&fs_worker, (uint8_t *)"MicroEJ FS", fs_worker_stack, FS_WORKER_PRIORITY);
	if(status == MICROEJ_ASYNC_WORKER_INVALID_ARGS){
		SNI_throwNativeException(status, "Invalid argument for FS async worker");
	}
	else if(status == MICROEJ_ASYNC_WORKER_ERROR){
		SNI_throwNativeException(status, "Error while initializing FS async worker");
	}
	// else OK

	LLFS_IMPL_mount();
}


int32_t LLFS_IMPL_get_max_path_length(void){
	return FS_PATH_LENGTH;
}

static int32_t LLFS_IMPL_get_last_modified_on_done(uint8_t* path, LLFS_date_t* date);

int32_t LLFS_IMPL_get_last_modified(uint8_t* path, LLFS_date_t* date)
{
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_get_last_modified, LLFS_IMPL_get_last_modified_action, (SNI_callback)LLFS_IMPL_get_last_modified_on_done);
}

static int32_t LLFS_IMPL_get_last_modified_on_done(uint8_t* path, LLFS_date_t* date){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_last_modified_t* params = (FS_last_modified_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_OK){
		*date = params->date;
	}
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}

static int32_t LLFS_IMPL_path_function_on_done(uint8_t* path){
	return LLFS_async_exec_path_result();
}

static int64_t LLFS_IMPL_path64_function_on_done(uint8_t* path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_path64_operation_t* params = (FS_path64_operation_t*)job->params;

	int64_t result = params->result;
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return result;
}

int32_t LLFS_IMPL_set_read_only(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_set_read_only, LLFS_IMPL_set_read_only_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

static int32_t LLFS_IMPL_create_on_done(uint8_t* path);

int32_t LLFS_IMPL_create(uint8_t* path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_IMPL_create);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return LLFS_NOK; // Unused value
	}

	FS_path_operation_t* params = (FS_path_operation_t*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) != LLFS_OK){
		SNI_throwNativeIOException(LLFS_NOK, "Path name too long");
	}
	else{
		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_IMPL_create_action, (SNI_callback)LLFS_IMPL_create_on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return LLFS_OK;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

static int32_t LLFS_IMPL_create_on_done(uint8_t* path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_create_t* params = (FS_create_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}

int32_t LLFS_IMPL_open_directory(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_open_directory, LLFS_IMPL_open_directory_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

static int32_t LLFS_IMPL_read_directory_on_done(int32_t directory_ID, uint8_t* path);

int32_t LLFS_IMPL_read_directory(int32_t directory_ID, uint8_t* path){
	return LLFS_async_exec_directory_job(directory_ID, (SNI_callback)LLFS_IMPL_read_directory, LLFS_IMPL_read_directory_action, (SNI_callback)LLFS_IMPL_read_directory_on_done);
}

static int32_t LLFS_IMPL_read_directory_on_done(int32_t directory_ID, uint8_t* path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_read_directory_t* params = (FS_read_directory_t*)job->params;

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

static int32_t LLFS_IMPL_close_directory_on_done(int32_t directory_ID);

int32_t LLFS_IMPL_close_directory(int32_t directory_ID){
	return LLFS_async_exec_directory_job(directory_ID, (SNI_callback)LLFS_IMPL_close_directory, LLFS_IMPL_close_directory_action, (SNI_callback)LLFS_IMPL_close_directory_on_done);
}

static int32_t LLFS_IMPL_close_directory_on_done(int32_t directory_ID){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_close_directory_t* params = (FS_close_directory_t*)job->params;

	int32_t result = params->result;
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return result;
}

static int32_t LLFS_IMPL_rename_to_on_done(uint8_t* path, uint8_t* new_path);

int32_t LLFS_IMPL_rename_to(uint8_t* path, uint8_t* new_path){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_IMPL_rename_to);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending.
		return LLFS_NOK;
	}

	FS_rename_to_t* params = (FS_rename_to_t*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) == LLFS_OK &&
	   LLFS_set_path_param(new_path, (uint8_t*)&params->new_path) == LLFS_OK){

		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_IMPL_rename_to_action, (SNI_callback)LLFS_IMPL_rename_to_on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return LLFS_OK;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

static int32_t LLFS_IMPL_rename_to_on_done(uint8_t* path, uint8_t* new_path){
	return LLFS_async_exec_path_result();
}

int64_t LLFS_IMPL_get_length(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_get_length, LLFS_IMPL_get_length_action, (SNI_callback)LLFS_IMPL_path64_function_on_done);
}

int32_t LLFS_IMPL_exist(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_exist, LLFS_IMPL_exist_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

int64_t LLFS_IMPL_get_space_size_on_done(uint8_t* path, int32_t space_type);

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
			return LLFS_OK;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

int64_t LLFS_IMPL_get_space_size_on_done(uint8_t* path, int32_t space_type){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_get_space_size* params = (FS_get_space_size*)job->params;

	int64_t result = params->result;
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return result;
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

static int32_t LLFS_IMPL_set_last_modified_on_done(uint8_t* path, LLFS_date_t* date);

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
			return LLFS_OK;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

static int32_t LLFS_IMPL_set_last_modified_on_done(uint8_t* path, LLFS_date_t* date){
	return LLFS_async_exec_path_result();
}

int32_t LLFS_IMPL_delete(uint8_t* path){
	return LLFS_async_exec_path_job(path, (SNI_callback)LLFS_IMPL_delete, LLFS_IMPL_delete_action, (SNI_callback)LLFS_IMPL_path_function_on_done);
}

static int32_t LLFS_IMPL_is_accessible_on_done(uint8_t* path, int32_t access);

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
			return LLFS_OK;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

static int32_t LLFS_IMPL_is_accessible_on_done(uint8_t* path, int32_t access){
	return LLFS_async_exec_path_result();
}

static int32_t LLFS_IMPL_set_permission_on_done(uint8_t* path, int32_t access, int32_t enable, int32_t owner);

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
			return LLFS_OK;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

static int32_t LLFS_IMPL_set_permission_on_done(uint8_t* path, int32_t access, int32_t enable, int32_t owner){
	return LLFS_async_exec_path_result();
}


#ifdef __cplusplus
	}
#endif


