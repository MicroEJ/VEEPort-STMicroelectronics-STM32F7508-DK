
  @verbatim
  ******************************************************************************
  *  
  *  Copyright 2020 MicroEJ Corp. All rights reserved.
  *  This library is provided in source code for use, modification and test, subject to license terms.
  *  Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
  *
  ******************************************************************************
  @endverbatim
### 28-May-2020 ###
========================
  + Handle MicroEJ specific errno
     - sys_arch.c: protect the errno global variable declaration in case it was defined via lwipopts.h inclusion.
     - cc.h: remove the LWIP_RAND global macro declaration as it was defined via lwipopts.h inclusion (cannot be protected due to wrong inclusion order in LwIP).
     - cc.h: protect the LWIP_PROVIDE_ERRNO global macro declaration in case it was defined via lwipopts.h inclusion.

 * <h3><center>&copy; COPYRIGHT MicroEJ</center></h3>
 */
