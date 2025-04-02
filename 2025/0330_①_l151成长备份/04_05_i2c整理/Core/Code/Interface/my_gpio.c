#include "my_gpio.h"


GPIO_Class_Struct GPIO_Class_default = {
    .Pin = LED_BLUE_PC13_Pin,
    .Port = LED_BLUE_PC13_GPIO_Port,
    .ON_Status = 0,
    .OFF_Status = 1
};


GPIO_PinState MY_GPIO_Read(GPIO_Class_Struct* input_GPIO_Class){
    return HAL_GPIO_ReadPin(
        input_GPIO_Class ->Port, 
        input_GPIO_Class ->Pin
    );
}

void MY_GPIO_ON(GPIO_Class_Struct* input_GPIO_Class){
    HAL_GPIO_WritePin(
        input_GPIO_Class ->Port,
        input_GPIO_Class ->Pin,
        input_GPIO_Class ->ON_Status
    );
}

void MY_GPIO_OFF(GPIO_Class_Struct* input_GPIO_Class){
    HAL_GPIO_WritePin(
        input_GPIO_Class ->Port,
        input_GPIO_Class ->Pin,
        input_GPIO_Class ->OFF_Status
    );
}

void MY_GPIO_Toggle(GPIO_Class_Struct* input_GPIO_Class){
    HAL_GPIO_TogglePin(
        input_GPIO_Class ->Port, 
        input_GPIO_Class ->Pin
    );
}


void my_gpio_self_test(void){
    if(GPIO_Class_default.ON_Status == MY_GPIO_Read(&GPIO_Class_default)){
        MY_GPIO_OFF(&GPIO_Class_default);
    }else{
        MY_GPIO_ON(&GPIO_Class_default);
    }
}


