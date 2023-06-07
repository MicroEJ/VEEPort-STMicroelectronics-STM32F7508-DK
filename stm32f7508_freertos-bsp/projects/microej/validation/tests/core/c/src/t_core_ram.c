/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include "../../../../framework/c/embunit/embUnit/embUnit.h"
#include "x_ram_checks.h"
#include "x_ram_speed.h"
#include "../../../../framework/c/utils/inc/u_print.h"


static void T_CORE_RAM_setUp(void)
{
    X_RAM_CHECKS_MemoryCleanup();
}

static void T_CORE_RAM_tearDown(void)
{
    X_RAM_CHECKS_MemoryCleanup();
}

/** 32-bit tests */
static void T_CORE_RAM_is_32bit_zones_defined(void)
{
	TEST_ASSERT_MESSAGE( X_RAM_CHECKS_get32bitZones() != NULL,  "No defined zones to test. Please implement RAM_TEST_get32bitZones and RAM_TEST_get32bitZoneNumber functions.\n" );
}

static void T_CORE_RAM_write_read_test_32bit_all_bits_low(void)
{ 
	if (X_RAM_CHECKS_get32bitZones() != NULL) 
	{
		TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check32bitWriteRead(X_RAM_CHECKS_ALL_LOW_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_32bit_all_bits_high(void)
{
		if (X_RAM_CHECKS_get32bitZones() != NULL) 
	{
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check32bitWriteRead(X_RAM_CHECKS_ALL_HIGH_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_32bit_odd_bits_high(void)
{
	if (X_RAM_CHECKS_get32bitZones() != NULL) 
	{
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check32bitWriteRead(X_RAM_CHECKS_ODD_BIT_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_32bit_even_bits_high(void)
{
	if (X_RAM_CHECKS_get32bitZones() != NULL) 
	{ 
		TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check32bitWriteRead(X_RAM_CHECKS_EVEN_BIT_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_32bit_generic_data(void)
{
	if (X_RAM_CHECKS_get32bitZones() != NULL) 
	{
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check32bitWriteRead(X_RAM_CHECKS_GENERIC_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_32bit_address_bits(void)
{
	if (X_RAM_CHECKS_get32bitZones() != NULL) 
	{
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_checkAddress32(), 0);
	}
}

/** 16-bit tests */
static void T_CORE_RAM_is_16bit_zones_defined(void)
{
	TEST_ASSERT_MESSAGE( 	X_RAM_CHECKS_get16bitZones() != NULL, "No defined zones to test. Please implement RAM_TEST_get16bitZones and RAM_TEST_get16bitZoneNumber functions.\n" );
}

static void T_CORE_RAM_write_read_test_16bit_all_bits_low(void)
{
	if (X_RAM_CHECKS_get16bitZones() != NULL) 
	{
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check16bitWriteRead(X_RAM_CHECKS_ALL_LOW_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_16bit_all_bits_high(void)
{
	if (X_RAM_CHECKS_get16bitZones() != NULL) 
	{
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check16bitWriteRead(X_RAM_CHECKS_ALL_HIGH_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_16bit_odd_bits_high(void)
{
	if (X_RAM_CHECKS_get16bitZones() != NULL) 
	{
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check16bitWriteRead(X_RAM_CHECKS_ODD_BIT_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_16bit_even_bits_high(void)
{
	if (X_RAM_CHECKS_get16bitZones() != NULL) 
	{
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check16bitWriteRead(X_RAM_CHECKS_EVEN_BIT_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_16bit_generic_data(void)
{
	if (X_RAM_CHECKS_get16bitZones() != NULL) 
	{
		TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check16bitWriteRead(X_RAM_CHECKS_GENERIC_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_16bit_address_bits(void)
{
  if (X_RAM_CHECKS_get16bitZones() != NULL) 
	{ 
		TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_checkAddress16(), 0);
	}
}


/** 8-bit tests */
static void T_CORE_RAM_is_8bit_zones_defined(void)
{
		TEST_ASSERT_MESSAGE( 	X_RAM_CHECKS_get8bitZones() != NULL, "No defined zones to test. Please implement RAM_TEST_get8bitZones and RAM_TEST_get8bitZoneNumber functions.\n" );
}

static void T_CORE_RAM_write_read_test_8bit_all_bits_low(void)
{
	if (X_RAM_CHECKS_get8bitZones() != NULL) 
	{ 
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check8bitWriteRead(X_RAM_CHECKS_ALL_LOW_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_8bit_all_bits_high(void)
{
	if (X_RAM_CHECKS_get8bitZones() != NULL) 
	{ 
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check8bitWriteRead(X_RAM_CHECKS_ALL_HIGH_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_8bit_odd_bits_high(void)
{
	if (X_RAM_CHECKS_get8bitZones() != NULL) 
	{ 
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check8bitWriteRead(X_RAM_CHECKS_ODD_BIT_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_8bit_even_bits_high(void)
{
	if (X_RAM_CHECKS_get8bitZones() != NULL) 
	{ 
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check8bitWriteRead(X_RAM_CHECKS_EVEN_BIT_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_8bit_generic_data(void)
{
	if (X_RAM_CHECKS_get8bitZones() != NULL) 
	{ 
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_check8bitWriteRead(X_RAM_CHECKS_GENERIC_DATA), 0);
	}
}

static void T_CORE_RAM_write_read_test_8bit_address_bits(void)
{
	if (X_RAM_CHECKS_get8bitZones() != NULL) 
	{ 
    TEST_ASSERT_EQUAL_INT(X_RAM_CHECKS_checkAddress8(), 0);
	}
}

/** speed benchmark */
static void T_CORE_RAM_read_speed_f(void)
{
	UTIL_print_string("RAM speed average read access (according to your configuration file 8/16/32 bits) : ");
	UTIL_print_float( (X_RAM_SPEED_getAverageReadSpeed() / 1024.0) / 1024.0);
	UTIL_print_string( "MBytes/s \n" );
}

static void T_CORE_RAM_write_speed_f(void)
{
	UTIL_print_string("RAM speed average write access (according to your configuration file 8/16/32 bits) : ");
	UTIL_print_float( (X_RAM_SPEED_getAverageWriteSpeed() / 1024.0) / 1024.0);
	UTIL_print_string(" MBytes/s \n");
}

static void T_CORE_RAM_transfert_speed_f(void)
{
	UTIL_print_string("RAM speed average transfert access (according to your configuration file 8/16/32 bits) : ");
	UTIL_print_float((X_RAM_SPEED_getAverageTransfertSpeed() / 1024.0) / 1024.0);
	UTIL_print_string("MBytes/s \n");
}

TestRef T_CORE_RAM_tests(void)
{

		EMB_UNIT_TESTFIXTURES(fixtures) { 
			new_TestFixture("Check for existing 32-bit zones", T_CORE_RAM_is_32bit_zones_defined),
			new_TestFixture("Write all bits low in 32-bit zones", T_CORE_RAM_write_read_test_32bit_all_bits_low),
			new_TestFixture("Write all bits high in 32-bit zones", T_CORE_RAM_write_read_test_32bit_all_bits_high),
			new_TestFixture("Write odd bits high in 32-bit zones", T_CORE_RAM_write_read_test_32bit_odd_bits_high),
			new_TestFixture("Write even bits high in 32-bit zones", T_CORE_RAM_write_read_test_32bit_even_bits_high),
			new_TestFixture("Write generic data in 32-bit zones", T_CORE_RAM_write_read_test_32bit_generic_data),
			new_TestFixture("Check address bits in 32-bit zones", T_CORE_RAM_write_read_test_32bit_address_bits),
			//
			new_TestFixture("Check for existing 16-bit zones", T_CORE_RAM_is_16bit_zones_defined),
			new_TestFixture("Write all bits low in 16-bit zones", T_CORE_RAM_write_read_test_16bit_all_bits_low),
			new_TestFixture("Write all bits high in 16-bit zones", T_CORE_RAM_write_read_test_16bit_all_bits_high),
			new_TestFixture("Write odd bits high in 16-bit zones", T_CORE_RAM_write_read_test_16bit_odd_bits_high),
			new_TestFixture("Write even bits high in 16-bit zones", T_CORE_RAM_write_read_test_16bit_even_bits_high),
			new_TestFixture("Write generic data in 16-bit zones", T_CORE_RAM_write_read_test_16bit_generic_data),
			new_TestFixture("Check address bits in 16-bit zones", T_CORE_RAM_write_read_test_16bit_address_bits),
			//
			new_TestFixture("Check for existing 8-bit zones", T_CORE_RAM_is_8bit_zones_defined),
			new_TestFixture("Write all bits low in 8-bit zones", T_CORE_RAM_write_read_test_8bit_all_bits_low),
			new_TestFixture("Write all bits high in 8-bit zones", T_CORE_RAM_write_read_test_8bit_all_bits_high),
			new_TestFixture("Write odd bits high in 8-bit zones", T_CORE_RAM_write_read_test_8bit_odd_bits_high),
			new_TestFixture("Write even bits high in 8-bit zones", T_CORE_RAM_write_read_test_8bit_even_bits_high),
			new_TestFixture("Write generic data in 8-bit zones", T_CORE_RAM_write_read_test_8bit_generic_data),
			new_TestFixture("Check address bits in 8-bit zones", T_CORE_RAM_write_read_test_8bit_address_bits),
		};
		UTIL_print_string("\nRAM tests:\n");
		EMB_UNIT_TESTCALLER(ramTest, "RAM_tests", T_CORE_RAM_setUp, T_CORE_RAM_tearDown, fixtures);

    return (TestRef)&ramTest;
}

TestRef T_CORE_RAM_speed_tests(void)
{
	EMB_UNIT_TESTFIXTURES(fixtures_speed) { 
        new_TestFixture("T_CORE_RAM_read_speed_f", T_CORE_RAM_read_speed_f),
        new_TestFixture("T_CORE_RAM_write_speed_f", T_CORE_RAM_write_speed_f),
        new_TestFixture("T_CORE_RAM_transfert_speed_f", T_CORE_RAM_transfert_speed_f),
	};
	UTIL_print_string("\nRAM speed benchmark:\n");
	EMB_UNIT_TESTCALLER(ramTestSpeed, "RAM speed_tests", T_CORE_RAM_setUp, T_CORE_RAM_tearDown, fixtures_speed);
	
	return (TestRef)&ramTestSpeed;
}
