/*
 * C
 *
 * Copyright 2020-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 */

/**
 * @file
 * @brief FatFs helper for LLFS.
 * @author MicroEJ Developer Team
 * @version 2.1.0
 */

#include <stdint.h>
#include <string.h>
#include "ff.h"
#include "fs_helper.h"
#include "microej_async_worker.h"
#include "microej_pool.h"
#include "LLFS_File_impl.h"
#include "diskio.h"

#ifdef __cplusplus
	extern "C" {
#endif

/** @brief define the amount of file in private pool module */
#define FS_MAX_NUMBER_OF_FILE_IN_POOL  (_FS_LOCK)

/** @brief define the amount of dir in private pool module */
#define FS_MAX_NUMBER_OF_DIR_IN_POOL  (_FS_LOCK)

/** @ brief private pool file */
static FIL gpst_pool_file[FS_MAX_NUMBER_OF_FILE_IN_POOL];
static POOL_item_status_t gpst_pool_file_item_status[FS_MAX_NUMBER_OF_FILE_IN_POOL];
static POOL_ctx_t gst_pool_file_ctx =
{
	gpst_pool_file,
	gpst_pool_file_item_status,
	sizeof(FIL),
	sizeof(gpst_pool_file)/sizeof(FIL)
};

/** @brief private pool directory */
static DIR gpst_pool_dir[FS_MAX_NUMBER_OF_DIR_IN_POOL];
static POOL_item_status_t gpst_pool_dir_item_status[FS_MAX_NUMBER_OF_DIR_IN_POOL];
static POOL_ctx_t gst_pool_dir_ctx =
{
	gpst_pool_dir,
	gpst_pool_dir_item_status,
	sizeof(DIR),
	sizeof(gpst_pool_dir)/sizeof(DIR)
};

void LLFS_IMPL_get_last_modified_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_last_modified_t* param = (FS_last_modified_t*) job->params;
	FILINFO fno = {0};
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;
	LLFS_date_t* out_date = &param->date;

	res = f_stat((TCHAR*)path, &fno);
	if (FR_OK != res) {
		param->result = LLFS_NOK;
	} else {
		out_date->second = (int32_t) ((fno.ftime & 31) * 2);
		out_date->minute = (int32_t) ((fno.ftime >> 5) & 63);
		out_date->hour = (int32_t) (fno.ftime >> 11);
		out_date->day = (int32_t) (fno.fdate & 31);
		out_date->month = (int32_t) ((fno.fdate >> 5) & 15);
		out_date->year = (int32_t) ((fno.fdate >> 9) + 1980);
		param->result = LLFS_OK;
	}

	LLFS_DEBUG_TRACE("[%s:%u] timestamp get : %ld/%02ld/%02ld, %02ld:%02ld:%02ld\n (err %d)\n", __func__, __LINE__,
			out_date->year, out_date->month, out_date->day, out_date->hour, out_date->minute, out_date->second, res);
}

void LLFS_IMPL_set_read_only_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_path_operation_t* param = (FS_path_operation_t*) job->params;
	FILINFO fno = {0};
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;

	res = f_stat((TCHAR*)path, &fno);
	if ((res != FR_OK) || (fno.fattrib & AM_DIR)) {
		/* If an error occurs or the file is directory returns error */
		param->result = LLFS_NOK;
	} else {
		res = f_chmod((TCHAR*)path, AM_RDO, AM_RDO);
		if (res != FR_OK) {
			param->result = LLFS_NOK;
		} else {
			param->result = LLFS_OK;
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] readonly set on %s (err %d)\n", __func__, __LINE__, path, res);
}

void LLFS_IMPL_create_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_create_t* param = (FS_create_t*) job->params;
	FIL fp = {0};
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;

	res = f_open(&fp, (TCHAR*)path, FA_CREATE_NEW);
	if (res == FR_OK) {
		res = f_close(&fp);
		if (res == FR_OK) {
			param->result = LLFS_OK;
		} else {
			param->result = LLFS_NOK;
			param->error_code = res;
			param->error_message = "f_close failed";
		}
	} else if (res == FR_EXIST) {
		param->result = LLFS_NOT_CREATED;
		param->error_code = res;
		param->error_message = "file exists";
	} else {
		param->result = LLFS_NOT_CREATED;
		param->error_code = res;
		param->error_message = "f_open failed";
	}

	LLFS_DEBUG_TRACE("[%s:%u] create file %s (err %d)\n", __func__, __LINE__, path, res);
}

