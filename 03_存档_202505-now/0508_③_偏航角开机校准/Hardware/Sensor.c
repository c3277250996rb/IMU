#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "motor.h"


//STEER4 		--> PB12 --> R2  红线
//STEER3 		--> PB13 --> R1  橘线
//			 		--> PB14  --> M0  黄线
//STEER1 		--> PB15  --> L1  绿线
//ENCODE1_A --> PA8	 --> L2  棕线


float Kp_sensor = 8.134, Ki_sensor = 0.021, Kd_sensor = 2.36;//pid弯道参数参数 
float sensor_bias = 0;
float sensor_bias_last = 0;
float P , I , D ,PID_value,PWM_value_R,PWM_value_L;  //pid相关参数
int decide;


void Sensor_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启A时钟  PA8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启B时钟  PB12 PB13 PB14 PB15
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void car_ggg(void)
{
    
    
     if((L2 == 0)&&(L1 == 0)&&(M0 == 0)&&(R1 == 0)&&(R2 == 0))// 0 0 0 00-
	{
		car_stop();
	}
    else if((L1 == 0)&&(M0 == 1) && (R1 == 0))// 0 1 0 -
	{
		car_go();
	}
    else if((L1 == 1)&&(M0 == 1) && (R1 == 1))// 0 1 0 -
	{
		car_go();
	}

	else if(L2 == 1)
    {
		car_left();
	}
	else if(R2 == 1)// 0 1 1 
	{
		car_right();
	}
    
    	else if((L1 == 1 )&&(M0 == 0) && (R1 == 0))// 1 0 0 
	{
		car_left();
	}
	else if((L1 == 0 )&&(M0 == 0) && (R1 == 1))// 0 0 1 
	{
		car_right();
	}

}

