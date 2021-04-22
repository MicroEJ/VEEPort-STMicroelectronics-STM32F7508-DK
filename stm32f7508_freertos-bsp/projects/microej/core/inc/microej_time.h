/*
 * C
 *
 * Copyright 2012-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _MICROEJ_TIME_H
#define _MICROEJ_TIME_H

#include <stdint.h>

/*
 * Returns -1 on error, 0 otherwise.
 */

void microej_time_init(void);

int64_t microej_time_getcurrenttime(uint8_t isPlatformTime);

int64_t microej_time_gettimenanos(void);

void microej_time_setapplicationtime(int64_t t);

int64_t microej_time_timeToTick(int64_t time);

#endif // _MICROEJ_TIME_H
