/*
 * C
 *
 * Copyright 2020-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief LLECOM_NETWORK implementation with async worker.
 * @author MicroEJ Developer Team
 * @version 2.3.1
 * @date 13 February 2023
 */

/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>
#include <stddef.h>
#include "sni.h"
#include "LLECOM_NETWORK_impl.h"
#include "ecom_network_configuration.h"
#include "ecom_network_helper.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * Sanity check between the expected version of the configuration and the actual version of
 * the configuration.
 * If an error is raised here, it means that a new version of the CCO has been installed and
 * the configuration ecom_network_configuration.h must be updated based on the one provided
 * by the new CCO version.
 */
#if ECOM_NETWORK_CONFIGURATION_VERSION != 2

	#error "Version of the configuration file ecom_network_configuration.h is not compatible with this implementation."

#endif

#ifndef ECOM_NETWORK_CUSTOM_WORKER
/* Async worker task declaration ---------------------------------------------*/
MICROEJ_ASYNC_WORKER_worker_declare(ecom_network_worker, ECOM_NETWORK_WORKER_JOB_COUNT, ECOM_NETWORK_worker_param_t, ECOM_NETWORK_WAITING_LIST_SIZE);
OSAL_task_stack_declare(ecom_network_worker_stack, ECOM_NETWORK_WORKER_STACK_SIZE);
#endif

static int32_t LLECOM_NETWORK_async_exec_netif_context(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done);
static int32_t LLECOM_NETWORK_async_exec_netif_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult);
static int32_t LLECOM_NETWORK_async_exec_dns_context(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int32_t index, int8_t getResult, bool exec_write, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done);
static int32_t LLECOM_NETWORK_async_exec_get_dns_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int32_t index, int8_t getResult);
static int32_t LLECOM_NETWORK_async_exec_set_dns_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int32_t index, int8_t getResult);
static int32_t LLECOM_NETWORK_async_exec_ip_context(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult, bool exec_write, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done);
static int32_t LLECOM_NETWORK_async_exec_get_ip_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult);
static int32_t LLECOM_NETWORK_async_exec_set_ip_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult);
static int32_t LLECOM_NETWORK_async_exec_ip_config(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t use, int8_t getResult, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done);
static int32_t LLECOM_NETWORK_async_exec_ip_config_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t use, int8_t getResult);

void LLECOM_NETWORK_initialize(void) {
#ifndef ECOM_NETWORK_CUSTOM_WORKER
	MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_initialize(&ecom_network_worker, (uint8_t*)"MicroEJ ECOM-NETWORK", ecom_network_worker_stack, ECOM_NETWORK_WORKER_PRIORITY);
	if (status == MICROEJ_ASYNC_WORKER_INVALID_ARGS) {
		SNI_throwNativeException(status, "Invalid argument for ECOM-NETWORK async worker");
	} else if (status == MICROEJ_ASYNC_WORKER_ERROR) {
		SNI_throwNativeException(status, "Error while initializing ECOM-NETWORK async worker");
	}
#endif

	llecom_network_init();
}

