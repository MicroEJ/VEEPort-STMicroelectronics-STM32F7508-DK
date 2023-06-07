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
#include "x_ram_speed.h"


/** Private constant declarations */

#define X_RAM_CHECKS_PATTERN_32_0    (uint32_t)0x00000000
#define X_RAM_CHECKS_PATTERN_32_1    (uint32_t)0x11111111
#define X_RAM_CHECKS_PATTERN_32_2    (uint32_t)0xAAAAAAAA
#define X_RAM_CHECKS_PATTERN_32_3    (uint32_t)0x55555555
#define X_RAM_CHECKS_PATTERN_32_4    (uint32_t)0xFFFF0000
#define X_RAM_CHECKS_PATTERN_32_5    (uint32_t)0x0000FFFF
#define X_RAM_CHECKS_PATTERN_32_6    (uint32_t)0x12345678
#define X_RAM_CHECKS_PATTERN_32_7    (uint32_t)0x87654321

#define X_RAM_CHECKS_PATTERN_16_0    (uint16_t)0x0000
#define X_RAM_CHECKS_PATTERN_16_1    (uint16_t)0x1111
#define X_RAM_CHECKS_PATTERN_16_2    (uint16_t)0xAAAA
#define X_RAM_CHECKS_PATTERN_16_3    (uint16_t)0x5555
#define X_RAM_CHECKS_PATTERN_16_4    (uint16_t)0xFF00
#define X_RAM_CHECKS_PATTERN_16_5    (uint16_t)0x00FF
#define X_RAM_CHECKS_PATTERN_16_6    (uint16_t)0x1234
#define X_RAM_CHECKS_PATTERN_16_7    (uint16_t)0x5678
#define X_RAM_CHECKS_PATTERN_16_8    (uint16_t)0x8765
#define X_RAM_CHECKS_PATTERN_16_9    (uint16_t)0x4321

#define X_RAM_CHECKS_PATTERN_8_0    (uint8_t)0x00
#define X_RAM_CHECKS_PATTERN_8_1    (uint8_t)0x11
#define X_RAM_CHECKS_PATTERN_8_2    (uint8_t)0xAA
#define X_RAM_CHECKS_PATTERN_8_3    (uint8_t)0x55
#define X_RAM_CHECKS_PATTERN_8_4    (uint8_t)0xF0
#define X_RAM_CHECKS_PATTERN_8_5    (uint8_t)0x0F
#define X_RAM_CHECKS_PATTERN_8_6    (uint8_t)0x12
#define X_RAM_CHECKS_PATTERN_8_7    (uint8_t)0x34
#define X_RAM_CHECKS_PATTERN_8_8    (uint8_t)0x56
#define X_RAM_CHECKS_PATTERN_8_9    (uint8_t)0x78
#define X_RAM_CHECKS_PATTERN_8_10   (uint8_t)0x87
#define X_RAM_CHECKS_PATTERN_8_11   (uint8_t)0x65
#define X_RAM_CHECKS_PATTERN_8_12   (uint8_t)0x43
#define X_RAM_CHECKS_PATTERN_8_13   (uint8_t)0x21

const uint32_t ram_patterns32[]= {
                                X_RAM_CHECKS_PATTERN_32_0,
                                X_RAM_CHECKS_PATTERN_32_1,
                                X_RAM_CHECKS_PATTERN_32_2,
                                X_RAM_CHECKS_PATTERN_32_3,
                                X_RAM_CHECKS_PATTERN_32_4,
                                X_RAM_CHECKS_PATTERN_32_5,
                                X_RAM_CHECKS_PATTERN_32_6,
                                X_RAM_CHECKS_PATTERN_32_7
};

const uint16_t ram_patterns16[]= {
                                X_RAM_CHECKS_PATTERN_16_0,
                                X_RAM_CHECKS_PATTERN_16_1,
                                X_RAM_CHECKS_PATTERN_16_2,
                                X_RAM_CHECKS_PATTERN_16_3,
                                X_RAM_CHECKS_PATTERN_16_4,
                                X_RAM_CHECKS_PATTERN_16_5,
                                X_RAM_CHECKS_PATTERN_16_6,
                                X_RAM_CHECKS_PATTERN_16_7,
                                X_RAM_CHECKS_PATTERN_16_8,
                                X_RAM_CHECKS_PATTERN_16_9
};

