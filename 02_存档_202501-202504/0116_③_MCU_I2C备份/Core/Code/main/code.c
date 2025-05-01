#include "code.h"
void CODE_MAIN();
void CODE_WHILE();


void CODE_MAIN(){

    
    CODE_WHILE();
}

void CODE_WHILE(){

    while(1){

        LED_PC13_read == LED_PC13_on_status ? LED_PC13_off : LED_PC13_on;
        LED_PA0_read == LED_PA0_on_status ? LED_PA0_off : LED_PA0_on;
        LED_PB13_read == LED_PB13_on_status ? LED_PB13_off : LED_PB13_on;
        HAL_Delay(1000);

    }

}



