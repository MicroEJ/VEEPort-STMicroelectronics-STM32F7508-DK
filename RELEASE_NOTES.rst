..
    Copyright 2020-2023 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

.. |BOARD_NAME| replace:: STM32F7508-DK
.. |VEEPORT_VER| replace:: 2.0.0
.. |MANUFACTURER| replace:: STMicroelectronics
.. |VEEPORT| replace:: VEE Port
.. |STM_IDE.PRINTABLENAME| replace:: STM32CubeIDE
.. |IAR_IDE.PRINTABLENAME| replace:: IAR Embedded Workbench
.. |SYSTEMVIEW_PROVIDER| replace:: SEGGER Microcontroller
.. |SYSTEMVIEW_VERSION| replace:: V2.52a

.. _release-notes:

========================================================
|VEEPORT| Release Notes for |MANUFACTURER| |BOARD_NAME|
========================================================

Description
===========

This is the release notes of the |VEEPORT| for |BOARD_NAME|.

Versions
========

|VEEPORT|
--------

|VEEPORT_VER|

Dependencies
------------

This |VEEPORT| contains the following dependencies:

- MicroEJ GCC specific packs:

.. list-table::

  * - flopi7G26 (Architecture)
    - 8.0.0
  * - flopi7G26UI (User Interface)
    - 13.4.1

- MicroEJ IAR specific packs:

.. list-table::

  * - flopi7I36 (Architecture)
    - 8.0.0
  * - flopi7I36UI (User Interface)
    - 13.4.1

- MicroEJ generic packs:

.. list-table::

  * - NET (Network & Secure Socket Layer)
    - 9.4.2
  * - HAL (Hardware Abstraction Layer)
    - 2.0.1
  * - FS (File System)
    - 6.0.3
  * - Device-pack
    - 1.1.1

Board Support Package
---------------------

- BSP provider: |MANUFACTURER|
- BSP version: v1.17.0
- The `StmCubeIDE v1.9.0` contains mbedTLS v2.16.2 and this version is not fully functional with
   `IAR Embedded Workbench IDE v8.50.5`, therefore the `CubeF7-MicroEJ.patch` file contains
   `mbedTLS v2.16.7` which fixes the issues, LWIP handle MicroEJ specific errno for integration,
   FreeRTOS extend IAR port as CM7 patch and add support for xPortIsInsideInterrupt()

Please refer to the |MANUFACTURER| GitHub git repository
available `here
<https://github.com/STMicroelectronics/STM32CubeF7/tree/v1.17.0>`__.

Third Party Software
--------------------

Third party softwares used in BSP can be found `here
<https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef7.html#overview>`__. Here
is a list of the most important ones:

.. list-table::
   :widths: 3 3 3

   * - RTOS 
     - FreeRTOS
     - 10.2.1
   * - TCP/IP stack 
     - LwIP
     - 2.1.2
   * - Cryptographic stack 
     - Mbed TLS
     - 2.16.7
   * - File System stack 
     - FatFS
     - R0.12c

Features
========

Graphical User Interface
------------------------

This |VEEPORT| features a graphical user interface. It includes a display,
a touch panel, a user button, a user LED and a runtime PNG and WebP decoder.

Display
~~~~~~~

The display module drives a 480 x 272 TFT display. The pixel format is 16 bits-per-pixel: 5 bits for red color 
component, 6 bits for green color component and 5 bits for blue color component. The display device is clocked 
at 60Hz and the MicroEJ application drawings are synchronized on this display tick.

.. note::

   The display stack implementation uses the double-buffering mode: the current MicroEJ application rendering is
   performed in a background buffer (called back buffer) and another buffer is used by the TFT display to refresh
   itself (called frame buffer). When the drawing is done, a copy of pixels data from the back buffer to the 
   frame buffer is performed (stack ``copy``). In order to avoid flickering, this copy is synchronized on
   display refresh tick.

   The back buffer is located in external RAM (SDRAM). The size depends on the
   display size in pixels and on the number of bits-per-pixel (BPP):

   ``bufferSize = width * height * bpp / 8;``, where:

   -  ``width`` is the display width in pixels: 480

   -  ``height`` is the display width in pixels: 272

   -  ``bpp`` is the number of bits-per-pixel: 16

   The buffers size is ``2 * 261120 = 510 KB``.