int32_t LLECOM_NETWORK_IMPL_enable(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_enable, LLECOM_NETWORK_IMPL_enable_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_disable(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_disable, LLECOM_NETWORK_IMPL_disable_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_isEnabled(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_isEnabled, LLECOM_NETWORK_IMPL_isEnabled_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_start(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_start, LLECOM_NETWORK_IMPL_start_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_stop(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_stop, LLECOM_NETWORK_IMPL_stop_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_isStarted(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_isStarted, LLECOM_NETWORK_IMPL_isStarted_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_getDNSCount(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_getDNSCount, LLECOM_NETWORK_IMPL_getDNSCount_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_getDNS(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int32_t index, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_dns_context(netifName, netifNameOffset, netifNameLength, address, addressOffset, addressLength, index, getResult, false, (SNI_callback)LLECOM_NETWORK_IMPL_getDNS, LLECOM_NETWORK_IMPL_getDNS_action, (SNI_callback)LLECOM_NETWORK_async_exec_get_dns_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_getGateway(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_ip_context(netifName, netifNameOffset, netifNameLength, address, addressOffset, addressLength, getResult, false, (SNI_callback)LLECOM_NETWORK_IMPL_getGateway, LLECOM_NETWORK_IMPL_getGateway_action, (SNI_callback)LLECOM_NETWORK_async_exec_get_ip_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_getNetmask(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_ip_context(netifName, netifNameOffset, netifNameLength, address, addressOffset, addressLength, getResult, false, (SNI_callback)LLECOM_NETWORK_IMPL_getNetmask, LLECOM_NETWORK_IMPL_getNetmask_action, (SNI_callback)LLECOM_NETWORK_async_exec_get_ip_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_isDNSStatic(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_isDNSStatic, LLECOM_NETWORK_IMPL_isDNSStatic_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_isStatic(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_isStatic, LLECOM_NETWORK_IMPL_isStatic_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_endConfiguration(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_endConfiguration, LLECOM_NETWORK_IMPL_endConfiguration_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_startConfiguration(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_startConfiguration, LLECOM_NETWORK_IMPL_startConfiguration_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_setDNS(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int32_t index, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_dns_context(netifName, netifNameOffset, netifNameLength, address, addressOffset, addressLength, index, getResult, true, (SNI_callback)LLECOM_NETWORK_IMPL_setDNS, LLECOM_NETWORK_IMPL_setDNS_action, (SNI_callback)LLECOM_NETWORK_async_exec_set_dns_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_setGateway(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_ip_context(netifName, netifNameOffset, netifNameLength, address, addressOffset, addressLength, getResult, true, (SNI_callback)LLECOM_NETWORK_IMPL_setGateway, LLECOM_NETWORK_IMPL_setGateway_action, (SNI_callback)LLECOM_NETWORK_async_exec_set_ip_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_setIP(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_ip_context(netifName, netifNameOffset, netifNameLength, address, addressOffset, addressLength, getResult, true, (SNI_callback)LLECOM_NETWORK_IMPL_setIP, LLECOM_NETWORK_IMPL_setIP_action, (SNI_callback)LLECOM_NETWORK_async_exec_set_ip_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_setNetmask(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_ip_context(netifName, netifNameOffset, netifNameLength, address, addressOffset, addressLength, getResult, true, (SNI_callback)LLECOM_NETWORK_IMPL_setNetmask, LLECOM_NETWORK_IMPL_setNetmask_action, (SNI_callback)LLECOM_NETWORK_async_exec_set_ip_context_on_done);
}

int32_t LLECOM_NETWORK_IMPL_useDHCP(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t use, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_ip_config(netifName, netifNameOffset, netifNameLength, use, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_useDHCP, LLECOM_NETWORK_IMPL_useDHCP_action, (SNI_callback)LLECOM_NETWORK_async_exec_ip_config_on_done);
}

int32_t LLECOM_NETWORK_IMPL_useStaticDNS(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t use, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_ip_config(netifName, netifNameOffset, netifNameLength, use, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_useStaticDNS, LLECOM_NETWORK_IMPL_useStaticDNS_action, (SNI_callback)LLECOM_NETWORK_async_exec_ip_config_on_done);
}

int32_t LLECOM_NETWORK_IMPL_setDefault(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	return LLECOM_NETWORK_async_exec_netif_context(netifName, netifNameOffset, netifNameLength, getResult, (SNI_callback)LLECOM_NETWORK_IMPL_setDefault, LLECOM_NETWORK_IMPL_setDefault_action, (SNI_callback)LLECOM_NETWORK_async_exec_netif_context_on_done);
}

/**
 * @brief Prepare and send an execution job to async_worker, called either from
 * <code>LLECOM_NETWORK_IMPL_enable</code>, <code>LLECOM_NETWORK_IMPL_disable</code>,
 * <code>LLECOM_NETWORK_IMPL_isEnabled</code>, <code>LLECOM_NETWORK_IMPL_start</code>,
 * <code>LLECOM_NETWORK_IMPL_stop</code>, <code>LLECOM_NETWORK_IMPL_isStarted</code>,
 * <code>LLECOM_NETWORK_IMPL_getDNSCount</code>, <code>LLECOM_NETWORK_IMPL_isDNSStatic</code>,
 * <code>LLECOM_NETWORK_IMPL_isStatic</code>, <code>LLECOM_NETWORK_IMPL_endConfiguration</code>,
 * <code>LLECOM_NETWORK_IMPL_startConfiguration</code> or <code>LLECOM_NETWORK_IMPL_setDefault</code>.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[in] getResult false to post the request, true to get the result
 * @param[in] retry_function if the current Java thread has been suspended, this function is called when it is resumed.
 * @param[in] action the function to execute asynchronously.
 * @param[in] on_done the <code>SNI_callback</code> called when the job is done.
 *
 * @return <code>SNI_IGNORED_RETURNED_VALUE</code> on success, else a negative error code.
 */
static int32_t LLECOM_NETWORK_async_exec_netif_context(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&ecom_network_worker, retry_function);
	if (job == NULL) {
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return ECOM_NETWORK_ERROR;
	}

	ECOM_NETWORK_netif_context_t* params = (ECOM_NETWORK_netif_context_t*)job->params;

	params->netifNameOffset = netifNameOffset;
	params->netifNameLength = netifNameLength;
	params->getResult = getResult;

	int32_t result = SNI_retrieveArrayElements(netifName, netifNameOffset, netifNameLength, (int8_t*)&params->netifBuffer,
			sizeof(params->netifBuffer), (int8_t**)&params->netifName, (uint32_t*)&params->netifNameLength, true);
	if (result != SNI_OK) {
		SNI_throwNativeIOException(result, "SNI_retrieveArrayElements: Internal error");
	} else {
		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&ecom_network_worker, job, action, on_done);
		if (status == MICROEJ_ASYNC_WORKER_OK) {
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);
	return ECOM_NETWORK_ERROR;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by
 * <code>LLECOM_NETWORK_IMPL_enable</code>, <code>LLECOM_NETWORK_IMPL_disable</code>,
 * <code>LLECOM_NETWORK_IMPL_isEnabled</code>, <code>LLECOM_NETWORK_IMPL_start</code>,
 * <code>LLECOM_NETWORK_IMPL_stop</code>, <code>LLECOM_NETWORK_IMPL_isStarted</code>,
 * <code>LLECOM_NETWORK_IMPL_getDNSCount</code>, <code>LLECOM_NETWORK_IMPL_isDNSStatic</code>,
 * <code>LLECOM_NETWORK_IMPL_isStatic</code>, <code>LLECOM_NETWORK_IMPL_endConfiguration</code>,
 * <code>LLECOM_NETWORK_IMPL_startConfiguration</code> or <code>LLECOM_NETWORK_IMPL_setDefault</code> is done.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[in] getResult unused parameter.
 *
 * @return <code>LLECOM_NETWORK_IMPL_enable_action</code>, <code>LLECOM_NETWORK_IMPL_disable_action</code>,
 * <code>LLECOM_NETWORK_IMPL_isEnabled_action</code>, <code>LLECOM_NETWORK_IMPL_start_action</code>,
 * <code>LLECOM_NETWORK_IMPL_stop_action</code>, <code>LLECOM_NETWORK_IMPL_isStarted_action</code>,
 * <code>LLECOM_NETWORK_IMPL_getDNSCount_action</code>, <code>LLECOM_NETWORK_IMPL_isDNSStatic_action</code>,
 * <code>LLECOM_NETWORK_IMPL_isStatic_action</code>, <code>LLECOM_NETWORK_IMPL_endConfiguration_action</code> or
 * <code>LLECOM_NETWORK_IMPL_startConfiguration_action</code> function return code.
 */
static int32_t LLECOM_NETWORK_async_exec_netif_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t getResult) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	ECOM_NETWORK_netif_context_t* params = (ECOM_NETWORK_netif_context_t*)job->params;

	(void)netifName;
	(void)netifNameOffset;
	(void)netifNameLength;
	(void)getResult;

	int32_t result = params->result;

	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);

	return result;
}

/**
 * @brief Prepare and send an execution job to async_worker, called either from
 * <code>LLECOM_NETWORK_IMPL_getDNS</code> or <code>LLECOM_NETWORK_IMPL_setDNS</code>.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[in/out] address the buffer where the address will be / is stored
 * @param[in] addressOffset the offset where the address will be stored
 * @param[in] addressLength the address buffer length
 * @param[in] index the index of the DNS server (from 0 to {@link #getDNSCount()} -1)
 * @param[in] getResult false to post the request, true to get the result
 * @param[in] exec_write true if the address buffer content need to be sent to the async_worker job, else false.
 * @param[in] retry_function if the current Java thread has been suspended, this function is called when it is resumed.
 * @param[in] action the function to execute asynchronously.
 * @param[in] on_done the <code>SNI_callback</code> called when the job is done.
 *
 * @return <code>SNI_IGNORED_RETURNED_VALUE</code> on success, else a negative error code.
 */
static int32_t LLECOM_NETWORK_async_exec_dns_context(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int32_t index, int8_t getResult, bool exec_write, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&ecom_network_worker, retry_function);
	if (job == NULL) {
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return ECOM_NETWORK_ERROR;
	}

	ECOM_NETWORK_dns_context_t* params = (ECOM_NETWORK_dns_context_t*)job->params;

	params->netifNameOffset = netifNameOffset;
	params->netifNameLength = netifNameLength;
	params->addressOffset = addressOffset;
	params->addressLength = addressLength;
	params->index = index;
	params->getResult = getResult;

	bool do_copy = exec_write;
	int32_t result = 0;
	result = SNI_retrieveArrayElements(netifName, netifNameOffset, netifNameLength, (int8_t*)&params->netifBuffer,
			sizeof(params->netifBuffer), (int8_t**)&params->netifName, (uint32_t*)&params->netifNameLength, true);
	result |= SNI_retrieveArrayElements(address, addressOffset, addressLength, (int8_t*)&params->addrBuffer,
			sizeof(params->addrBuffer), (int8_t**)&params->address, (uint32_t*)&params->addressLength, do_copy);
	if (result != SNI_OK) {
		SNI_throwNativeIOException(result, "SNI_retrieveArrayElements: Internal error");
	} else {
		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&ecom_network_worker, job, action, on_done);
		if (status == MICROEJ_ASYNC_WORKER_OK) {
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);
	return ECOM_NETWORK_ERROR;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by
 * <code>LLECOM_NETWORK_IMPL_getDNS</code> is done.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[out] address the buffer where the address will be stored
 * @param[in] addressOffset the offset where the address will be stored
 * @param[in] addressLength the address buffer length
 * @param[in] index the index of the DNS server (from 0 to {@link #getDNSCount()} -1)
 * @param[in] getResult unused parameter.
 *
 * @return <code>LLECOM_NETWORK_IMPL_getDNS_action</code> function return code.
 */
static int32_t LLECOM_NETWORK_async_exec_get_dns_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int32_t index, int8_t getResult) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	ECOM_NETWORK_dns_context_t* params = (ECOM_NETWORK_dns_context_t*)job->params;

	(void)netifName;
	(void)netifNameOffset;
	(void)netifNameLength;
	(void)index;
	(void)getResult;

	int32_t result = params->result;
	if (result >= 0) {
		int32_t release_result = SNI_flushArrayElements(address, addressOffset, addressLength, (int8_t*)(params->address + params->addressOffset), params->addressLength);
		if (release_result != SNI_OK) {
			SNI_throwNativeIOException(release_result, "SNI_flushArrayElements: Internal error");
		}
	}
	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);

	return result;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by
 * <code>LLECOM_NETWORK_IMPL_setDNS</code> is done.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[in] address the buffer where the address is stored
 * @param[in] addressOffset the offset where the address will be stored
 * @param[in] addressLength the address buffer length
 * @param[in] index the index of the DNS server (from 0 to {@link #getDNSCount()} -1)
 * @param[in] getResult unused parameter.
 *
 * @return <code>LLECOM_NETWORK_IMPL_setDNS_action</code> function return code.
 */
static int32_t LLECOM_NETWORK_async_exec_set_dns_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int32_t index, int8_t getResult) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	ECOM_NETWORK_dns_context_t* params = (ECOM_NETWORK_dns_context_t*)job->params;

	(void)netifName;
	(void)netifNameOffset;
	(void)netifNameLength;
	(void)address;
	(void)addressOffset;
	(void)addressLength;
	(void)index;
	(void)getResult;

	int32_t result = params->result;

	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);

	return result;
}

/**
 * @brief Prepare and send an execution job to async_worker, called either from
 * <code>LLECOM_NETWORK_IMPL_getGateway</code>, <code>LLECOM_NETWORK_IMPL_getNetmask</code>,
 * <code>LLECOM_NETWORK_IMPL_setGateway</code>, <code>LLECOM_NETWORK_IMPL_setIP</code> or
 * <code>LLECOM_NETWORK_IMPL_setNetmask</code>.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[in/out] address the buffer where the address will be / is stored
 * @param[in] addressOffset the offset where the address will be stored
 * @param[in] addressLength the address buffer length
 * @param[in] getResult false to post the request, true to get the result
 * @param[in] exec_write true if the address buffer content need to be sent to the async_worker job, else false.
 * @param[in] retry_function if the current Java thread has been suspended, this function is called when it is resumed.
 * @param[in] action the function to execute asynchronously.
 * @param[in] on_done the <code>SNI_callback</code> called when the job is done.
 *
 * @return <code>SNI_IGNORED_RETURNED_VALUE</code> on success, else a negative error code.
 */
static int32_t LLECOM_NETWORK_async_exec_ip_context(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult, bool exec_write, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&ecom_network_worker, retry_function);
	if (job == NULL) {
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return ECOM_NETWORK_ERROR;
	}

	ECOM_NETWORK_ip_context_t* params = (ECOM_NETWORK_ip_context_t*)job->params;

	params->netifNameOffset = netifNameOffset;
	params->netifNameLength = netifNameLength;
	params->addressOffset = addressOffset;
	params->addressLength = addressLength;
	params->getResult = getResult;

	bool do_copy = exec_write;
	int32_t result = 0;
	result = SNI_retrieveArrayElements(netifName, netifNameOffset, netifNameLength, (int8_t*)&params->netifBuffer,
			sizeof(params->netifBuffer), (int8_t**)&params->netifName, (uint32_t*)&params->netifNameLength, true);
	result |= SNI_retrieveArrayElements(address, addressOffset, addressLength, (int8_t*)&params->addrBuffer,
			sizeof(params->addrBuffer), (int8_t**)&params->address, (uint32_t*)&params->addressLength, do_copy);
	if (result != SNI_OK) {
		SNI_throwNativeIOException(result, "SNI_retrieveArrayElements: Internal error");
	} else {
		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&ecom_network_worker, job, action, on_done);
		if (status == MICROEJ_ASYNC_WORKER_OK) {
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);
	return ECOM_NETWORK_ERROR;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by
 * <code>LLECOM_NETWORK_IMPL_getGateway</code> or <code>LLECOM_NETWORK_IMPL_getNetmask</code> is done.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[out] address the buffer where the address will be stored
 * @param[in] addressOffset the offset where the address will be stored
 * @param[in] addressLength the address buffer length
 * @param[in] getResult unused parameter.
 *
 * @return <code>LLECOM_NETWORK_IMPL_getGateway_action</code> or <code>LLECOM_NETWORK_IMPL_getNetmask_action</code> function return code.
 */
static int32_t LLECOM_NETWORK_async_exec_get_ip_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	ECOM_NETWORK_ip_context_t* params = (ECOM_NETWORK_ip_context_t*)job->params;

	(void)netifName;
	(void)netifNameOffset;
	(void)netifNameLength;
	(void)getResult;

	int32_t result = params->result;
	if (result >= 0) {
		int32_t release_result = SNI_flushArrayElements(address, addressOffset, addressLength, (int8_t*)(params->address + params->addressOffset), params->addressLength);
		if (release_result != SNI_OK) {
			SNI_throwNativeIOException(release_result, "SNI_flushArrayElements: Internal error");
		}
	}
	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);

	return result;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by
 * <code>LLECOM_NETWORK_IMPL_setGateway</code>, <code>LLECOM_NETWORK_IMPL_setIP</code> or
 * <code>LLECOM_NETWORK_IMPL_setNetmask</code> is done.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[in] address the buffer where the address is stored
 * @param[in] addressOffset the offset where the address will be stored
 * @param[in] addressLength the address buffer length
 * @param[in] getResult unused parameter.
 *
 * @return <code>LLECOM_NETWORK_IMPL_setGateway_action</code>,
 * <code>LLECOM_NETWORK_IMPL_setIP_action</code> or <code>LLECOM_NETWORK_IMPL_setNetmask_action</code> function return code.
 */
static int32_t LLECOM_NETWORK_async_exec_set_ip_context_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t* address, int32_t addressOffset, int32_t addressLength, int8_t getResult) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	ECOM_NETWORK_ip_context_t* params = (ECOM_NETWORK_ip_context_t*)job->params;

	(void)netifName;
	(void)netifNameOffset;
	(void)netifNameLength;
	(void)address;
	(void)addressOffset;
	(void)addressLength;
	(void)getResult;

	int32_t result = params->result;

	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);

	return result;
}

/**
 * @brief Prepare and send an execution job to async_worker, called either from
 * <code>LLECOM_NETWORK_IMPL_useDHCP</code> or <code>LLECOM_NETWORK_IMPL_useStaticDNS</code>.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[in] use true to use DHCP/DNS, false otherwise
 * @param[in] getResult false to post the request, true to get the result
 * @param[in] retry_function if the current Java thread has been suspended, this function is called when it is resumed.
 * @param[in] action the function to execute asynchronously.
 * @param[in] on_done the <code>SNI_callback</code> called when the job is done.
 *
 * @return <code>SNI_IGNORED_RETURNED_VALUE</code> on success, else a negative error code.
 */
static int32_t LLECOM_NETWORK_async_exec_ip_config(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t use, int8_t getResult, SNI_callback retry_function, MICROEJ_ASYNC_WORKER_action_t action, SNI_callback on_done) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_allocate_job(&ecom_network_worker, retry_function);
	if (job == NULL) {
		// No job available, either:
		// - wait for a job to be available and this function to be executed again,
		// - or an exception is pending
		return ECOM_NETWORK_ERROR;
	}

	ECOM_NETWORK_ip_config_t* params = (ECOM_NETWORK_ip_config_t*)job->params;

	params->netifNameOffset = netifNameOffset;
	params->netifNameLength = netifNameLength;
	params->use = use;
	params->getResult = getResult;

	int32_t result = SNI_retrieveArrayElements(netifName, netifNameOffset, netifNameLength, (int8_t*)&params->netifBuffer,
			sizeof(params->netifBuffer), (int8_t**)&params->netifName, (uint32_t*)&params->netifNameLength, true);
	if (result != SNI_OK) {
		SNI_throwNativeIOException(result, "SNI_retrieveArrayElements: Internal error");
	} else {
		MICROEJ_ASYNC_WORKER_status_t status = MICROEJ_ASYNC_WORKER_async_exec(&ecom_network_worker, job, action, on_done);
		if (status == MICROEJ_ASYNC_WORKER_OK) {
			// Wait for the action to be done
			return SNI_IGNORED_RETURNED_VALUE;//returned value not used
		} // else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	}

	// Else an error occurred and MICROEJ_ASYNC_WORKER_async_exec has thrown a SNI exception
	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);
	return ECOM_NETWORK_ERROR;
}

/**
 * @brief The <code>SNI_callback</code> called when the async_worker job requested by
 * <code>LLECOM_NETWORK_IMPL_useDHCP</code> or <code>LLECOM_NETWORK_IMPL_useStaticDNS</code> is done.
 *
 * @param[in] netifName the buffer that store interface name
 * @param[in] netifNameOffset the offset where the interface name is stored
 * @param[in] netifNameLength the interface name buffer length
 * @param[in] use true to use DHCP/DNS, false otherwise
 * @param[in] getResult unused parameter.
 *
 * @return <code>LLECOM_NETWORK_IMPL_useDHCP_action</code> or
 * <code>LLECOM_NETWORK_IMPL_useStaticDNS_action</code> function return code.
 */
static int32_t LLECOM_NETWORK_async_exec_ip_config_on_done(int8_t* netifName, int32_t netifNameOffset, int32_t netifNameLength, int8_t use, int8_t getResult) {
	MICROEJ_ASYNC_WORKER_job_t* job = MICROEJ_ASYNC_WORKER_get_job_done();
	ECOM_NETWORK_ip_config_t* params = (ECOM_NETWORK_ip_config_t*)job->params;

	(void)netifName;
	(void)netifNameOffset;
	(void)netifNameLength;
	(void)use;
	(void)getResult;

	int32_t result = params->result;

	MICROEJ_ASYNC_WORKER_free_job(&ecom_network_worker, job);

	return result;
}

#ifdef __cplusplus
	}
#endif
