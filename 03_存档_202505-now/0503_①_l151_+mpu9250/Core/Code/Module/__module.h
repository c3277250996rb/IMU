#ifndef __MODULE_H
#define __MODULE_H


    #include "my_i2c.h"
    #include "multi_printf.h"
    #include "my_qmc5883.h"
    #include "my_mpu9250.h"
     

    #define LSM_for_1_and_MPU_for_0 0

    
    #define IMU_Init() {\
        if (LSM_for_1_and_MPU_for_0){\
            QMC5883_Init();\
            /*LSM6DS3_Init();*/\
        }else{\
            MPU9250_Init();\
        }\
    }


#endif // !__MODULE_H    

