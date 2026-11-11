/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* 三色灯 —— 走 TIM5 PWM，低电平点亮 */
#define LED_BLUE_Pin      GPIO_PIN_10
#define LED_BLUE_Port     GPIOH
#define LED_GREEN_Pin     GPIO_PIN_11
#define LED_GREEN_Port    GPIOH
#define LED_RED_Pin       GPIO_PIN_12
#define LED_RED_Port      GPIOH

/* 三个灯的 TIM5 通道 */
#define LED_BLUE_CH       TIM_CHANNEL_1
#define LED_GREEN_CH      TIM_CHANNEL_2
#define LED_RED_CH        TIM_CHANNEL_3

/* 三色灯亮度换算：ARR = 999，所以 0~1000 */
#define LED_FULL          0      /* 低电平点亮 → 0 是最亮 */
#define LED_OFF           1000   /* 1000 是全灭 */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_Pin GPIO_PIN_0
#define KEY_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
