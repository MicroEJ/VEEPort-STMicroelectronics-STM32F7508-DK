/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../../../../framework/c/utils/inc/bsp_util.h"
#include "../../../../framework/c/utils/inc/u_time_base.h"
#include "x_ram_checks.h"
#include "x_ram_speed.h"


/* public variables */

/* private constant definition */

/* private variable definitions */

/* private function declaration */


static double X_RAM_SPEED_getWriteSpeed32(void);
static double X_RAM_SPEED_getReadSpeed32(void);
static double X_RAM_SPEED_getTransfertSpeed32(void);

static double X_RAM_SPEED_getWriteSpeed16(void);
static double X_RAM_SPEED_getReadSpeed16(void);
static double X_RAM_SPEED_getTransfertSpeed16(void);

static double X_RAM_SPEED_getWriteSpeed8(void);
static double X_RAM_SPEED_getReadSpeed8(void);
static double X_RAM_SPEED_getTransfertSpeed8(void);



/* function definitions */
double X_RAM_SPEED_getAverageWriteSpeed(void)
{
    double avrSpeed = 0;
    double counter = 0;

		if (X_RAM_CHECKS_get32bitZoneNumber() != 0)
		{
			avrSpeed += X_RAM_SPEED_getWriteSpeed32();
			counter++;
		}

		if (X_RAM_CHECKS_get16bitZoneNumber() != 0)
		{
			avrSpeed += X_RAM_SPEED_getWriteSpeed16();
			counter++;
		}

		if (X_RAM_CHECKS_get8bitZoneNumber() != 0)
		{
			avrSpeed += X_RAM_SPEED_getWriteSpeed8();
			counter++;
		}


    if (counter != 0)
    {
        return (avrSpeed / counter);
    }
    else
    {
        return 0;
    }
}

double X_RAM_SPEED_getAverageReadSpeed(void)
{
    double avrSpeed = 0;
    double counter = 0;

		if (X_RAM_CHECKS_get32bitZoneNumber() != 0)
		{
			avrSpeed += X_RAM_SPEED_getReadSpeed32();
			counter++;
		}

		if (X_RAM_CHECKS_get16bitZoneNumber() != 0)
		{
			avrSpeed += X_RAM_SPEED_getReadSpeed16();
			counter++;
		}
		
		
		if (X_RAM_CHECKS_get8bitZoneNumber() != 0)
		{
			avrSpeed += X_RAM_SPEED_getReadSpeed8();
			counter++;
		}

    if (counter != 0)
    {
        return (avrSpeed / counter);
    }
    else
    {
        return 0;
    }
}

double X_RAM_SPEED_getAverageTransfertSpeed(void)
{
    double avrSpeed = 0;
    double counter = 0;

		if ( X_RAM_CHECKS_get32bitSourceZone() != NULL) 
		{
			avrSpeed += X_RAM_SPEED_getTransfertSpeed32();
			counter++;
		}
		
		if ( X_RAM_CHECKS_get16bitSourceZone() != NULL) 
		{
			avrSpeed += X_RAM_SPEED_getTransfertSpeed16();
			counter++;
		}

		if ( X_RAM_CHECKS_get8bitSourceZone() != NULL) 
		{
			avrSpeed += X_RAM_SPEED_getTransfertSpeed8();
			counter++;
		}


    if (counter != 0)
    {
        return (avrSpeed / counter);
    }
    else
    {
        return 0;
    }
}


static double X_RAM_SPEED_getWriteSpeed32(void)
{
	int64_t time;
	double speed, average_speed = 0;
	uint32_t z;
	
	for( z = 0; z < X_RAM_CHECKS_get32bitZoneNumber(); z++ )
	{
		// gets zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get32bitZones();	
		
		time = UTIL_TIME_BASE_getTime();
    X_RAM_SPEED_writeBlock32((volatile uint32_t *)zones[z].start_address, X_RAM_SPEED_PATTERN32, zones[z].end_address - zones[z].start_address);
    time = UTIL_TIME_BASE_getTime() - time;

    speed = ( zones[z].end_address - zones[z].start_address) / time;   /* bytes per us */
    speed *= 1000.0;                /* bytes per ms */
    speed *= 1000.0;                /* bytes per  s */
		zones[z].write_speed = speed;
		average_speed += speed;
    TEST_DBG("X_RAM_Speed with 32-bit write access : %f MBytes/s \n", (speed / 1024.0) / 1024.0);		
	}
	return average_speed / X_RAM_CHECKS_get32bitZoneNumber();
}