const uint8_t ram_patterns8[]= {
                                X_RAM_CHECKS_PATTERN_8_0,
                                X_RAM_CHECKS_PATTERN_8_1,
                                X_RAM_CHECKS_PATTERN_8_2,
																X_RAM_CHECKS_PATTERN_8_3,
                                X_RAM_CHECKS_PATTERN_8_4,
                                X_RAM_CHECKS_PATTERN_8_5,
                                X_RAM_CHECKS_PATTERN_8_6,
                                X_RAM_CHECKS_PATTERN_8_7,
                                X_RAM_CHECKS_PATTERN_8_8,
                                X_RAM_CHECKS_PATTERN_8_9,
                                X_RAM_CHECKS_PATTERN_8_10,
                                X_RAM_CHECKS_PATTERN_8_11,
                                X_RAM_CHECKS_PATTERN_8_12,
                                X_RAM_CHECKS_PATTERN_8_13
};





/** private variable definitions */

/** private function definitions */

/**
 * @brief Writes the pattern in all 32-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get32bitZones function. Next, reads all 32-bit words of defined zones, to check 
 *        the memory integrity.
 *
 * @param[in] pattern used to check RAM zones
 * @return count of the pattern read error
 */
static uint32_t checkPatternAllMemory32(uint32_t pattern)
{
    uint32_t *pointer;
    uint32_t i,z;
    uint32_t error_count = 0;
	
		// get zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get32bitZones();

		// checks zone defintions
		if ( ( zones == NULL)
				|| (X_RAM_CHECKS_get32bitZoneNumber() == 0))
		{
			return UINT32_MAX;
		}
		
    // for each zone,  ...
		for (z = 0; z < X_RAM_CHECKS_get32bitZoneNumber(); z++)
    {
    	pointer = (uint32_t *) zones[z].start_address;
    	
			// ... writes pattern in each word
			for (i = 0; i < ((zones[z].end_address - zones[z].start_address) / (sizeof(uint32_t))); i++)
    	{
    		*(pointer + i) = pattern;
    	}
			
			// ... reads each word and counts errors
    	for (i = 0 ; i <((zones[z].end_address - zones[z].start_address) / (sizeof(uint32_t))); i++)
    	{
    		if (*(pointer + i) != pattern)
    		{
            error_count++;
    		}
    	}
    }


    if (error_count != 0)
    {
        TEST_DBG("X_RAM_CHECKS_CheckPatternAllMemory with 32-bit access : data bus problems - pattern 0x%x ( %i errors )\n", pattern, error_count);
    }
    else
    {
        TEST_DBG("X_RAM_CHECKS_CheckPatternAllMemory with 32-bit access : data bus pattern 0x%x - no error \n", pattern);
    }

    return error_count;
}



/**
 * @brief Writes and reads the pattern for each 32-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get32bitZones function. 
 *
 * @param[in] pattern used to check RAM zones
 * @return count of the pattern read error
 */
static uint32_t checkPatternMemory32(uint32_t pattern)
{
	uint32_t *pointer;
	uint32_t i, z;
	uint32_t error_count = 0;

	// get zone definition array 
	X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get32bitZones();	
	
	// checks zone defintions
	if ( ( zones == NULL)
				|| (X_RAM_CHECKS_get32bitZoneNumber() == 0))
	{
		return UINT32_MAX;
	}
	
	// for each zone,  ...
	for (z = 0; z < X_RAM_CHECKS_get32bitZoneNumber(); z++)
	{  
		pointer = (uint32_t *) zones[z].start_address;
		
		// ... writes each word, next reads it and counts errors 
		for (i = 0; i < ((zones[z].end_address - zones[z].start_address) / (sizeof(uint32_t))); i++)
    {
        *(pointer + i) = pattern;
        if (*(pointer + i) != pattern)
        {
            error_count++;
        }
    }
	}
	if (error_count != 0)
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternMemory with 32-bit access: data bus problems - pattern 0x%x ( %i errors )\n", pattern, error_count);
	}
	else
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternMemory with 32-bit access: data bus pattern 0x%x - no error \n", pattern);
	}
    return error_count;
}





/**
 * @brief Writes the pattern in all 16-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get16bitZones function. Next, reads all 16-bit words of defined zones, to check 
 *        the memory integrity.
 *
 * @param[in] pattern used to check RAM zones
 * @return count of the pattern read error
 */
