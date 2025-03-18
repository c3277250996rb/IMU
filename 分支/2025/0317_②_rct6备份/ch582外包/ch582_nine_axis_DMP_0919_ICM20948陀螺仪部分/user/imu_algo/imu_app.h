/*
 * imu_app.h
 *
 *  Created on: Jul 29, 2024
 *      Author: liu
 */

#ifndef IMU_APP_H_
#define IMU_APP_H_

#include "main.h"


typedef struct
{
    int16_t i_gyro[3];
    int16_t i_acc[3];
    int16_t i_mag[3];
    float f_gyro[3];
    float f_acc[3];
    float f_mag[3];
    float mag_xsf;
    float mag_ysf;
    int use_mag;
    int16_t gyro_zero[3];
    int16_t acc_zero[3];
    int16_t mag_zero[3];

    float pitch;
    float roll;
    float yaw;
    float mag_yaw_test;
    float yaw_temp;
    float velocity[3];
    float displacement[3];
    uint8_t cali_flag;
    float accelerationMagnitude;
    uint8_t shell_cmd_ok;
    uint8_t output_mode;
    uint8_t output_flag;
    uint16_t output_freq;
    uint32_t time_tick;

} imu_t;

bool imu_sample_data(void);
bool imu_final_data_get(void);
void data_output_mode(uint8_t mode) ;
void vofa_FireWater_USB_output(float s1, float s2, float s3,float s4);
void imu_app_timer_1ms_callback(void);

#endif /* IMU_APP_H_ */
