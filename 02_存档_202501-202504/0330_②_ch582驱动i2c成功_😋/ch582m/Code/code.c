#include "code.h"

void CODE_MAIN(){
    CODE_INIT();
    CODE_SELF_TEST();
    CODE_WHILE();
}

void CODE_INIT(){
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    USART_APP_Init();
    QMC5883_Init();
    // LSM6DSOW_APP_Init();
    // LIS2MDLTR_APP_Init();
    // LSM6DSV16X_APP_Init();
}

void CODE_SELF_TEST(){
    while(1){
        // my_test();
        // my_usart_self_test();
        // my_i2c_self_test();
        my_qmc5883_self_test();
        // my_lsm6dsow_self_test();
        // my_lis2mdltr_self_test();
        // my_lsm6dsv16x_self_test();
    }
}

void CODE_WHILE(){
    int i1;
    while(1){
        
        DelayMs(1000);
    }
}


void my_test(){
    multi_printf_self_test();
}
