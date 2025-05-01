#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
//LED IO define					
#define IO_LED_ARRAY GPIOA
#define RCC_IO_LED RCC_APB2Periph_GPIOA
#define IO_sysLED GPIO_Pin_1
#define IO_LED1 GPIO_Pin_2
#define IO_LED2 GPIO_Pin_3
#define IO_LED3 GPIO_Pin_4

#define IO_RS GPIO_Pin_8


//KEY IO define					
#define IO_KEY_ARRAY GPIOA
#define RCC_IO_KEY RCC_APB2Periph_GPIOA
#define IO_CORREC_KEY GPIO_Pin_0


//define extern function
extern int CORREC_KEYSCAN(void);
extern void LedShow(void);
extern void LedGpioConfiguration(void);
extern void KEYGpioConfiguration(void);
extern void Delay(__IO uint32_t nCount);

#endif
