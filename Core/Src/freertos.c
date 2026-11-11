/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usart.h"
#include "motor6020.h"
#include "pid.h"
#include "can.h"
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
volatile float   g_target_rpm   = 0.0f;
float            g_actual_rpm   = 0.0f;
volatile float   g_servo_deg    = 90.0f;
volatile uint8_t g_motor_online = 0;

pid_t g_pid;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LedTask */
osThreadId_t LedTaskHandle;
const osThreadAttr_t LedTask_attributes = {
  .name = "LedTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for PrintTask */
osThreadId_t PrintTaskHandle;
const osThreadAttr_t PrintTask_attributes = {
  .name = "PrintTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for KeyTask */
osThreadId_t KeyTaskHandle;
const osThreadAttr_t KeyTask_attributes = {
  .name = "KeyTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ServoTask */
osThreadId_t ServoTaskHandle;
const osThreadAttr_t ServoTask_attributes = {
  .name = "ServoTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UartTask */
osThreadId_t UartTaskHandle;
const osThreadAttr_t UartTask_attributes = {
  .name = "UartTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartLedTask(void *argument);
void StartMotorTask(void *argument);
void StartPrintTask(void *argument);
void StartKeyTask(void *argument);
void StartServoTask(void *argument);
void StartUartTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LedTask */
  LedTaskHandle = osThreadNew(StartLedTask, NULL, &LedTask_attributes);

  /* creation of MotorTask */
  MotorTaskHandle = osThreadNew(StartMotorTask, NULL, &MotorTask_attributes);

  /* creation of PrintTask */
  PrintTaskHandle = osThreadNew(StartPrintTask, NULL, &PrintTask_attributes);

  /* creation of KeyTask */
  KeyTaskHandle = osThreadNew(StartKeyTask, NULL, &KeyTask_attributes);

  /* creation of ServoTask */
  ServoTaskHandle = osThreadNew(StartServoTask, NULL, &ServoTask_attributes);

  /* creation of UartTask */
  UartTaskHandle = osThreadNew(StartUartTask, NULL, &UartTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartLedTask */
/**
* @brief Function implementing the LedTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLedTask */
void StartLedTask(void *argument)
{
  /* USER CODE BEGIN StartLedTask */
  /* Infinite loop */
  uint16_t pwm = 0;
  uint8_t  up  = 1;
  (void)argument;

  for(;;)
  {
    if (g_motor_online)
    {
      __HAL_TIM_SET_COMPARE(&htim5, LED_GREEN_CH, 1000);
      __HAL_TIM_SET_COMPARE(&htim5, LED_RED_CH,   0);
    }
    else
    {
      __HAL_TIM_SET_COMPARE(&htim5, LED_GREEN_CH, 0);

      if (up) { pwm += 20; if (pwm >= 1000) up = 0; }
      else    { pwm -= 20; if (pwm <= 0)    up = 1; }

      __HAL_TIM_SET_COMPARE(&htim5, LED_RED_CH, pwm);
    }

    osDelay(20);
  }
  /* USER CODE END StartLedTask */
}

/* USER CODE BEGIN Header_StartMotorTask */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotorTask */
void StartMotorTask(void *argument)
{
  /* USER CODE BEGIN StartMotorTask */
  float   speed;
  int16_t volt;

  (void)argument;


  PID_Init(&g_pid, 0.2f, 20.0f, 0.0f, 30000.0f, -30000.0f);

  for(;;)
  {

    speed = Motor6020_GetSpeed(1);
    g_actual_rpm = speed;


    g_pid.target = g_target_rpm;
    volt = (int16_t)PID_Calc(&g_pid, speed);


    Motor6020_SetVolt(1, volt);

    Motor6020_UpdateOnline(100);
    g_motor_online = Motor6020_GetState(1)->online;


    osDelay(1);
  }
  /* USER CODE END StartMotorTask */
}

/* USER CODE BEGIN Header_StartPrintTask */
/**
* @brief Function implementing the PrintTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPrintTask */
void StartPrintTask(void *argument)
{
  /* USER CODE BEGIN StartPrintTask */
  /* Infinite loop */
  char buf[48];
  int  n;
  int  s10;
  (void)argument;

  for(;;) {

    s10 = (int)(g_actual_rpm * 10.0f);


    n = snprintf(buf, sizeof(buf), "rpm=%d.%d online=%d\r\n",
                 s10 / 10, (s10 < 0 ? -s10 : s10) % 10,
                 g_motor_online);


    if (n > 0)
    {
      HAL_UART_Transmit(&huart6, (uint8_t *)buf, (uint16_t)n, 100);
    }

    osDelay(2000);
  }
  /* USER CODE END StartPrintTask */
}

/* USER CODE BEGIN Header_StartKeyTask */
  /**
  * @brief Function implementing the KeyTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartKeyTask */
void StartKeyTask(void *argument)
{
  /* USER CODE BEGIN StartKeyTask */
    /* Infinite loop */
  uint8_t last = 1;
  uint8_t now;
  (void)argument;

  for(;;)
  {
    now = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);

    if (last == 1 && now == 0)
    {
      g_servo_deg += 20.0f;
      if (g_servo_deg > 180.0f)
      {
        g_servo_deg = 0.0f;
      }
    }

    last = now;
    osDelay(20);
  }
  /* USER CODE END StartKeyTask */
}

/* USER CODE BEGIN Header_StartServoTask */
  /**
  * @brief Function implementing the ServoTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartServoTask */
void StartServoTask(void *argument)
{
  /* USER CODE BEGIN StartServoTask */
  uint32_t ccr;
  (void)argument;

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1500);
  osDelay(500);
  for(;;)
  {
    ccr = (uint32_t)(500.0f + g_servo_deg / 180.0f * 2000.0f);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr);
    osDelay(50);
  }
  /* USER CODE END StartServoTask */
}

/* USER CODE BEGIN Header_StartUartTask */
  /**
  * @brief Function implementing the UartTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartUartTask */
void StartUartTask(void *argument)
{
  /* USER CODE BEGIN StartUartTask */
    /* Infinite loop */
    uint8_t ch;
    char    buf[16];
    uint8_t len = 0;
    (void)argument;

    for(;;)
    {
      if (HAL_UART_Receive(&huart6, &ch, 1, 5) == HAL_OK)
      {
        if (ch == '\n')
        {
          buf[len] = '\0';

          if (strncmp(buf, "motor", 5) == 0)
          {
            g_target_rpm = (float)atoi(&buf[5]);
          }

          len = 0;
        }
        else if (ch != '\r' && len < 15)
        {
          buf[len++] = ch;
        }
      }
      else
      {
        len = 0;
      }
      osDelay(2);
    }
  /* USER CODE END StartUartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

