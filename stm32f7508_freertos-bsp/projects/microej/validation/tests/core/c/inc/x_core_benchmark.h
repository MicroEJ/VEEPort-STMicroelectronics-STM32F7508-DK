/*
 * C
 *
 * Copyright 2019-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Prevent recursive inclusion */

#ifndef __X_CORE_BENCHMARK_H
#define __X_CORE_BENCHMARK_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Public constant declaration */

/* public structure declaration */

/* public function declaration */

// Symbol to redirect CoreMark main to. Use `#define main core_main`
// in core_portme.h.
void core_main(void);

// weak function to implement

/**
 * @fn bool X_CORE_BENCHMARK_run(void)
 * @brief This function is called to run the CoreMark benchmark.
 *
 * This function should call core_main().
 *
 * @return true if the benchmark was run, false otherwise.
 */
bool X_CORE_BENCHMARK_run(void);

#ifdef __cplusplus
}
#endif

#endif
