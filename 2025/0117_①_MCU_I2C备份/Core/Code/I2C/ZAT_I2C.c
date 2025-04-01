#include "main.h"
#include "ZAT_I2C.h"



#ifndef __ZAT__APP__Layer__
#define __ZAT__APP__Layer__
/* APP Layer Begin */

	void I2C_APP_Write_Single_Byte(	I2C_Class_Struct	input_I2C_Class, 
									uint8_t				DeviceAddress, 
									uint8_t				RegisterAddress, 
									uint8_t				input_Data)
	{
		I2C_Driver_Start(input_I2C_Class);
		I2C_Driver_Write(	input_I2C_Class, 
							DeviceAddress | I2C_Direction_Transmitter);
		I2C_Driver_WaitForAck(input_I2C_Class);
		I2C_Driver_Write(	input_I2C_Class, 
							RegisterAddress);
		I2C_Driver_WaitForAck(input_I2C_Class);
		I2C_Driver_Write(	input_I2C_Class, 
							input_Data);
		I2C_Driver_WaitForAck(input_I2C_Class);
		I2C_Driver_Stop(input_I2C_Class);
	}

	void I2C_APP_Read_Single_Byte(	I2C_Class_Struct	input_I2C_Class, 
									uint8_t				DeviceAddress, 
									uint8_t				RegisterAddress)
	{
		uint8_t output_Data = 0;
	
		I2C_Driver_Start(input_I2C_Class);
		I2C_Driver_Write(	input_I2C_Class, 
							DeviceAddress | I2C_Direction_Transmitter);
		I2C_Driver_WaitForAck(input_I2C_Class);
		I2C_Driver_Write(	input_I2C_Class, 
							RegisterAddress);
		I2C_Driver_WaitForAck(input_I2C_Class);
		I2C_Driver_Start(input_I2C_Class);
		I2C_Driver_Write(	input_I2C_Class, 
							DeviceAddress | I2C_Direction_Receiver);
		I2C_Driver_WaitForAck(input_I2C_Class);
		output_Data = I2C_Driver_Read(	input_I2C_Class, 
										enum_NACK);
		I2C_Driver_Stop(input_I2C_Class);
		
		return output_Data;
	}

/* APP Layer End */
#endif


#ifndef __ZAT__Driver__Layer__
#define __ZAT__Driver__Layer__
/* Driver Layer Begin */

	void I2C_Driver_Start(I2C_Class_Struct	input_I2C_Class){
		I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);
		I2C_GPIO_SDA_Write_1(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SCL_Write_1(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SDA_Write_0(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SCL_Write_0(input_I2C_Class);
		Delay_us(1);
	}

	void I2C_Driver_Stop(I2C_Class_Struct input_I2C_Class){
		I2C_GPIO_SCL_Write_0(input_I2C_Class);
		I2C_GPIO_SDA_Write_0(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SCL_Write_1(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SDA_Write_1(input_I2C_Class);
		Delay_us(1);
	}

	bool I2C_Driver_WaitForAck(I2C_Class_Struct input_I2C_Class){
		uint8_t Retry = 0;
		
		I2C_GPIO_SCL_Write_0(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SCL_Write_1(input_I2C_Class);
		I2C_GPIO_SDA_Mode_IN(input_I2C_Class);

		while(I2C_GPIO_SDA_Read(input_I2C_Class)){
			if(++ Retry > 200){
				I2C_Driver_Stop(input_I2C_Class);
				return false;
			}
		}

		I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);
		I2C_GPIO_SCL_Write_0(input_I2C_Class);
		Delay_us(1);
		
		return true;
	}

	void I2C_Driver_Ack(I2C_Class_Struct input_I2C_Class){
		I2C_GPIO_SCL_Write_0(input_I2C_Class);
		I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);
		I2C_GPIO_SDA_Write_0(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SCL_Write_1(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SCL_Write_0(input_I2C_Class);
	}

	void I2C_Driver_NAck(I2C_Class_Struct input_I2C_Class){
		I2C_GPIO_SCL_Write_0(input_I2C_Class);
		I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);
		I2C_GPIO_SDA_Write_1(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SCL_Write_1(input_I2C_Class);
		Delay_us(1);
		I2C_GPIO_SCL_Write_0(input_I2C_Class);
	}

	void I2C_Driver_Write(	I2C_Class_Struct	input_I2C_Class,
							uint8_t				input_Data)
	{
		uint8_t i;
		
		I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);

		for(i = 0; i < 8; i ++){
			I2C_GPIO_SCL_Write_0(input_I2C_Class);
			Delay_us(1);

			if(input_Data & 0x80){
				I2C_GPIO_SDA_Write_1(input_I2C_Class);
			}else{
				I2C_GPIO_SDA_Write_0(input_I2C_Class);
			}

			input_Data <<= 1;
			Delay_us(1);
			I2C_GPIO_SCL_Write_1(input_I2C_Class);
			Delay_us(1);
		}

	}

	uint8_t I2C_Driver_Read(	I2C_Class_Struct	input_I2C_Class,
								uint8_t				Ack)
	{
		uint8_t i, output_Data = 0;

		I2C_GPIO_SDA_Mode_IN(input_I2C_Class);

		for(i = 0; i < 8; i ++){
			I2C_GPIO_SCL_Write_0(input_I2C_Class);
			Delay_us(1);
			I2C_GPIO_SCL_Write_1(input_I2C_Class);
			output_Data <<= 1;

			if(I2C_GPIO_SDA_Read(input_I2C_Class)){
				output_Data |= 0x01;
			}else{
				output_Data &= ~0x01;
			}

			Delay_us(1);
		}

		if(enum_ACK == Ack){
			I2C_Driver_Ack();
		}else{
			I2C_Driver_NAck();
		}

		return output_Data;
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

	#define I2C_GPIO_SDA_Mode_IN(input_I2C_Class)  I2C_GPIO_SDA_Mode(input_I2C_Class, enum_INPUT)
	#define I2C_GPIO_SDA_Mode_OUT(input_I2C_Class) I2C_GPIO_SDA_Mode(input_I2C_Class, enum_OUTPUT)

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




