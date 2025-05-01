
#include "System.h"

int16_t TestmagnBuff[9]={0};
bool Flag_Check_Magn= false; 
extern uint32_t KeyValue;
extern MPU9250_TypeDef_Off MPU9250_Magn_Offset;
void TestMagn(void); 


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
		if(!Flag_Check_Magn)
		{
			if(CORREC_KEYSCAN())
			{
				TestMagn();
				LedShow();	
			}
		}
		if(IMU_SampleFlag)
		{
			IMU_GetYawPitchRoll(angles);	
			IMU_SampleFlag=0;
		}
		if(Press_SampleFlag&&u8PressureType==IMU_PRES_TYPE_BM280)
		{ 				
      BMP280_CalTemperatureAndPressureAndAltitude(&TemperatureVal, &PressureVal, &AltitudeVal);
			Press_SampleFlag=0;
		}
		if(UART_UpdataFlag)
		{		

			printf("\r\n /-------------------------------------------------------------/ \r\n");
			printf("\r\n Roll: %.2f     Pitch: %.2f     Yaw: %.2f \r\n",angles[2],angles[1],angles[0]);
			printf("\r\n Acceleration: X: %d     Y: %d     Z: %d \r\n",accel[0],accel[1],accel[2]);
			printf("\r\n Gyroscope: X: %d     Y: %d     Z: %d \r\n",gyro[0],gyro[1],gyro[2]);
 			printf("\r\n Magnetic: X: %d     Y: %d     Z: %d \r\n",magn[0],magn[1],magn[2]);
 			printf("\r\n Pressure: %.2f     Altitude: %.2f \r\n",(float)PressureVal / 100, (float)AltitudeVal / 100);
 			printf("\r\n Temperature: %.1f \r\n", (float)TemperatureVal / 10);
	
			UART_UpdataFlag = 0;
		}
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
