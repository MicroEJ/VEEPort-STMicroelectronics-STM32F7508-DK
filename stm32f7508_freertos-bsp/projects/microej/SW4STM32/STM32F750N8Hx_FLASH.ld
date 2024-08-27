/*
 * Copyright 2020-2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20050000;    /* end of RAM */

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
    *main.o(.text.startup.main .text.main .text.init .text.CPU_CACHE_Enable .text.SCB_EnableDCache .text.SCB_EnableICache .text.SystemClock_Config)
    *memory_msp.o(.text.QSPI_MspInit)
    *qspi.o(.text .text.*)
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

    /* VM Core (optional, performance increases compared with default QSPI mapping) */
    *VMCOREMicroJvm*.o(.text .text.*)
    *iceTea_lang*.o(.text .text.*)
    *iceTea_support_armv7m*.o(.text .text.*)
    *ist_mowana_vm*.o(.text .text.*)

    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)
    
    /* VM core BSP support functions */
    *LLBSP_generic.o(.text .text.*)
    *LLMJVM_FreeRTOS.o(.text .text.*)

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

    /* ui dma2d cco */
    *drawing_dma2d.o(.text .text.*)

    . = ALIGN(4);
    _eitcm = .;        /* define a global symbol at ITCM end */
  } >ITCM AT> QSPI

  /* The program code goes into QSPI */
  .text_qspi :
  {
    . = ALIGN(4);

    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */

    *(.rodata.microej.features)

    . = ALIGN(4);
  } >QSPI

  /* The constant data goes into QSPI */
  .rodata_qspi :
  {
    . = ALIGN(4);

    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    *(.rodata.microej.soar.*)
    *(.rodata.microej.resource.*)

    . = ALIGN(4);
  } >QSPI

  /* Place in DTCM .bss sections that improves the system performance */
  .dtcm :
  {
    . = ALIGN(4);
    _sdbss = .;         /* define a global symbol at bss start */

    /* Ethernet buffers */
    *ethernetif.o(.RxDescripSection .TxDescripSection .RxBUF .TxBUF)

    /* VM stacks */
    *(.bss.microej.stacks)

    /* VM runtime structures */
    *(.bss.microej.runtime)

    . = ALIGN(4);
    _edbss = .;         /* define a global symbol at bss end */
  } >DTCM

  ._display_stack :
  {
    . = ALIGN(512K);
    _sdisplay_stack = .;
    
    *LLUI_DISPLAY.o(.DisplayMem)
    . = ALIGN(512K);
    _edisplay_stack = .;
  } > SDRAM

  ._network_heap :
  {
    . = ALIGN(4);
    *LLNET_NETWORK_MEM.o(.NetworkHeap)
    . = ALIGN(4);
  } > SDRAM

  /* SDRAM used for: MicroEJ heap, immortals heap and MicroUI heap */
  .sdram :
  {
    *(.bss.microej.heap)
    *(.bss.microej.immortals)
    PROVIDE ( _microui_heap_start = . );       /* define a global symbol at MicroUI heap start */
    *(.bss.microui.display.imagesHeap)
    PROVIDE ( _microui_heap_end = . );         /* define a global symbol at MicroUI heap end */
  } >SDRAM

  ._kf_heap :
  {
    . = ALIGN(4);
    *LLKERNEL_SDRAM.o(.KfHeap)
    . = ALIGN(4);
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
