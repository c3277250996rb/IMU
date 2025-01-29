#include "my_gpio.h"  // 根据实际情况修改头文件



GPIO_PinState MY_GPIO_Read(GPIO_Class_Struct input_GPIO_Class) {
    GPIO_TypeDef* temp_Port = input_GPIO_Class.Port;
    uint16_t temp_Pin = input_GPIO_Class.Pin;
    return HAL_GPIO_ReadPin(temp_Port, temp_Pin);
}

void MY_GPIO_ON(GPIO_Class_Struct input_GPIO_Class) {
    GPIO_TypeDef* temp_Port = input_GPIO_Class.Port;
    uint16_t temp_Pin = input_GPIO_Class.Pin;
    GPIO_PinState temp_PinState = input_GPIO_Class.ON_Status;
    HAL_GPIO_WritePin(temp_Port, temp_Pin, temp_PinState);
}

void MY_GPIO_OFF(GPIO_Class_Struct input_GPIO_Class) {
    GPIO_TypeDef* temp_Port = input_GPIO_Class.Port;
    uint16_t temp_Pin = input_GPIO_Class.Pin;
    GPIO_PinState temp_PinState = input_GPIO_Class.OFF_Status;
    HAL_GPIO_WritePin(temp_Port, temp_Pin, temp_PinState);
}

void MY_GPIO_Toggle(GPIO_Class_Struct input_GPIO_Class) {
    GPIO_TypeDef* temp_Port = input_GPIO_Class.Port;
    uint16_t temp_Pin = input_GPIO_Class.Pin;
    HAL_GPIO_TogglePin(temp_Port, temp_Pin);
}

    GPIO_Class_Struct GPIO_Class_test = {
        .Pin = GPIO_PIN_13,
        .Port = GPIOC,
        .ON_Status = 0,
        .OFF_Status = 1
    };

#include "my_led.h"
void my_gpio_self_test(void){

    if(GPIO_Class_test.ON_Status = MY_GPIO_Read(GPIO_Class_test)){
        // MY_GPIO_OFF(GPIO_Class_test);
        RED_OFF();
        printf("red\r\n");
    }else{
        // MY_GPIO_ON(GPIO_Class_test);
        YELLOW_OFF();
        printf("yellow\r\n");
    }
    HAL_Delay(100);

    // MY_GPIO_OFF(GPIO_Class_test);
    MY_GPIO_Toggle(GPIO_Class_test);
    printf("self test\r\n");
    HAL_Delay(1000);
}

