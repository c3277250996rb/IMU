#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f10x.h"
#include "my_i2c.h"

#define MPU6050_ADDRESS  0x68  // Ĭ�� I2C ��ַ��δ�� AD0 ʱ��
#define Kp 4.5
#define Ki 1.0

#define QUATRE_LSB_ACCEL (float) \
        1.0  

    #define QUATER_LSB_GYRO (float) \
        1.0  /32.8 *0.0175

    #define QUATER_LSB_MAG (float) \
        1.0  
extern int16_t mpu6050_accel[3];
extern int16_t mpu6050_gyro[3];
extern int8_t mpu6050_temp;
extern float euler_angle[3];                       // ���������� - ŷ�������飨roll, pitch, yaw��
extern float buffer_9axes[9];                      // ��ʱ���洫�������� - �洢9�ᴫ����ת���������

uint8_t mpu6050_i2c_read(I2C_DEPLOY *bus, uint8_t RegisterAddress);

uint8_t MPU6050_Init(I2C_DEPLOY *bus);
void mpu6050_read_all(I2C_DEPLOY *bus);

void imu_get_euler(void);

void imu_convert_9axes(void);

void imu_get_quater(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);

float invSqrt(float x);

#endif
