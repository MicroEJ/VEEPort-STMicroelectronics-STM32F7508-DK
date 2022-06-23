# Overview

This platform use a Multi-BSP project GCC or IAR, so we need one bsp.properties per BSP project (`bsp_gcc.properties` and `bsp_iar.properties`).

In the module.ant file we choose the right bsp.properties file depending of the value of `xpf.toolchain.name` property.

---
_Copyright 2022 MicroEJ Corp. All rights reserved._
_Use of this source code is governed by a BSD-style license that can be found with this software._