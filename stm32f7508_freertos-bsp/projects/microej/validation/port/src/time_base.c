/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../../framework/c/utils/inc/u_time_base.h"
#include "../../framework/c/utils/inc/bsp_util.h"
#include "../../framework/c/embunit/embunit/config.h"

#include "microej_time.h"

/* Private constant declarations */



/* private variable definitions */


/* public function definitions */

void UTIL_TIME_BASE_initialize(void)
{
	microej_time_init();
}

int64_t UTIL_TIME_BASE_getTime(void)
{
	return (microej_time_get_time_nanos()/1000); /* returns the time in microseconds */
}

/** private function definitions */
