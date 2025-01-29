#include "code.h"

void CODE_MAIN(){
    Open_UART1_Receive_IT();
    CODE_SELF_TEST();
    CODE_WHILE();
}

void CODE_SELF_TEST(){
    while(1){
        // my_gpio_self_test();
        my_led_self_test();
    }
}

void CODE_WHILE(){
    int i1;

    while(1){


    }

}



