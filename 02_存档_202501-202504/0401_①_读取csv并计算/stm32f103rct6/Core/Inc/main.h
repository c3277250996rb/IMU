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
#include "stm32f1xx_hal.h"

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
#define LED_D4_PC2_Pin GPIO_PIN_2
#define LED_D4_PC2_GPIO_Port GPIOC
#define SPI1_CS_PA4_Pin GPIO_PIN_4
#define SPI1_CS_PA4_GPIO_Port GPIOA
#define SPI1_SCK_PA5_Pin GPIO_PIN_5
#define SPI1_SCK_PA5_GPIO_Port GPIOA
#define SPI1_MISO_PA6_Pin GPIO_PIN_6 
#define SPI1_MISO_PA6_GPIO_Port GPIOA
#define SPI1_MOSI_PA7_Pin GPIO_PIN_7
#define SPI1_MOSI_PA7_GPIO_Port GPIOA
#define I2C2_SCL_PB10_Pin GPIO_PIN_10
#define I2C2_SCL_PB10_GPIO_Port GPIOB
#define I2C2_SDA_PB11_Pin GPIO_PIN_11
#define I2C2_SDA_PB11_GPIO_Port GPIOB
#define USART1_PA9_Pin GPIO_PIN_9
#define USART1_PA9_GPIO_Port GPIOA
#define USART1_PA10_Pin GPIO_PIN_10
#define USART1_PA10_GPIO_Port GPIOA
#define I2C1_SCL_PB6_Pin GPIO_PIN_6
#define I2C1_SCL_PB6_GPIO_Port GPIOB
#define I2C1_SDA_PB7_Pin GPIO_PIN_7
#define I2C1_SDA_PB7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define USE_MY_ALIENTEK
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
