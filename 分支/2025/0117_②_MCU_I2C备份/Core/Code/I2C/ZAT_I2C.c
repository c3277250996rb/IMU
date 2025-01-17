#include "ZAT_I2C.h"

#if !Definition

	#ifndef __ZAT__GPIO__Definition__
	#define __ZAT__GPIO__Definition__

		// I2C SCL写数据
		void I2C_GPIO_SCL_Write(I2C_Class_Struct input_I2C_Class, GPIO_PinState input_PinState);

		// I2C SDA写数据
		void I2C_GPIO_SDA_Write(I2C_Class_Struct input_I2C_Class, GPIO_PinState input_PinState);

		// I2C SDA模式设置
		void I2C_GPIO_SDA_Mode(I2C_Class_Struct input_I2C_Class, uint8_t input_SDA_Mode);

		// 读取I2C SDA数据
		GPIO_PinState I2C_GPIO_SDA_Read(I2C_Class_Struct input_I2C_Class);

		#define I2C_GPIO_SCL_Write_1(input_I2C_Class) I2C_GPIO_SCL_Write(input_I2C_Class, 1)
		#define I2C_GPIO_SCL_Write_0(input_I2C_Class) I2C_GPIO_SCL_Write(input_I2C_Class, 0)
		#define I2C_GPIO_SDA_Write_1(input_I2C_Class) I2C_GPIO_SDA_Write(input_I2C_Class, 1)
		#define I2C_GPIO_SDA_Write_0(input_I2C_Class) I2C_GPIO_SDA_Write(input_I2C_Class, 0)

		#define I2C_GPIO_SDA_Mode_IN(input_I2C_Class)  I2C_GPIO_SDA_Mode(input_I2C_Class, enum_INPUT)
		#define I2C_GPIO_SDA_Mode_OUT(input_I2C_Class) I2C_GPIO_SDA_Mode(input_I2C_Class, enum_OUTPUT)

	#endif

	#ifndef __ZAT__Driver__Definition__
	#define __ZAT__Driver__Definition__

		// I2C驱动启动
		void I2C_Driver_Start(I2C_Class_Struct input_I2C_Class);

		// I2C驱动停止
		void I2C_Driver_Stop(I2C_Class_Struct input_I2C_Class);

		// 等待ACK
		bool I2C_Driver_WaitForAck(I2C_Class_Struct input_I2C_Class);

		// 发送ACK
		void I2C_Driver_Ack(I2C_Class_Struct input_I2C_Class);

		// 发送NAck
		void I2C_Driver_NAck(I2C_Class_Struct input_I2C_Class);

		// I2C写数据
		void I2C_Driver_Write(I2C_Class_Struct input_I2C_Class, uint8_t input_Data);

		// I2C读数据
		uint8_t I2C_Driver_Read(I2C_Class_Struct input_I2C_Class, uint8_t Ack);

	#endif 

	#ifndef __ZAT__APP__Definition__
	#define __ZAT__APP__Definition__

		// I2C写单字节数据
		void I2C_APP_Write_Single_Byte(	I2C_Class_Struct input_I2C_Class, 
										uint8_t DeviceAddress, 
										uint8_t RegisterAddress, 
										uint8_t input_Data);

		// I2C读单字节数据
		uint8_t I2C_APP_Read_Single_Byte(	I2C_Class_Struct input_I2C_Class, 
											uint8_t DeviceAddress, 
											uint8_t RegisterAddress);

	#endif

#endif


#if !Reference

	#ifndef __ZAT__APP__Reference__
	#define __ZAT__APP__Reference__
	/* APP Reference Begin */

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

		uint8_t I2C_APP_Read_Single_Byte(	I2C_Class_Struct	input_I2C_Class, 
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

	/* APP Reference End */
	#endif

	#ifndef __ZAT__Driver__Reference__
	#define __ZAT__Driver__Reference__
	/* Driver Reference Begin */

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
				I2C_Driver_Ack(input_I2C_Class);
			}else{
				I2C_Driver_NAck(input_I2C_Class);
			}

			return output_Data;
		}

	/* Driver Reference End */
	#endif

	#ifndef __ZAT__GPIO__Reference__
	#define __ZAT__GPIO__Reference__
	/* GPIO Reference Begin */


		void I2C_GPIO_SCL_Write(	I2C_Class_Struct	input_I2C_Class,
									GPIO_PinState		input_PinState)
		{	
			GPIO_TypeDef* temp_Port  = input_I2C_Class.SCL_Port;
			uint16_t temp_Pin       = input_I2C_Class.SCL_Pin;

			HAL_GPIO_WritePin(temp_Port, temp_Pin, input_PinState);
		}

		void I2C_GPIO_SDA_Write(	I2C_Class_Struct	input_I2C_Class,
									GPIO_PinState		input_PinState)
		{
			GPIO_TypeDef* temp_Port	= input_I2C_Class.SDA_Port;
			uint16_t temp_Pin       = input_I2C_Class.SDA_Pin;

			HAL_GPIO_WritePin(temp_Port, temp_Pin, input_PinState);
		}

		void I2C_GPIO_SDA_Mode(	I2C_Class_Struct	input_I2C_Class,
								uint8_t				input_SDA_Mode)						
		{
			GPIO_TypeDef* temp_Port	= input_I2C_Class.SDA_Port;
			uint16_t temp_Pin       = input_I2C_Class.SDA_Pin;

			GPIO_InitTypeDef GPIO_InitStruct;
			GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
			GPIO_InitStruct.Pin = temp_Pin;

			if(input_SDA_Mode){
				GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; 
			}else{
				GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
			}

			HAL_GPIO_Init(temp_Port, &GPIO_InitStruct);
			Delay_us(10);
		}

		GPIO_PinState I2C_GPIO_SDA_Read(I2C_Class_Struct input_I2C_Class){
			GPIO_TypeDef* temp_Port	= input_I2C_Class.SDA_Port;
			uint16_t temp_Pin       = input_I2C_Class.SDA_Pin;

			GPIO_PinState output_PinState;
			output_PinState = HAL_GPIO_ReadPin(temp_Port, temp_Pin);
			return output_PinState;
		}

	/* GPIO Reference End */
	#endif

#endif


