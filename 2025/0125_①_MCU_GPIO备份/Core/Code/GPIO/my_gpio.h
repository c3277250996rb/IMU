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

#define MY_GPIO_Read(input_GPIO_Class) {    \
            GPIO_TypeDef* temp_Port = input_GPIO_Class.Port;    \
            uint16_t temp_Pin       = input_GPIO_Class.Pin;     \
            return HAL_GPIO_ReadPin(temp_Port, temp_Pin); \
        }

#define MY_GPIO_ON(input_GPIO_Class) {    \
            GPIO_TypeDef* temp_Port     = input_GPIO_Class.Port;    \
            uint16_t temp_Pin           = input_GPIO_Class.Pin;     \
            GPIO_PinState temp_PinState = input_GPIO_Class.ON_Status; \
            HAL_GPIO_WritePin(temp_Port, temp_Pin, temp_PinState); \
        }

#define MY_GPIO_OFF(input_GPIO_Class) {    \
            GPIO_TypeDef* temp_Port     = input_GPIO_Class.Port;    \
            uint16_t temp_Pin           = input_GPIO_Class.Pin;     \
            GPIO_PinState temp_PinState = input_GPIO_Class.OFF_Status; \
            HAL_GPIO_WritePin(temp_Port, temp_Pin, temp_PinState); \
        }

#define MY_GPIO_Toggle(input_GPIO_Class) {    \
            GPIO_TypeDef* temp_Port = input_GPIO_Class.Port;    \
            uint16_t temp_Pin       = input_GPIO_Class.Pin;     \
            HAL_GPIO_TogglePin(temp_Port, temp_Pin); \
        }




#endif // !__MY_GPIO_H  