static uint32_t checkPatternAllMemory16(uint16_t pattern)
{
	uint16_t *pointer;
	uint32_t i, z;
	uint32_t error_count = 0;
	
	// gets zone definition array 
	X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get16bitZones();	

	// checks zone defintions
	if ( ( zones == NULL)
				|| (X_RAM_CHECKS_get16bitZoneNumber() == 0))
	{
		return UINT32_MAX;
	}
	
	// for each zone,  ...
	for (z = 0; z < X_RAM_CHECKS_get16bitZoneNumber(); z++)
	{
    
		pointer = (uint16_t *) zones[z].start_address;
		// ... writes pattern in each word
		for (i = 0; i < ((zones[z].end_address - zones[z].start_address)/ (sizeof(uint16_t))); i++)
    {
        *(pointer + i) = pattern;
    }
		// ... reads each word and counts errors
    for (i = 0; i < ((zones[z].end_address - zones[z].start_address) / (sizeof(uint16_t))); i++)
    {
        if (*(pointer + i) != pattern)
        {
            error_count++;
        }
    }
	}

	if (error_count != 0)
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternAllMemory with 16-bit access: data bus problems - pattern 0x%x ( %i errors )\n", pattern, error_count);
	}
	else
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternAllMemory with 16-bit access: data bus pattern 0x%x - no error \n", pattern);
	}
	return error_count;
}

/**
 * @brief Writes and reads the pattern for each 16-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get16bitZones function. 
 *
 * @param[in] pattern used to check RAM zones
 * @return count of the pattern read error
 */
static uint32_t checkPatternMemory16(uint16_t pattern)
{
	uint16_t *pointer;
	uint32_t i, z;
	uint32_t error_count = 0;

	// gets zone definition array 
	X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get16bitZones();	

	// checks zone defintions
	if ( ( zones == NULL)
				|| (X_RAM_CHECKS_get16bitZoneNumber() == 0))
	{
		return UINT32_MAX;
	}
	
	// for each zone,  ...
	for (z = 0; z < X_RAM_CHECKS_get16bitZoneNumber(); z++)
	{
		pointer = (uint16_t *) zones[z].start_address;
		
		for (i = 0; i < ((zones[z].end_address - zones[z].start_address) / (sizeof(uint16_t))); i++)
    {
        *(pointer + i) = pattern;
        if (*(pointer + i) != pattern)
        {
            error_count++;
        }
    }
	}
	if (error_count != 0)
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternMemory with 16-bit access: data bus problems - pattern 0x%x ( %i errors )\n", pattern, error_count);
	}
	else
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternMemorywith 16-bit access: data bus pattern 0x%x - no error \n", pattern);
	}

    return error_count;
}



/**
 * @brief Writes and reads the pattern for each 8-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get8bitZones function. 
 *
 * @param[in] pattern used to check RAM zones
 * @return count of the pattern read error
 */
static uint32_t checkPatternAllMemory8(uint8_t pattern)
{
	uint8_t *pointer;
	uint32_t i, z;
	uint32_t error_count = 0;
	
	// gets zone definition array 
	X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get8bitZones();	

	// checks zone defintions
	if (( zones == NULL)
				|| (X_RAM_CHECKS_get8bitZoneNumber() == 0))
	{
		return UINT32_MAX;
	}
	
	// for each zone,  ...
	for (z = 0; z < X_RAM_CHECKS_get8bitZoneNumber(); z++)
	{
		pointer = (uint8_t *) zones[z].start_address;
		for (i = 0; i < ((zones[z].end_address - zones[z].start_address) / (sizeof(uint8_t))); i++)
    {
        *(pointer + i) = pattern;
    }

    for (i = 0; i < ((zones[z].end_address - zones[z].start_address) / (sizeof(uint8_t))); i++)
    {
        if (*(pointer + i) != pattern)
        {
            error_count++;
        }
    }
	}

	if (error_count != 0)
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternAllMemory with 8-bit access: data bus problems - pattern 0x%x ( %i errors )\n", pattern, error_count);
	}
	else
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternAllMemory with 8-bit access: data bus pattern 0x%x - no error \n", pattern);
	}

	return error_count;
}

/**
 * @brief Writes and reads the pattern for each 8-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get8bitZones function. 
 *
 * @param[in] pattern used to check RAM zones
 * @return count of the pattern read error
 */
