/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef DEVICE_H
#define DEVICE_H

#include "microej.h"

/**
 * STM32F7 Unique device ID register (96bits)
 * @see section 41.1 of the STM32F74xxx and STM32F75xxx Reference Manual
 */
#define UNIQUE_DEVICE_ID_ADDRESS 	((const char*)0x1FF0F420)
#define UNIQUE_DEVICE_ID_SIZE 		(96/8)

#endif

