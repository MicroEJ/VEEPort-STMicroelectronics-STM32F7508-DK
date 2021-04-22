/*
 * C
 *
 * Copyright 2017-2020 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

/**
 * @file
 * @brief Asynchronous network select implementation over OSAL API.
 * @author MicroEJ Developer Team
 * @version 1.0.1
 * @date 19 February 2020
 */

#include "async_select.h"
#include "async_select_configuration.h"
#include "osal.h"
#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Initializes the requests FIFOs.
 * This function must be called prior to any call of async_select().
 * It can be called several times.
 */
extern void async_select_request_fifo_init(void);
/**
 * @brief The entry point for the async_select task.
 * This function must be called from a dedicated task.
 */
extern void async_select_task_main(void);

/*
 * See implementations for descriptions.
 */
void async_select_lock(void);
void async_select_unlock(void);
static int32_t async_select_start_task(void);

/**
 * @brief Stack of the async_select task.
 */
OSAL_task_stack_declare(async_select_task_stack, ASYNC_SELECT_TASK_STACK_SIZE);
/**
 * @brief async_select OS task.
 */
static OSAL_task_handle_t async_select_task;
/**
 * @brief Mutex used for critical sections.
 */
static OSAL_mutex_handle_t async_select_mutex;

/**
 * @brief Initialize the async_select component. This function must be called prior to any call of
 * async_select().
 *
 * @return 0 on success, -1 on failure.
 */
int32_t async_select_init(){
	int32_t res;
	res = async_select_start_task();
	if(res == 0){
		async_select_request_fifo_init();
	}
	return res;
}

/**
 * @brief Start RTOS task and init RTOS specific structures.
 */
static int32_t async_select_start_task(){
	OSAL_status_t status;

	status = OSAL_mutex_create(ASYNC_SELECT_MUTEX_NAME, &async_select_mutex);
	
	if(status == OSAL_OK){
		status = OSAL_task_create((OSAL_task_entry_point_t) async_select_task_main, ASYNC_SELECT_TASK_NAME, async_select_task_stack, ASYNC_SELECT_TASK_PRIORITY, NULL, &async_select_task);
	}

	if(status == OSAL_OK){
		return 0;
	}
	else{
		return -1;
	}
}

/**
 * @brief Enter critical section for the async_select component.
 */
void async_select_lock(void){
	OSAL_mutex_take(&async_select_mutex, OSAL_INFINITE_TIME);
}

/**
 * @brief Exit critical section for the async_select component.
 */
void async_select_unlock(void){
	OSAL_mutex_give(&async_select_mutex);
}

#ifdef __cplusplus
	}
#endif