void LLFS_IMPL_open_directory_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_path_operation_t* param = (FS_path_operation_t*) job->params;
	DIR * pdir;
	FRESULT res = FR_OK;
	POOL_status_t pool_res;

	uint8_t* path = (uint8_t*)&param->path;

	pool_res = POOL_reserve_f(&gst_pool_dir_ctx, (void**)&pdir);
	if (pool_res != POOL_NO_ERROR) {
		param->result = LLFS_NOK;
	} else {
		res = f_opendir(pdir, (TCHAR*)path);
		if (res == FR_OK) {
			param->result = (int32_t)pdir;
		} else {
			POOL_free_f(&gst_pool_dir_ctx, (void*)pdir);
			param->result = LLFS_NOK;
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] open dir %s fd %ld (err %d)\n", __func__, __LINE__, path, param->result, res);
}

void LLFS_IMPL_read_directory_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_read_directory_t* param = (FS_read_directory_t*) job->params;
	FILINFO fno = {0};
	FRESULT res = FR_OK;

	int32_t directory_ID = param->directory_ID;
	uint8_t* path = (uint8_t*)&param->path;
	res = f_readdir((DIR*)directory_ID, &fno);
	if ((res != FR_OK) || (fno.fname[0] == 0)) {
		param->result = LLFS_NOK;
	} else {
		strcpy((char*)path, (char*)(fno.fname));
		param->result = LLFS_OK;
	}

	LLFS_DEBUG_TRACE("[%s:%u] read dir %ld return %s (err %d)\n", __func__, __LINE__, directory_ID, path, res);
}

void LLFS_IMPL_close_directory_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_close_directory_t* param = (FS_close_directory_t*) job->params;
	FRESULT res = FR_OK;

	int32_t directory_ID = param->directory_ID;

	res = f_closedir((DIR*)directory_ID);
	if (res != FR_OK) {
		param->result = LLFS_NOK;
	} else {
		param->result = LLFS_OK;
	}

	POOL_free_f(&gst_pool_dir_ctx, (void*)directory_ID);

	LLFS_DEBUG_TRACE("[%s:%u] close dir %ld (err %d)\n", __func__, __LINE__, directory_ID, res);
}

void LLFS_IMPL_rename_to_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_rename_to_t* param = (FS_rename_to_t*) job->params;
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;
	uint8_t* new_path = (uint8_t*)&param->new_path;

	res = f_rename((TCHAR*)path, (TCHAR*)new_path);
	if (res != FR_OK) {
		param->result = LLFS_NOK;
	} else {
		param->result = LLFS_OK;
	}

	LLFS_DEBUG_TRACE("[%s:%u] rename : old name %s, new name %s (err %d)\n", __func__, __LINE__, path, new_path, res);
}

void LLFS_IMPL_get_length_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_path64_operation_t* param = (FS_path64_operation_t*) job->params;
	FILINFO fno = {0};
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;

	res = f_stat((TCHAR*)path, &fno);
	if (res == FR_OK) {
		param->result = fno.fsize;
	} else if (res == FR_NO_FILE) {
		param->result = 0;
	} else {
		param->result = LLFS_NOK;
	}

	LLFS_DEBUG_TRACE("[%s:%u] length of %s : %lld bytes (err %d)\n", __func__, __LINE__, path, param->result, res);
}

void LLFS_IMPL_exist_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_path_operation_t* param = (FS_path_operation_t*) job->params;
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;

	res = f_stat((TCHAR*)path, NULL);
	if (res != FR_OK) {
		param->result = LLFS_NOK;
	} else {
		param->result = LLFS_OK;
	}

	LLFS_DEBUG_TRACE("[%s:%u] exist file %s (err %d)\n", __func__, __LINE__, path, res);
}

