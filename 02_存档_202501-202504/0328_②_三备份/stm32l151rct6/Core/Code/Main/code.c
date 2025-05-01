/*金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮*/

#include "code.h"

void CODE_MAIN(){
    CODE_INIT();
    CODE_SELF_TEST();
    CODE_WHILE();
}

void CODE_INIT(){
    UART1_APP_Init();  // init UART1
    // MPU9250_Init();  // init IMU
    QMC5883_Init();  // init Mag
    LSM6DS3_Init();  // init Accel & Gyro
    SPI_APP_Init(SPI_Class_1);
    MALLOC_APP_Init();
    FATFS_Init();
}

void CODE_SELF_TEST(){
    printf("启动成功!\r\n");
    printf("野火-F103-迷你\r\n");
    while(1){
        delay_ms(1000);
        // my_w25q64_self_test();
        // my_test();
        my_gpio_self_test();
        // my_led_self_test();
        // my_usart_self_test();
        // my_mpu9250_self_test();
        // my_qmc5883_self_test();
        // my_lsm6ds3_self_test();
        // quaternion_self_test();
        // muti_printf_self_test();
    }
}

void CODE_WHILE(){
    int i1;

    while(1){
        BLUE_Toggle();
        delay_ms(1000);
    }

}



