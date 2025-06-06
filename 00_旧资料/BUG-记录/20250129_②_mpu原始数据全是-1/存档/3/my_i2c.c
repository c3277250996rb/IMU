#include "my_i2c.h"

// #if __STDC_VERSION__ < 199901L  // 这会在非 C99 模式下报错
//     #error "This program requires C99 mode"
// #endif

// void Delay_us(uint16_t Time){
//     uint8_t i;	
//     while(Time --){
//         for(i = 0; i < 10; i ++){
//             // just wait
//         };
//     }
// }

// void Delay_ms(uint16_t Time){
//     while(Time --){
//         Delay_us(1000);
//     }
// }

    // .SCL_Pin = I2C2_SCL_PB10_Pin,       
    // .SDA_Pin = I2C2_SDA_PB11_Pin,       
    // .SCL_Port = I2C2_SCL_PB10_GPIO_Port,
    // .SDA_Port = I2C2_SDA_PB11_GPIO_Port 

    
I2C_Class_Struct I2C_Class_2 = {
    .SCL_Pin = GPIO_Pin_10,       
    .SDA_Pin = GPIO_Pin_11,       
    .SCL_Port = GPIOB,
    .SDA_Port = GPIOB 
};


// #ifndef __I2C__APP__Reference__
// #define __I2C__APP__Reference__
// /* APP Reference Begin */

//     void I2C_APP_Write_Single_Byte(	I2C_Class_Struct	input_I2C_Class, 
//                                     uint8_t				DeviceAddress, 
//                                     uint8_t				RegisterAddress, 
//                                     uint8_t				input_Data)
//     {
//         I2C_Driver_Start(input_I2C_Class);
//         I2C_Driver_Write(	input_I2C_Class, 
//                             DeviceAddress | I2C_Direction_Transmitter);
//         I2C_Driver_WaitForAck(input_I2C_Class);
//         I2C_Driver_Write(	input_I2C_Class, 
//                             RegisterAddress);
//         I2C_Driver_WaitForAck(input_I2C_Class);
//         I2C_Driver_Write(	input_I2C_Class, 
//                             input_Data);
//         I2C_Driver_WaitForAck(input_I2C_Class);
//         I2C_Driver_Stop(input_I2C_Class);
//     }

//     uint8_t I2C_APP_Read_Single_Byte(	I2C_Class_Struct	input_I2C_Class, 
//                                         uint8_t				DeviceAddress, 
//                                         uint8_t				RegisterAddress)
//     {
//         uint8_t output_Data = 0;
    
//         I2C_Driver_Start(input_I2C_Class);
//         I2C_Driver_Write(	input_I2C_Class, 
//                             DeviceAddress | I2C_Direction_Transmitter);
//         I2C_Driver_WaitForAck(input_I2C_Class);
//         I2C_Driver_Write(	input_I2C_Class, 
//                             RegisterAddress);
//         I2C_Driver_WaitForAck(input_I2C_Class);
//         I2C_Driver_Start(input_I2C_Class);
//         I2C_Driver_Write(	input_I2C_Class, 
//                             DeviceAddress | I2C_Direction_Receiver);
//         I2C_Driver_WaitForAck(input_I2C_Class);
//         output_Data = I2C_Driver_Read(	input_I2C_Class, 
//                                         enum_NACK);
//         I2C_Driver_Stop(input_I2C_Class);
        
//         return output_Data;
//     }

// /* APP Reference End */
// #endif

// #ifndef __I2C__Driver__Reference__
// #define __I2C__Driver__Reference__
// /* Driver Reference Begin */

//     void I2C_Driver_Start(I2C_Class_Struct input_I2C_Class){
//         I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);
//         I2C_GPIO_SDA_Write_1(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SCL_Write_1(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SDA_Write_0(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SCL_Write_0(input_I2C_Class);
//         Delay_us(1);
//     }

//     void I2C_Driver_Stop(I2C_Class_Struct input_I2C_Class){
//         I2C_GPIO_SCL_Write_0(input_I2C_Class);
//         I2C_GPIO_SDA_Write_0(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SCL_Write_1(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SDA_Write_1(input_I2C_Class);
//         Delay_us(1);
//     }

//     bool I2C_Driver_WaitForAck(I2C_Class_Struct input_I2C_Class){
//         uint8_t Retry = 0;
        
//         I2C_GPIO_SCL_Write_0(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SCL_Write_1(input_I2C_Class);
//         I2C_GPIO_SDA_Mode_IN(input_I2C_Class);

//         while(I2C_GPIO_SDA_Read(input_I2C_Class)){
//             if(++ Retry > 200){
//                 I2C_Driver_Stop(input_I2C_Class);
//                 return false;
//             }
//         }

//         I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);
//         I2C_GPIO_SCL_Write_0(input_I2C_Class);
//         Delay_us(1);
        
//         return true;
//     }

//     void I2C_Driver_Ack(I2C_Class_Struct input_I2C_Class){
//         I2C_GPIO_SCL_Write_0(input_I2C_Class);
//         I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);
//         I2C_GPIO_SDA_Write_0(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SCL_Write_1(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SCL_Write_0(input_I2C_Class);
//     }

//     void I2C_Driver_NAck(I2C_Class_Struct input_I2C_Class){
//         I2C_GPIO_SCL_Write_0(input_I2C_Class);
//         I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);
//         I2C_GPIO_SDA_Write_1(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SCL_Write_1(input_I2C_Class);
//         Delay_us(1);
//         I2C_GPIO_SCL_Write_0(input_I2C_Class);
//     }

//     void I2C_Driver_Write(	I2C_Class_Struct	input_I2C_Class,
//                             uint8_t				input_Data)
//     {
//         uint8_t i;
        
