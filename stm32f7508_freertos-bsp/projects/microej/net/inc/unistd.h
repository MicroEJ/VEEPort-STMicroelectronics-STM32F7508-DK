/*
 * C
 *
 * Copyright 2024MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef __UNISTD_H
#define __UNISTD_H


// Not implemented: LLNET_NETWORKADDRESS implementation will use a default hostname.
#define gethostname(name, len)	(-1)

#endif // __UNISTD_H