static uint32_t checkPatternMemory8(uint8_t pattern)
{
	uint8_t *pointer;
	uint32_t i, z;
	uint32_t error_count = 0;

	// gets zone definition array 
	X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get8bitZones();	

	// checks zone defintions
	if (( zones == NULL)
				|| (X_RAM_CHECKS_get8bitZoneNumber() == 0))
	{
		return UINT32_MAX;
	}
  
	for (z = 0; z < X_RAM_CHECKS_get8bitZoneNumber(); z++)
	{
		pointer = (uint8_t *) zones[z].start_address;
		for (i = 0; i < ((zones[z].end_address - zones[z].start_address) / sizeof(uint8_t)); i++)
    {
        *(pointer + i) = pattern;
        if (*(pointer + i) != pattern)
        {
            error_count++;
        }
    }
	}
	if (error_count != 0)
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternMemory with 8-bit access: data bus problems - pattern 0x%x ( %i errors )\n", pattern, error_count);
	}
	else
	{
			TEST_DBG("X_RAM_CHECKS_CheckPatternMemory with 8-bit access: data bus pattern 0x%x - no error \n", pattern);
	}
	
	return error_count;
}


/** public function definitions */

/**
* @brief Checks the memory addressing: writes a memory index in each 32-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get32bitZones function; next reads each memory word.
 *
 * @return count of the pattern read errors
 */
uint32_t X_RAM_CHECKS_checkAddress32(void)
{
	uint32_t *pointer;
	uint32_t i, z;
	uint32_t error_count = 0;
	
	// get zone definition array 
	X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get32bitZones();	
	
	// checks zone defintions
	if ( ( zones == NULL)
				|| (X_RAM_CHECKS_get32bitZoneNumber() == 0))
	{
		return UINT32_MAX;
	}
	
	// for each zone,  ...
	for (z = 0; z < X_RAM_CHECKS_get32bitZoneNumber(); z++)
	{
    pointer = (uint32_t *) zones[z].start_address;
		// ... writes index in the memory words
		for (i = 0; i < ((zones[z].end_address - zones[z].start_address) / (sizeof(uint32_t))); i++)
    {
        *(pointer + i) = i;
    }
		// ... reads and checks index in the memory words
    for (i = 0; i < ((zones[z].end_address - zones[z].start_address) / (sizeof(uint32_t))); i++)
    {
        if (*(pointer + i) != i)
        {
            error_count++;					// ... counts errors
        }
    }

    if (error_count != 0)
    {
        TEST_DBG("X_RAM_CHECKS: address bus problems : %i errors \n", error_count);
    }
    else
    {
        TEST_DBG("X_RAM_CHECKS: address bus  no error \n");
    }
	}
	return error_count;
}

/**
* @brief Checks the memory addressing: writes a memory index in each 16-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get16bitZones function; next reads each memory word.
 *
 * @return count of the pattern read errors
 */
uint32_t X_RAM_CHECKS_checkAddress16(void)
{
    uint16_t *pointer;
    uint32_t i, z, p = 0;
    uint32_t error_count = 0;

		// get zone definition array 
	X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get16bitZones();	
	
	// checks zone defintions
	if ( ( zones == NULL)
				|| (X_RAM_CHECKS_get16bitZoneNumber() == 0))
	{
		return UINT32_MAX;
	}
	
	for (z = 0; z < X_RAM_CHECKS_get16bitZoneNumber(); z++)
	{
    while (p < ((zones[z].end_address - zones[z].start_address) / (sizeof(uint16_t) * (UINT16_MAX + 1))))
    {
      pointer = (uint16_t *) zones[z].start_address;  
			for (i = 0; i < UINT16_MAX; i++)
        {
            *(pointer + (p * UINT16_MAX) + i) = i;
        }

        for (i = 0; i < UINT16_MAX; i++)
        {
            if (*(pointer + (p * UINT16_MAX) + i) != i)
            {
                error_count++;
            }
        }

        p++;
    }
	}
	if (error_count != 0)
	{
		TEST_DBG("X_RAM_CHECKS: address bus problems : %i errors \n", error_count);
	}
	else
	{
        TEST_DBG("X_RAM_CHECKS: address bus  no error \n");
	}

	return error_count;
}


