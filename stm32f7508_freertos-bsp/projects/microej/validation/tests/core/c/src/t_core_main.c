/*
 * C
 *
 * Copyright 2019-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
#include <stdio.h>
#include <stdarg.h>
#include "../../../../framework/c/embunit/embUnit/embUnit.h"
#include "t_core_print.h"
#include "t_core_time_base.h"
#include "t_core_ram.h"
#include "t_core_core_benchmark.h"



void T_CORE_main(void) {
	TestRunner_start();
	TestRunner_runTest(T_CORE_PRINT_tests());
	TestRunner_runTest(T_CORE_TIME_BASE_tests());
	TestRunner_runTest(T_CORE_RAM_tests());
	TestRunner_runTest(T_CORE_RAM_speed_tests());
	TestRunner_runTest(T_CORE_COREBENCH_tests());
	TestRunner_end();
	return;
}