void LLFS_IMPL_get_space_size_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_get_space_size* param = (FS_get_space_size*) job->params;
	FATFS* fs;
	DWORD dw_free_cluster;
	WORD ssize = _MAX_SS;
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;
	int32_t space_type = param->space_type;

	res = f_getfree((TCHAR*)path, &dw_free_cluster, &fs);
	if (res != FR_OK) {
		param->result = LLFS_NOK;
	} else if (res == FR_NO_PATH) {
		param->result = 0;
	} else {
#if _MAX_SS != _MIN_SS
		if (disk_ioctl(fs->pdrv, GET_SECTOR_SIZE, &ssize) != RES_OK) {
			param->result = LLFS_NOK;
		} else {
#endif
			switch (space_type) {
			case LLFS_FREE_SPACE:
			case LLFS_USABLE_SPACE:
				param->result = (dw_free_cluster * fs->csize) * ssize;
				break;
			case LLFS_TOTAL_SPACE:
				/* minus 2 -> one for boot and one reserved */
				param->result = ((fs->n_fatent - 2) * fs->csize) * ssize;
				break;
			default:
				param->result = LLFS_NOK;
				break;
			}
#if _MAX_SS != _MIN_SS
		}
#endif
	}

	LLFS_DEBUG_TRACE("[%s:%u] get space type %ld on %s : %lld bytes (err %d)\n", __func__, __LINE__, space_type, path, param->result, res);
}

void LLFS_IMPL_make_directory_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_path_operation_t* param = (FS_path_operation_t*) job->params;
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;

	res = f_mkdir((TCHAR*)path);
	if (FR_OK != res) {
		param->result = LLFS_NOK;
	} else {
		param->result = LLFS_OK;
	}

	LLFS_DEBUG_TRACE("[%s:%u] create dir %s (err %d)\n", __func__, __LINE__, path, res);
}

void LLFS_IMPL_is_hidden_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_path_operation_t* param = (FS_path_operation_t*) job->params;
	FILINFO fno = {0};
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;

	res = f_stat((TCHAR*)path, &fno);
	if (FR_OK != res) {
		param->result = LLFS_NOK;
	} else {
		if (fno.fattrib & AM_HID) {
			param->result = LLFS_OK;
		} else {
			param->result = LLFS_NOK;
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] %s is hidden ? (err %d)\n", __func__, __LINE__, path, res);
}

void LLFS_IMPL_is_directory_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_path_operation_t* param = (FS_path_operation_t*) job->params;
	FILINFO fno = {0};
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;

	res = f_stat((TCHAR*)path, &fno);
	if (FR_OK != res) {
		param->result = LLFS_NOK;
	} else {
		if (fno.fattrib & AM_DIR) {
			param->result = LLFS_OK;
		} else {
			param->result = LLFS_NOK;
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] %s is directory ? (err %d)\n", __func__, __LINE__, path, res);
}

void LLFS_IMPL_is_file_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_path_operation_t* param = (FS_path_operation_t*) job->params;
	FILINFO fno = {0};
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;

	res = f_stat((TCHAR*)path, &fno);
	if (FR_OK != res) {
		param->result = LLFS_NOK;
	} else {
		if (fno.fattrib & AM_DIR) {
			param->result = LLFS_NOK;
		} else {
			param->result = LLFS_OK;
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] %s is file ? (err %d)\n", __func__, __LINE__, path, res);
}

void LLFS_IMPL_set_last_modified_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_last_modified_t* param = (FS_last_modified_t*) job->params;
	FILINFO fno = {0};
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;
	LLFS_date_t* new_date = &param->date;

	fno.fdate = (WORD)((new_date->year - 1980) * 512U | new_date->month * 32U | new_date->day);
	fno.ftime = (WORD)(new_date->hour * 2048U | new_date->minute * 32U | new_date->second / 2U);

	res = f_utime((TCHAR*)path, &fno);
	if (FR_OK != res) {
		param->result = LLFS_NOK;
	} else {
		param->result = LLFS_OK;
	}

	LLFS_DEBUG_TRACE("[%s:%u] timestamp set : %ld/%02ld/%02ld, %02ld:%02ld:%02ld\n (err %d)\n", __func__, __LINE__,
			new_date->year, new_date->month, new_date->day, new_date->hour, new_date->minute, new_date->second, res);
}

