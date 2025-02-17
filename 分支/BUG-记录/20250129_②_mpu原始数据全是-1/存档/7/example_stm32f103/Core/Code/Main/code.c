#include "code.h"

void CODE_MAIN(){
    // DWT_Delay_Init();  // 初始化DWT计数器
    Open_UART1_Receive_IT();
    MPU9250_Init();
    CODE_SELF_TEST();
    CODE_WHILE();
}

void CODE_SELF_TEST(){
    while(1){
        // my_gpio_self_test();
        // my_led_self_test();
        // my_usart_self_test();
        my_mpu9250_self_test();
    }
}

void CODE_WHILE(){
    int i1;

    while(1){
        BLUE_Toggle();
        Delay_ms(1000);
    }

}



