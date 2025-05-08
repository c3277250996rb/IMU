#ifndef EXTEND_HEADFIEL__I2C__H_
#define EXTEND_HEADFIEL__I2C__H_
#include "stm32f10x.h"
#include "generic.h"

/*
bilibili СŬ�� UID:437280309
@time:2024.12.1
@verson:V1_3
@updata: Fixed an issue when multiple I2C cannot be used.Using object oriented programming thinking,Hide a lot lof API function,softerware I2C have arrived 390K
�޸��˶��I2C�޷�ʹ�õ����⡣ʹ���������������˼ά.���ش���API�����ӿڡ����I2C�ٶ��Ѿ��ﵽ390Khz
*/

/*Ӳ��I2C��ʼ����Ӧ����for STM32F103
SCL1��PB6
SDA1��PB7

SCL2��PB10
SDA2��PB11
*/


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

	#define SI2C_delay_time 0															//����??��I2C����ʱ�ٶ�
	#define I2C(obj) (Pthis = &obj)																				//Pthisȫ��ָ��궨??
	typedef struct I2C_Private I2C_Private;		
	typedef struct I2C_BUS
	{
		//�Ͻ�ʹ�ø�ָ��! It is strictly forbidden to use this pointer
		I2C_Private* Private;

		///////////////////////User//////////////////////////////////                   					    //�û�API�����ӿ�
		uint8_t (*AckTest)();																					//��Ӧ�ӿ�,it can check that if our I2C bus is init succese
		void (*Write_Reg)(uint8_t RegAddress, uint8_t Data);													//д�Ĵ�������,write register by I2C bus
		uint8_t (*Read_Reg)(uint8_t RegAddress);																//���Ĵ�������,read register by I2C bus
		void (*Rest_Speed)(uint32_t Speed);																		//Ӳ��I2C���������ٶ�,you can reset your Hardware I2C Speed
		
		uint8_t (*Write_Reg_continue)(uint8_t Device_Add,uint8_t RegAddress,uint8_t Count,uint8_t* Data);		//����д�Ĵ�������,continue write register by I2C bus��Ϊ����Ӧ��ֲ
		uint8_t (*Read_Reg_continue)(uint8_t Device_Add,uint8_t RegAddress,uint8_t Count,uint8_t* Data);		//�������Ĵ�������,continue read register by I2C bus��Ϊ����Ӧ��ֲ
	}I2C_BUS;
	extern I2C_BUS* Pthis;
	I2C_BUS Create_SI2C(GPIO_TypeDef* GPIOx,uint16_t SCL,uint16_t SDA,uint8_t Address);							//�������I2C����,create a softwere I2C
	I2C_BUS Create_HI2C(I2C_TypeDef* I2Cx,uint8_t Address);														//����Ӳ��I2C����,create a hardware I2C

#ifdef __cplusplus
extern "C"
}
#endif /* __cplusplus */

#endif /* EXTEND_HEADFIEL__I2C__H_ */