void LLFS_IMPL_delete_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_path_operation_t* param = (FS_path_operation_t*) job->params;
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;

	res = f_unlink((TCHAR*)path);
	if (FR_OK != res) {
		param->result = LLFS_NOK;
	} else {
		param->result = LLFS_OK;
	}

	LLFS_DEBUG_TRACE("[%s:%u] delete %s (err %d)\n", __func__, __LINE__, path, res);
}

void LLFS_IMPL_is_accessible_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_is_accessible_t* param = (FS_is_accessible_t*) job->params;
	FILINFO fno = {0};
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;
	int32_t checked_access = param->access;

	res = f_stat((TCHAR*)path, &fno);
	if (res != FR_OK) {
		param->result = LLFS_NOK;
	} else {
		switch (checked_access) {
		case LLFS_ACCESS_WRITE:
			if (fno.fattrib & AM_RDO) {
				param->result = LLFS_NOK;
			} else {
				param->result = LLFS_OK;
			}
			break;
		/* FatFs doesn't support other permissions so return always ok */
		case LLFS_ACCESS_READ:
			param->result = LLFS_OK;
			break;
		case LLFS_ACCESS_EXECUTE:
			param->result = LLFS_OK;
			break;
		default:
			param->result = LLFS_NOK;
			break;
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] %s is accessible ? access %ld (err %d)\n", __func__, __LINE__, path, checked_access, res);
}

void LLFS_IMPL_set_permission_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_set_permission_t* param = (FS_set_permission_t*) job->params;
	FILINFO fno = {0};
	BYTE attr;
	FRESULT res = FR_OK;

	uint8_t* path = (uint8_t*)&param->path;
	int32_t access = param->access;
	int32_t enable = param->enable;

	res = f_stat((TCHAR*)path, &fno);
	if ((res != FR_OK) || (fno.fattrib & AM_DIR)) {
		/* If an error occurs or the file is directory returns error */
		param->result = LLFS_NOK;
	} else {
		attr = enable ? AM_RDO : 0;
		switch (access) {
		case LLFS_ACCESS_WRITE:
			/* FatFs doesn't identify the owner */
			res = f_chmod((TCHAR*)path, attr, AM_RDO);
			if (res == FR_OK) {
				param->result = LLFS_OK;
			} else {
				param->result = LLFS_NOK;
			}
			break;
		/* FatFs doesn't support other permissions so return always ok */
		case LLFS_ACCESS_READ:
			param->result = LLFS_OK;
			break;
		case LLFS_ACCESS_EXECUTE:
			param->result = LLFS_OK;
			break;
		default:
			param->result = LLFS_NOK;
			break;
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] set permission %ld for %s as %ld (err %d)\n", __func__, __LINE__, access, path, enable, res);
}

void LLFS_File_IMPL_open_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_open_t* param = (FS_open_t*) job->params;
	FIL * fp;
	FRESULT res = FR_OK;
	POOL_status_t pool_res;
	BYTE b_internal_mode = 0xFF;

	uint8_t* path = (uint8_t*)&param->path;
	uint8_t mode = param->mode;

	/* Map input mode to FatFs mode */
	switch(mode) {
	case LLFS_FILE_MODE_APPEND:
		b_internal_mode = FA_WRITE | FA_OPEN_APPEND;
	break;
	case LLFS_FILE_MODE_READ:
		b_internal_mode = FA_READ | FA_OPEN_EXISTING;
		break;
	case LLFS_FILE_MODE_WRITE:
		b_internal_mode = FA_WRITE | FA_CREATE_ALWAYS;
		break;
	case LLFS_FILE_MODE_READ_WRITE:
	case LLFS_FILE_MODE_READ_WRITE_DATA_SYNC:
	case LLFS_FILE_MODE_READ_WRITE_SYNC:
		b_internal_mode = FA_READ | FA_WRITE | FA_OPEN_ALWAYS;
		break;
	default:
		param->error_code = mode;
		param->error_message = "Invalid opening mode";
		return;
	}

	pool_res = POOL_reserve_f(&gst_pool_file_ctx, (void**)&fp);
	if (pool_res != POOL_NO_ERROR) {
		param->result = LLFS_NOK;
		param->error_code = pool_res;
		param->error_message = "POOL_reserve_f failed";
	} else {
		res = f_open(fp, (TCHAR*)path, b_internal_mode);
		if (res != FR_OK) {
			POOL_free_f(&gst_pool_file_ctx, (void*)fp);
			param->result = LLFS_NOK;
			param->error_code = res;
			param->error_message = "f_open failed";
		} else {
			/* An f_lseek() following f_open() is no longer needed if FA_OPEN_APPEND mode, since FatFs R0.12a */
			param->result = (int32_t)fp;
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] open file %s in %c mode, fd %ld (err %d)\n", __func__, __LINE__, path, mode, param->result, res);
}

