#ifndef __QUATERNION_H
#define __QUATERNION_H

#include "my_qmc5883.h"
#include "my_lsm6ds3.h"
#include "my_fatfs.h"
#include "atk_delay.h"
#include "data_management.h"

#define Kp 4.5
#define Ki 1.0

#define QUATRE_LSB_ACCEL (float) \
    1.0  /1000000  *61  *16384

#define QUATER_LSB_GYRO (float) \
    1.0  /1000000  *3500  *0.075

#define QUATER_LSB_MAG (float) \
    1.0  *1000  /6  /3000  

extern float euler_angle[3];
extern float buffer_9axes[9];

extern float DM_angle_record[Cache_Max_Row][Info_Column];
extern uint8_t Q_current_index;
extern uint8_t is_Q_stop;
enum
{
    enum_Stop = 0,
    enum_Ready,
    eunm_Continuous
};

void imu_convert_9axes(void);
void imu_get_euler(void);


#endif
