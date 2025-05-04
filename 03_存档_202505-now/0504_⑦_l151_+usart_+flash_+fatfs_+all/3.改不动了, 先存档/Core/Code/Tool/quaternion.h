#ifndef __QUATERNION_H
#define __QUATERNION_H

#include "my_qmc5883.h"
#include "my_lsm6ds3.h"
#include "my_mpu9250.h"
#include "my_fatfs.h"
#include "delay.h"
#include "data_management.h"
#include "windows_filter.h"

#define Kp 4.5
#define Ki 1.0


#define LSM_for_1_and_MPU_for_0 0

#define IMU_Init() {\
    if (LSM_for_1_and_MPU_for_0){\
        QMC5883_Init();\
        LSM6DS3_Init();\
    }else{\
        MPU9250_Init();\
    }\
}

#if LSM_for_1_and_MPU_for_0
// LSM6DS3 + QMC5883
    #define QUATRE_LSB_ACCEL (float) \
        1.0  /1000000  *61  *16384

    #define QUATER_LSB_GYRO (float) \
        1.0  /1000000  *3500  *0.075

    #define QUATER_LSB_MAG (float) \
        1.0  *1000  /6  /3000  

#else
// MPU9250
    #define QUATRE_LSB_ACCEL (float) \
        1.0  

    #define QUATER_LSB_GYRO (float) \
        1.0  /32.8 *0.0175

    #define QUATER_LSB_MAG (float) \
        1.0  

#endif


extern float euler_angle[3];
extern float buffer_9axes[9];

extern float Q_Record_Array[Max_Q_Row][Max_Q_Column];
extern uint8_t Q_Record_Index;
extern uint8_t Q_Record_Flag;
enum
{
    F_Q_Stop = 0,
    F_Q_Ready,
    F_Q_Continuous
};

void imu_convert_9axes(void);
void imu_get_euler(void);

void write_imu_array_to_file(void);

#endif
