/*
 * C
 *
 * Copyright 2014-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file fault_handlers.c
 *
 * @brief This file contains the fault exception handlers.
 *
 * @details The CMSIS names are used.
 *
 * There are two modes:
 * - verbose
 * - lite
 *
 * In verbose mode, each fault exception handler will:
 * - print the stack frame,
 * - print its name,
 * - print the associated fault status register,
 * - try to make an analysis of the fault cause,
 * - make an infinite loop.
 *
 * In lite mode, each fault exception handler will:
 * - print its name,
 * - make an infinite loop.
 *
 * Define the @ref VERBOSE_MODE macro to activate verbose mode.
 */

/* Includes ______________________________________________________________________*/
#include <stdbool.h>
#include <stdio.h>

#include "stm32f7xx.h"

#include "fault_handlers.h" // will check prototypes and import public types

/* Macros ________________________________________________________________________*/

/**
 * @brief Define this macro to if you are using Keil's ARMCC toolchain. If you are using IAR or GCC toolchains, do not define it.
 */
//#define COMPILE_WITH_ARMCC

/**
 * @brief Define this macro to enable verbose handlers. Verbose handlers will consume  more memory than lite handlers.
 */
#define VERBOSE_MODE

/**
 * @brief Make an infinite while loop.
 */
#define INFINITE_LOOP()		{while(1);}

#ifdef VERBOSE_MODE
/**
 * @brief This macro is called as the begining of each handlers to print the stack frame.
 *
 * @details There are two stack pointers in Cortex-M processors:
 * - MSP (Main Stack Pointer)
 * - PSP (Process Stack Pointer)
 * When handling faults, we need to know which one was used before entering the faults so that
 * we can retrieve interesting information.
 *
 * To do so, we need to test the value of EXC_RETURN, which is stored into LR during exception
 * (see Section 8.1.4 EXC_RETURN). Then, we can fetch the correct stack pointer with the MRS instruction
 * (MRS = Move from special register to general register).
 *
 * NOTE: IAR's compiler doesn't like then comments are interlaced with macro definition lines.
 * Here is what the macro does:
 * - Check if bit 2 (stack return) is set in EXC_RETURN / LR
 * - If-Then condition instruction
 * - If equals, store MSP in R0
 * - Else, store PSP in RO (1st parameter)
 * - MSP is stored in R1 (2nd parameter)
 * - PSP is stored in R2 (3rd parameter)
 * - LR is stored in R3 (4th parameter)
 * - Call C function for printing
 */
#if defined(__CC_ARM)
void printer(uint32_t current_sp, uint32_t msp, uint32_t psp, uint32_t exc_return);

__asm void print_stacked_registers()
{
    TST     LR, #4
    ITE     EQ
    MRSEQ   R0, MSP
    MRSNE   R0, PSP
    MRS     R1, MSP
    MRS     R2, PSP
    MOV     R3, LR
	  LDR     R4, =__cpp(printer)
	  BX		  R4
}
#else
// Compatible with GCC and IAR
#define print_stacked_registers() {\
    __asm("TST LR, #4");\
    __asm("MRS R1, MSP");\
    __asm("MRS R2, PSP");\
    __asm("MOV R3, LR");\
    __asm("BL printer");\
}
#endif

// CMSIS doesn't provide macro to get bits from BFSR (which is the 2nd byte in CFSR), so we create our own macros
#define BFSR_IBUSERR_Pos        (0)
#define BFSR_IBUSERR_Msk        (1 << BFSR_IBUSERR_Pos)

#define BFSR_PRECISERR_Pos      (1)
#define BFSR_PRECISERR_Msk      (1 << BFSR_PRECISERR_Pos)

#define BFSR_IMPRECISERR_Pos    (2)
#define BFSR_IMPRECISERR_Msk    (1 << BFSR_IMPRECISERR_Pos)

#define BFSR_BFARVALID_Pos      (15)
#define BFSR_BFARVALID_Msk      (1 << BFSR_BFARVALID_Pos)

// CMSIS doesn't provide macro to get bits from UFSR (which is the upper half-word in CFSR), so we create our own macros
#define UFSR_UNDEFINSTR_Pos        (0)
#define UFSR_UNDEFINSTR_Msk        (1 << UFSR_UNDEFINSTR_Pos)

#define UFSR_INVSTATE_Pos          (1)
#define UFSR_INVSTATE_Msk          (1 << UFSR_INVSTATE_Pos)


/* Global variables  ___________________________________________________________*/


/* Private types and variables  ___________________________________________________*/


/* Private functions  ______________________________________________________________*/
// WARNING:
/**
 * @brief C function to print the stacked registers (== the stack frame) along with EXEC_RETURN.
 *
 * @warning this function cannot be static because it won't be seen by assembly code during link edition.
 * You should not called this function directly.
 */
