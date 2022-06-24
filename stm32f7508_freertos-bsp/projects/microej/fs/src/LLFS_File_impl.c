/* 
 * C
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief LLFS_File implementation with async worker.
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
#include "LLFS_File_impl.h"
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

static int32_t LLFS_File_IMPL_open_on_done(uint8_t* path, uint8_t mode);
static int32_t LLFS_File_IMPL_write_on_done(int32_t file_id, uint8_t* data, int32_t offset, int32_t length);
static void LLFS_File_IMPL_write_byte_on_done(int32_t file_id, int32_t data);
static int32_t LLFS_File_IMPL_read_on_done(int32_t file_id, uint8_t* data, int32_t offset, int32_t length);
static int32_t LLFS_File_IMPL_read_byte_on_done(int32_t file_id);
static void LLFS_File_IMPL_close_on_done(int32_t file_id);
static int64_t LLFS_File_IMPL_skip_on_done(int32_t file_id, int64_t n);
static int32_t LLFS_File_IMPL_available_on_done(int32_t file_id);
static void LLFS_File_IMPL_flush_on_done(int32_t file_id);

int32_t LLFS_File_IMPL_open(uint8_t* path, uint8_t mode){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_File_IMPL_open);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return LLFS_NOK; // Unused value
	}

	FS_open_t* params = (FS_open_t*)job->params;
	if(LLFS_set_path_param(path, (uint8_t*)&params->path) != LLFS_OK){
		SNI_throwNativeIOException(LLFS_NOK, "Path name too long");
	}
	else{
		params->mode = mode;

		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_File_IMPL_open_action, (SNI_callback)LLFS_File_IMPL_open_on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return LLFS_OK;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

static int32_t LLFS_File_IMPL_open_on_done(uint8_t* path, uint8_t mode){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_open_t* params = (FS_open_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}

static int32_t LLFS_async_exec_write_read_job(int32_t file_id, uint8_t* data, int32_t offset, int32_t length, bool exec_write, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done){

	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, retry_function);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return LLFS_NOK; // Unused value
	}

	FS_write_read_t* params = (FS_write_read_t*)job->params;

	bool do_copy = exec_write;
	int32_t result = SNI_retrieveArrayElements((int8_t *)data, offset, length, (int8_t*)&params->buffer, sizeof(params->buffer), (int8_t**)&params->data, (uint32_t *)&params->length, do_copy);

	if(result != SNI_OK){
		SNI_throwNativeIOException(result, "SNI_retrieveArrayElements: Internal error");
	}
	else {
		params->file_id = file_id;

		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, action, on_done);
		if(status == MICROEJ_ASYNC_WORKER_OK){
			// Wait for the action to be done
			return LLFS_OK;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

int32_t LLFS_File_IMPL_write(int32_t file_id, uint8_t* data, int32_t offset, int32_t length){
	return LLFS_async_exec_write_read_job(file_id, data, offset, length, true, (SNI_callback)LLFS_File_IMPL_write, LLFS_File_IMPL_write_action, (SNI_callback)LLFS_File_IMPL_write_on_done);
}

int32_t LLFS_File_IMPL_write_on_done(int32_t file_id, uint8_t* data, int32_t offset, int32_t length){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_write_read_t* params = (FS_write_read_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}


int32_t LLFS_File_IMPL_read(int32_t file_id, uint8_t* data, int32_t offset, int32_t length){
	return LLFS_async_exec_write_read_job(file_id, data, offset, length, false, (SNI_callback)LLFS_File_IMPL_read, LLFS_File_IMPL_read_action, (SNI_callback)LLFS_File_IMPL_read_on_done);
}

static int32_t LLFS_File_IMPL_read_on_done(int32_t file_id, uint8_t* data, int32_t offset, int32_t length){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_write_read_t* params = (FS_write_read_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}
	else if(result != LLFS_EOF){
		int32_t release_result = SNI_flushArrayElements((int8_t *)data, offset, length, (int8_t *)params->data, result);
		if(release_result != SNI_OK){
			SNI_throwNativeIOException(release_result, "SNI_flushArrayElements: Internal error");
		}
	}
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}

static int32_t LLFS_async_exec_write_read_byte_job(int32_t file_id, int32_t data, bool exec_write, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, retry_function);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return LLFS_NOK; // Unused value
	}

	FS_write_read_t* params = (FS_write_read_t*)job->params;

	params->file_id = file_id;
	params->data = (uint8_t*)&params->buffer;
	params->length = sizeof(uint8_t);
	if(exec_write == true){
		params->buffer[0] = (uint8_t)data;
	}

	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, action, on_done);
	if(status == MICROEJ_ASYNC_WORKER_OK){
		// Wait for the action to be done
		return LLFS_OK;//returned value not used
	} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}



void LLFS_File_IMPL_write_byte(int32_t file_id, int32_t data){
	LLFS_async_exec_write_read_byte_job(file_id, data, true, (SNI_callback)LLFS_File_IMPL_write_byte, LLFS_File_IMPL_write_action, (SNI_callback)LLFS_File_IMPL_write_byte_on_done);
}

static void LLFS_File_IMPL_write_byte_on_done(int32_t file_id, int32_t data){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_write_read_t* params = (FS_write_read_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
}


int32_t LLFS_File_IMPL_read_byte(int32_t file_id){
	return LLFS_async_exec_write_read_byte_job(file_id, 0, false, (SNI_callback)LLFS_File_IMPL_read_byte, LLFS_File_IMPL_read_action, (SNI_callback)LLFS_File_IMPL_read_byte_on_done);
}

int32_t LLFS_File_IMPL_read_byte_on_done(int32_t file_id){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_write_read_t* params = (FS_write_read_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}
	else if(result == 1) {
		// 1 byte read: return the read byte
		result = (uint8_t)params->data[0];
	}
	else if(result != LLFS_EOF) {
		// Invalid value returned by the read_byte action
		SNI_throwNativeIOException(result, "Internal error");
	}
	// else: result==LLFS_EOF: just return LLFS_EOF
	
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}

void LLFS_File_IMPL_close(int32_t file_id){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_File_IMPL_close);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return;
	}

	FS_close_t* params = (FS_close_t*)job->params;
	params->file_id = file_id;

	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_File_IMPL_close_action, (SNI_callback)LLFS_File_IMPL_close_on_done);
	if(status == MICROEJ_ASYNC_WORKER_OK){
		// Wait for the action to be done
		return;
	} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
}

static void LLFS_File_IMPL_close_on_done(int32_t file_id){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_close_t* params = (FS_close_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}

	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
}

int64_t LLFS_File_IMPL_skip(int32_t file_id, int64_t n){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_File_IMPL_skip);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return LLFS_NOK;
	}

	FS_skip_t* params = (FS_skip_t*)job->params;
	params->file_id = file_id;
	params->n = n;

	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_File_IMPL_skip_action, (SNI_callback)LLFS_File_IMPL_skip_on_done);
	if(status == MICROEJ_ASYNC_WORKER_OK){
		// Wait for the action to be done
		return LLFS_OK;
	} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

static int64_t LLFS_File_IMPL_skip_on_done(int32_t file_id, int64_t n){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_skip_t* params = (FS_skip_t*)job->params;

	int64_t skipped_count;
	if(params->result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
		skipped_count = 0;
	}
	else {
		skipped_count = params->skipped_count;
	}

	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return skipped_count;
}

int32_t LLFS_File_IMPL_available(int32_t file_id){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_File_IMPL_available);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return LLFS_NOK;
	}

	FS_available_t* params = (FS_available_t*)job->params;
	params->file_id = file_id;

	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_File_IMPL_available_action, (SNI_callback)LLFS_File_IMPL_available_on_done);
	if(status == MICROEJ_ASYNC_WORKER_OK){
		// Wait for the action to be done
		return LLFS_OK;
	} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
	return LLFS_NOK;
}

static int32_t LLFS_File_IMPL_available_on_done(int32_t file_id){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_available_t* params = (FS_available_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}

	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);

	return result;
}


void LLFS_File_IMPL_flush(int32_t file_id){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&fs_worker, (SNI_callback)LLFS_File_IMPL_flush);
	if(job == NULL){
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return;
	}

	FS_flush_t* params = (FS_flush_t*)job->params;
	params->file_id = file_id;

	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&fs_worker, job, LLFS_File_IMPL_flush_action, (SNI_callback)LLFS_File_IMPL_flush_on_done);
	if(status == MICROEJ_ASYNC_WORKER_OK){
		// Wait for the action to be done
		return;
	} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception

	// Error
	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
}

static void LLFS_File_IMPL_flush_on_done(int32_t file_id){
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	FS_flush_t* params = (FS_flush_t*)job->params;

	int32_t result = params->result;
	if(result == LLFS_NOK){
		// Exception
		SNI_throwNativeIOException(params->error_code, params->error_message);
	}

	MICROEJ_ASYNC_WORKER_free_job(&fs_worker, job);
}


#ifdef __cplusplus
	}
#endif


