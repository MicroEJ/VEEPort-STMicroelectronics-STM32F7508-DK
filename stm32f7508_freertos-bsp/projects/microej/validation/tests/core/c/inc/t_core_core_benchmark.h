/*
 * C
 *
 * Copyright 2019-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Prevent recursive inclusion */

#ifndef __T_CORE_CORE_BENCHMARK_H
#define __T_CORE_CORE_BENCHMARK_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "../../../../framework/c/embunit/embUnit/embUnit.h"

/* Public function declarations */
/**
 *@brief This test start an external CORE benchmark. If the benchmark is not implemented or integrated, this test fails.
 */
TestRef T_CORE_COREBENCH_tests(void);

#ifdef __cplusplus
}
#endif

#endif	
