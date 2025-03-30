/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32l1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define LED_BLUE_PC13_Pin GPIO_PIN_13
#define LED_BLUE_PC13_GPIO_Port GPIOC
#define USART1_TX_PA9_Pin GPIO_PIN_9
#define USART1_TX_PA9_GPIO_Port GPIOA
#define USART1_RX_PA10_Pin GPIO_PIN_10
#define USART1_RX_PA10_GPIO_Port GPIOA
#define I2C1_SCL_PB6_Pin GPIO_PIN_6
#define I2C1_SCL_PB6_GPIO_Port GPIOB
#define I2C1_SDA_PB7_Pin GPIO_PIN_7
#define I2C1_SDA_PB7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define USE_MY_ALIENTEK
#define OD_for_1_and_PP_for_0 1
#define test_my_i2c 1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
