/*
 * C
 *
 * Copyright 2020-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef LLECOM_NETWORK_H
#define LLECOM_NETWORK_H

/**
 * @file
 * @brief ECOM-NETWORK exported interface.
 * @author MicroEJ Developer Team
 * @version 2.3.1
 * @date 13 February 2023
 */

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief High level interface used for generic ecom-network module initialization.
 * It initializes internal components, like the worker and the helper.
 */
void LLECOM_NETWORK_initialize(void);

#ifdef __cplusplus
	}
#endif

#endif /* LLECOM_NETWORK_H */