MicroUI, the module defining the low-level UI framework, requires a RAM buffer to store the dynamic images data. This buffer is called the _image heap_. A dynamic image is an image decoded
at runtime (PNG image) or an image created by the MicroEJ application using ``Image.create(width, height)``.
This buffer is located in SDRAM and the size is configurable from the application launcher,
``Configuration -> Libraries -> MicroUI -> Images heap size (in bytes)``.

.. note::

   An image buffer size follows the same rule as the LCD buffer (see before).
   The display module uses the STM32F7 hardware acceleration to perform some drawings: the ChromArt (DMA2D).
   The DMA2D renders all ``fill rectangles`` (``GraphicsContext.fillRectangle()``) and performs
   the drawings of all images.

Inputs
~~~~~~

Touch panel: All touch panel events are sent to the MicroEJ application thanks to a ``Pointer`` event generator.

.. note::

   A touch ``press`` event is detected under interrupt. It wakes up a dedicated OS task. This task is used to communicate
   with the touch (I2C communication). For all next ``drag`` events, the touch task runs in polling mode. 
   When a ``release`` is detected, the touch task goes to sleep, waiting for a touch interrupt.

User button: The user button is reserved to the multi applications feature: it allows the user to force to kill a sandboxed application.

LEDs
~~~~

User LED: The board provides access to ``LED1`` (green LED) which is available on digital pin ``D13`` from Arduino UNO connector ``CN7 pin 6``.

Network
-------

This |VEEPORT| features a network interface with Ethernet as an underlying
hardware media. A limited number of 8 sockets could be used for TCP
connections, 8 for TCP listening (server) connections and 8 for UDP
connections. A DHCP client could be activated to retrieve IP address.
All DNS requests could be handled by a MicroEJ software implementation
or a native one.

.. note::

   This implementation uses a heap in SDRAM and a Best-Fit
   memory allocator for all its memory allocation. The TCP MSS is 1460
   bytes.

   The network portage use a BSD (Berkley Software Distribution) API
   with select feature. A mechanism named async_select, with a
   dedicated task, is used to request non blocking operations and wait
   for completion or timeout.

   The DHCP client is handled by LwIP and the DNS features use a MicroEJ
   software implementation.

SSL
---

|VEEPORT| features a network secure interface. Available secured
protocols are SSL 3.0, TLS 1.0, TLS 1.1, TLS 1.2. Supported keys and
certificates formats are PKCS#5 and PKCS#12, PEM or DER encoded.

.. note::

   mbedTLS uses a heap in SDRAM to store certificates.

File System
-----------

|VEEPORT| features a file system interface. An SD card is
used for the storage (previously formated to a FAT32 file system). Up
to 2 files can be opened simultaneously.

UART Connector
--------------

|VEEPORT| provides one serial connection (ECOM COMM) on UART6 port. UART6 pins are (RTS/CTS mode is not used):

-  TX: PC6; available on connector CN4 D1

-  RX: PC7; available on connector CN4 D0

.. note::

   This implementation uses interrupts
   and relies on the MicroEJ ``LLCOMM_BUFFERED_CONNECTION`` API. This API is FIFO oriented. It requires two
   distincts software buffers for reception and transmission: reception buffer uses 1024 bytes and transmission buffer
   uses 5 bytes. These buffers are statically allocated in internal RAM.

HAL
---
  
|VEEPORT| provides several GPIOs programmable via the HAL foundation library. All GPIOs are available on
ARDUINO connectors (CN4 to CN7). Digital pins are implemented by a GPIO access.

Analog input pins (ADC) are driven by ADC channels of ADC 3 and analog output pins (DAC) drive PWM channels of timers 1, 3, 5 and 12.
  
Each GPIO port / pin value is accessible using either:

-  The global MCU designation: all pins of all ports are grouped under only one virtual 
    port (port 0) and have consecutive values: PA0 has the ID 0, PA1, the ID 1, PA15 the ID 15, PB0 
    the ID 16 and so on. For instance pin **PF6** is accessible by ``(0, 86)``.
    This designation is useful to target all MCU pins using only one virtual port.

-  The standard MCU designation: PortA has the ID 1, PortB the ID 2 etc. Each pin of each
    port is a value between 0 (PortN-0) to 15 (PortN-15).
    For instance pin **PF6** is accessible by ``(6, 6)``. This designation 
    is useful to target a specific MCU pin.

