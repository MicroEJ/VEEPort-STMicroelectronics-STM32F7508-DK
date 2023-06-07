/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Prevent recursive inclusion */

#ifndef __X_RAM_CHECKS_H
#define __X_RAM_CHECKS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

/* Public constant declaration */

/*
 *    pattern type definitions
 */
#define X_RAM_CHECKS_ALL_LOW_DATA       0x0001			/**<  */
#define X_RAM_CHECKS_ALL_HIGH_DATA      0x0002
#define X_RAM_CHECKS_HALF_HIGH_DATA     0x0004
#define X_RAM_CHECKS_ODD_BIT_DATA       0x0008
#define X_RAM_CHECKS_EVEN_BIT_DATA      0x0010
#define X_RAM_CHECKS_GENERIC_DATA       0x0020



/* public structure declaration */

/**
 * @typedef X_RAM_CHECKS_zone_t
 * @brief provide a memory zone definition and a storage location for memory benchmark result
 */
typedef struct {
	uint32_t start_address;		/**< start address of the memory area */
	uint32_t end_address;		/**< end address of the memory area */
	double write_speed;			/**< result of the write speed benchmark in this memory area */
	double read_speed;				/**< result of the read speed benchmark in this memory area */
	double transfert_speed;		/**< result of the transfert speed benchmark in this memory area */
} X_RAM_CHECKS_zone_t;

/* public function declaration */

uint32_t X_RAM_CHECKS_check32bitWriteAllNextReadAll(uint16_t patternType);
uint32_t X_RAM_CHECKS_check32bitWriteRead(uint16_t patternType);
uint32_t X_RAM_CHECKS_checkAddress32(void);
uint32_t X_RAM_CHECKS_check16bitWriteAllNextReadAll(uint16_t patternType);
uint32_t X_RAM_CHECKS_check16bitWriteRead(uint16_t patternType);
uint32_t X_RAM_CHECKS_checkAddress16(void);
uint32_t X_RAM_CHECKS_check8bitWriteAllNextReadAll(uint16_t patternType);
uint32_t X_RAM_CHECKS_check8bitWriteRead(uint16_t patternType);
uint32_t X_RAM_CHECKS_checkAddress8(void);

void X_RAM_CHECKS_MemoryCleanup(void);

// weak function to implement
/**
 * @fn X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitZones(void)
 * @brief this function provide a definition array of  memory zones to test with 32-bit accesses.
 * @return array of X_RAM_CHECKS_zone_t which define each 32-bit zone to test.
 *
 * @fn X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitZones(void)
 * @brief this function provide a definition array of  memory zones to test with 16-bit accesses.
 * @return array of X_RAM_CHECKS_zone_t which define each 16-bit zone to test.
 *
 * @fn X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitZones(void)
 * @brief this function provide a definition array of  memory zones to test with 8-bit accesses.
 * @return array of X_RAM_CHECKS_zone_t which define each 8-bit zone to test.
 *
 * @fn X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitSourceZone(void)
 * @brief this function provide a definition of  memory area to use as a source for 32-bit transfert speed measurement.
 * @return a pointer to the structure which define the memory source area. 
 *
 * @fn X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitSourceZone(void)
 * @brief this function provide a definition of  memory area to use as a source for 16-bit transfert speed measurement.
 * @return a pointer to the structure which define the memory source area. 
 *
 * @fn X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitSourceZone(void)
 * @brief this function provide a definition of  memory area to use as a source for 8-bit transfert speed measurement.
 * @return a pointer to the structure which define the memory source area. 
  *
 * @fn uint8_t X_RAM_CHECKS_get32bitZoneNumber(void)
 * @brief this function gives the number of memory areas to test with 32-bit accesses.
 * @return the number of memory areas. 
   *
 * @fn uint8_t X_RAM_CHECKS_get16bitZoneNumber(void)
 * @brief this function gives the number of memory areas to test with 32-bit accesses.
 * @return the number of memory areas. 
   *
 * @fn uint8_t X_RAM_CHECKS_get8bitZoneNumber(void)
 * @brief this function gives the number of memory areas to test with 32-bit accesses.
 * @return the number of memory areas. 
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitZones(void);
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitZones(void);
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitZones(void);
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitSourceZone(void);
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitSourceZone(void);
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitSourceZone(void);
uint8_t X_RAM_CHECKS_get32bitZoneNumber(void);
uint8_t X_RAM_CHECKS_get16bitZoneNumber(void);
uint8_t X_RAM_CHECKS_get8bitZoneNumber(void);

#ifdef __cplusplus
}
#endif

#endif
