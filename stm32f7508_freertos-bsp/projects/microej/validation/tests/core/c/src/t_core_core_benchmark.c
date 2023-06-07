/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include "../../../../framework/c/embunit/embUnit/embUnit.h"
#include "../../../../framework/c/embunit/embUnit/config.h"
#include "x_core_benchmark.h"
#include "../../../../framework/c/utils/inc/u_time_base.h"

/** Private function definitions */
static void T_CORE_COREBENCH_setUp(void)
{
   UTIL_TIME_BASE_initialize();
}

static void T_CORE_COREBENCH_tearDown(void)
{
	
}

static void T_CORE_COREBENCH_check(void)
{
	TEST_ASSERT_MESSAGE(X_CORE_BENCHMARK_run(), "\nNo core benchmark can be found.\nPlease install a core benchmark in your platform and implement X_CORE_BENCHMARK_run function.\n");
}

/* Public function definitions */

TestRef T_CORE_COREBENCH_tests(void)
{
    EMB_UNIT_TESTFIXTURES(fixtures) { 
			new_TestFixture("Run core benchmark", T_CORE_COREBENCH_check),

		};
		stdimpl_print("\nCore/Flash benchmark:\n");
		EMB_UNIT_TESTCALLER(corebenchTest, "Flash and core tests", T_CORE_COREBENCH_setUp, T_CORE_COREBENCH_tearDown, fixtures);

    return (TestRef)&corebenchTest;
}


