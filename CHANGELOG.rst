..
    Copyright 2020-2024 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

===========
 Changelog
===========

----------------------
[2.2.0] - 2024-01-16
----------------------

Changed
=======

- Update the CI build configuration to build ``prod`` and ``eval`` VEE Port modules.
- Update UI pack to 13.7.2.
- Update UI test suite to 1.6.0
- Update PCA scripts to 2.1.0.
- Update BSP build scripts behavior on Windows, allow default toolchain path to be overridden from system environment variables.

Fixed
=====

- Fix ``run.sh`` to accept ``application.out`` as parameter.
- Fix path to the external loader file in ``run.sh`` that prevented the application to be flashed properly.

Removed
=======

- Remove ``mjvm.properties`` from ``-configuration``, not required since Architecture 8.0.0.

----------------------
[2.1.2] - 2023-10-02
----------------------

Fixed
=====

- Fix SystemView live analysis, missing task name for Core Engine tasks.

----------------------
[2.1.1] - 2023-09-28
----------------------

Fixed
=====

- Fix CI configuration issue.

----------------------
[2.1.0] - 2023-09-27
----------------------

Added
=====

- Script ``dump_mcu_memory_stm32.py`` to dump memory section of the MCU
- Documentation GUI badge

Changed
=======

- SystemView documentation in RELEASE NOTES

Fixed
=====

- Fix CORE validation execution by renaming testFloat and testDouble natives
- Fix hardfault as soon as the SystemView analysis is started when the JVM is not running

----------------------
[2.0.0] - 2023-09-07
----------------------

Added
=====

- STM32Cube IDE ``build.sh`` and ``run.sh`` Linux scripts
- IAR BXARM ``build.sh`` Linux script

Changed
=======

- Update MicroEJ architecture to 8.0.0
- Rename ICETEA_HEAP region to .bss.microej.runtime
- Rename _java_heap region to .bss.microej.heap
- Rename _java_immortals region to .bss.microej.immortals
- Rename .bss.vm.stacks.java region to .bss.microej.stacks
- Added new region .bss.ecom
- Remove SD card buffers from DTCM region
- Added DMA2D object to ITCM region
- Update mjvm-freertos cco to 1.4.0
- Call STM32CubeF7 patch apply from build script
- Align display stack to 512kB
- Configure MPU for SDRAM as not-bufferable, not-shareable and cacheable
- Configure MPU section in SDRAM for display stack, if present, as not-bufferable, shareable and cacheable

Fixed
=====

- Fix issue launching ``run.bat`` script when previous launch failed.
- Fix issue launching ``run.bat`` script when project directory is located on a drive different from the system drive (``C:\``).
- Fix the place where the application.out file is copied (IAR) in the ``build.bat`` file.
- Fix too big projects list when import of VEE Port repository.

---------------------
[1.6.1] - 2023-06-29
---------------------

Fixed
=====

- Invalid HIL client message when simulation used
- Packaged VEE Port zip file size too big when ``skip.publish=false``
- ECOM-COMM Asbtraction Layer initialization issue

----------------------
 [1.6.0] - 2023-04-20
----------------------

Added
=====

- Validation projects (CORE, FS, NET, SSL, SECURITY, BLUETOOTH, ESP32)
- CoreMark as a submodule
- iperf validation project

Changed
=======

- Update STM32CubeF7 to v1.17.0
- Use STMicroelectronics STM32CubeF7 GitHub Git repository as a submodule.
- Enable SO_LINGER option for LwIP
- Update fs-generic cco to 2.1.0
- Update fs-fatfs cco to 2.1.0
- Update fs-pack to 6.0.3
- Replace pool-fixed cco with microej-pool cco 0.1.0
- Update ui-pack to 13.4.1
- Update imageio-webp tool to 1.0.2
- Update microui cco to 2.0.1
- Update libwebp cco to 1.0.2
- Update MicroEJ architecture to 7.18.1
- Disable list file generation for STM32Cube release build
- Update drawing-dma2d cco to 3.1.0

----------------------
 [1.5.0] - 2022-06-24
----------------------

Added
=====

- Add support for WebP image format

----------------------
 [1.4.0] - 2022-06-17
----------------------

Added
=====

- LLMJVM FreeRTOS 1.1.0

Changed
=======

- Increase FreeRTOS heap size
- Trace the DHCP requests
- Change license terms to MicroEJ Corp. BSD-style license.

Fixed
=====

- Added non-inclusion COREMARK and IPERF guarding blocks for OS specific HAL_Delay implementation.
- Overriden putchar function is not thread safe.
- OS specific HAL_Delay implementation.
- Add ITCM write protection.
- Enabled MPU instruction access in SDRAM.
- Dropped specific MPU region for display implementation and used SDRAM cacheable.
- Linker file correction to be able to build with various optimization levels.
- Linker file move some sections to allow the build of the firmware.
- Clear cache in the sd card driver.
- Some paths are not valid in case-sensitive environment
- Corrected initialization for external SDRAM for the display buffer - eliminated LTDC fifo underrun errors.

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
