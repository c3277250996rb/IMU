/**
  ******************************************************************************
  * @file    IMU.h
  * @author  Waveshare Team
  * @version V1.0
  * @date    29-August-2014
  * @brief   This file contains all the functions prototypes for the IMU firmware 
  *          library.

  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */



#ifndef __IMU_H
#define __IMU_H

#include "Public_StdTypes.h"
#include "MPU9250.h"
#include "BMP280.h"
#include "invMotionSensor.h"

#define PI 3.14159265358979323846
#define IMU_PRES_TYPE_ER     0
#define IMU_PRES_TYPE_BM280     1

typedef struct imu_st_sensor_data_tag
{
	int16_t s16X;
	int16_t s16Y;
	int16_t s16Z;
}IMU_ST_SENSOR_DATA;

extern int16_t accel[3], gyro[3];
extern float angles[3];
extern uint8_t u8PressureType;
extern IMU_ST_SENSOR_DATA gstMagOffset;

extern void IMU_Init(void); 
extern void IMU_GetYawPitchRoll(float *Angles) ;

static double a = 1, g = 1;
// #define lsm
#define alpha_before_pid

#endif

/******************* (C) COPYRIGHT 2014 Waveshare *****END OF FILE*******************/