/**
* @brief Checks the memory addressing: writes a memory index in each byte of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get8bitZones function; next reads each memory byte.
 *
 * @return count of the pattern read errors
 */
uint32_t X_RAM_CHECKS_checkAddress8(void )
{
	uint8_t *pointer;
	uint32_t i, z, p = 0;
	uint32_t error_count = 0;

	// gets zone definition array 
	X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get8bitZones();	

	// checks zone defintions
	if (( zones == NULL)
				|| (X_RAM_CHECKS_get8bitZoneNumber() == 0))
	{
		return UINT32_MAX;
	}
	
	for (z = 0; z < X_RAM_CHECKS_get8bitZoneNumber(); z++)
	{
		pointer = (uint8_t *) zones[z].start_address;
	
    while (p < ((zones[z].end_address - zones[z].start_address)  / (sizeof(uint8_t) * (UINT8_MAX + 1))))
    {
        for (i = 0; i < UINT8_MAX; i++)
        {
            *(pointer + (p * UINT8_MAX) + i) = i;
        }

        for (i = 0; i < UINT8_MAX; i++)
        {
            if (*(pointer + (p * UINT8_MAX) + i) != i)
            {
                error_count++;
            }
        }

        p++;
    }
	}
	if (error_count != 0)
	{
			TEST_DBG("X_RAM_CHECKS: address bus problems : %i errors \n", error_count);
	}
	else
	{
			TEST_DBG("X_RAM_CHECKS: address bus  no error \n");
	}
	return error_count;
}



/**
 * @brief Writes the pattern in all 32-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get32bitZones function. Next, reads all 32-bit words of defined zones, to check 
 *        the memory integrity.
 *
 * @param[in] testType  test to run :
 * - X_RAM_CHECKS_ALL_LOW_DATA : the function uses 0x00000000 pattern value to check memory.
 * - X_RAM_CHECKS_ALL_HIGH_DATA : the function uses 0xFFFFFFFF pattern value to check memory.
 * - X_RAM_CHECKS_HALF_HIGH_DATA : the function uses 0x0000FFFF and 0xFFFF0000 pattern values to check memory.
 * - X_RAM_CHECKS_ODD_BIT_DATA : the function uses 0x55555555 pattern value to check memory.
 * - X_RAM_CHECKS_EVEN_BIT_DATA : the function uses 0xAAAAAAAA pattern value to check memory.
 * - X_RAM_CHECKS_GENERIC_DATA : the function uses 0x12345678, 0x87654321 pattern values to check memory.
 *
 * @return error count
 */
uint32_t X_RAM_CHECKS_check32bitWriteAllNextReadAll(uint16_t patternType)
{
    uint32_t error_count = 0;
		uint8_t index;
		uint8_t pattern_idx_min, pattern_idx_max;
	
	switch (patternType)
	{
		case X_RAM_CHECKS_ALL_LOW_DATA :
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
			break;
		case X_RAM_CHECKS_ALL_HIGH_DATA :
			pattern_idx_min = 1;
		  pattern_idx_max = 1;
			break;
		case X_RAM_CHECKS_ODD_BIT_DATA :
			pattern_idx_min = 2;
		  pattern_idx_max = 2;
			break;
		case X_RAM_CHECKS_EVEN_BIT_DATA :
			pattern_idx_min = 3;
		  pattern_idx_max = 3;
			break;
		case X_RAM_CHECKS_HALF_HIGH_DATA :
			pattern_idx_min = 4;
		  pattern_idx_max = 5;
			break;
		case X_RAM_CHECKS_GENERIC_DATA :
			pattern_idx_min = 6;
		  pattern_idx_max = 7;
			break;
		default:
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
	}
	for (index = pattern_idx_min; index < (pattern_idx_max + 1) ; index++)
	{
			error_count += checkPatternAllMemory32(ram_patterns32[index]);
	}	
	return error_count;
}

