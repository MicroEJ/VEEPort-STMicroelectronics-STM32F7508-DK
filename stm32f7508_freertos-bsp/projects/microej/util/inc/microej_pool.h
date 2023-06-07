/*
 * C
 *
 * Copyright 2020-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/**
 * @file
 * @brief MicroEJ memory pool implementation
 * @author MicroEJ Developer Team
 * @version 0.1.0
 */

/*
 * The module provide function to simply manage a
 * Fixed memory pool size.
 */

#ifndef MICROEJ_POOL_H
#define MICROEJ_POOL_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief list of status for one element */
typedef enum
{
	POOL_FREE,
	POOL_USED
}POOL_item_status_t;

/** @brief define pool type */
typedef struct {
	void * pv_first_item;                 /**< pointer on first element in pool */
	POOL_item_status_t * puc_item_status; /**< pointer on array status item */
	unsigned int ui_size_of_item;         /**< size of one element */
	unsigned int uc_num_item_in_pool;    /**< number of element in pool */
}POOL_ctx_t;

/** @brief list of module constant */
typedef enum
{
	POOL_NO_ERROR,
	POOL_ERROR_IN_ENTRY_PARAMETERS,
	POOL_NO_SPACE_AVAILABLE,
	POOL_ITEM_NOT_FOUND_IN_POOL,
}POOL_status_t;

/** @brief Define a function prototype used to compare a pool item with a characteristic */
typedef bool (*microej_pool_compare_functor_t)(void *item, void *characteristic);


/** @brief pool declaration macro */
#define POOL_declare(name, pool_type, size)	\
	static pool_type name ## _pool_array[size];	\
	static POOL_item_status_t name ## _pool_item_status[size];	\
	static POOL_ctx_t name =	\
	{	\
		name ## _pool_array,	\
		name ## _pool_item_status,	\
		sizeof(pool_type),	\
		sizeof(name ## _pool_array) / sizeof(pool_type)	\
	}

/**
 * @brief function to reserved one place in pool
 *
 * @param[in] _st_pool_ctx        pool context
 * @param[in,out]    _ppv_item_reserved  pointer on reserved item
 *
 * @return @see POOL_status_t
 */
POOL_status_t POOL_reserve_f(POOL_ctx_t * _st_pool_ctx,
		                     void ** _ppv_item_reserved);

/**
 * @brief Get an item in according to a comparison function and a characteristic.
 *
 * @param[in] 		_st_pool_ctx        	pool context
 * @param[in,out]  	_ppv_item_retrieved  	pointer on item to be retrieved
 * @param[in] 		compare_to  			functor on the comparison function.
 * @param[in]    	characteristic 			characteristic on which the comparison will occurred.
 *
 * @return @see POOL_status_t
 */
POOL_status_t POOL_get_f(POOL_ctx_t * _st_pool_ctx, void ** _ppv_item_retrieved,
		microej_pool_compare_functor_t compare_to, void* characteristic);

/**
 * @brief function to free an item of the pool
 *
 * @param[in,out] _st_pool_ctx      pool context
 * @param[in]     _pv_item_to_free  pointer item to free
 *
 * @return @see POOL_status_t
 */
POOL_status_t POOL_free_f(POOL_ctx_t * _st_pool_ctx,
		                  void * const _pv_item_to_free);

#ifdef __cplusplus
}
#endif

#endif /* MICROEJ_POOL_H */
