#include "code.h"
// #include "my_led.h"
// #include "my_gpio.h"

void CODE_MAIN();
void CODE_WHILE();


void CODE_MAIN(){
    Open_UART1_Receive_IT();
    CODE_WHILE();
}

void CODE_WHILE(){

    // int i1;
    while(1){

        MY_GPIO_ON(LED_Class_BLUE);
        BLUE_ON();
        HAL_Delay(1000);
        GREEN_ON();
        HAL_Delay(1000);
        YELLOW_ON();
        HAL_Delay(1000);
        RED_ON();
        HAL_Delay(1000);

        BLUE_OFF();
        HAL_Delay(1000);
        GREEN_OFF();
        HAL_Delay(1000);
        YELLOW_OFF();
        HAL_Delay(1000);
        RED_OFF();
        HAL_Delay(1000);

    }

}



