/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../../../../framework/c/utils/inc/u_time_base.h"
#include "../../../../framework/c/utils/inc/bsp_util.h"
#include "../../../../framework/c/embunit/embunit/config.h"
/* Private constant declarations */



/* private variable definitions */

/* public function definitions */

BSP_DECLARE_WEAK_FCNT void UTIL_TIME_BASE_initialize(void)
{
	stdimpl_print("[Warning]: No implementation of UTIL_TIME_BASE_initialize found!\n");
}	

BSP_DECLARE_WEAK_FCNT int64_t UTIL_TIME_BASE_getTime(void)
{
	return 0;
}

/** private function definitions */

