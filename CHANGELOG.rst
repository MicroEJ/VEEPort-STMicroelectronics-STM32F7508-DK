..
    Copyright 2020-2021 MicroEJ Corp. All rights reserved.
    This library is provided in source code for use, modification and test, subject to license terms.
    Any modification of the source code will break MicroEJ Corp. warranties on the whole library.

===========
 Changelog
===========

----------------------
 [1.3.2] - 2021-06-04
----------------------

Fixed
=====

- Fix platform type (MonoSandbox by default).
- Fix documentation of the FS properties.
- Enable printf for floats in STM32CubeIDE project.

----------------------
 [1.3.1] - 2021-05-06
----------------------

Fixed
=====

- Cleanup useless Git submodules.

----------------------
 [1.3.0] - 2021-04-20
----------------------

Added
=====

- Scripts to build and flash (GCC and IAR).
- Platform documentation.
- Initial Hello World application files to compile the BSP without any platform (GCC and IAR).
- New platform builder.
- New platform build type.

Changed
=======

- Update modules:
    - Pack UI 13.0.6.
    - Pack NET 9.2.3.
- Improve footprint.
- Remove wadapps dependencies from ``module.ivy`` and add wadapps in dropins.

Fixed
=====

- Remove useless watchdog message on simulation.
- Fix TCP/IP task priority.
- Remove unused fonts.

----------------------
 [1.2.0] - 2020-11-20
----------------------

Fixed
=====

- Declare display and network heaps dynamically instead of statically in the linker file.

Added
=====

- Added iperf benchmark.
- Added build time configuration for System View support.
- Update documentation with guidance of avoiding link errors with certain applications.
- IAR platform build compliancy.
- Update FS pack to 5.1.2 that fixes a simulation issue with architectures 7.13+.
- Update architecture to 7.14.0.

----------------------
 [1.1.0] - 2020-10-13
----------------------

Added
=====

- Use native DNS instead of soft DNS.
- Added support for System View.
- Update to IAR Embedded Workbench version 8.50.5.
- Change display buffering mode to switch mode.
- Print MCU revision identifier and MCU device identifier at startup.
- Add support for IAR Embedded Workbench version 8.30.0 or higher.
- Add support for `xPortIsInsideInterrupt` inside FreeRTOS port of IAR.
- Update mbedTLS to version 2.16.7.
- Optimize memory layout for both GCC and IAR toolchains for improving overall performances.
- Update architecture to 7.13.0 that provides SNI-1.4.
- Update architecture UI to 13.0.1 that provides MicroUI-3.0 and Drawing-1.0.
- Add support for Multi-Sandbox Firmware

Fixed
=====

- Fixed FS writes to SD card with non-aligned DMA buffer address.
- Avoid double SDRAM initialization during LCD initialization.
- Fixed LTDC FIFO underruns.
- Fixed board freeze during UI transfers.
- Fixed SSL memory allocator, switching from libc to FreeRTOS.
- Fix interrupt detection through MicroEJ modules.
- Removed stubbed LwIP headers from the net module.

----------------------
 [1.0.0] - 2020-08-05
----------------------

Added
=====

- Add MicroEJ CORE support.
- Add MicroEJ UI support.
- Add MicroEJ HAL support.
- Add MicroEJ NET support.
- Add MicroEJ SSL support.
- Add MicroEJ FS support.
- Add MicroEJ WATCHDOG support.
- Initial release of the platform.