-  The virtual board connectors designation. Board has 2 virtual connectors: ARDUINO digital port and ARDUINO
    analog port, with respectively these IDs 30 and 31. For instance pin **PF6** 
    is accessible on connector ARDUINO analog, pin A4: ``(31, 5)``. This designation is useful to target a 
    virtual connector pin without knowing which MCU pin it is and on which physical connector pin is connected.

-  The physical board connectors designation. Board has 3 connectors: CN4, CN5 and CN7 (CN6 is not connected
    to the MCU), with respectively these IDs: 64, 65 and 67. For instance pin **PF6** 
    is accessible on connector CN5, pin6: ``(65, 6)``. This designation is useful to target a 
    physical connector pin without knowing which MCU pin it is.

The following table summarizes the exhaustive list of GPIOs ports accessible from HAL library, and the ranges of pin IDs:

+--------------------------------------+-------------+------------+
|               Port name              | HAL port ID | Pins range |
+======================================+=============+============+
| Global MCU virtual port              |      0      |  0 to 143  |
+--------------------------------------+-------------+------------+
| MCU port A                           |      1      |  0 to 15   |
+--------------------------------------+-------------+------------+
| MCU port B                           |      2      |  0 to 15   |
+--------------------------------------+-------------+------------+
| MCU port F                           |      6      |  0 to 15   |
+--------------------------------------+-------------+------------+
| MCU port G                           |      7      |  0 to 15   |
+--------------------------------------+-------------+------------+
| MCU port H                           |      8      |  0 to 15   |
+--------------------------------------+-------------+------------+
| MCU port I                           |      9      |  0 to 15   |
+--------------------------------------+-------------+------------+
| Board virtual port "ARDUINO digital" |      30     |  0 to 15   |
+--------------------------------------+-------------+------------+
| Board virtual port "ARDUINO analog"  |      31     |  0 to 7    |
+--------------------------------------+-------------+------------+
| Board physical port "CN4"            |      64     |  1 to 8    |
+--------------------------------------+-------------+------------+
| Board physical port "CN5"            |      65     |  1 to 6    |
+--------------------------------------+-------------+------------+
| Board physical port "CN7"            |      67     |  1 to 10   |
+--------------------------------------+-------------+------------+

The following table shows the exhaustive list of GPIOs connected to the HAL library, their IDs according the ports IDs and pins IDs (see before):

+------------+----------------------+--------------+------------------------+-------------------------+
| Port / Pin | MCU virtual port (1) | MCU port (2) | Board virtual port (3) | Board physical port (4) |
+============+======================+==============+========================+=========================+
| PA0        |         0, 0         |      1, 0    |          31, 0         |          65, 1          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PA8        |         0, 8         |      1, 8    |          30, 10        |          67, 3          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PA15       |         0, 15        |      1, 15   |          30, 9         |          67, 2          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PA4        |         0, 20        |      2, 4    |          30, 3         |          64, 4          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PA14       |         0, 30        |      2, 14   |          30, 12        |          67, 5          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PB15       |         0, 31        |      2, 15   |          30, 11        |          67, 4          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PF6        |         0, 86        |      6, 6    |          31, 5         |          65, 6          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PF7        |         0, 87        |      6, 7    |          31, 4         |          65, 5          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PF8        |         0, 88        |      6, 8    |          31, 3         |          65, 4          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PF9        |         0, 89        |      6, 9    |          31, 2         |          65, 3          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PF10       |         0, 90        |      2, 10   |          31, 1         |          65, 2          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PG6        |         0, 102       |      7, 6    |          30, 2         |          64, 3          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PG7        |         0, 103       |      7, 7    |          30, 4         |          65, 5          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PH6        |         0, 118       |      8, 6    |          30, 6         |          64, 7          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PI0        |         0, 128       |      9, 0    |          30, 5         |          64, 6          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PI1        |         0, 129       |      9, 1    |          30, 13        |          67, 6          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PI2        |         0, 130       |      9, 2    |          30, 8         |          67, 1          |
+------------+----------------------+--------------+------------------------+-------------------------+
| PI3        |         0, 131       |      9, 3    |          30, 7         |          64, 8          |
+------------+----------------------+--------------+------------------------+-------------------------+

The following table lists the hardware analog devices (ADC / DAC channels) used by HAL analog pins:

