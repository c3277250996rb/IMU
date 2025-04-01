#include "select_GPIO.h"


GPIO_PinState select_GPIO_status(int label, int action){
    int temp_PinState;

    // on
    switch (label){
        case LED_PC13:
            temp_PinState = LED_PC13_on_status;
            break;
        
        default:
            break;
    }

    // off
    if(action == LED_OFF){
        temp_PinState = 1 - temp_PinState;
    }

		HAL_Delay(1);
    return temp_PinState;
}



