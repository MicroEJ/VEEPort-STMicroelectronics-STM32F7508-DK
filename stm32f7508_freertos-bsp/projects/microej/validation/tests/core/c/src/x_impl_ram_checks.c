/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "x_ram_checks.h"
#include "bsp_util.h"


/* public weak function default definitions */


/**
 * 
 * @brief  this function provide a definition array of  memory zones to test with 32-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitZones(void)
{
	return (X_RAM_CHECKS_zone_t*)NULL;
}


/**
 * @brief this function provide a definition array of  memory zones to test with 16-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitZones(void)
{
	return (X_RAM_CHECKS_zone_t*)NULL;
}


/**
 * @brief 
 *
 * @return array of X_RAM_CHECKS_zone_t
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitZones(void)
{
	return (X_RAM_CHECKS_zone_t*)NULL;
}


/**
 * @brief 
 *
 * @return number of zones to test
 */

BSP_DECLARE_WEAK_FCNT uint8_t X_RAM_CHECKS_get32bitZoneNumber(void)
{
	return 0;
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT uint8_t X_RAM_CHECKS_get16bitZoneNumber(void)
{
	return 0;
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT uint8_t X_RAM_CHECKS_get8bitZoneNumber(void)
{
	return 0;
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitSourceZone(void)
{
	return (X_RAM_CHECKS_zone_t*)NULL;
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitSourceZone(void)
{
	return (X_RAM_CHECKS_zone_t*)NULL;
}


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitSourceZone(void)
{
		return (X_RAM_CHECKS_zone_t*)NULL;
}
