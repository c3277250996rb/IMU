#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f10x.h"
/*
bilibili СŬ�� UID:437280309
@timeʱ��: 2024.12.4
@version�汾:V1_1
@attention : ûʲôֵ��ע��ģ�ע��Ű��߾ͺ���

if you can't display Chinese correctly,please check your encoding mode(please set encoding mode to GB2312)
*/

extern struct I2C_BUS MPU6050_I2C; 

#define MPU6050_ADDRESS		      0x68//i2c address
//**************************** register below
#define	MPU6050_SMPLRT_DIV		  0x19
#define	MPU6050_CONFIG			  0x1A
#define	MPU6050_GYRO_CONFIG		  0x1B
#define	MPU6050_ACCEL_CONFIG	  0x1C
#define MPU6050_FIFO_EN           0x23

#define MPU6050_INTBP_CFG_REG	  0X37	//�жϼĴ���
#define MPU6050_INT_ENABLE        0x38

#define	MPU6050_ACCEL_XOUT_H	  0x3B
#define	MPU6050_ACCEL_XOUT_L	  0x3C
#define	MPU6050_ACCEL_YOUT_H	  0x3D
#define	MPU6050_ACCEL_YOUT_L	  0x3E
#define	MPU6050_ACCEL_ZOUT_H	  0x3F
#define	MPU6050_ACCEL_ZOUT_L	  0x40
#define	MPU6050_TEMP_OUT_H		  0x41
#define	MPU6050_TEMP_OUT_L		  0x42
#define	MPU6050_GYRO_XOUT_H		  0x43
#define	MPU6050_GYRO_XOUT_L		  0x44
#define	MPU6050_GYRO_YOUT_H		  0x45
#define	MPU6050_GYRO_YOUT_L		  0x46
#define	MPU6050_GYRO_ZOUT_H		  0x47
#define	MPU6050_GYRO_ZOUT_L		  0x48
#define MPU6050_SIGNAL_PATH_RESET 0x68

#define MPU6050_USER_CTRL         0x6A
#define	MPU6050_PWR_MGMT_1		  0x6B
#define	MPU6050_WHO_AM_I		  0x75
//������ԭ�����ݽṹ��
typedef struct MPU6050_raw
{
    int16_t AccX;
    int16_t AccY;
    int16_t AccZ;
    int16_t GyroX;
    int16_t GyroY;
    int16_t GyroZ;
    uint16_t Temp;
}MPU6050_raw;
//�����ǽǶȽṹ��
typedef struct MPU6050
{
    float yaw;
    float roll;
    float pitch;
}MPU6050;

void MPU6050_init(GPIO_TypeDef* GPIOx,uint16_t SCl,uint16_t SDA);   //��ʼ��������
void MPU6050_Get_Raw(MPU6050_raw* this);                            //�õ�ԭ������
void MPU6050_Get_Angle(MPU6050* this);                              //�õ��Ƕ�
float MPU6050_GetTemp(void);                                            //�õ��¶�
uint8_t MPU6050_ID(void);                                               //��ȡID

#endif
