/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 *
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "watchdog_config.h"

#ifdef __cplusplus
	extern "C" {
#endif

extern int _watchdog_config;
extern int _watchdog_period;

#define WATCHDOG_ENABLED 2
#define WATCHDOG_DISABLED 1

static inline bool MEJ_WATCHDOG(int mode) {

	const volatile uint32_t watchdog_config = ((uint32_t)(&_watchdog_config));

	return (watchdog_config == mode);
}

static inline uint32_t MEJ_WATCHDOG_PERIOD(void) {

	const volatile uint32_t watchdog_period = ((uint32_t)(&_watchdog_period));

	return watchdog_period;
}

#define MEJ_WATCHDOG_ENABLED() MEJ_WATCHDOG(WATCHDOG_ENABLED)

typedef enum {
	WATCHDOG_OK,             /*!< Function executed successfully */
	WATCHDOG_ERROR,          /*!< Function executed with error */
	WATCHDDOG_INVALID_PARAM, /*!< Input parameters are invalid */
	WATCHDOG_DRIVER_ERROR    /*!< Underlying driver error */
} watchdog_t;

typedef enum {
	WATCHDOG_STOP_MODE,    /*!< Suspend or resume the watchdog in the stop mode */
	WATCHDOG_STANDBY_MODE  /*!< Suspend or resume the watchdog in the standby mode */
} watchdog_mode_type_t;

/**
 * @brief This function starts the system watchdog with a configured period or the default period.
 *        Note that once started, the watchdog cannot be stopped.
 *
 * @return see watchdog_t for possible return codes
 */
watchdog_t start_watchdog(void);

/**
 * @brief This function reload the watchdog counter with the reload register value.
 *
 * @return see watchdog_t for possible return codes
 */
watchdog_t refresh_watchdog(void);

/**
 * @brief This function check if the watchdog triggered the reset.
 *
 * @return true if the watchdog triggered the reset, false otherwise.
 */
bool is_watchdog_reset(void);

/**
 * @brief This function clear the watchdog reset flags.
 *
 * @return no return
 */
void clear_watchdog_flags(void);

#if defined(WD_SUSPENDED_IN_STANDBY_MODE) || defined(WD_SUSPENDED_IN_STOP_MODE)

/**
 * @brief This function freeze the watchdog counter for the watchdog mode.
 *        Note that a system reset is required to make the change effective.
 *
 * @param see watchdog_mode_type_t for possible watchdog mode type
 */
void watchdog_suspend(watchdog_mode_type_t wd_mode);

/**
 * @brief This function active the watchdog counter for the watchdog mode.
 *        Note that a system reset is required to make the change effective.
 *
 * @param see watchdog_mode_type_t for possible watchdog mode type
 */
void watchdog_resume(watchdog_mode_type_t wd_mode);

#endif

#ifdef __cplusplus
	}
#endif


#endif /* WATCHDOG_H */