/**
 * @brief Writes and reads the pattern for each 32-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get32bitZones function. 
 *
 * @param[in] testType  test to run :
 * - X_RAM_CHECKS_ALL_LOW_DATA : the function uses 0x00000000 pattern value to check memory.
 * - X_RAM_CHECKS_ALL_HIGH_DATA : the function uses 0xFFFFFFFF pattern value to check memory.
 * - X_RAM_CHECKS_HALF_HIGH_DATA : the function uses 0x0000FFFF and 0xFFFF0000 pattern values to check memory.
 * - X_RAM_CHECKS_ODD_BIT_DATA : the function uses 0x55555555 pattern value to check memory.
 * - X_RAM_CHECKS_EVEN_BIT_DATA : the function uses 0xAAAAAAAA pattern value to check memory.
 * - X_RAM_CHECKS_GENERIC_DATA : the function uses 0x12345678, 0x87654321 pattern values to check memory.
 *
 * @return error count
 */
uint32_t X_RAM_CHECKS_check32bitWriteRead(uint16_t patternType)
{
    uint32_t error_count = 0;
		uint8_t index;
		uint8_t pattern_idx_min, pattern_idx_max;
	
	switch (patternType)
	{
		case X_RAM_CHECKS_ALL_LOW_DATA :
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
			break;
		case X_RAM_CHECKS_ALL_HIGH_DATA :
			pattern_idx_min = 1;
		  pattern_idx_max = 1;
			break;
		case X_RAM_CHECKS_ODD_BIT_DATA :
			pattern_idx_min = 2;
		  pattern_idx_max = 2;
			break;
		case X_RAM_CHECKS_EVEN_BIT_DATA :
			pattern_idx_min = 3;
		  pattern_idx_max = 3;
			break;
		case X_RAM_CHECKS_HALF_HIGH_DATA :
			pattern_idx_min = 4;
		  pattern_idx_max = 5;
			break;
		case X_RAM_CHECKS_GENERIC_DATA :
			pattern_idx_min = 6;
		  pattern_idx_max = 7;
			break;
		default:
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
	}
	for (index = pattern_idx_min; index < (pattern_idx_max + 1) ; index++)
	{
			error_count += checkPatternMemory32(ram_patterns32[index]);
	}	
	return error_count;
}

/**
 * @brief Writes the pattern in all 16-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get16bitZones function. Next, reads all 16-bit words of defined zones, to check 
 *        the memory integrity.
 *
 * @param[in] testType  test to run :
 * - X_RAM_CHECKS_ALL_LOW_DATA : the function uses 0x0000 pattern value to check memory.
 * - X_RAM_CHECKS_ALL_HIGH_DATA : the function uses 0xFFFF pattern value to check memory.
 * - X_RAM_CHECKS_HALF_HIGH_DATA : the function uses 0x00FF and 0xFF00 pattern values to check memory.
 * - X_RAM_CHECKS_ODD_BIT_DATA : the function uses 0x5555 pattern value to check memory.
 * - X_RAM_CHECKS_EVEN_BIT_DATA : the function uses 0xAAAA pattern value to check memory.
 * - X_RAM_CHECKS_GENERIC_DATA : the function uses 0x1234, 0x5678, 0x8765 and 0x4321 pattern values to check memory.
 *
 * @return error count
 */
uint32_t X_RAM_CHECKS_check16bitWriteAllNextReadAll(uint16_t patternType)
{
    uint32_t error_count = 0;
		uint8_t index;
		uint8_t pattern_idx_min, pattern_idx_max;
	
	switch (patternType)
	{
		case X_RAM_CHECKS_ALL_LOW_DATA :
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
			break;
		case X_RAM_CHECKS_ALL_HIGH_DATA :
			pattern_idx_min = 1;
		  pattern_idx_max = 1;
			break;
		case X_RAM_CHECKS_ODD_BIT_DATA :
			pattern_idx_min = 2;
		  pattern_idx_max = 2;
			break;
		case X_RAM_CHECKS_EVEN_BIT_DATA :
			pattern_idx_min = 3;
		  pattern_idx_max = 3;
			break;
		case X_RAM_CHECKS_HALF_HIGH_DATA :
			pattern_idx_min = 4;
		  pattern_idx_max = 5;
			break;
		case X_RAM_CHECKS_GENERIC_DATA :
			pattern_idx_min = 6;
		  pattern_idx_max = 9;
			break;
		default:
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
	}
	for (index = pattern_idx_min; index < (pattern_idx_max + 1) ; index++)
	{
			error_count += checkPatternAllMemory16(ram_patterns16[index]);
	}	
	return error_count;
}


