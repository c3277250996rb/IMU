#include "main.h"

enum
{
	enum_INPUT,
	enum_OUTPUT
};


typedef struct I2C_Class_Typedef
{
    uint16_t SCL_Pin, 
    uint16_t SDA_Pin,
	GPIO_TypeDef SCL_Port, 
	GPIO_TypeDef SDA_Port
};




#ifndef I2C_Direction_Transmitter
	#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#endif

#ifndef I2C_Direction_Receiver
	#define  I2C_Direction_Receiver         ((uint8_t)0x01)
#endif


I2C_Class_Typedef I2C_Class_1 = {
    .SCL_Pin = 5,       // 设置SCL引脚为5
    .SDA_Pin = 6,       // 设置SDA引脚为6
    .SCL_Port = GPIOB,  // 设置SCL引脚所在端口为GPIOB
    .SDA_Port = GPIOB   // 设置SDA引脚所在端口为GPIOB
};

I2C_Class_Typedef I2C_Class_2 = {
    .SCL_Pin = 5,       // 设置SCL引脚为5
    .SDA_Pin = 6,       // 设置SDA引脚为6
    .SCL_Port = GPIOB,  // 设置SCL引脚所在端口为GPIOB
    .SDA_Port = GPIOB   // 设置SDA引脚所在端口为GPIOB
};

