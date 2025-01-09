
#include "System.h"
#include <stdlib.h>

#define PI 3.14159265358979323846
int16_t TestmagnBuff[9]={0};
double world_accelerate[6];
int i = 0;
bool Flag_Check_Magn= false; 
extern uint32_t KeyValue;
extern MPU9250_TypeDef_Off MPU9250_Magn_Offset;
void TestMagn(void); 
void rotate_acceleration();


int main(void)
{
    System_Init();	//系统初始化 --- 串口波特率115200 I2C初始化 传感器初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    LedGpioConfiguration();
    KEYGpioConfiguration();
	
	  GPIO_SetBits(IO_LED_ARRAY, IO_RS);
	  GPIO_SetBits(IO_LED_ARRAY, IO_sysLED);//IO_LED1 输出高电平 LED1灭

	while(1)
	{	
		// if(!Flag_Check_Magn)
		// {
		// 	if(CORREC_KEYSCAN())
		// 	{
		// 		TestMagn();
		// 		LedShow();	
		// 	}
		// }
		// if(IMU_SampleFlag)
		// {
			IMU_GetYawPitchRoll(angles);	
	// 		IMU_SampleFlag=0;
	// 	}
	// 	if(Press_SampleFlag&&u8PressureType==IMU_PRES_TYPE_BM280)
	// 	{ 				
    //   BMP280_CalTemperatureAndPressureAndAltitude(&TemperatureVal, &PressureVal, &AltitudeVal);
	// 		Press_SampleFlag=0;
	// 	}
	// 	if(UART_UpdataFlag)
	// 	{		

/*			printf("\r\n /-------------------------------------------------------------/ \r\n");
			printf("\r\n Roll: %.2f     Pitch: %.2f     Yaw: %.2f \r\n",angles[2],angles[1],angles[0]);
			printf("\r\n Acceleration: X: %d     Y: %d     Z: %d \r\n",accel[0],accel[1],accel[2]);
			printf("\r\n Gyroscope: X: %d     Y: %d     Z: %d \r\n",gyro[0],gyro[1],gyro[2]);
 			printf("\r\n Magnetic: X: %d     Y: %d     Z: %d \r\n",magn[0],magn[1],magn[2]);
 			printf("\r\n Pressure: %.2f     Altitude: %.2f \r\n",(float)PressureVal / 100, (float)AltitudeVal / 100);
 			printf("\r\n Temperature: %.1f \r\n", (float)TemperatureVal / 10); */
			
			// printf("channels:%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\r\n", 
			// angles[2], angles[1], angles[0], 
			// ((float)accel[0]) , ((float)accel[1]) , ((float)accel[2]) , 
			// 	((float)gyro[0]) , ((float)gyro[1]) , ((float)gyro[2]) );
	



			rotate_acceleration((double)accel[0], (double)accel[1], (double)accel[2], \
								(double)angles[2], (double)angles[1], (double)angles[0], \
								&world_accelerate[0], &world_accelerate[1], &world_accelerate[2]);

			// oa
			printf("%d,%d,%d", accel[0], accel[1], accel[2]);

			// ra
			// printf("%f,%f,%f", world_accelerate[0], world_accelerate[1], world_accelerate[2]);

			// angle
			printf(",%f,%f,%f\r\n", angles[2], angles[1], angles[0]);

			// PID_data
			// for(i = 0; i < 8; i++){
			// 	printf("%f,", PID_data[i]);
			// }
			// printf("%f\r\n", PID_data[8]);
		// printf("111\r\n");
		// Delay_ms(500);



	// 		UART_UpdataFlag = 0;
	// 	}
	}
}
/**
  * @brief  check magn
  * @param  None
  * @retval  none
  */
void TestMagn(void)
{
	TestmagnBuff[(KeyValue-1)*3]=magn[0];
	TestmagnBuff[(KeyValue-1)*3+1]=magn[1];
	TestmagnBuff[(KeyValue-1)*3+2]=magn[2];
	
	if(KeyValue>=3)
	{
			gstMagOffset.s16X = (TestmagnBuff[0]+TestmagnBuff[3])/2;
			gstMagOffset.s16Y = (TestmagnBuff[1]+TestmagnBuff[4])/2;
			gstMagOffset.s16Z = (TestmagnBuff[5]+TestmagnBuff[8])/2;
			Flag_Check_Magn=true;
	}
}
/******************* (C) COPYRIGHT 2014 Waveshare *****END OF FILE*******************/


// 旋转矩阵
#if 20241208
double deg_to_rad(double deg) {
    return deg * PI / 180.0;
}

// 旋转矩阵和加速度变换
void rotate_acceleration(double ax, double ay, double az, double in_roll, double in_pitch, double in_yaw, double *ax_world, double *ay_world, double *az_world) {
    // 将欧拉角转成弧度
    double roll = deg_to_rad(in_roll);
    double pitch = deg_to_rad(in_pitch);
    double yaw = deg_to_rad(in_yaw);

    // 计算旋转矩阵的元素
    double cos_roll = cos(roll), sin_roll = sin(roll);
    double cos_pitch = cos(pitch), sin_pitch = sin(pitch);
    double cos_yaw = cos(yaw), sin_yaw = sin(yaw);

    // 旋转矩阵 R = Rz * Ry * Rx
    double R[3][3];

	R[0][0] = cos_yaw * cos_pitch;
	R[0][1] = cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll;
	R[0][2] = cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll;

	R[1][0] = sin_yaw * cos_pitch;
	R[1][1] = sin_yaw * sin_pitch * sin_roll + cos_yaw * cos_roll;
	R[1][2] = sin_yaw * sin_pitch * cos_roll - cos_yaw * sin_roll;

	R[2][0] = -sin_pitch;
	R[2][1] = cos_pitch * sin_roll;
	R[2][2] = cos_pitch * cos_roll;
	

    // // 世界坐标系下的加速度 a' = R * a
    *ax_world = R[0][0] * ax + R[0][1] * ay + R[0][2] * az;
    *ay_world = R[1][0] * ax + R[1][1] * ay + R[1][2] * az;
    *az_world = R[2][0] * ax + R[2][1] * ay + R[2][2] * az;
}


#endif






