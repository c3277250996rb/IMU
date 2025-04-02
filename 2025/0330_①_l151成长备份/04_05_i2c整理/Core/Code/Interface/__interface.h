#ifndef __INTERFACE_H
#define __INTERFACE_H

    #include "main.h"

    #include <stdbool.h>
    #include <stdio.h>
    #include <string.h>
    #include <math.h>


    #define USE_ATK_SYSTEM

    #define OD_for_1_and_PP_for_0  0

    /**
     * @brief Debug i2c or not.
     * @param USE_DEBUG_I2C:
     *        @arg 0: Disable;
     *        @arg 1: QMC5883;
     *        @arg 2: LSM6DS3;
     *        @arg 3: MPU9250
     */
    #define USE_DEBUG_I2C 0
    
#endif // !__INTERFACE_H 
