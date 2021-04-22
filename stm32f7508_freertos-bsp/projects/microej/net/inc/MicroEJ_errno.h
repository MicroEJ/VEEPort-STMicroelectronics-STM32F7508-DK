/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef __MICROEJ_ERRNO_H
#define __MICROEJ_ERRNO_H
#define ERRNO
#include <FreeRTOS.h>
#include <task.h>
#include <stdint.h>
#define set_errno(err) (vTaskSetThreadLocalStoragePointer(NULL, 0, (void*)err))
#define errno ((int32_t)pvTaskGetThreadLocalStoragePointer(NULL, 0))

#endif
