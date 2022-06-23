/*
 * C
 *
 * Copyright 2018-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @brief This is the definition of the MicroEJ time API.
 * 
 * To implement this API the use of a hardware timer is recommended. The validation is done with the Core Engine Test Suite: https://github.com/MicroEJ/PlatformQualificationTools/tree/master/tests/core
 * 
 * There are 2 types of time in MicroEJ:
 * - The platform time: an arbitrary time that is only relevant to measure elapsed time
 * - The application time: the time elapsed since midnight, January 1, 1970 UTC, this time should be set by the MicroEJ application
 * 
 * An implementation example is available on Espressif WROVER: https://github.com/MicroEJ/Platform-Espressif-ESP-WROVER-KIT-V4.1/blob/master/ESP32-WROVER-Xtensa-FreeRTOS-bsp/Projects/microej/core/src/microej_time.c
 * 
 */

#ifndef MICROEJ_TIME_H
#define MICROEJ_TIME_H

#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Initialisation of the timebase used by MicroEJ. Initialize in this function everything that is needed to implement the API.
 * 
 */
void microej_time_init(void);

/**
 * @brief Gets the current platform or application time.
 * 
 * @param is_platform_time set to 1 to get the platform time or set to 0 to get the time elapsed since midnight, January 1, 1970 UTC.
 * @return int64_t the time in milliseconds.
 */
int64_t microej_time_get_current_time(uint8_t is_platform_time);

/**
 * @brief Gets an arbitrary time.
 * 
 * @return int64_t the time in nanoseconds. The time should have a nanosecond precision but not necessarily nanosecond resolution.
 */
int64_t microej_time_get_time_nanos(void);

/**
 * @brief Sets the application time.
 * 
 * @param time value in milliseconds of the time elapsed since midnight, January 1, 1970 UTC.
 */
void microej_time_set_application_time(int64_t time);

/**
 * @brief Converts a time in milliseconds to a number of system ticks.
 * 
 * @param time the time to convert in milliseconds,
 * @return int64_t the number of ticks.
 */
int64_t microej_time_time_to_tick(int64_t time);

#ifdef __cplusplus
    }
#endif

#endif /* _MICROEJ_TIME_H */
