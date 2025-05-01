/*
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



#include "IMU.h"

//int16_t accel[3], gyro[3];
float angles[3];
volatile float q0, q1, q2, q3; 
uint8_t u8PressureType;
IMU_ST_SENSOR_DATA gstMagOffset = {0,0,0};
/**
  * @brief  invSqrt
  * @param  
  * @retval 
  */

float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	
	long i = *(long*)&y;                //get bits for floating value
	i = 0x5f3759df - (i >> 1);          //gives initial guss you
	y = *(float*)&i;                    //convert bits back to float
	y = y * (1.5f - (halfx * y * y));   //newtop step, repeating increases accuracy
	
	return y;
}


/**
  * @brief  initializes IMU
  * @param  None
  * @retval None
  */
void IMU_Init(void)
{	
    uint8_t u8Ret; 

    invMSInit();

    u8Ret = BMP280_Init();
    // if(u8Ret == BMP280_RET_OK)
    {
			  u8PressureType = IMU_PRES_TYPE_BM280;
        printf("\r\nPressure type is BM280\r\n");
    }
    // else
    // {
	// 		  u8PressureType = IMU_PRES_TYPE_ER;
    //     printf("\r\nNo BMP280 chip was detected\r\n");

    // }
	
  	q0 = 1.0f;  
  	q1 = 0.0f;
  	q2 = 0.0f;
  	q3 = 0.0f;
}

#define Kp 4.50f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 1.0f    // integral gain governs rate of convergence of gyroscope biases
// #define Ki 1250.0f    // integral gain governs rate of convergence of gyroscope biases
// #define Ki 0.00001f    // integral gain governs rate of convergence of gyroscope biases

/**
  * @brief  Updata attitude and heading 
  * @param  ax: accelerometer X
  * @param  ay: accelerometer Y
  * @param  az: accelerometer Z
  * @param  gx: gyroscopes X
  * @param  gy: gyroscopes Y
  * @param  gz: gyroscopes Z
  * @param  mx: magnetometer X
  * @param  my: magnetometer Y
  * @param  mz: magnetometer Z
  * @retval None
  */
void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) 
{
	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float exInt = 0.0, eyInt = 0.0, ezInt = 0.0;
	float ex, ey, ez, halfT = 0.024f;

	float q0q0 = q0 * q0;
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q0q3 = q0 * q3;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;   
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;          

/* 归一化 */
  /* 求模 */
	norm = invSqrt(ax * ax + ay * ay + az * az);       
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;
  /* 求模 */
	norm = invSqrt(mx * mx + my * my + mz * mz);          
	mx = mx * norm;
	my = my * norm;
	mz = mz * norm;

/* 磁场水平 垂直分量计算 */
  /* 旋转矩阵 */
	hx = 2 * mx * (0.5f - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
	hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5f - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
	hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5f - q1q1 - q2q2);         
  /* 计算水平和垂直分量 */
	bx = sqrt((hx * hx) + (hy * hy));
	bz = hz;     

/* 将重力从物体坐标系转换到世界坐标系 */
  /* 计算分量 */
	vx = 2 * (q1q3 - q0q2);
	vy = 2 * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;

/* 将磁力从物体坐标系转换到世界坐标系 */
  /* 计算分量 */
	wx = 2 * bx * (0.5 - q2q2 - q3q3) + 2 * bz * (q1q3 - q0q2);
	wy = 2 * bx * (q1q2 - q0q3) + 2 * bz * (q0q1 + q2q3);
	wz = 2 * bx * (q0q2 + q1q3) + 2 * bz * (0.5 - q1q1 - q2q2);  

/* 计算误差 叉积*/
  /* 通过加速计观测值 世界磁力计 世界重力 */
	ex = (ay * vz - az * vy) + (my * wz - mz * wy);
	ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
	ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

	if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
	{
/* 积分 累加误差 Ki*/
		exInt = exInt + ex * Ki * halfT;
		eyInt = eyInt + ey * Ki * halfT;	
		ezInt = ezInt + ez * Ki * halfT;

/* 消除误差 */
		gx = gx + Kp * ex + exInt;
		gy = gy + Kp * ey + eyInt;
		gz = gz + Kp * ez + ezInt;
	}

/* 更新四元数 */
	q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
	q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
	q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
	q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;  

	norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
}


