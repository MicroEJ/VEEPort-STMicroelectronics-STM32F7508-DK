/**
  ******************************************************************************
  * @file    timer_msp.c
  * @author  MCD Application Team
  * @brief   TIM HAL module driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
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
#include "timer_msp.h"

/** @addtogroup STM32F7xx_HAL_Driver
  * @{
  */

/** @defgroup TIM TIM
  * @brief TIM HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @addtogroup TIM_Private_Functions
  * @{
  */

/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  /* TIM5 Peripheral clock enable */
  __TIM5_CLK_ENABLE();

  /* Configure the NVIC for TIM5 */
  HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);

  /* Enable the TIM5 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM5_IRQn);
}

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
