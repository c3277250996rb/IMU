#include "code.h"
#include "zat_usart.h"
void CODE_MAIN();
void CODE_WHILE();


void CODE_MAIN(){
    Open_UART1_Receive_IT();
    CODE_WHILE();
}

void CODE_WHILE(){

    int i1;
    while(1){

        LED_PC13_read == LED_PC13_on_status ? LED_PC13_off : LED_PC13_on;
        LED_PA0_read == LED_PA0_on_status ? LED_PA0_off : LED_PA0_on;
        LED_PB13_read == LED_PB13_on_status ? LED_PB13_off : LED_PB13_on;
        printf("1111\r\n");
        // scanf("%d", &i1);
        if(i1 == 1){
            printf("222\r\n");
        }
        HAL_Delay(1000);

    }

}



