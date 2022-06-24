  @verbatim
  ******************************************************************************
  * 
  *  Copyright 2020 MicroEJ Corp. All rights reserved.
  *  This library is provided in source code for use, modification and test, subject to license terms.
  *  Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
  *
  ******************************************************************************
  @endverbatim
### 08-October-2020 ###
- add FreeRTOS patch for System View from https://forum.segger.com/index.php/Thread/6158-SOLVED-SystemView-Kernelpatch-for-FreeRTOS-10-2-0
and extend it also for IAR port as CM7 patch is not included in the System View 3.12 distribution.

### 21-July-2020 ###
- add support for xPortIsInsideInterrupt() to .\portable\IAR\ARM_CM7\r0p1\portmacro.h.