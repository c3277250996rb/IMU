#include "main.h"
#include "ZAT_I2C.h"



#ifndef __ZAT__APP__Layer__
#define __ZAT__APP__Layer__
/* APP Layer Begin */

	void I2C_APP_Write_Single_Byte(input_I2C_Class, DeviceAddress, RegisterAddress, input_Data){
		I2C_Driver_Start();
		I2C_Driver_WriteByte(DeviceAddress | I2C_Direction_Transmitter);
		I2C_Driver_WaiteForAck();
		I2C_Driver_WriteByte(RegisterAddress);
		I2C_Driver_WaiteForAck();
		I2C_Driver_WriteByte(input_Data);
		I2C_Driver_WaiteForAck();
		I2C_Driver_Stop();
	}

	void I2C_APP_Read_Single_Byte(input_I2C_Class, DeviceAddress, RegisterAddress, input_Data){
		uint8_t Data;

		MyI2C_Start();
		MyI2C_SendByte(MPU6050_ADDRESS);
		MyI2C_ReceiveAck();
		MyI2C_SendByte(RegAddress);
		MyI2C_ReceiveAck();

		MyI2C_Start();
		MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
		MyI2C_ReceiveAck();
		Data = MyI2C_ReceiveByte();
		MyI2C_SendAck(1);
		MyI2C_Stop();

		return Data;
	}

/* APP Layer End */
#endif


#ifndef __ZAT__Driver__Layer__
#define __ZAT__Driver__Layer__
/* Driver Layer Begin */

	void I2C_Driver_Start();
	void I2C_Driver_Ack();
	void I2C_Driver_Write(input_I2C_Class, input_Data){
		uint8_t i;
		GPIO_TypeDef temp_SDA_Port  = input_I2C_Class->SDA_Port;
		uint16_t temp_SDA_Pin       = input_I2C_Class->SDA_Pin;
		
		I2C_GPIO_SDA_Mode_OUT();
		for(i = 0; i < 8; i ++){
			I2C_GPIO_SCL_Write_0();
			Delay_us(1);

			if(input_Data & 0x80){
				I2C_GPIO_SDA_Write_1();
			}else{
				I2C_GPIO_SDA_Write_0();
			}

			input_Data <<= 1;
			Delay_us(1);
			I2C_GPIO_SCL_Write_1();
			Delay_us(1);
		}

	}

	void I2C_Driver_Read(	I2C_Class_Struct	input_I2C_Class,
							uint8_t				input_Data){
		
	}

/* Driver Layer End */
#endif


#ifndef __ZAT__GPIO__Layer__
#define __ZAT__GPIO__Layer__
/* GPIO Layer Begin */

	#define I2C_GPIO_SCL_Write_1(input_I2C_Class) I2C_GPIO_SCL_Write(input_I2C_Class, 1)
	#define I2C_GPIO_SCL_Write_0(input_I2C_Class) I2C_GPIO_SCL_Write(input_I2C_Class, 0)
	#define I2C_GPIO_SDA_Write_1(input_I2C_Class) I2C_GPIO_SDA_Write(input_I2C_Class, 1)
	#define I2C_GPIO_SDA_Write_0(input_I2C_Class) I2C_GPIO_SDA_Write(input_I2C_Class, 0)

	#define I2C_GPIO_SDA_Mode_IN  I2C_GPIO_SDA_Mode(enum_INPUT)
	#define I2C_GPIO_SDA_Mode_OUT I2C_GPIO_SDA_Mode(enum_OUTPUT)

	void I2C_GPIO_SCL_Write(	I2C_Class_Struct	input_I2C_Class,
								GPIO_PinState		input_PinState)
	{	
		GPIO_TypeDef temp_Port  = input_I2C_Class->SCL_Port;
		uint16_t temp_Pin       = input_I2C_Class->SCL_Pin;

		HAL_GPIO_WritePin(temp_Port, temp_Pin, input_PinState);
	}

	void I2C_GPIO_SDA_Write(	I2C_Class_Struct	input_I2C_Class,
								GPIO_PinState		input_PinState)
	{
		GPIO_TypeDef temp_Port  = input_I2C_Class->SDA_Port;
		uint16_t temp_Pin       = input_I2C_Class->SDA_Pin;

		HAL_GPIO_WritePin(temp_Port, temp_Pin, input_PinState);
	}

	void I2C_GPIO_SDA_Mode(	I2C_Class_Struct	input_I2C_Class,
							uint8_t				input_SDA_Mode)						
	{
		GPIO_TypeDef temp_Port  = input_I2C_Class->SDA_Port;
		uint16_t temp_Pin       = input_I2C_Class->SDA_Pin;

		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Pin = temp_Pin;

		if(input_SDA_Mode){
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; 
		}else{
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
		}

		GPIO_Init(temp_Port, &GPIO_InitStruct);
		Delay_us(10);
	}

	GPIO_PinState I2C_GPIO_SDA_Read(I2C_Class_Struct input_I2C_Class){
		GPIO_TypeDef temp_Port  = input_I2C_Class->SDA_Port;
		uint16_t temp_Pin       = input_I2C_Class->SDA_Pin;

		GPIO_PinState output_PinState;
		output_PinState = HAL_GPIO_ReadPin(temp_Port, temp_Pin);
		return output_PinState;
	}

/* GPIO Layer End */
#endif




