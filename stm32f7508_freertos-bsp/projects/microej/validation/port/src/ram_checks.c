/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdio.h>

#include "x_ram_checks.h"
#include "bsp_util.h"

#include "stm32f7xx_hal.h"
#include "microej.h"

#define SDRAM_START_ADDR    0xC0000000
#define SDRAM_END_ADDR      0xC07FFFFF


/* Define the RAM zone structure on which the tests will run */
static const X_RAM_CHECKS_zone_t RAM_zones[] = {
{
	.start_address = SDRAM_START_ADDR,
	.end_address = SDRAM_END_ADDR,
	.write_speed = 0,
	.read_speed = 0,
	.transfert_speed = 0
}

};

/**
 *
 * @brief  this function provide a definition array of  memory zones to test with 32-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitZones(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief this function provide a definition array of  memory zones to test with 16-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitZones(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitZones(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return number of zones to test
 */
uint8_t X_RAM_CHECKS_get32bitZoneNumber(void)
{
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}

/**
 * @brief
 *
 * @return
 */
uint8_t X_RAM_CHECKS_get16bitZoneNumber(void)
{
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}

/**
 * @brief
 *
 * @return
 */
uint8_t X_RAM_CHECKS_get8bitZoneNumber(void)
{
	return (sizeof(RAM_zones) / sizeof(RAM_zones[0]));
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitSourceZone(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitSourceZone(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitSourceZone(void)
{
	return (X_RAM_CHECKS_zone_t*)&RAM_zones[0];
}