/**
 * @brief Writes and reads the pattern for each 16-bit words of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get16bitZones function. 
 *
 * @param[in] testType  test to run :
 * - X_RAM_CHECKS_ALL_LOW_DATA : the function uses 0x00 pattern value to check memory.
 * - X_RAM_CHECKS_ALL_HIGH_DATA : the function uses 0xFF pattern value to check memory.
 * - X_RAM_CHECKS_HALF_HIGH_DATA : the function uses 0x0F and 0xF0 pattern values to check memory.
 * - X_RAM_CHECKS_ODD_BIT_DATA : the function uses 0x55 pattern value to check memory.
 * - X_RAM_CHECKS_EVEN_BIT_DATA : the function uses 0xAA pattern value to check memory.
 * - X_RAM_CHECKS_GENERIC_DATA : the function uses 0x12, 0x34, 0x56, 0x78, 0x87, 0x65, 0x43 and 0x21 pattern values to check memory.
 *
 * @return error count
 */
uint32_t X_RAM_CHECKS_check16bitWriteRead(uint16_t testType)
{
    uint32_t error_count = 0;
		uint8_t index;
		uint8_t pattern_idx_min, pattern_idx_max;
	
	switch (testType)
	{
		case X_RAM_CHECKS_ALL_LOW_DATA :
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
			break;
		case X_RAM_CHECKS_ALL_HIGH_DATA :
			pattern_idx_min = 1;
		  pattern_idx_max = 1;
			break;
		case X_RAM_CHECKS_ODD_BIT_DATA :
			pattern_idx_min = 2;
		  pattern_idx_max = 2;
			break;
		case X_RAM_CHECKS_EVEN_BIT_DATA :
			pattern_idx_min = 3;
		  pattern_idx_max = 3;
			break;
		case X_RAM_CHECKS_HALF_HIGH_DATA :
			pattern_idx_min = 4;
		  pattern_idx_max = 5;
			break;
		case X_RAM_CHECKS_GENERIC_DATA :
			pattern_idx_min = 6;
		  pattern_idx_max = 9;
			break;
		default:
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
	}
	for (index = pattern_idx_min; index < (pattern_idx_max + 1) ; index++)
	{
			error_count += checkPatternMemory16(ram_patterns16[index]);
	}	
	return error_count;
}


/**
 * @brief Writes the pattern in all bytes of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get8bitZones function. Next, reads all bytes of defined zones, to check 
 *        the memory integrity.
 *
 * @param[in] testType  test to run :
 * - X_RAM_CHECKS_ALL_LOW_DATA : the function uses 0x0000 pattern value to check memory.
 * - X_RAM_CHECKS_ALL_HIGH_DATA : the function uses 0xFFFF pattern value to check memory.
 * - X_RAM_CHECKS_HALF_HIGH_DATA : the function uses 0x00FF and 0xFF00 pattern values to check memory.
 * - X_RAM_CHECKS_ODD_BIT_DATA : the function uses 0x5555 pattern value to check memory.
 * - X_RAM_CHECKS_EVEN_BIT_DATA : the function uses 0xAAAA pattern value to check memory.
 * - X_RAM_CHECKS_GENERIC_DATA : the function uses 0x1234, 0x5678, 0x8765 and 0x4321 pattern values to check memory.
 *
 * @return error count
 */
uint32_t X_RAM_CHECKS_check8bitWriteAllNextReadAll(uint16_t patternType)
{
    uint32_t error_count = 0;
		uint8_t index;
		uint8_t pattern_idx_min, pattern_idx_max;
	
	switch (patternType)
	{
		case X_RAM_CHECKS_ALL_LOW_DATA :
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
			break;
		case X_RAM_CHECKS_ALL_HIGH_DATA :
			pattern_idx_min = 1;
		  pattern_idx_max = 1;
			break;
		case X_RAM_CHECKS_ODD_BIT_DATA :
			pattern_idx_min = 2;
		  pattern_idx_max = 2;
			break;
		case X_RAM_CHECKS_EVEN_BIT_DATA :
			pattern_idx_min = 3;
		  pattern_idx_max = 3;
			break;
		case X_RAM_CHECKS_HALF_HIGH_DATA :
			pattern_idx_min = 4;
		  pattern_idx_max = 5;
			break;
		case X_RAM_CHECKS_GENERIC_DATA :
			pattern_idx_min = 6;
		  pattern_idx_max = 13;
			break;
		default:
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
	}
	for (index = pattern_idx_min; index < (pattern_idx_max + 1) ; index++)
	{
			error_count += checkPatternAllMemory8(ram_patterns8[index]);
	}	
	return error_count;
}