//         I2C_GPIO_SDA_Mode_OUT(input_I2C_Class);

//         for(i = 0; i < 8; i ++){
//             I2C_GPIO_SCL_Write_0(input_I2C_Class);
//             Delay_us(1);

//             if(input_Data & 0x80){
//                 I2C_GPIO_SDA_Write_1(input_I2C_Class);
//             }else{
//                 I2C_GPIO_SDA_Write_0(input_I2C_Class);
//             }

//             input_Data <<= 1;
//             Delay_us(1);
//             I2C_GPIO_SCL_Write_1(input_I2C_Class);
//             Delay_us(1);
//         }

//     }

//     uint8_t I2C_Driver_Read(	I2C_Class_Struct	input_I2C_Class,
//                                 uint8_t				Ack)
//     {
//         uint8_t i, output_Data = 0;

//         I2C_GPIO_SDA_Mode_IN(input_I2C_Class);

//         for(i = 0; i < 8; i ++){
//             I2C_GPIO_SCL_Write_0(input_I2C_Class);
//             Delay_us(1);
//             I2C_GPIO_SCL_Write_1(input_I2C_Class);
//             output_Data <<= 1;

//             if(I2C_GPIO_SDA_Read(input_I2C_Class)){
//                 output_Data |= 0x01;
//             }else{
//                 output_Data &= ~0x01;
//             }

//             Delay_us(1);
//         }

//         if(enum_ACK == Ack){
//             I2C_Driver_Ack(input_I2C_Class);
//         }else{
//             I2C_Driver_NAck(input_I2C_Class);
//         }

//         return output_Data;
//     }

// /* Driver Reference End */
// #endif

#ifndef __I2C__GPIO__Reference__
#define __I2C__GPIO__Reference__
/* GPIO Reference Begin */

    void I2C_GPIO_SCL_Write(	I2C_Class_Struct	input_I2C_Class,
                                GPIO_PinState		input_PinState)
    {	
        GPIO_TypeDef* temp_Port = input_I2C_Class.SCL_Port;
        uint16_t temp_Pin       = input_I2C_Class.SCL_Pin;

        #if use_hal
            HAL_GPIO_WritePin(temp_Port, temp_Pin, input_PinState);
        #else
            GPIO_WriteBit(temp_Port, temp_Pin, input_PinState);
        #endif
    }

    void I2C_GPIO_SDA_Write(	I2C_Class_Struct	input_I2C_Class,
                                GPIO_PinState		input_PinState)
    {
        GPIO_TypeDef* temp_Port	= input_I2C_Class.SDA_Port;
        uint16_t temp_Pin       = input_I2C_Class.SDA_Pin;

        #if use_hal
            HAL_GPIO_WritePin(temp_Port, temp_Pin, input_PinState);
        #else
            GPIO_WriteBit(temp_Port, temp_Pin, input_PinState);
        #endif
    }

    void I2C_GPIO_SDA_Mode(	I2C_Class_Struct	input_I2C_Class,
                            uint8_t				input_SDA_Mode)						
    {
        GPIO_TypeDef* temp_Port	= input_I2C_Class.SDA_Port;
        uint16_t temp_Pin       = input_I2C_Class.SDA_Pin;


        #if use_hal
            GPIO_InitTypeDef GPIO_InitStruct;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
            GPIO_InitStruct.Pin = temp_Pin;

            if(input_SDA_Mode){
                GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; 
            }else{
                GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            }
            HAL_GPIO_Init(temp_Port, &GPIO_InitStruct);
        #else
        	GPIO_InitTypeDef GPIO_InitStructure;

        	GPIO_InitStructure.GPIO_Pin = temp_Pin;

        	if(input_SDA_Mode)
        		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
        	else
        		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
                
        	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
            GPIO_Init(temp_Port, &GPIO_InitStructure);
        #endif
        Delay_us(10);
    }

    GPIO_PinState I2C_GPIO_SDA_Read(I2C_Class_Struct input_I2C_Class){
        GPIO_TypeDef* temp_Port	= input_I2C_Class.SDA_Port;
        uint16_t temp_Pin       = input_I2C_Class.SDA_Pin;

        GPIO_PinState output_PinState;
        #if use_hal
            output_PinState = HAL_GPIO_ReadPin(temp_Port, temp_Pin);
        #else
            output_PinState = GPIO_ReadInputDataBit(temp_Port, temp_Pin);
        #endif
        return output_PinState;
    }

/* GPIO Reference End */
#endif


// uint8_t I2C_1_Read(uint8_t DeviceAddress, uint8_t RegisterAddress){
//     return I2C_APP_Read_Single_Byte(I2C_Class_1, DeviceAddress, RegisterAddress);
// }
// void I2C_1_Write(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data){
//     I2C_APP_Write_Single_Byte(I2C_Class_1, DeviceAddress, RegisterAddress, input_Data);
// }


// uint8_t I2C_ReadOneByte(uint8_t DeviceAddress, uint8_t RegisterAddress){
//     return I2C_APP_Read_Single_Byte(I2C_Class_2, DeviceAddress, RegisterAddress);
// }
// void I2C_WriteOneByte(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data){
//     I2C_APP_Write_Single_Byte(I2C_Class_2, DeviceAddress, RegisterAddress, input_Data);
// }

// #define stm32f10x_gpio.h
// void I2Cx_Init(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //     GPIO_Mode_Out_PP
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
// 	I2C_GPIO_SCL_Write_1(I2C_Class_2);
// 	I2C_GPIO_SDA_Write_1(I2C_Class_2);
// 	Delay_ms(10);
// }


