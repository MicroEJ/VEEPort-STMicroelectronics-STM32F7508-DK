/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/* Includes ------------------------------------------------------------------*/

#include "device.h"
#include "LLDEVICE_impl.h"
#include <stm32f7xx_hal.h>
#include <stdio.h>
#include <string.h>

#include <sni.h>

uint8_t LLDEVICE_IMPL_getArchitecture(uint8_t* buffer, int32_t length) {
	if (length >= 6) {
		buffer[0]= 'S';
		buffer[1]= 'T';
		buffer[2]= 'M';
		buffer[3]= '3';
		buffer[4]= '2';
		buffer[5]= 0;
		return 1;
	} 
	else {
		return 0;
	}
}

uint32_t LLDEVICE_IMPL_getId(uint8_t* buffer, int32_t length) {
	if(length < UNIQUE_DEVICE_ID_SIZE){
		return 0;
	}
	memcpy(buffer, UNIQUE_DEVICE_ID_ADDRESS, UNIQUE_DEVICE_ID_SIZE);
	return UNIQUE_DEVICE_ID_SIZE;
}