/**
 * @brief Writes and reads the pattern for each byte of the memory zones defined in the array returned
 * 				by the  X_RAM_CHECKS_get8bitZones function. 
 *
 * @param[in] testType  test to run :
 * - X_RAM_CHECKS_ALL_LOW_DATA : the function uses 0x0000 pattern value to check memory.
 * - X_RAM_CHECKS_ALL_HIGH_DATA : the function uses 0xFFFF pattern value to check memory.
 * - X_RAM_CHECKS_HALF_HIGH_DATA : the function uses 0x00FF and 0xFF00 pattern values to check memory.
 * - X_RAM_CHECKS_ODD_BIT_DATA : the function uses 0x5555 pattern value to check memory.
 * - X_RAM_CHECKS_EVEN_BIT_DATA : the function uses 0xAAAA pattern value to check memory.
 * - X_RAM_CHECKS_GENERIC_DATA : the function uses 0x1234, 0x5678, 0x8765 and 0x4321 pattern values to check memory.
 *
 * @return error count
 */
uint32_t X_RAM_CHECKS_check8bitWriteRead(uint16_t testType)
{
    uint32_t error_count = 0;
		uint8_t index;
		uint8_t pattern_idx_min, pattern_idx_max;
	
	switch (testType)
	{
		case X_RAM_CHECKS_ALL_LOW_DATA :
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
			break;
		case X_RAM_CHECKS_ALL_HIGH_DATA :
			pattern_idx_min = 1;
		  pattern_idx_max = 1;
			break;
		case X_RAM_CHECKS_ODD_BIT_DATA :
			pattern_idx_min = 2;
		  pattern_idx_max = 2;
			break;
		case X_RAM_CHECKS_EVEN_BIT_DATA :
			pattern_idx_min = 3;
		  pattern_idx_max = 3;
			break;
		case X_RAM_CHECKS_HALF_HIGH_DATA :
			pattern_idx_min = 4;
		  pattern_idx_max = 5;
			break;
		case X_RAM_CHECKS_GENERIC_DATA :
			pattern_idx_min = 6;
		  pattern_idx_max = 13;
			break;
		default:
			pattern_idx_min = 0;
		  pattern_idx_max = 0;
	}
	for (index = pattern_idx_min; index < (pattern_idx_max + 1) ; index++)
	{
			error_count += checkPatternMemory8(ram_patterns8[index]);
	}	
	return error_count;
}


/**
 * @brief Writes 0x00 value in each byte of the area returned by the functions X_RAM_CHECKS_get32bitZones,
 * X_RAM_CHECKS_get16bitZones and X_RAM_CHECKS_get8bitZones.
 */
void X_RAM_CHECKS_MemoryCleanup(void)
{
    uint8_t *pointer;
    uint32_t i,j;
		// get the 32-bit zone definition array 
		X_RAM_CHECKS_zone_t* zones = X_RAM_CHECKS_get32bitZones();
	  // reset 32-bit zones
    for( j = 0; j < X_RAM_CHECKS_get32bitZoneNumber(); j++ )
		{
			pointer = (uint8_t *) zones[j].start_address;
			for (i = 0; i < ((zones[j].end_address - zones[j].start_address)); i++)
			{
        *(pointer + i) = 0x00;
			}
		}
		
		// get the 16-bit zone definition array 
		zones = X_RAM_CHECKS_get16bitZones();
	  // reset 16-bit zones
    for( j = 0; j < X_RAM_CHECKS_get16bitZoneNumber(); j++ )
		{
			pointer = (uint8_t *) zones[j].start_address;
			for (i = 0; i < ((zones[j].end_address - zones[j].start_address)); i++)
			{
        *(pointer + i) = 0x00;
			}
		}
		
		// get the 8-bit zone definition array 
		zones = X_RAM_CHECKS_get8bitZones();
	  // reset 8-bit zones
    for( j = 0; j < X_RAM_CHECKS_get8bitZoneNumber(); j++ )
		{
			pointer = (uint8_t *) zones[j].start_address;
			for (i = 0; i < ((zones[j].end_address - zones[j].start_address)); i++)
			{
        *(pointer + i) = 0x00;
			}
		}
		
}




