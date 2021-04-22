/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 *
 */

#ifndef  WATCHDOG_CONFIG_H
#define  WATCHDOG_CONFIG_H

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Set this define to freeze the independent watchdog counter in standby mode.
 */
//#define WD_SUSPENDED_IN_STANDBY_MODE

/**
 * @brief Set this define to freeze the independent watchdog counter in stop mode.
 */
//#define WD_SUSPENDED_IN_STOP_MODE

/**
 * @brief Note that maximum delay corresponding to 32KHz LSI frequency is 32767 ms.
 *        The watchdog timeout may vary due to LSI frequency dispersion.
 */
#define WATCHDOG_MAX_DELAY_LSI_32KHz 32767

#ifdef __cplusplus
	}
#endif

#endif // WATCHDOG_CONFIG_H