void LLFS_File_IMPL_write_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_write_read_t* param = (FS_write_read_t*) job->params;
	unsigned int byteswritten;
	FRESULT res = FR_OK;

	FIL* fd = (FIL*)param->file_id;
	uint8_t* data = param->data;
	int32_t length = param->length;

	res = f_write(fd, (void*)data, length, &byteswritten);
	if (res != FR_OK) {
		param->result = LLFS_NOK;
		param->error_code = res;
		param->error_message = "f_write failed";
	} else {
		param->result = (int32_t)byteswritten;
	}

	LLFS_DEBUG_TRACE("[%s:%u] written %ld bytes to file %ld (err %d)\n", __func__, __LINE__, param->result, (int32_t)fd, res);
}

void LLFS_File_IMPL_read_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_write_read_t* param = (FS_write_read_t*) job->params;
	unsigned int bytesread;
	FRESULT res = FR_OK;

	FIL* fd = (FIL*)param->file_id;
	uint8_t* data = param->data;
	int32_t length = param->length;

	res = f_read(fd, (void*)data, length, &bytesread);
	if (res != FR_OK) {
		param->result = LLFS_NOK;
		param->error_code = res;
		param->error_message = "f_read failed";
	} else {
		if (bytesread == 0) {
			param->result = LLFS_EOF;
		} else {
			param->result = (int32_t)bytesread;
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] read %ld bytes from file %ld (err %d)\n", __func__, __LINE__, param->result, (int32_t)fd, res);
}

void LLFS_File_IMPL_close_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_close_t* param = (FS_close_t*) job->params;
	FRESULT res = FR_OK;

	FIL* fd = (FIL*)param->file_id;

	res = f_close(fd);
	if (res != FR_OK) {
		param->result = LLFS_NOK;
		param->error_code = res;
		param->error_message = "f_close failed";
	} else {
		param->result = LLFS_OK;
	}

	POOL_free_f(&gst_pool_file_ctx, (void*)fd);

	LLFS_DEBUG_TRACE("[%s:%u] close file %ld (status %ld err %d)\n", __func__, __LINE__, (int32_t)fd, param->result, res);
}

static FRESULT seek(FIL* fd, QWORD n, FSIZE_t *pos) {
	// Convert given offset in a type accepted by f_lseek
	*pos = n;

	// Check if the conversion from long long int to FSIZE_t is correct
	if (*pos != n) {
		// An overflow occurs, saturate the value
#if FF_FS_EXFAT
		*pos = INT64_MAX;
#else
		*pos = INT32_MAX;
#endif
	}

	return f_lseek(fd, *pos);
}

void LLFS_File_IMPL_seek_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_seek_t* param = (FS_seek_t*) job->params;
	FRESULT res = FR_OK;
	FIL* fd = (FIL*)param->file_id;
	FSIZE_t pos = 0;

	if (param->n < 0) {
		param->result = LLFS_NOK;
		param->error_code = FR_INVALID_PARAMETER;
		param->error_message = "f_lseek failed";
	} else {
		res = seek(fd, param->n, &pos);
		if (res != FR_OK) {
			param->result = LLFS_NOK;
			param->error_code = res;
			param->error_message = "f_lseek failed";
		}
	}
