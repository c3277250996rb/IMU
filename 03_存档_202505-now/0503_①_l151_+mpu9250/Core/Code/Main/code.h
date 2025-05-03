#ifndef __CODE_H
#define __CODE_H

    #include "main.h"
    
    

    #include "my_gpio.h"
    // #include "my_led.h"
    #include "my_usart.h"
    #include "my_qmc5883.h"
    // #include "my_lsm6ds3.h"
    #include "my_mpu9250.h"
    // #include "my_spi.h"
    // #include "my_w25q64.h"
    // #include "my_fatfs.h"

    #include "__interface.h"
    #include "__module.h"

    // #include "quaternion.h"
    // #include "multi_printf.h"
    // #include "convert_string.h"

    #include "atk_malloc.h"

	#include "test.h"


    #define Dev_for_3_and_Core_for_2_and_L151_for_1 1


    #define POWER_ON() { \
        printf("启动成功!\r\n"); \
        switch(Dev_for_3_and_Core_for_2_and_L151_for_1){ \
            case 3: \
                printf("野火-F103-迷你\r\n"); \
                break; \
            case 2: \
                printf("四季-F103-核心\r\n"); \
                break; \
            case 1: \
                printf("四季-L151-核心\r\n"); \
                break; \
        } \
    }


    void CODE_MAIN();
    void CODE_INIT();
    void CODE_SELF_TEST();
    void CODE_WHILE();

#endif // !__CODE_H