+------------+---------------+---------------------+
| Port / Pin | ADC 3 channel | PWM timer / channel |
+============+===============+=====================+
| PA0        |        0      |         n/a         |
+------------+---------------+---------------------+
| PA8        |      n/a      |        1 / 1        |
+------------+---------------+---------------------+
| PB4        |       n/a     |        3 / 1        |
+------------+---------------+---------------------+
| PB15       |       n/a     |       12 / 2        |
+------------+---------------+---------------------+
| PF6        |        4      |         n/a         |
+------------+---------------+---------------------+
| PF7        |        5      |         n/a         |
+------------+---------------+---------------------+
| PF8        |        6      |         n/a         |
+------------+---------------+---------------------+
| PF9        |        7      |         n/a         |
+------------+---------------+---------------------+
| PF10       |        8      |         n/a         |
+------------+---------------+---------------------+
| PH6        |       n/a     |       12 / 1        |
+------------+---------------+---------------------+
| PI0        |       n/a     |        5 / 4        |
+------------+---------------+---------------------+

Watchdog
--------

|VEEPORT| features a watchdog. The independent watchdog peripheral detects and solves malfunctions due
to software failures and triggers a system reset when the counter reaches a given timeout value. The independent
watchdog is clocked by its own dedicated low-speed clock (LSI) and thus stays active even if the main clock
fails.

Watchdog main features:

- free-running downcounter

- clocked from an independent RC oscillator

- conditional reset – if watchdog activated, a reset signal is generated when the downcounter value becomes
  lower than 0x000

.. note::

   The watchdog starts by default when the application begins with the watchdog maximum delay enabled (~32 seconds).

.. note::

   For 32KHz (LSI) the minimum timeout value is ~125µs and the maximum timeout value is ~32.7s.

The watchdog can be disabled by the user before running the application. To disable the watchdog, the user has to set
``watchdog.enabled=false`` in the application properties file.

The watchdog period can be customized by the user before running the application. To set a custom watchdog period, the user has to set
``watchdog.period=xxx`` in the application properties file, where ``xxx`` is the period in milliseconds. The minimum period is 1ms and the
maximum one is determined at runtime, based on precise LSI frequency (~32700ms). If the user supplies a period too big, a warning is raised
on the console when the application starts and the watchdog timer is not started.

.. note::

    Once running, the watchdog cannot be stopped.

System View
-----------

This |VEEPORT| supports System View. For more information about System View, please visit `<https://www.segger.com/products/development-tools/systemview/>`_

The following setup is needed to have System View functional:

- Enable `ENABLE_SYSTEM_VIEW` compile switch at project level, either in |STM_IDE.PRINTABLENAME| or |IAR_IDE.PRINTABLENAME|, depending on the user's choice

  - with `STM32CubeIDE` ``open the workspace .cproject -> go to the application  -> right click -> Properties -> C/C++Build -> Settings -> MCU GCC Compiler -> Preprocessor -> add the define``

  - with `IAR Embedded Workbench IDE` ``open the workspace .eww -> go to the application -> right click -> option -> C/C++Compiler -> Preprocessor ->Defined symbols -> add the define``

- Re-build the BSP project

- Download System View |SYSTEMVIEW_VERSION| PC application `<https://www.segger.com/downloads/systemview/>`_

- Build and flash an application binary on the board

- Follow the instructions provided by |SYSTEMVIEW_PROVIDER| `<https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/>`_
  to re-flash the ST-LINK on board with a J-Link firmware, making it J-Link compatible

- Open System View PC application

- Go to ``Target > Recorder Configuration``

- Select ``J-Link`` as SystemView Recorder

- Click ``Ok``

- Select the following Recorder Configuration:

  - ``J-Link Connection = USB``

  - ``Target Connection = STM32F750N8``

  - ``Target Interface = SWD``

  - ``Interface Speed (kHz) = 4000``

  - ``RTT Control Block Detection = Auto``

- Click ``Ok``

- Select ``Target > Start Recording``

.. note::

  To re-flash a new binary on the board, the user needs to:
    - Follow the instructions provided by |SYSTEMVIEW_PROVIDER| `<https://www.segger.com/products/debug-probes/j-link/models/other-j-links/st-link-on-board/>`_ to restore ST-LINK on a board with J-Link firmware
    - Re-flash the `ST-LINK` on board with a `ST-LINK firmware`. For this, download the latest version of `STSW-LINK007` `<https://www.st.com/en/development-tools/stsw-link007.html>`_, unarchive zip and run `ST-LinkUpgrade.exe` ``Device Connect -> Yes`` to Upgrade Firmware