static double X_RAM_SPEED_getReadSpeed32(void)
{
	int64_t time;
	double speed, average_speed = 0;
	uint32_t z;
	
	for( z = 0; z < X_RAM_CHECKS_get32bitZoneNumber(); z++ )
	{
    
		// gets zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get32bitZones();	
		
		time = UTIL_TIME_BASE_getTime();
		
    X_RAM_SPEED_readBlock32((volatile uint32_t *) zones[z].start_address, zones[z].end_address - zones[z].start_address);
    time = UTIL_TIME_BASE_getTime() - time;

    speed = (zones[z].end_address - zones[z].start_address) / time;   /* bytes per us */
    speed *= 1000.0;                /* bytes per ms */
    speed *= 1000.0;                /* bytes per  s */
		zones[z].read_speed = speed;
		average_speed += speed;
    TEST_DBG("X_RAM_Speed with 32-bit read access : %f MBytes/s \n", (speed / 1024.0) / 1024.0);
	}
	return average_speed / X_RAM_CHECKS_get32bitZoneNumber();
}

static double X_RAM_SPEED_getTransfertSpeed32(void)
{
	int64_t time;
	double speed, average_speed = 0;
	uint32_t z;
	// if no source zone defined, the test is stopped 
	if (X_RAM_CHECKS_get32bitSourceZone() == NULL )
	{
		return 0.0;
	}
	for( z = 0; z < X_RAM_CHECKS_get32bitZoneNumber(); z++ )
	{
    // gets zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get32bitZones();
		
		time = UTIL_TIME_BASE_getTime();
		
    X_RAM_SPEED_transfertBlock32((volatile uint32_t *) X_RAM_CHECKS_get32bitSourceZone()->start_address, 
			(volatile uint32_t *) zones[z].start_address, 
			X_RAM_CHECKS_get32bitSourceZone()->end_address - X_RAM_CHECKS_get32bitSourceZone()->start_address);
    
		time = UTIL_TIME_BASE_getTime() - time;

    speed = (X_RAM_CHECKS_get32bitSourceZone()->end_address - X_RAM_CHECKS_get32bitSourceZone()->start_address) / time;   /* bytes per us */
    speed *= 1000.0;                      /* bytes per ms */
    speed *= 1000.0;                      /* bytes per  s */
		zones[z].transfert_speed = speed;
		average_speed += speed;
    TEST_DBG("X_RAM_Speed with 32-bit transfert access : %f MBytes/s \n", (speed / 1024.0) / 1024.0);
		}
		return average_speed / X_RAM_CHECKS_get32bitZoneNumber();
}



static double X_RAM_SPEED_getWriteSpeed16(void)
{
	int64_t time;
	double speed, average_speed = 0;
	uint32_t z;
	
	for( z = 0; z < X_RAM_CHECKS_get16bitZoneNumber(); z++ )
	{
    // gets zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get16bitZones();

    time = UTIL_TIME_BASE_getTime();
    X_RAM_SPEED_writeBlock16((volatile uint16_t *) zones[z].start_address, X_RAM_SPEED_PATTERN16, 
			zones[z].end_address - zones[z].start_address);
    time = UTIL_TIME_BASE_getTime() - time;

    speed = (zones[z].end_address - zones[z].start_address) / time;   /* bytes per us */
    speed *= 1000.0;                /* bytes per ms */
    speed *= 1000.0;                /* bytes per  s */
		zones[z].write_speed = speed;
		average_speed += speed;
    TEST_DBG("X_RAM_Speed with 16-bit write access : %f MBytes/s \n", (speed / 1024.0) / 1024.0);
	}
	return average_speed / X_RAM_CHECKS_get16bitZoneNumber();
}

static double X_RAM_SPEED_getReadSpeed16(void)
{
	int64_t time;
	double speed, average_speed = 0;
	uint32_t z;
	
	for( z = 0; z < X_RAM_CHECKS_get16bitZoneNumber(); z++ )
	{
    // gets zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get16bitZones();
		
    time = UTIL_TIME_BASE_getTime();
    X_RAM_SPEED_readBlock16((volatile uint16_t *) zones[z].start_address, zones[z].end_address - zones[z].start_address);
    time = UTIL_TIME_BASE_getTime() - time;

    speed = (zones[z].end_address - zones[z].start_address) / time;   /* bytes per us */
    speed *= 1000.0;                /* bytes per ms */
    speed *= 1000.0;                /* bytes per  s */
		zones[z].read_speed = speed;
		average_speed += speed;
		TEST_DBG("X_RAM_Speed with 16-bit read access : %f MBytes/s \n", (speed / 1024.0) / 1024.0);
	}

	return average_speed / X_RAM_CHECKS_get16bitZoneNumber();
}


