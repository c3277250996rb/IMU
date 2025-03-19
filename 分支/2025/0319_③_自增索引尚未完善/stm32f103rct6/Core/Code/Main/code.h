#ifndef __CODE_H
#define __CODE_H

    #include "main.h"
    
    

    #include "my_gpio.h"
    #include "my_led.h"
    #include "my_usart.h"
    #include "my_qmc5883.h"
    #include "my_lsm6ds3.h"
    #include "my_spi.h"
    #include "my_w25q64.h"

    #include "quaternion.h"
    #include "muti_printf.h"
    #include "convert_string.h"

	#include "test.h"

    void CODE_MAIN();
    void CODE_INIT();
    void CODE_SELF_TEST();
    void CODE_WHILE();

#endif // !__CODE_H




