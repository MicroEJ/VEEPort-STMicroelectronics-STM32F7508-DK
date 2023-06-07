/**
  ******************************************************************************
  * @file    stm32f7xx_custom.c
  * @author  MCD Application Team
  * @brief   HAL module driver.
  *          This is the common part of the HAL initialization
  *
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    The common HAL driver contains a set of generic and common APIs that can be
    used by the PPP peripheral drivers and the user to start using the HAL. 
    [..]
    The HAL contains two APIs' categories: 
         (+) Common HAL APIs
         (+) Services HAL APIs

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * Copyright 2020 MicroEJ Corp. This file has been modified by MicroEJ Corp.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* @brief If COREMARK or IPERF tests are active, we don't use this implementation */
#if !defined(VALIDATION_BUILD) && !defined(IPERF_BUILD)
#include "FreeRTOS.h"
#include "task.h"
#endif

/** @addtogroup STM32F7xx_HAL_Driver
  * @{
  */

/** @defgroup HAL HAL
  * @brief HAL module driver.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/**< @brief This is the minimum delay in RTOS ticks for which a non-blocking
 * delay is preferred.
 */
#define MIN_RTOS_TICKS_DELAY 10

/* Private macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/** @addtogroup HAL_Exported_Variables
  * @{
  */
extern __IO uint32_t uwTick;
extern HAL_TickFreqTypeDef uwTickFreq;


/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_Exported_Functions_Group2 HAL Control functions 
 *  @brief    HAL Control functions
 *
@verbatim
 ===============================================================================
                      ##### HAL Control functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Provide a tick value in millisecond
      (+) Provide a blocking delay in millisecond

@endverbatim
  * @{
  */

/**
  * @brief This function is called to increment  a global variable "uwTick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in SysTick ISR.
  * @note This function overrides the __weak implementation in the sdk.
  * @retval None
  */
void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
}


/* @brief If COREMARK or IPERF tests are active, we don't use this implementation */
#if !defined(VALIDATION_BUILD) && !defined(IPERF_BUILD)

/*============================================================================*/
/**
 * @brief This function provides a blocking delay (in milliseconds).
 * @details It uses the system tick timer.
 * @param Delay specifies the delay time length, in milliseconds.
 * @retval None
 * @author
 * @date
 * @version 1: DDP (2021-07-9): adopted a previous implementation.
 */
__STATIC_INLINE void HAL_Delay_Blocking(__IO uint32_t Delay) {

	uint32_t tickstart = HAL_GetTick();
	while ((HAL_GetTick() - tickstart) < Delay){
	}
}
/*============================================================================*/

/*============================================================================*/
/**
 * @brief This function provides a delay (in milliseconds).
 * @details It uses default implementation if the RTOS is not started or the
 * delay is to small (in respect to the RTOS ticks).
 * @param Delay specifies the delay time length, in milliseconds.
 * @retval None
 * @author
 * @date
 * @version 1: DDP (2021-07-9): adapted a previous implementation.
 */
void HAL_Delay(__IO uint32_t Delay)
{

#if (1 == INCLUDE_xTaskGetSchedulerState)
	/* if the RTOS scheduler is started we can use the RTOS delay task */
	if (taskSCHEDULER_NOT_STARTED != xTaskGetSchedulerState()){
#else
#error Definition 'INCLUDE_xTaskGetSchedulerState' required but not set.
#endif  /* INCLUDE_xTaskGetSchedulerState */

/* we also check if the vTaskDelay is included */
#if (1 == INCLUDE_vTaskDelay)
		TickType_t ticks = Delay / portTICK_PERIOD_MS;
		if (ticks > ((TickType_t) MIN_RTOS_TICKS_DELAY)){
			vTaskDelay(ticks);
		} else{
			HAL_Delay_Blocking(Delay); /* implement the default blocking delay */
		} /* minimum threshold ticks not reached */

#else /* the RTOS specific non-blocking delay is not included */
		HAL_Delay_Blocking(Delay); /* so we implement blocking delay */
#endif /* INCLUDE_vTaskDelay */
		return;
#if (1 == INCLUDE_xTaskGetSchedulerState)
	}
#endif  /* INCLUDE_xTaskGetSchedulerState */

/* we use the blocking delay implementation (RTOS not started) */
	HAL_Delay_Blocking(Delay);
}
/*============================================================================*/

#endif


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