static double X_RAM_SPEED_getTransfertSpeed16(void)
{
	int64_t time;
	double speed, average_speed = 0;
	uint32_t z;
	
	// if no source zone defined, the test is stopped 
	if (X_RAM_CHECKS_get16bitSourceZone() == NULL )
	{
		return 0.0;
	}
	
	for( z = 0; z < X_RAM_CHECKS_get16bitZoneNumber(); z++ )
	{
    // gets zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get16bitZones();
		

    time = UTIL_TIME_BASE_getTime();
    X_RAM_SPEED_transfertBlock16((volatile uint16_t *) X_RAM_CHECKS_get16bitSourceZone()->start_address, 
			(volatile uint16_t *) zones[z].start_address, 
				X_RAM_CHECKS_get16bitSourceZone()->end_address - X_RAM_CHECKS_get16bitSourceZone()->start_address);
    time = UTIL_TIME_BASE_getTime() - time;

    speed = (X_RAM_CHECKS_get16bitSourceZone()->end_address - X_RAM_CHECKS_get16bitSourceZone()->start_address) / time;   /* bytes per us */
    speed *= 1000.0;                      /* bytes per ms */
    speed *= 1000.0;                      /* bytes per  s */
		zones[z].transfert_speed = speed;
		average_speed += speed;
    TEST_DBG("X_RAM_Speed with 16-bit transfert access : %f MBytes/s \n", (speed / 1024.0) / 1024.0);
	}
	return average_speed / X_RAM_CHECKS_get16bitZoneNumber();
}


static double X_RAM_SPEED_getWriteSpeed8(void)
{
	int64_t time;
	double speed, average_speed = 0;
	uint32_t z;
	
	for( z = 0; z < X_RAM_CHECKS_get8bitZoneNumber(); z++ )
	{
    // gets zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get8bitZones();
    time = UTIL_TIME_BASE_getTime();
    X_RAM_SPEED_writeBlock8((volatile uint8_t *) zones[z].start_address, X_RAM_SPEED_PATTERN8, 
			zones[z].end_address - zones[z].start_address);
    time = UTIL_TIME_BASE_getTime() - time;

    speed = (zones[z].end_address - zones[z].start_address) / time;   /* bytes per us */
    speed *= 1000.0;                /* bytes per ms */
    speed *= 1000.0;                /* bytes per  s */
		zones[z].write_speed = speed;
		average_speed += speed;
    TEST_DBG("X_RAM_Speed with 8-bit write access : %f MBytes/s \n", (speed / 1024.0) / 1024.0);
		
	}
  return average_speed / X_RAM_CHECKS_get8bitZoneNumber();
}


static double X_RAM_SPEED_getReadSpeed8(void)
{
	int64_t time;
	double speed, average_speed = 0;
	uint32_t z;
	
	for( z = 0; z < X_RAM_CHECKS_get8bitZoneNumber(); z++ )
	{
		// gets zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get8bitZones();
    time = UTIL_TIME_BASE_getTime();
    X_RAM_SPEED_readBlock8((volatile uint8_t *) zones[z].start_address, 
			zones[z].end_address - zones[z].start_address);
    time = UTIL_TIME_BASE_getTime() - time;

    speed = (zones[z].end_address - zones[z].start_address) / time;   /* bytes per us */
    speed *= 1000.0;                /* bytes per ms */
    speed *= 1000.0;                /* bytes per  s */
		zones[z].read_speed = speed;
		average_speed += speed;
    TEST_DBG("X_RAM_Speed with 8-bit read access : %f MBytes/s \n", (speed / 1024.0) / 1024.0);
	}
  return average_speed / X_RAM_CHECKS_get8bitZoneNumber();
}

static double X_RAM_SPEED_getTransfertSpeed8(void)
{
	int64_t time;
	double speed, average_speed = 0;
	uint32_t z;
	
	// if no source zone defined, the test is stopped 
	if (X_RAM_CHECKS_get8bitSourceZone() == NULL )
	{
		return 0.0;
	}
	
	for( z = 0; z < X_RAM_CHECKS_get8bitZoneNumber(); z++ )
	{
		// gets zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get8bitZones();
    time = UTIL_TIME_BASE_getTime();
    X_RAM_SPEED_transfertBlock8((volatile uint8_t *)  X_RAM_CHECKS_get8bitSourceZone()->start_address, 
			(volatile uint8_t *) zones[z].start_address, 
				X_RAM_CHECKS_get8bitSourceZone()->end_address - X_RAM_CHECKS_get8bitSourceZone()->start_address);
    time = UTIL_TIME_BASE_getTime() - time;

    speed = (X_RAM_CHECKS_get8bitSourceZone()->end_address - X_RAM_CHECKS_get8bitSourceZone()->start_address) / time;   /* bytes per us */
    speed *= 1000.0;                      /* bytes per ms */
    speed *= 1000.0;                      /* bytes per  s */
		zones[z].transfert_speed = speed;
		average_speed += speed;
    TEST_DBG("X_RAM_Speed with 8-bit transfert access : %f MBytes/s \n", (speed / 1024.0) / 1024.0);
	}
	return average_speed / X_RAM_CHECKS_get8bitZoneNumber();
}






