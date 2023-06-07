/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "x_ram_speed.h"
#include "bsp_util.h"


/** public weak function default definitions */


/**
 * @brief 
 *
 * @return 
 */

BSP_DECLARE_WEAK_FCNT void X_RAM_SPEED_writeBlock32(volatile uint32_t *address, volatile uint32_t value, uint32_t size)
{
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint32_t)); i++)
    {
        *address++ = value;
    }
}


/**
 * @brief 
 *
 * @return 
 */
BSP_DECLARE_WEAK_FCNT void X_RAM_SPEED_readBlock32(volatile uint32_t *address, uint32_t size)
{
    volatile uint32_t value;
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint32_t)); i++)
    {
        value = *address++;
    }
}


/**
 * @brief 
 *
 * @return 
 */
BSP_DECLARE_WEAK_FCNT void X_RAM_SPEED_transfertBlock32(volatile uint32_t *src, volatile uint32_t *dest, uint32_t size)
{
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint32_t)); i++)
    {
        *dest++ = *src++;
    }
}


/**
 * @brief 
 *
 * @return 
 */
BSP_DECLARE_WEAK_FCNT void X_RAM_SPEED_writeBlock16(volatile uint16_t *address, volatile uint16_t value, uint32_t size)
{
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint16_t)); i++)
    {
        *address++ = value;
    }
}


/**
 * @brief 
 *
 * @return 
 */
BSP_DECLARE_WEAK_FCNT void X_RAM_SPEED_readBlock16(volatile uint16_t * address, uint32_t size)
{
    volatile uint16_t value;
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint16_t)); i++)
    {
        value = *address++;
    }
}


/**
 * @brief 
 *
 * @return 
 */
BSP_DECLARE_WEAK_FCNT void X_RAM_SPEED_transfertBlock16(volatile uint16_t *src, volatile uint16_t *dest, uint32_t size)
{
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint16_t)); i++)
    {
        *dest++ = *src++;
    }
}


/**
 * @brief 
 *
 * @return 
 */
BSP_DECLARE_WEAK_FCNT void X_RAM_SPEED_writeBlock8(volatile uint8_t *address, volatile uint8_t value, uint32_t size)
{
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint8_t)); i++)
    {
        *address++ = value;
    }
}


/**
 * @brief 
 *
 * @return 
 */
BSP_DECLARE_WEAK_FCNT void X_RAM_SPEED_readBlock8(volatile uint8_t *address, uint32_t size)
{
    volatile uint8_t value;
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint8_t)); i++)
    {
        value = *address++;
    }
}


/**
 * @brief 
 *
 * @return 
 */
BSP_DECLARE_WEAK_FCNT void X_RAM_SPEED_transfertBlock8(volatile uint8_t *src, volatile uint8_t *dest, uint32_t size)
{
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint8_t)); i++)
    {
        *dest++ = *src++;
    }
}