/**
  * @brief  Get quaters
  * @param  None
  * @retval None
  */
void IMU_GetQuater(void)
{
	  float MotionVal[9];
    int16_t s16Gyro[3], s16Accel[3], s16Magn[3];

    invMSAccelRead(&s16Accel[0], &s16Accel[1], &s16Accel[2]);
    invMSGyroRead(&s16Gyro[0], &s16Gyro[1], &s16Gyro[2]);
    invMSMagRead(&s16Magn[0], &s16Magn[1], &s16Magn[2]);
  
    //for log
    gyro[0] = s16Gyro[0];
    gyro[1] = s16Gyro[1];
    gyro[2] = s16Gyro[2];
    accel[0] = s16Accel[0];
    accel[1] = s16Accel[1];
    accel[2] = s16Accel[2];

    magn[0] = s16Magn[0] - gstMagOffset.s16X;
    magn[1] = s16Magn[1] - gstMagOffset.s16Y;
    magn[2] = s16Magn[2] - gstMagOffset.s16Z;
    
#ifndef lsm
    MotionVal[0]=gyro[0] / 32.8 * 0.075 / 11;
    MotionVal[1]=gyro[1] / 32.8 * 0.075 / 11;
    MotionVal[2]=gyro[2] / 32.8 * 0.075 / 11;
    MotionVal[3]=accel[0] ;
    MotionVal[4]=accel[1] ;
    MotionVal[5]=accel[2] ;
    MotionVal[6]=magn[0] ;
    MotionVal[7]=magn[1] ;
    MotionVal[8]=magn[2] ;

    // MotionVal[0]=gyro[0] * 8.75 / 1000.0;
    // MotionVal[1]=gyro[1] * 8.75 / 1000.0;
    // MotionVal[2]=gyro[2] * 8.75 / 1000.0;
    // MotionVal[3]=accel[0] * 0.061 / 1000.0;
    // MotionVal[4]=accel[1] * 0.061 / 1000.0;
    // MotionVal[5]=accel[2] * 0.061 / 1000.0;
    // MotionVal[6]=magn[0] / 3000;
    // MotionVal[7]=magn[1] / 3000;
    // MotionVal[8]=magn[2] / 3000;

 	IMU_AHRSupdate((float)MotionVal[0]  , (float)MotionVal[1]  , (float)MotionVal[2]  ,
   	(float)MotionVal[3], (float)MotionVal[4], (float)MotionVal[5], (float)MotionVal[6], (float)MotionVal[7], MotionVal[8]);
#else
    MotionVal[0]=gyro[0];
    MotionVal[1]=gyro[1];
    MotionVal[2]=gyro[2];
    MotionVal[3]=accel[0];
    MotionVal[4]=accel[1];
    MotionVal[5]=accel[2];
    MotionVal[6]=magn[0];
    MotionVal[7]=magn[1];
    MotionVal[8]=magn[2];

 	IMU_AHRSupdate((float)MotionVal[0] * g, (float)MotionVal[1] * g, (float)MotionVal[2] * g,
   	(float)MotionVal[3] * a, (float)MotionVal[4] * a, (float)MotionVal[5] * a, (float)MotionVal[6], (float)MotionVal[7], MotionVal[8]);
#endif

}


/**
  * @brief  Get Yaw Pitch Roll
  * @param  None
  * @retval None
  */
void IMU_GetYawPitchRoll(float *Angles) 
{
  IMU_GetQuater(); 
	Angles[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
	Angles[2] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	Angles[0] = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3;   // yaw
  
  // accel[0] *= 0.061 / 1000.0;
  // accel[1] *= 0.061 / 1000.0;
  // accel[2] *= 0.061 / 1000.0;

  // accel[0] = accel[0] - sin(Angles[2]) * cos(Angles[1]) * 9.81;
  // accel[1] = accel[1] - cos(Angles[2]) * cos(Angles[1]) * 9.81;
  // accel[2] = accel[2] + sin(Angles[1]) * 9.81;
}

/******************* (C) COPYRIGHT 2014 Waveshare *****END OF FILE*******************/

