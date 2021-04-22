/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 *
 */

#ifndef FS_HELPER_H
#define FS_HELPER_H

/**
 * @file
 * @brief LLFS implementation.
 * @author MicroEJ Developer Team
 * @version 1.1.0
 * @date 27 May 2020
 */

#include "fs_configuration.h"
#include "microej_async_worker.h"
#include "LLFS_impl.h"

#ifdef __cplusplus
	extern "C" {
#endif

/*
 * IMPORTANT NOTICES
 *
 * This file lists the data structures used by all the LLFS_* functions.
 * If you add a new function and its corresponding data structure, do not forget to add
 * the data structure to the FS_worker_param_t union.
 *
 * Some structures duplicate the definition of fields specified in generic structures instead of
 * referencing the generic structures (e.g. the <code>path</code> and <code>result</code> fields).
 * This decision has been made to simplify usage and readability at the expense of maintainability.
 * Be careful when modifying the fields of the structures or when defining new structures.
 * See the comment of each structure to identify the ones that are affected by these constraints.
 */


int32_t LLFS_set_path_param(uint8_t* path, uint8_t* path_param);
extern MICROEJ_ASYNC_WORKER_handle_t fs_worker;

/**
 * @brief Data structure for path operations.
 *
 * This structure is used by <code>LLFS_IMPL_create</code>, <code>LLFS_IMPL_set_read_only</code>,
 * <code>LLFS_IMPL_open_directory</code>, <code>LLFS_IMPL_exist</code>,
 * <code>LLFS_IMPL_make_directory</code>, <code>LLFS_IMPL_is_directory</code>,
 * <code>LLFS_IMPL_is_file</code> and <code>LLFS_IMPL_delete</code>.
 *
 * Fields defined in this structure correspond to the parameters of these functions, and
 * <code>result</code> field corresponds to the value returned by them.
 */
typedef struct {
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Path of the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
} FS_path_operation_t;

/**
 * @brief Data structure for path operations that return a 64-bit result.
 *
 * This structure is used by <code>LLFS_IMPL_get_length</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 */
typedef struct {
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Path of the operation. */
	int64_t result; /*!< [OUT] Result of the operation. */
} FS_path64_operation_t;

/**
 * @brief Data structure for operations getting/setting the last modification of a file.
 *
 * This structure is used by <code>LLFS_IMPL_get_last_modified()</code> and
 * <code>LLFS_IMPL_set_last_modified()</code>.
 *
 * Fields defined in this structure correspond to the parameters of these functions, and
 * <code>result</code> field corresponds to the value returned by them.
 *
 * @warning <code>path</code> and <code>result</code> fields must be declared in the same way as in
 * <code>FS_path_operation_t</code> structure.
 */
typedef struct {
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Path of the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	LLFS_date_t date; /*!< [IN/OUT] Date of the last modification. Input for the date setting, output for the date getting. */
} FS_last_modified_t;

/**
 * @brief Data structure for creation operations.
 *
 * This structure is used by <code>LLFS_IMPL_create</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 *
 * @warning <code>path</code> and <code>result</code> fields must be declared in the same way as in
 * <code>FS_path_operation_t</code> structure.
 */
typedef struct {
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Path of the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
} FS_create_t;

/**
 * @brief Data structure for renaming operations.
 *
 * This structure is used by <code>LLFS_IMPL_rename_to</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 *
 * @warning <code>path</code> and <code>result</code> fields must be declared in the same way as in
 * <code>FS_path_operation_t</code> structure.
 */
typedef struct {
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Old path of the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	uint8_t new_path[FS_PATH_LENGTH]; /*!< [IN] New path of the operation. */
} FS_rename_to_t;

/**
 * @brief Data structure for get space operations.
 *
 * This structure is used by <code>LLFS_IMPL_get_space_size</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 *
 * @warning <code>path</code> and <code>result</code> fields must be declared in the same way as in
 * <code>FS_path64_operation_t</code> structure.
 */
typedef struct {
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Path of the operation. */
	int64_t result; /*!< [OUT] Result of the operation. */
	int32_t space_type; /*!< [IN] Type of space. */
} FS_get_space_size;

/**
 * @brief Data structure for operations about file accessibility.
 *
 * This structure is used by <code>LLFS_IMPL_is_accessible</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 *
 * @warning <code>path</code> and <code>result</code> fields must be declared in the same way as in
 * <code>FS_path_operation_t</code> structure.
 */
typedef struct {
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Path of the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t access; /*!< [IN] Type of access. */
} FS_is_accessible_t;

/**
 * @brief Data structure for permission operations.
 *
 * This structure is used by <code>LLFS_IMPL_set_permission</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 *
 * @warning <code>path</code> and <code>result</code> fields must be declared in the same way as in
 * <code>FS_path_operation_t</code> structure.
 */
typedef struct {
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Path of the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t access; /*!< [IN] Type of the new access. */
	int32_t enable; /*!< [IN] Enable/Disable switch. */
	int32_t owner; /*!< [IN] File owner. */
} FS_set_permission_t;

/**
 * @brief Data structure for file opening operations.
 *
 * This structure is used by <code>LLFS_File_IMPL_open</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 *
 * @warning <code>path</code> and <code>result</code> fields must be declared in the same way as in
 * <code>FS_path_operation_t</code> structure.
 */
typedef struct {
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Path of the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	uint8_t mode; /*!< [IN] Opening mode. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
} FS_open_t;

/**
 * @brief Data structure for directory operations.
 *
 * This structure is used by <code>LLFS_async_exec_directory_job</code>.
 *
 * The field defined in this structure corresponds to the parameters of this function.
 */
typedef struct {
	int32_t directory_ID; /*!< [IN] ID of the directory on which to perform the operation. */
} FS_directory_operation_t;

/**
 * @brief Data structure for directory read operations.
 *
 * This structure is used by <code>LLFS_IMPL_read_directory</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 *
 * @warning <code>path</code> and <code>result</code> fields must be declared in the same way as in
 * <code>FS_path_operation_t</code> structure.
 */
typedef struct {
	int32_t directory_ID; /*!< [IN] ID of the directory on which to perform the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	uint8_t path[FS_PATH_LENGTH]; /*!< [IN] Path of the operation. */
} FS_read_directory_t;

/**
 * @brief Data structure for directory close operation.
 *
 * This structure is used by <code>LLFS_IMPL_close_directory</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 */
typedef struct {
	int32_t directory_ID; /*!< [IN] ID of the directory on which to perform the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
} FS_close_directory_t;

/**
 * @brief Data structure for read/write operations.
 *
 * This structure is used by <code>LLFS_File_IMPL_write</code>, <code>LLFS_File_IMPL_read</code>,
 * <code>LLFS_File_IMPL_write_byte</code> and <code>LLFS_File_IMPL_read_byte</code>.
 *
 * Fields defined in this structure correspond to the parameters of these functions, and
 * <code>result</code> field corresponds to the value returned by them.
 */
typedef struct {
	int32_t file_id; /*!< [IN] ID of the file on which to perform the operation. */
	uint8_t* data; /*!< [IN/OUT] Data read (output) or data to write (input). */
	int32_t length; /*!< [IN] Length of the data. */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
	uint8_t buffer[FS_IO_BUFFER_SIZE]; /*!< Internal buffer. Content must not be modified. */
} FS_write_read_t;

/**
 * @brief Data structure for file closing operations.
 *
 * This structure is used by <code>LLFS_File_IMPL_close</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 */
typedef struct {
	int32_t file_id; /*!< [IN] ID of the file on which to perform the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
} FS_close_t;

/**
 * @brief Data structure for skip operation.
 *
 * This structure is used by <code>LLFS_File_IMPL_skip</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 */
typedef struct {
	int32_t file_id; /*!< [IN] ID of the file on which to perform the operation. */
	int64_t n; /*!< [IN] Number of bytes to skip. */
	int64_t skipped_count; /*!< [OUT] Number of bytes skipped. */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
} FS_skip_t;

/**
 * @brief Data structure for availability checking operation.
 *
 * This structure is used by <code>LLFS_File_IMPL_available</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 */
typedef struct {
	int32_t file_id; /*!< [IN] ID of the file on which to perform the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
} FS_available_t;


/**
 * @brief Data structure for file flushing operations.
 *
 * This structure is used by <code>LLFS_File_IMPL_flush</code>.
 *
 * Fields defined in this structure correspond to the parameters of this function, and
 * <code>result</code> field corresponds to the value returned by it.
 */
typedef struct {
	int32_t file_id; /*!< [IN] ID of the file on which to perform the operation. */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
} FS_flush_t;
/**
 * @union FS_worker_param_t
 */
typedef union {
	FS_path_operation_t path_operation;
	FS_path64_operation_t path64_operation;
	FS_last_modified_t get_last_modified;
	FS_create_t create;
	FS_rename_to_t rename_to;
	FS_directory_operation_t directory_operation;
	FS_read_directory_t read_directory;
	FS_close_directory_t close_directory;
	FS_last_modified_t set_last_modified;
	FS_is_accessible_t is_accessible;
	FS_set_permission_t set_permission;
	FS_open_t open;
	FS_write_read_t write;
	FS_close_t close;
	FS_skip_t skip;
	FS_available_t available;
	FS_flush_t flush;
} FS_worker_param_t;

void LLFS_IMPL_mount(void);

void LLFS_IMPL_get_last_modified_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_set_read_only_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_create_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_open_directory_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_read_directory_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_close_directory_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_rename_to_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_get_length_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_exist_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_get_space_size_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_make_directory_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_is_hidden_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_is_directory_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_is_file_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_set_last_modified_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_delete_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_is_accessible_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_IMPL_set_permission_action(MICROEJ_ASYNC_WORKER_job_t* job);

void LLFS_File_IMPL_open_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_File_IMPL_write_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_File_IMPL_read_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_File_IMPL_close_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_File_IMPL_skip_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_File_IMPL_available_action(MICROEJ_ASYNC_WORKER_job_t* job);
void LLFS_File_IMPL_flush_action(MICROEJ_ASYNC_WORKER_job_t* job);

#ifdef __cplusplus
	}
#endif



#endif /* FS_HELPER_H */
