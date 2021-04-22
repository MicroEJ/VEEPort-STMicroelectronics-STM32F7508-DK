/*
*****************************************************************************
**

**  File        : LinkerScript.ld
**
**  Abstract    : Linker script for STM32F750N8Hx Device
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
**  (c)Copyright Ac6.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Ac6 permit registered System Workbench for MCU users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the System Workbench for MCU toolchain.
**
**  Copyright 2020 MicroEJ Corp. This file has been modified by MicroEJ Corp.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20050000;    /* end of RAM */

/* Define the size of the network heap used by LwIP */
__NETWORK_HEAP_Size__ = 0x40000;

/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x400;  /* required amount of heap  */
_Min_Stack_Size = 0x800; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
  ITCM (xrw)      : ORIGIN = 0x00000000, LENGTH = 16K
  FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 64K
  QSPI (rx)       : ORIGIN = 0x90000000, LENGTH = 16384K
  DTCM (rw)       : ORIGIN = 0x20000000, LENGTH = 64K
  SRAM (rw)       : ORIGIN = 0x20010000, LENGTH = 256K
  SDRAM (rw)      : ORIGIN = 0xC0000000, LENGTH = 8192K
}

/* Define output sections */
SECTIONS
{
  /* The vector table goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Vector table */
    . = ALIGN(4);
  } >FLASH

  /* Map in FLASH the startup code (mandatory), along with optional code mapped for performance reasons */
  .text_flash :
  {
    . = ALIGN(4);

    /* Map in FLASH the startup code (mandatory) */
    *main.o(.text.startup.main .text.init)
    *memory_msp.o(.text.QSPI_MspInit)
    *qspi.o(.text.QSPI_Startup)
    *startup_stm32f750xx.o(.text .text.*)
    *system_stm32f7xx.o(.text .text.*)

    /* Map in FLASH the HAL driver code used for startup (mandatory) */
    *stm32f7xx_hal.o(.text .text.*)
    *stm32f7xx_hal_cortex.o(.text .text.*)
    *stm32f7xx_hal_gpio.o(.text .text.*)
    *stm32f7xx_hal_pwr_ex.o(.text .text.*)
    *stm32f7xx_hal_qspi.o(.text .text.*)
    *stm32f7xx_hal_rcc.o(.text .text.*)

    /* Toolchain libraries used for startup (mandatory) */
    *_aeabi_uldivmod.o(.text)
    *_udivmoddi4.o(.text)
    *crtbegin.o(.text)
    *libc_nano.a:(.text.__libc_init_array)

    /* Toolchain libraries (optional, performance increases compared with default QSPI mapping) */
    *libgcc.a:(.text .text.*)
    *lib_a-nano*.o(.text .text.*)

    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
  } >FLASH

  /* Map in FLASH the startup data (mandatory) */
  .rodata_flash :
  {
    . = ALIGN(4);
    *system_stm32f7xx.o(.rodata)
    . = ALIGN(4);
  } >FLASH

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Place in FLASH the readwrite region initializers (mandatory) */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */

    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >SRAM AT> FLASH

  /* Used by the startup to initialize the ITCM region */
  _siitcm = LOADADDR(.itcm);

  /* Place in ITCM code sections that improves the system performance */
  .itcm :
  {
    . = ALIGN(4);
    _sitcm = .;        /* create a global symbol at ITCM start */

    /* FreeRTOS code */
    *cmsis_os.o(.text .text.*)
    *heap_4.o(.text .text.*)
    *list.o(.text .text.*)
    *port.o(.text .text.*)
    *queue.o(.text .text.*)
    *tasks.o(.text .text.*)
    *timers.o(.text .text.*)

    /* SysTick interrupt handler and calees */
    *stm32f7xx_it.o(.text .text.*)
    *stm32f7xx_custom.o(.text .text.*)

    . = ALIGN(4);
    _eitcm = .;        /* define a global symbol at ITCM end */
  } >ITCM AT> QSPI

  /* The program code goes into QSPI */
  .text_qspi :
  {
    . = ALIGN(4);

    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */

    *(.text.features.installed)
    *(.text.soar)

    . = ALIGN(4);
  } >QSPI

  /* The constant data goes into QSPI */
  .rodata_qspi :
  {
    . = ALIGN(4);

    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.rodata.resources)

    . = ALIGN(4);
  } >QSPI

  /* Place in DTCM .bss sections that improves the system performance */
  .dtcm :
  {
    . = ALIGN(4);
    _sdbss = .;         /* define a global symbol at bss start */

    /* Ethernet buffers */
    *ethernetif.o(.RxDescripSection .TxDescripSection .RxBUF .TxBUF)

    . = ALIGN(4);
    _edbss = .;         /* define a global symbol at bss end */
  } >DTCM

  ._network_heap :
  {
    . = ALIGN(4);
    PROVIDE ( __NETWORK_HEAP_Start__ = . );
    . = . + __NETWORK_HEAP_Size__;
    . = ALIGN(4);
    __NETWORK_HEAP_End__ = .;         /* define a global symbol at NETWORK heap end */
  } > SDRAM

  /* SDRAM empty */
  .sdram :
  {
  } >SDRAM

  /* Uninitialized data section */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;         /* define a global symbol at bss start */

    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
  } >SRAM

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >SRAM

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
