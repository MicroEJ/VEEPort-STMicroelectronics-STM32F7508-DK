/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Prevent recursive inclusion */

#ifndef __X_RAM_SPEED_H
#define __X_RAM_SPEED_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

//#define CONFIG_TEST_PRINTF_ON

/* Debug macro */
#ifndef TEST_DBG
#ifdef CONFIG_TEST_PRINTF_ON
    #define TEST_DBG      printf
#else
    #define TEST_DBG(...) (void)0
#endif
#endif

/* Ram speed tests pattern definition */

#define X_RAM_SPEED_PATTERN32                    0xAAAAAAAA
#define X_RAM_SPEED_PATTERN16                    0xAAAA
#define X_RAM_SPEED_PATTERN8                     0xAA

/* public function declaration */
/**
 * @fn  X_RAM_SPEED_getAverageWriteSpeed(void)
 * @brief writes the patterns X_RAM_SPEED_PATTERNxx, in all words of areas returned by X_RAM_CHECKS_getxxbitZones functions, and mesures the average time for this write.
 * @return the average write speed of all memory areas in Mbytes/s
 *
 *
 * @fn  X_RAM_SPEED_getAverageReadSpeed(void)
 * @brief reads, in all words of areas returned by X_RAM_CHECKS_getxxbitZones functions, and mesures the average time for this read.
 * @return the average read speed of all memory areas in Mbytes/s
 *
 * @fn  X_RAM_SPEED_getAverageTransfertSpeed(void)
 * @brief copies all words of memory source areas returned by X_RAM_CHECKS_getxxbitSourceZone functions into memoy areas defined by X_RAM_CHECKS_getxxbitZones functions, and mesures the average time for this copy.
 * @return the average transfert speed of all memory areas in Mbytes/s
 *
 */
double X_RAM_SPEED_getAverageWriteSpeed(void);
double X_RAM_SPEED_getAverageReadSpeed(void);
double X_RAM_SPEED_getAverageTransfertSpeed(void);

/* weak function declaration */
    void X_RAM_SPEED_writeBlock32(volatile uint32_t *address, volatile uint32_t value, uint32_t size);
    void X_RAM_SPEED_readBlock32(volatile uint32_t *address, uint32_t size);
    void X_RAM_SPEED_transfertBlock32(volatile uint32_t *src, volatile uint32_t *dest, uint32_t size);
    void X_RAM_SPEED_writeBlock16(volatile uint16_t *address, volatile uint16_t value, uint32_t size);
    void X_RAM_SPEED_readBlock16(volatile uint16_t *address, uint32_t size);
    void X_RAM_SPEED_transfertBlock16(volatile uint16_t *src, volatile uint16_t *dest, uint32_t size);
    void X_RAM_SPEED_writeBlock8(volatile uint8_t *address, volatile uint8_t value, uint32_t size);
    void X_RAM_SPEED_readBlock8(volatile uint8_t *address, uint32_t size);
    void X_RAM_SPEED_transfertBlock8(volatile uint8_t *src, volatile uint8_t *dest, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif
