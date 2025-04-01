
#include "System.h"

int16_t TestmagnBuff[9]={0};
bool Flag_Check_Magn= false; 
extern uint32_t KeyValue;
extern MPU9250_TypeDef_Off MPU9250_Magn_Offset;
void TestMagn(void); 

void mag();
void mag_init();
void lsm();
void lsm_init();
void release_gravity();

int main(void)
{
	int i = 0;

    System_Init();	//系统初始化 --- 串口波特率115200 I2C初始化 传感器初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    LedGpioConfiguration();
    KEYGpioConfiguration();
	  GPIO_SetBits(IO_LED_ARRAY, IO_RS);
	  GPIO_SetBits(IO_LED_ARRAY, IO_sysLED);//IO_LED1 输出高电平 LED1灭

	mag_init();
	lsm_init();
	Delay_ms(10);

#ifndef lsm
    a = 0.0175;
    g = 1.0 / 32.8;
#else
    a = 0.061 * 1.000 / 1000.000;
    g = 4.375 * 2.000 / 1000.000;
#endif

	while(1)
	{	
		if(!Flag_Check_Magn)
		{
			if(CORREC_KEYSCAN())
			{
				TestMagn();
				LedShow();	
			}
		}
		// if(IMU_SampleFlag)
		{
			IMU_GetYawPitchRoll(angles);	
			IMU_SampleFlag=0;
		}
		// if(Press_SampleFlag&&u8PressureType==IMU_PRES_TYPE_BM280)
		// { 				
      	// 	BMP280_CalTemperatureAndPressureAndAltitude(&TemperatureVal, &PressureVal, &AltitudeVal);
		// 	Press_SampleFlag=0;
		// }
		// if(UART_UpdataFlag)
		{		

			// printf("\r\n /-------------------------------------------------------------/ \r\n");
			// printf("\r\n Roll: %.2f     Pitch: %.2f     Yaw: %.2f \r\n",angles[2],angles[1],angles[0]);
			// printf("\r\n Acceleration: X: %d     Y: %d     Z: %d \r\n",accel[0],accel[1],accel[2]);
			// printf("\r\n Gyroscope: X: %d     Y: %d     Z: %d \r\n",gyro[0],gyro[1],gyro[2]);
			// printf("\r\n Magnetic: X: %d     Y: %d     Z: %d \r\n\r\n",magn[0],magn[1],magn[2]);
 			// printf("\r\n Pressure: %.2f     Altitude: %.2f \r\n",(float)PressureVal / 100, (float)AltitudeVal / 100);
 			// printf("\r\n Temperature: %.1f \r\n", (float)TemperatureVal / 10);
			
			// printf("channels:%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\r\n", angles[2], angles[1], angles[0] - 30.0/20.0, ((float)accel[0]) * a, ((float)accel[1]) * a, ((float)accel[2]) * a, ((float)gyro[0]) * g, ((float)gyro[1]) * g, ((float)gyro[2]) * g);
			printf("channels:%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\r\n", angles[2], angles[1], angles[0] , ((float)accel[0]) * a, ((float)accel[1]) * a, ((float)accel[2]) * a, ((float)gyro[0]) * g, ((float)gyro[1]) * g, ((float)gyro[2]) * g);
	
			UART_UpdataFlag = 0;
		}

		// mag();
		// lsm();

		if(i++ < 200){

		}
		Delay_ms(10);
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
	
	// TestmagnBuff[3]=magn[0];
	// TestmagnBuff[3+3]=magn[1];
	// TestmagnBuff[3+6]=magn[2];
	
	// if(KeyValue>=3)
	{
			gstMagOffset.s16X = (TestmagnBuff[0]+TestmagnBuff[3])/2;
			gstMagOffset.s16Y = (TestmagnBuff[1]+TestmagnBuff[4])/2;
			gstMagOffset.s16Z = (TestmagnBuff[5]+TestmagnBuff[8])/2;
			Flag_Check_Magn=true;
	}
}
/******************* (C) COPYRIGHT 2014 Waveshare *****END OF FILE*******************/



void mag(){
    int i, BUF[6], X, Y, Z;
	switch_lq(1);
    for(i = 0; i < 6; i++){
        BUF[i] = I2C_ReadOneByte(0x1A, i);
		printf("[%d]: 0x%x  \r", i, BUF[i]);
    }

    X = (BUF[1] << 8) | BUF[0];
    Y = (BUF[3] << 8) | BUF[2];
    Z = (BUF[5] << 8) | BUF[4];
    // printf("X: %f  Y: %f  Z: %f\r\n", X, Y, Z);
}


void mag_init(){
	switch_lq(1);
	I2C_WriteOneByte(0x1A, 0x09, 0x1D);
	I2C_WriteOneByte(0x1A, 0x0B, 0x01);
	I2C_WriteOneByte(0x1A, 0x20, 0x40);
	I2C_WriteOneByte(0x1A, 0x21, 0x01);
}


void lsm(){
	int i;
	switch_lq(0);
	for(i = 0x22; i < 0x22 + 12; i++){
		printf("[%d]: 0x%x  \r", i, I2C_ReadOneByte(0xD6, i));
	}
	printf("\n");
	switch_lq(1);
}


void lsm_init(){
	// MyI2C_Init();
	switch_lq(0);
	I2C_WriteOneByte(0xD6, 0x18, 0x38);

	I2C_WriteOneByte(0xD6, 0x19, 0x38);

	I2C_WriteOneByte(0xD6, 0x10, 0x60);
	// I2C_WriteOneByte(0xD6, 0x10, 0x62);

	I2C_WriteOneByte(0xD6, 0x11, 0x60);
	// I2C_WriteOneByte(0xD6, 0x11, 0x62);

	switch_lq(1);
}


void release_gravity(){
	// float accX = imuIn->linear_acceleration.y - sin(roll)*cos(pitch)*9.81;
	// float accY = imuIn->linear_acceleration.z - cos(roll)*cos(pitch)*9.81;
	// float accZ = imuIn->linear_acceleration.x + sin(pitch)*9.81;

}
