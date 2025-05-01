/*金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮*/

#include "code.h"

void CODE_MAIN(){
    CODE_INIT();
    CODE_SELF_TEST();
    CODE_WHILE();
}

void CODE_INIT(){
    UART1_APP_Init();  // init UART1
    I2C_APP_Init();  // init I2C
    // MPU9250_Init();  // init IMU
    QMC5883_Init();  // init Mag
    // LSM6DS3_Init();  // init Accel & Gyro
    SPI_APP_Init(SPI_Class_1);  // init SPI
    // MALLOC_Init();  // init MALLOC
    // FATFS_Init();  // init FATFS
}

void CODE_SELF_TEST(){
    printf("启动成功!\r\n");
    printf("四季-L151-核心\r\n");
    while(1){
        // for(int i = 1000*1000; i--; i > 0);
        my_w25q64_self_test();
        // my_test();
        // my_gpio_self_test();
        // my_led_self_test();
        // my_usart_self_test();
        // my_mpu9250_self_test();
        // my_qmc5883_self_test();
        // my_lsm6ds3_self_test();
        // quaternion_self_test();
        // muti_printf_self_test();
        // atk_malloc_self_test();
    }
}

void CODE_WHILE(){
    int i1;

    while(1){
        // BLUE_Toggle();
        // delay_ms(1000);
    }

}