/*static*/ void printer(uint32_t current_sp, uint32_t msp, uint32_t psp, uint32_t exc_return)
{
	  uint32_t *sp;
    puts("---------------------------------------------------------------------");
    // Show stack pointers
    printf("Current SP = %.8lX\n", current_sp);
    printf("MSP = %.8lX\n", msp);
    printf("PSP = %.8lX\n", psp);
    putchar('\n');

    // Show stacked registers == stack frame (see section 8.1.3 and figure 12.4)
    sp = (uint32_t*) current_sp;
    puts("Stack frame:");
    printf("R0 =\t%.8lX\n", *sp++);
    printf("R1 =\t%.8lX\n", *sp++);
    printf("R2 =\t%.8lX\n", *sp++);
    printf("R3 =\t%.8lX\n", *sp++);
    printf("R12 =\t%.8lX\n", *sp++);
    printf("LR =\t%.8lX\n", *sp++);
    printf("PC =\t%.8lX\n", *sp++);
    printf("xPSR =\t%.8lX\n", *sp++);
    putchar('\n');

    printf("EXC_RETURN (LR) = %.8lX\n", exc_return);

    puts("---------------------------------------------------------------------");
}

/* Public functions  _______________________________________________________________*/

/*
 * @brief Hard Fault exception handler.
 */
void HardFault_Handler(void)
{
	  uint32_t hfsr;
    print_stacked_registers();
    puts(__func__);

    hfsr = SCB->HFSR;
    printf("Hard Fault Status Register =\t%lX\n", hfsr);

    if(hfsr & SCB_HFSR_FORCED_Msk)
    {
    	printf("FORCED");
    }

    INFINITE_LOOP();
}


/*
 * @brief Memory Management Fault exception handler.
 */
void MemFault_Handler (void)
{
    print_stacked_registers();
    puts(__func__);

    INFINITE_LOOP();
}


/*
 * @brief Bus Fault exception handler.
 */
void BusFault_Handler (void)
{
	  uint32_t cfsr;
	  uint8_t bfsr;
	  uint32_t bfar;
	  bool still_valid;
    print_stacked_registers();
    puts(__func__);

    cfsr = SCB->CFSR;
    printf("Configurable Fault Status Register =\t%.8lX\n", cfsr);

    bfsr = (cfsr & SCB_CFSR_BUSFAULTSR_Msk) >> SCB_CFSR_BUSFAULTSR_Pos;
    printf("Bus Fault Status Register =\t%.2X\n", bfsr);

    if(bfsr & BFSR_IBUSERR_Msk)
    {
        puts("Instruction access error");
        // This case has not been experimented yet
    }

    if(bfsr & BFSR_PRECISERR_Msk)
    {
        puts("Precise data access error");

        // Faulting instruction
        // --> see the PC of the printed stack frame

        // Address of faulting data access
        bfar = SCB->BFAR;
        printf("Address of faulting data access (BFAR) = %.8lX\n", bfar);

        still_valid = (cfsr & BFSR_BFARVALID_Msk);
        if(!still_valid)
        {
            puts("WARNING: BFAR is no longer valid!");
        }
    }

    if(bfsr & BFSR_IMPRECISERR_Msk)
    {
        puts("Imprecise data access error");
    }

    INFINITE_LOOP();
}


/*
 * @brief Usage Fault exception handler.
 */
void UsageFault_Handler (void)
{
	  uint32_t cfsr;
	  uint16_t ufsr;
	// TODO Activate usage for for division by 0 and unaligned access? (see page 384)

    print_stacked_registers();
    puts(__func__);

    cfsr = SCB->CFSR;
    printf("Configurable Fault Status Register =\t%lX\n", cfsr);

    ufsr = (cfsr & SCB_CFSR_USGFAULTSR_Msk) >> SCB_CFSR_USGFAULTSR_Pos;
    printf("Usage Fault Status Register =\t%X\n", ufsr);

    if(ufsr & UFSR_UNDEFINSTR_Msk)
    {
        puts("Attempt to execute an undefined instruction");
    }

    if(ufsr & UFSR_INVSTATE_Msk)
    {
        puts("Attempt to switch to invalid mode (like ARM state)");
    }

    INFINITE_LOOP();
}
#else
/*
 * @brief Hard Fault exception handler.
 */
void HardFault_Handler (void)
{
	puts(__func__);
	INFINITE_LOOP();
}


/*
 * @brief Memory Management Fault exception handler.
 */
void MemFault_Handler (void)
{
	puts(__func__);
	INFINITE_LOOP();
}


/*
 * @brief Bus Fault exception handler.
 */
void BusFault_Handler (void)
{
	puts(__func__);
	INFINITE_LOOP();
}


/*
 * @brief Usage Fault exception handler.
 */
void UsageFault_Handler (void)
{
	puts(__func__);
	INFINITE_LOOP();
}

#endif