#if FF_FS_EXFAT
	LLFS_DEBUG_TRACE("[%s:%u] seek to %lld on %ld (status %ld)\n", __func__, __LINE__, pos, (int32_t)fd, param->result);
#else
	LLFS_DEBUG_TRACE("[%s:%u] seek to %ld on %ld (status %ld)\n", __func__, __LINE__, pos, (int32_t)fd, param->result);
#endif
}

void LLFS_File_IMPL_get_file_pointer_action(MICROEJ_ASYNC_WORKER_job_t* job) {
	FS_getfp_t* param = (FS_getfp_t*) job->params;
	FIL* fd = (FIL*)param->file_id;
	param->result = f_tell(fd);;

	if (param->result < 0) {
		// Error occurred
		param->error_code = param->result;
		param->result = LLFS_NOK;
		param->error_message = "f_tell failed";
	}

#ifdef LLFS_DEBUG
	printf("[%s:%u] get file pointer file %ld (status %lld)\n",__func__, __LINE__, (int32_t) fd, param->result);
#endif
}

void LLFS_File_IMPL_set_length_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_set_length_t* param = (FS_set_length_t*) job->params;
	FRESULT res = FR_OK;
	FSIZE_t pos = 0;
	FIL* fd = (FIL*)param->file_id;
	FSIZE_t oldSize = f_size(fd);
	FSIZE_t oldPos = f_tell(fd);
	DWORD newSize = param->length;

	// First we do a seek
	res = seek(fd, newSize, &pos);
	if ((res == FR_OK) && (f_tell(fd) == pos)) {
		if (oldSize > newSize) {
			// second if the new size is less than the old size we do a truncate
			res = f_truncate(fd);
		}
		if ((res == FR_OK) && (oldPos < newSize)) {
			// third reset the file pointer if the old pos is less than the new length
			res = f_lseek(fd, oldPos);
		}
	}

	if (res != FR_OK) {
		param->result = LLFS_NOK;
		param->error_code = res;
		param->error_message = "set length failed";
	}

	LLFS_DEBUG_TRACE("[%s:%u] set length to %ld on %ld size=%ld (status %ld)\n", __func__, __LINE__, newSize, (int32_t)fd, f_size(fd),  param->result);
}


void LLFS_File_IMPL_get_length_with_fd_action(MICROEJ_ASYNC_WORKER_job_t* job) {
	FS_get_length_with_fd_t* param = (FS_get_length_with_fd_t*) job->params;
	FIL* fd = (FIL*)param->file_id;
	param->result = f_size(fd);
#if FF_FS_EXFAT
	LLFS_DEBUG_TRACE("[%s:%u] get length with fd on %lld length=%ld \n", __func__, __LINE__, (int32_t)fd, param->result);
#else
	LLFS_DEBUG_TRACE("[%s:%u] get length with fd on %ld length=%lld \n", __func__, __LINE__, (int32_t)fd, param->result);
#endif
}

void LLFS_File_IMPL_available_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_available_t* param = (FS_available_t*) job->params;

	FIL* fd = (FIL*)param->file_id;

	DWORD currentPtr = f_tell(fd);
	DWORD size = f_size(fd);

	if (currentPtr > size) {
		param->result = 0;
	} else {
		param->result = size - currentPtr;
	}

	LLFS_DEBUG_TRACE("[%s:%u] available %ld bytes on %ld\n", __func__, __LINE__, param->result, (int32_t)fd);
}

void LLFS_File_IMPL_flush_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_flush_t* param = (FS_flush_t*) job->params;
	FRESULT res = FR_OK;

	FIL* fd = (FIL*)param->file_id;

	res = f_sync(fd);
	if (res != FR_OK) {
		param->result = LLFS_NOK;
		param->error_code = res;
		param->error_message = "f_sync failed";
	} else {
		param->result = LLFS_OK;
	}

	LLFS_DEBUG_TRACE("[%s:%u] flush file %ld (status %ld err %d)\n", __func__, __LINE__, (int32_t)fd, param->result, res);
}

#ifdef __cplusplus
}
#endif
