#include "code.h"

enum
{
	enum_OUTPUT = 0,
	enum_INPUT
};

enum
{
	enum_NACK = 0,
	enum_ACK
};

typedef struct 
{
    uint16_t SCL_Pin;
    uint16_t SDA_Pin;
	GPIO_TypeDef* SCL_Port;
	GPIO_TypeDef* SDA_Port;
}I2C_Class_Struct;

#ifndef I2C_Direction_Transmitter
	#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#endif

#ifndef I2C_Direction_Receiver
	#define  I2C_Direction_Receiver         ((uint8_t)0x01)
#endif


I2C_Class_Struct I2C_Class_1 = {
    .SCL_Pin = I2C1_SCL_PB6_Pin,       
    .SDA_Pin = I2C1_SDA_PB7_Pin,       
    .SCL_Port = I2C1_SCL_PB6_GPIO_Port,
    .SDA_Port = I2C1_SDA_PB7_GPIO_Port 
};

I2C_Class_Struct I2C_Class_2 = {
    .SCL_Pin = I2C2_SCL_PB10_Pin,       
    .SDA_Pin = I2C2_SDA_PB11_Pin,       
    .SCL_Port = I2C2_SCL_PB10_GPIO_Port,
    .SDA_Port = I2C2_SDA_PB11_GPIO_Port 
};

#define I2C_1_Read(DeviceAddress, RegisterAddress) \
            I2C_APP_Read_Single_Byte(I2C_Class_1, DeviceAddress, RegisterAddress)
#define I2C_1_Write(DeviceAddress, RegisterAddress, input_Data) \
            I2C_APP_Write_Single_Byte(I2C_Class_1, DeviceAddress, RegisterAddress, input_Data)
