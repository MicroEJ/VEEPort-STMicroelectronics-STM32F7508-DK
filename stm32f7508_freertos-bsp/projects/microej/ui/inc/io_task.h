/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef IO_TASK_H
#define IO_TASK_H

/* Includes ------------------------------------------------------------------*/

#include "microej.h"

/* API -----------------------------------------------------------------------*/

/**
 * @brief  Create and start the IO Expander task that call IOExpander16_interrupt() when it is notified
 * @retval 0 if an error occurred, a non null value otherwise
 */
uint8_t IO_TASK_create_task(void);

#endif
