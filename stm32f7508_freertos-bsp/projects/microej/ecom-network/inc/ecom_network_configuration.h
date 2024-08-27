/*
 * C
 *
 * Copyright 2020-2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef  ECOM_NETWORK_CONFIGURATION_H
#define  ECOM_NETWORK_CONFIGURATION_H

/**
 * @file
 * @brief LLECOM_NETWORK configuration.
 * @author MicroEJ Developer Team
 * @version 2.2.1
 * @date 18 March 2021
 */

#include "microej_async_worker.h"

#ifdef __cplusplus
	extern "C" {
#endif

//#error "This header must be customized with platform specific configuration. Remove this #error when done. This file is not modified when a new version of the CCO is installed."

/**
 * @brief Compatibility sanity check value.
 * This define value is checked in the implementation to validate that the version of this configuration
 * is compatible with the implementation.
 *
 * This value must not be changed by the user of the CCO.
 * This value must be incremented by the implementor of the CCO when a configuration define is added, deleted or modified.
 */
#define ECOM_NETWORK_CONFIGURATION_VERSION (2)

/**
 * @brief Use this macro to define the initialization function of the network stack.
 * Called from LLECOM_NETWORK_initialize().
 * By default this macro does nothing.
 */
#define llecom_network_init() ((void)0)

/**
 * @brief Set this define to use a custom worker to handle ECOM-NETWORK asynchronous jobs.
 */
//#define ECOM_NETWORK_CUSTOM_WORKER

/**
 * @brief Define ECOM-NETWORK custom or module specific worker.
 */
#ifdef ECOM_NETWORK_CUSTOM_WORKER
extern MICROEJ_ASYNC_WORKER_handle_t my_custom_worker;
#define ecom_network_worker my_custom_worker
#else
extern MICROEJ_ASYNC_WORKER_handle_t ecom_network_worker;
#endif

/**
 * @brief Number of workers dedicated to the ECOM-NETWORK in async_worker.
 */
#define ECOM_NETWORK_WORKER_JOB_COUNT (4)

/**
 * @brief Size of the waiting list for ECOM-NETWORK jobs in async_worker.
 */
#define ECOM_NETWORK_WAITING_LIST_SIZE (16)

/**
 * @brief Size of the ECOM-NETWORK stack in bytes.
 */
#define ECOM_NETWORK_WORKER_STACK_SIZE (1024*2)

/**
 * @brief ECOM-NETWORK worker stack size must be calibrated, unless using a custom worker defined in another module.
 */
#if !defined(ECOM_NETWORK_CUSTOM_WORKER) && !defined(ECOM_NETWORK_WORKER_STACK_SIZE)
#error "ECOM_NETWORK_WORKER_STACK_SIZE not declared. Please uncomment the line above to enable macro declaration and put the right value according to the stack size calibration done for your environment"
#endif // ECOM_NETWORK_WORKER_STACK_SIZE

/**
 * @brief Priority of the ECOM-NETWORK workers.
 */
#define ECOM_NETWORK_WORKER_PRIORITY (6)

/**
 * @brief Generic error returned by native functions.
 */
#define ECOM_NETWORK_ERROR (-1)

/**
 * @brief Size of a generic network interface in bytes (255 bytes).
 */
#define NETIF_NAME_SIZE (255)

/**
 * @brief Size of an IP address in bytes (16 bytes).
 */
#define IP_ADDR_SIZE (16)

/**
 * @brief Set this define to print debug traces.
 */
//#define LLECOM_NETWORK_DEBUG

#ifdef __cplusplus
	}
#endif

#endif // ECOM_NETWORK_CONFIGURATION_H
