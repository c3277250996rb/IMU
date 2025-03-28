/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l1xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
    /*金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮*/
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    printf("HardFault_Handler: 硬件错误辣.\r\n");
    printf("尝试重启...\r\n");
    #if 1
    char kun[] = "\
                                 &BGG#&     \r\n\
                                P55PPP5G&   \r\n\
                      &##BB5?B#BGBBG5GGGP&  \r\n\
                     #BBBBBG^PBBBGB57J5PP&  \r\n\
                   #B##BBBB#!J#BBBBBYJPGB   \r\n\
                 &###&#BBBB#J!BB###BB#      \r\n\
                 ###&5GBBBB#P!PBPB###B&     \r\n\
                 &###GBBB###75##BY?5B##     \r\n\
                  &#######&P~#####GJ7YB     \r\n\
                    ######&?Y&#####&GJP     \r\n\
                  &#PY5GBBBJB######&&#&     \r\n\
                  PJ??JJJYYY55PGB####       \r\n\
               &GY??JJJYYYYYYYYYY5G&        \r\n\
             &PJ???JJJYYYYYYYYYYYYJ5&       \r\n\
            BJ?JJJJYYY55PPP555555YYJB       \r\n\
          &Y?JYYYY5PGB#&    #G555YYYJ#      \r\n\
          Y7J55PG#&           &G5555YY&     \r\n\
         #7?JYG                 #GPPY?J&    \r\n\
         G7?JY#                   #YJ??P    \r\n\
         5?JJ5                     GJ??J&   \r\n\
         Y?JY&                     &Y???G   \r\n\
        &J?JG                       &Y??5   \r\n\
        &PGB                         #55P   \r\n\
        BGPB                          #GPB  \r\n\
      &BGBGPB                         #PGBB&\r\n\
    ";
    #else
    char kun[] = "\
      @@@@@@@@@@@@@@@@@@@@@@@@@@@&BGG#&@@@@@\r\n\
      @@@@@@@@@@@@@@@@@@@@@@@@@@P55PPP5G&@@@\r\n\
      @@@@@@@@@@@@@@@@&##BB5?B#BGBBG5GGGP&@@\r\n\
      @@@@@@@@@@@@@@@#BBBBBG^PBBBGB57J5PP&@@\r\n\
      @@@@@@@@@@@@@#B##BBBB#!J#BBBBBYJPGB@@@\r\n\
      @@@@@@@@@@@&###&#BBBB#J!BB###BB#@@@@@@\r\n\
      @@@@@@@@@@@###&5GBBBB#P!PBPB###B&@@@@@\r\n\
      @@@@@@@@@@@&###GBBB###75##BY?5B##@@@@@\r\n\
      @@@@@@@@@@@@&#######&P~#####GJ7YB@@@@@\r\n\
      @@@@@@@@@@@@@@######&?Y&#####&GJP@@@@@\r\n\
      @@@@@@@@@@@@&#PY5GBBBJB######&&#&@@@@@\r\n\
      @@@@@@@@@@@@PJ??JJJYYY55PGB####@@@@@@@\r\n\
      @@@@@@@@@&GY??JJJYYYYYYYYYY5G&@@@@@@@@\r\n\
      @@@@@@@&PJ???JJJYYYYYYYYYYYYJ5&@@@@@@@\r\n\
      @@@@@@BJ?JJJJYYY55PPP555555YYJB@@@@@@@\r\n\
      @@@@&Y?JYYYY5PGB#&@@@@#G555YYYJ#@@@@@@\r\n\
      @@@@Y7J55PG#&@@@@@@@@@@@&G5555YY&@@@@@\r\n\
      @@@#7?JYG@@@@@@@@@@@@@@@@@#GPPY?J&@@@@\r\n\
      @@@G7?JY#@@@@@@@@@@@@@@@@@@@#YJ??P@@@@\r\n\
      @@@5?JJ5@@@@@@@@@@@@@@@@@@@@@GJ??J&@@@\r\n\
      @@@Y?JY&@@@@@@@@@@@@@@@@@@@@@&Y???G@@@\r\n\
      @@&J?JG@@@@@@@@@@@@@@@@@@@@@@@&Y??5@@@\r\n\
      @@&PGB@@@@@@@@@@@@@@@@@@@@@@@@@#55P@@@\r\n\
      @@BGPB@@@@@@@@@@@@@@@@@@@@@@@@@@#GPB@@\r\n\
      &BGBGPB@@@@@@@@@@@@@@@@@@@@@@@@@#PGBB&\r\n\
    ";
    #endif
    // printf(&kun);
    HAL_NVIC_SystemReset();
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
}
#if 0
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */
#endif
/* USER CODE END 1 */
