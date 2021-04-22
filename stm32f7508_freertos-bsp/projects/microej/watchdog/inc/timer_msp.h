/**
  ******************************************************************************
  * @file    timer_msp.h
  * @author  MCD Application Team
  * @brief   Header file of TIM HAL module.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_MSP_H
#define __TIMER_MSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/** @addtogroup TIM
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
 void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim);

/* Exported macro ------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __TIMER_MSP_H */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