.. note::

  Depending on the application, OVERFLOW events can be seen in System View. To mitigate this problem, the default `SEGGER_SYSVIEW_RTT_BUFFER_SIZE` was increased
  from the default 1kB to a more appropriate size of 4kB. Still, if OVERFLOW events are still visible, the user can further increase this configuration found in
  ``/stm32f7508_freertos-bsp/thirdparty/systemview/inc/SEGGER_SYSVIEW_configuration.h``.

.. note::

  If the RTT Control Block Address is not detected by System View:

- Run first on UART with `ENABLE_SYSTEM_VIEW` enabled to see at boot `SEGGER_RTT block address`

- Open System View PC application

- Go to ``Target > Recorder Configuration``

- Select ``J-Link`` as SystemView Recorder

- Click ``Ok``

- Select the following Recorder Configuration:

  - ``J-Link Connection = USB``

  - ``Target Connection = STM32F750N8``

  - ``Target Interface = SWD``

  - ``Interface Speed (kHz) = 4000``

  - ``RTT Control Block Detection = Search Range`` add `SEGGER_RTT block address` that you see at boot on UART, i.e. 0x20000000 2048

- Click ``Ok``

- Select ``Target > Start Recording``

.. note::

    To have a fully functional System View trace, a FreeRTOS patch needed to be applied. Neither |SYSTEMVIEW_PROVIDER|, nor |MANUFACTURER|
    provide such patch for |BOARD_NAME|, therefore a custom patch was applied, see
    ``/stm32f7508_freertos-bsp/sdk/STM32CubeF7/Middlewares/Third_Party/FreeRTOS/Source/microej_readme.txt`` for more details. Also, the custom patch removes the tracing of the SysTick interrupt, as
    with an interrupt period of 1ms, tracing this interrupt was triggering OVERFLOW events without bringing much benefit for the user to see a periodic interrupt
    in the trace logs.

Known issues/limitations
========================

- Implementation of ``snprintf`` does not support the ``%llx`` format
 
|VEEPORT| Memory Layout
======================

Memory Sections
---------------

Each memory section is discribed in the GCC linker file available
`here
<stm32f7508_freertos-bsp/projects/microej/SW4STM32/STM32F750N8Hx_FLASH.ld>`__ 
and in the IAR linker file available
`here
<stm32f7508_freertos-bsp/projects/microej/EWARM/STM32F750N8Hx_FLASH.icf>`__


Memory Layout
-------------

.. list-table::
   :header-rows: 1
   
   * - Section Content
     - Section Source
     - Section Destination
     - Memory Type
   * - Startup code
     - ``-``
     - ``.text_flash``
     - FLASH
   * - MicroEJ Application static
     - ``.bss.soar``
     - ``.bss``
     - SRAM
   * - MicroEJ Application threads stack blocks 
     - ``.bss.vm.stacks.java``
     - ``.dtcm``
     - DTCM
   * - MicroEJ Core Engine internal heap 
     - ``ICETEA_HEAP``
     - ``.dtcm``
     - DTCM
   * - MicroEJ Application heap 
     - ``_java_heap``
     - ``.sdram``
     - SDRAM
   * - MicroEJ Application Immortal Heap 
     - ``_java_immortals``
     - ``.sdram``
     - SDRAM
   * - MicroEJ Application resources 
     - ``.rodata.resources``
     - ``.rodata_qspi``
     - QSPI
   * - MicroEJ Application and Library code 
     - ``.text.soar``
     - ``.text_qspi``
     - QSPI
   * - Display stack
     - ``.DisplayMem``
     - ``._display_stack``
     - SDRAM
   * - Network Heap
     - ``.NetworkHeap``
     - ``._network_heap``
     - SDRAM
   * - MicroUI images heap
     - ``.bss.microui.display.imagesHeap``
     - ``.sdram``
     - SDRAM
   * - KF heap
     - ``.KfHeap``
     - ``._kf_heap``
     - SDRAM

Information on MicroEJ memory sections can be found `here
<https://docs.microej.com/en/latest/ApplicationDeveloperGuide/linker.html#link>`__.

Please also refer to the MicroEJ docs website page available `here
<https://docs.microej.com/en/latest/VEEPortingGuide/coreEngine.html#link>`__
for more details.
