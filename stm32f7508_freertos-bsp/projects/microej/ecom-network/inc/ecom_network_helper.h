/*
 * C
 *
 * Copyright 2020-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef ECOM_NETWORK_HELPER_H
#define ECOM_NETWORK_HELPER_H

/**
 * @file
 * @brief ECOM-NETWORK helper implementation.
 * @author MicroEJ Developer Team
 * @version 2.3.1
 * @date 13 February 2023
 */

#include "ecom_network_configuration.h"
#include "microej_async_worker.h"

#ifdef __cplusplus
	extern "C" {
#endif

/*
 * IMPORTANT NOTICES
 *
 * This file lists the data structures used by all the LLECOM_NETWORK_* functions.
 * If you add a new function and its corresponding data structure, do not forget to add
 * the data structure to the ECOM_NETWORK_worker_param_t union.
 *
 * Some structures duplicate the definition of fields specified in generic structures instead of
 * referencing the generic structures (e.g. the <code>path</code> and <code>result</code> fields).
 * This decision has been made to simplify usage and readability at the expense of maintainability.
 * Be careful when modifying the fields of the structures or when defining new structures.
 * See the comment of each structure to identify the ones that are affected by these constraints.
 */

/**
 * @brief Data structure for enable/disable and start/stop operations.
 *
 * This structure is used by <code>LLECOM_NETWORK_IMPL_enable</code>, <code>LLECOM_NETWORK_IMPL_disable</code>,
 * <code>LLECOM_NETWORK_IMPL_isEnabled</code>, <code>LLECOM_NETWORK_IMPL_start</code>,
 * <code>LLECOM_NETWORK_IMPL_stop</code>, <code>LLECOM_NETWORK_IMPL_isStarted</code>,
 * <code>LLECOM_NETWORK_IMPL_getDNSCount</code>, <code>LLECOM_NETWORK_IMPL_isDNSStatic</code>,
 * <code>LLECOM_NETWORK_IMPL_isStatic</code>, <code>LLECOM_NETWORK_IMPL_startConfiguration</code>,
 * <code>LLECOM_NETWORK_IMPL_endConfiguration</code> and <code>LLECOM_NETWORK_IMPL_setDefault</code>.
 *
 * Fields defined in this structure correspond to the parameters of these functions and
 * <code>result</code> field corresponds to the value returned by them.
 */
typedef struct {
	int8_t* netifName; /*!< [IN] The buffer that store the interface name. */
	int32_t netifNameOffset; /*!< [IN] The offset where the interface name is stored. */
	int32_t netifNameLength; /*!< [IN] The interface name buffer length. */
	int8_t getResult; /*!< [IN] false to post the request, true to get the result */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
	uint8_t netifBuffer[NETIF_NAME_SIZE]; /*!< Internal buffer. Content must not be modified. */
} ECOM_NETWORK_netif_context_t;

/**
 * @brief Data structure for set/get DNS operations.
 *
 * This structure is used by <code>LLECOM_NETWORK_IMPL_setDNS</code>
 * and <code>LLECOM_NETWORK_IMPL_getDNS</code>.
 *
 * Fields defined in this structure correspond to the parameters of these functions and
 * <code>result</code> field corresponds to the value returned by them.
 */
typedef struct {
	int8_t* netifName; /*!< [IN] The buffer that store the interface name. */
	int32_t netifNameOffset; /*!< [IN] The offset where the interface name is stored. */
	int32_t netifNameLength; /*!< [IN] The interface name buffer length. */
	int8_t* address; /*!< [IN/OUT] The buffer that store IP address to be set (input) or the buffer where the address will be get (output). */
	int32_t addressOffset; /*!< [IN/OUT] The offset where the IP address to be set is stored (input) or the offset where the address will be get (output). */
	int32_t addressLength; /*!< [IN/OUT] The IP address buffer length (input) or the address buffer length (output). */
	int32_t index; /*!< [IN/OUT] The index of the DNS (input) or the index of the DNS server (from 0 to {@link #getDNSCount()} -1) (output). */
	int8_t getResult; /*!< [IN] false to post the request, true to get the result */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
	uint8_t netifBuffer[NETIF_NAME_SIZE]; /*!< Internal buffer. Content must not be modified. */
	uint8_t addrBuffer[IP_ADDR_SIZE]; /*!< Internal buffer. Content must not be modified. */
} ECOM_NETWORK_dns_context_t;

/**
 * @brief Data structure for get Gateway or Netmask operations.
 *
 * This structure is used by <code>LLECOM_NETWORK_IMPL_getGateway</code>,
 * <code>LLECOM_NETWORK_IMPL_setGateway</code>, <code>LLECOM_NETWORK_IMPL_setIP</code>, 
 * <code>LLECOM_NETWORK_IMPL_getNetmask</code> and <code>LLECOM_NETWORK_IMPL_setNetmask</code>.
 *
 * Fields defined in this structure correspond to the parameters of these functions and
 * <code>result</code> field corresponds to the value returned by them.
 */
typedef struct {
	int8_t* netifName; /*!< [IN] The buffer that store the interface name. */
	int32_t netifNameOffset; /*!< [IN] The offset where the interface name is stored. */
	int32_t netifNameLength; /*!< [IN] The interface name buffer length. */
	int8_t* address; /*!< [IN/OUT] The buffer that store IP address to be set (input) or the buffer where the address will be get (output). */
	int32_t addressOffset; /*!< [IN/OUT] The offset where the IP address to be set is stored (input) or the offset where the address will be get (output). */
	int32_t addressLength; /*!< [IN/OUT] The IP address buffer length (input) or the address buffer length (output). */
	int8_t getResult; /*!< [IN] false to post the request, true to get the result */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
	uint8_t netifBuffer[NETIF_NAME_SIZE]; /*!< Internal buffer. Content must not be modified. */
	uint8_t addrBuffer[IP_ADDR_SIZE]; /*!< Internal buffer. Content must not be modified. */
} ECOM_NETWORK_ip_context_t;

/**
 * @brief Data structure for use DHCP and static DNS operations.
 *
 * This structure is used by <code>LLECOM_NETWORK_IMPL_useDHCP</code>
 * and <code>LLECOM_NETWORK_IMPL_useStaticDNS</code>.
 *
 * Fields defined in this structure correspond to the parameters of these functions and
 * <code>result</code> field corresponds to the value returned by them.
 */
typedef struct {
	int8_t* netifName; /*!< [IN] The buffer that store the interface name. */
	int32_t netifNameOffset; /*!< [IN] The offset where the interface name is stored. */
	int32_t netifNameLength; /*!< [IN] The interface name buffer length. */
	int8_t use; /*!< [IN] True to use DHCP / static DNS, false otherwise. */
	int8_t getResult; /*!< [IN] false to post the request, true to get the result */
	int32_t result; /*!< [OUT] Result of the operation. */
	int32_t error_code; /*!< [OUT] Error code returned in case of error. */
	char* error_message; /*!< [OUT] Error message related to the error code. */
	uint8_t netifBuffer[NETIF_NAME_SIZE]; /*!< Internal buffer. Content must not be modified. */
} ECOM_NETWORK_ip_config_t;

/**
 * @union ECOM_NETWORK_worker_param_t
 */
typedef union {
	ECOM_NETWORK_netif_context_t start_context;
	ECOM_NETWORK_dns_context_t set_get_DNS;
	ECOM_NETWORK_ip_context_t ip_context;
	ECOM_NETWORK_ip_config_t use_dhcp_dns;
} ECOM_NETWORK_worker_param_t;

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_enable</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_enable_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_disable</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_disable_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_isEnabled</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_isEnabled_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_start</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_start_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_stop</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_stop_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_isStarted</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_isStarted_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_getDNSCount</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_getDNSCount_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_getDNS</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_getDNS_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_getGateway</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_getGateway_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_getNetmask</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_getNetmask_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_isDNSStatic</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_isDNSStatic_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_isStatic</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_isStatic_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_startConfiguration</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_startConfiguration_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_endConfiguration</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_endConfiguration_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_setDNS</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_setDNS_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_setGateway</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_setGateway_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_setIP</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_setIP_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_setNetmask</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_setNetmask_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_useDHCP</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_useDHCP_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_useStaticDNS</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_useStaticDNS_action(MICROEJ_ASYNC_WORKER_job_t* job);

/**
 * @brief Action requested by <code>LLECOM_NETWORK_IMPL_setDefault</code> and executed asynchronously via async_worker.
 *
 * @param[in] job the context of the job, containing input/output parameters
 */
void LLECOM_NETWORK_IMPL_setDefault_action(MICROEJ_ASYNC_WORKER_job_t* job);

#ifdef __cplusplus
	}
#endif

#endif /* ECOM_NETWORK_HELPER_H */
