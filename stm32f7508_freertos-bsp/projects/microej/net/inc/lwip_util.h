/*
 * C
 *
 * Copyright 2017-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef __LWIP_UTIL_H
#define __LWIP_UTIL_H

/**
 * @file
 * @brief LLNET utility functions for LWIP.
 * @author MicroEJ Developer Team
 * @version 0.2.1
 * @date 3 August 2020
 */

#include <stdint.h>

uint32_t lwip_getRandomNumber(void);
int32_t llnet_lwip_init(void);

#endif // __LWIP_UTIL_H
