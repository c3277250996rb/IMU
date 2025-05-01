#include "code.h"

void CODE_MAIN(){
    CODE_INIT();
    CODE_SELF_TEST();
    CODE_WHILE();
}

void CODE_INIT(){
    // DWT_Delay_Init();  // init DWT
    // Open_UART1_Receive_IT();  // init UART1
    // MPU9250_Init();  // init IMU
}

void CODE_SELF_TEST(){
    while(1){
        // my_test();
        // my_gpio_self_test();
        my_led_self_test();
        // my_usart_self_test();
        // my_mpu9250_self_test();
    }
}

void CODE_WHILE(){
    int i1;

    while(1){
        BLUE_Toggle();
        // delay_ms(1000);
    }

}



