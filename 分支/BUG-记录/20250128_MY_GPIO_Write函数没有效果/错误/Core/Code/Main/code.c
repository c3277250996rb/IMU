#include "code.h"

void CODE_MAIN(){
    Open_UART1_Receive_IT();
    CODE_WHILE();
}

void CODE_WHILE(){
    BLUE_ON();
    HAL_Delay(1000);
    // int i1;
    while(1){

        my_gpio_self_test();

    }

}



