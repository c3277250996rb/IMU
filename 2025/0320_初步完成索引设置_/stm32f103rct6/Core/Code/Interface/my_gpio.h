#ifndef __MY_GPIO_H
#define __MY_GPIO_H

#include "main.h"


typedef struct __GPIO_Class_Struct
{
    uint16_t Pin;
	GPIO_TypeDef* Port;
    uint8_t ON_Status;
    uint8_t OFF_Status;
}GPIO_Class_Struct;

// ?????¨´?¡Â
GPIO_PinState MY_GPIO_Read(GPIO_Class_Struct input_GPIO_Class);
void MY_GPIO_ON(GPIO_Class_Struct input_GPIO_Class);
void MY_GPIO_OFF(GPIO_Class_Struct input_GPIO_Class);
void MY_GPIO_Toggle(GPIO_Class_Struct input_GPIO_Class);



#endif // !__MY_GPIO_H  

