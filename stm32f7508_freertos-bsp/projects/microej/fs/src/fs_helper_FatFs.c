/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 *
 */

/**
 * @file
 * @brief FatFs helper for LLFS.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 10 June 2020
 */

#include <stdint.h>
#include <string.h>
#include "ff.h"
#include "fs_helper.h"
#include "ffconf.h"
#include "microej_async_worker.h"
#include "pool.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma_rtos.h"
#include "LLFS_File_impl.h"

#ifdef LLFS_DEBUG
#include <stdio.h>
#endif

#ifdef LLFS_DEBUG
#define LLFS_DEBUG_TRACE printf("[DEBUG] ");printf
#else
#define LLFS_DEBUG_TRACE(...) ((void) 0)
#endif

/** @brief define the amount of file in private pool module */
#define FS_MAX_NUMBER_OF_FILE_IN_POOL  (_FS_LOCK)

/** @brief define the amount of dir in private pool module */
#define FS_MAX_NUMBER_OF_DIR_IN_POOL  (_FS_LOCK)

/** @ brief private pool file */
ALIGN_32BYTES(static FIL gpst_pool_file[FS_MAX_NUMBER_OF_FILE_IN_POOL]) __attribute__((section(".FileBUF")));
static POOL_item_status_t gpst_pool_file_item_status[FS_MAX_NUMBER_OF_FILE_IN_POOL];
static POOL_ctx_t gst_pool_file_ctx =
{
	gpst_pool_file,
	gpst_pool_file_item_status,
	sizeof(FIL),
	sizeof(gpst_pool_file)/sizeof(FIL)
};

/** @brief private pool directory */
ALIGN_32BYTES(static DIR gpst_pool_dir[FS_MAX_NUMBER_OF_DIR_IN_POOL]) __attribute__((section(".DirBUF")));
static POOL_item_status_t gpst_pool_dir_item_status[FS_MAX_NUMBER_OF_DIR_IN_POOL];
static POOL_ctx_t gst_pool_dir_ctx =
{
	gpst_pool_dir,
	gpst_pool_dir_item_status,
	sizeof(DIR),
	sizeof(gpst_pool_dir)/sizeof(DIR)
};

ALIGN_32BYTES(FATFS SDFatFs) __attribute__((section(".FsBUF")));
char SDPath[4];

#ifdef __cplusplus
extern "C" {
#endif

void LLFS_IMPL_mount(void) {

	/* Link the micro SD disk I/O driver */
	if (FATFS_LinkDriver(&SD_Driver, (TCHAR*)&SDPath) != 0) {
		while(1);
	}

	/* Register the file system object to the FatFs module */
	if (f_mount(&SDFatFs, (TCHAR*)&SDPath, 0) != FR_OK) {
		while(1);
	}

	/* Initialize private pool */
	memset(gpst_pool_file, 0, sizeof(gpst_pool_file));
	memset(gpst_pool_file_item_status, POOL_FREE, sizeof(gpst_pool_file_item_status));

	memset(gpst_pool_dir, 0, sizeof(gpst_pool_dir));
	memset(gpst_pool_dir_item_status, POOL_FREE, sizeof(gpst_pool_dir_item_status));
}

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
	if (mode == LLFS_FILE_MODE_APPEND) {
		b_internal_mode = FA_WRITE | FA_OPEN_APPEND;
	} else if (mode == LLFS_FILE_MODE_READ) {
		b_internal_mode = FA_READ | FA_OPEN_EXISTING;
	} else if (mode == LLFS_FILE_MODE_WRITE) {
		b_internal_mode = FA_WRITE | FA_CREATE_ALWAYS;
	} else if (mode == (LLFS_FILE_MODE_READ | LLFS_FILE_MODE_WRITE)) {
		b_internal_mode = FA_READ | FA_WRITE | FA_CREATE_ALWAYS;
	} else if (mode == (LLFS_FILE_MODE_READ | LLFS_FILE_MODE_APPEND)) {
		b_internal_mode = FA_READ | FA_WRITE | FA_OPEN_APPEND;
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

void LLFS_File_IMPL_skip_action(MICROEJ_ASYNC_WORKER_job_t* job) {

	FS_skip_t* param = (FS_skip_t*) job->params;
	FRESULT res = FR_OK;

	FIL* fd = (FIL*)param->file_id;
	int64_t n = param->n;

	DWORD currentPtr = f_tell(fd);
	DWORD size = f_size(fd);
	long long int newPtr = currentPtr + n;

	/* Check for overflow when computing newPtr */
	if (n > 0) {
		if (newPtr < currentPtr) {
			/* an overflow occurs: saturate the newPtr value */
			newPtr = INT64_MAX;
		}
	} else {
		if (newPtr > currentPtr) {
			/* an underflow occurs: saturate the newPtr value */
			newPtr = INT64_MIN;
		}
	}

	if (newPtr < 0) {
		param->skipped_count = 0;
		param->result = LLFS_NOK;
		param->error_code = LLFS_NOK;
		param->error_message = "skip backwards failed";
	} else {
		/* limit newPtr value to the size of the file */
		if (newPtr > size) {
			newPtr = size;
		}

		/* info: cast of newPtr is safe here because 0 <= newPtr <= size */
		res = f_lseek(fd, (DWORD)newPtr);
		if (res != FR_OK) {
			param->skipped_count = 0;
			param->result = LLFS_NOK;
			param->error_code = res;
			param->error_message = "f_lseek failed";
		} else {
			param->skipped_count = newPtr - currentPtr;
			if (newPtr == size) {
				param->result = LLFS_EOF;
			} else {
				param->result = LLFS_OK;
			}
		}
	}

	LLFS_DEBUG_TRACE("[%s:%u] skip %lld bytes on %ld (status %ld skip count %lld)\n", __func__, __LINE__, n, (int32_t)fd, param->result, param->skipped_count);
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
