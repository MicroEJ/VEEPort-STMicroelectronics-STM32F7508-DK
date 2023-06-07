/*
 * C
 *
 * Copyright 2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Includes ------------------------------------------------------------------*/

#include "fs_configuration.h"

#include "ff_gen_drv.h"
#include "sd_diskio_dma_rtos.h"

ALIGN_32BYTES(FATFS SDFatFs);

void LLFS_STM32_init_sdcard(void) {
    char SDPath[4];
    uint8_t res;
    res = FATFS_LinkDriver(&SD_Driver, (TCHAR*)&SDPath);
    /* Link the micro SD disk I/O driver */
    if (res != 0) {
         SNI_throwNativeException(res, "Failed to link FATFS driver");
    }
    
    res = f_mount(&SDFatFs, (TCHAR*)&SDPath, 0);
    /* Register the file system object to the FatFs module */
    if (res != FR_OK) {
         SNI_throwNativeException(res, "Failed to mount the SD card");
    }
}
