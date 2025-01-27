#ifndef __MY_LED_H
#define __MY_LED_H

#include "main.h"

#include "my_gpio.h"


GPIO_Class_Struct LED_Class_BLUE = {
    .Pin = LED_BLUE_PC13_Pin,
	.Port = LED_BLUE_PC13_GPIO_Port,
    .ON_Status = 0,
    .OFF_Status = 1
};

GPIO_Class_Struct LED_Class_GREEN = {
    .Pin = LED_GREEN_PB9_Pin,
	.Port = LED_GREEN_PB9_GPIO_Port,
    .ON_Status = 0,
    .OFF_Status = 1
};

GPIO_Class_Struct LED_Class_YELLOW = {
    .Pin = LED_YELLOW_PB13_Pin,
	.Port = LED_YELLOW_PB13_GPIO_Port,
    .ON_Status = 0,
    .OFF_Status = 1
};

GPIO_Class_Struct LED_Class_RED = {
    .Pin = LED_RED_PB1_Pin,
	.Port = LED_RED_PB1_GPIO_Port,
    .ON_Status = 0,
    .OFF_Status = 1
};


// extern GPIO_Class_Struct LED_Class_BLUE;

// extern GPIO_Class_Struct LED_Class_GREEN;

// extern GPIO_Class_Struct LED_Class_YELLOW;

// extern GPIO_Class_Struct LED_Class_RED;


// 声明BLUE LED相关函数
#if !BLUE
GPIO_PinState BLUE_READ(void);
void BLUE_ON(void);
void BLUE_OFF(void);
void BLUE_Toggle(void);
#endif

// 声明GREEN LED相关函数
#if !GREEN
GPIO_PinState GREEN_READ(void);
void GREEN_ON(void);
void GREEN_OFF(void);
void GREEN_Toggle(void);
#endif

// 声明YELLOW LED相关函数
#if !YELLOW
GPIO_PinState YELLOW_READ(void);
void YELLOW_ON(void);
void YELLOW_OFF(void);
void YELLOW_Toggle(void);
#endif

// 声明RED LED相关函数
#if !RED
GPIO_PinState RED_READ(void);
void RED_ON(void);
void RED_OFF(void);
void RED_Toggle(void);
#endif


#endif // !__MY_LED_H   


