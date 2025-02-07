#ifndef myi2ch
#define myi2ch

#include "System.h"

#include "Public_StdTypes.h"


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

typedef struct __I2C_Class_Struct
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


// I2C_Class_Struct I2C_Class_1 = {
//     .SCL_Pin = I2C1_SCL_PB6_Pin,       
//     .SDA_Pin = I2C1_SDA_PB7_Pin,       
//     .SCL_Port = I2C1_SCL_PB6_GPIO_Port,
//     .SDA_Port = I2C1_SDA_PB7_GPIO_Port 
// };

extern I2C_Class_Struct I2C_Class_2;
void Delay_us(uint16_t Time);
void Delay_ms(uint16_t Time);

#ifdef HAL_OK
    #define use_hal
#endif

#ifdef use_hal  // HAL库

#else  // 库函数
    typedef enum
    {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
    } GPIO_PinState;

#endif


#ifndef __I2C__GPIO__Definition__
#define __I2C__GPIO__Definition__

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

#ifndef __I2C__Driver__Definition__
#define __I2C__Driver__Definition__

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

#ifndef __I2C__APP__Definition__
#define __I2C__APP__Definition__

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


// uint8_t I2C_1_Read(uint8_t DeviceAddress, uint8_t RegisterAddress);
// void I2C_1_Write(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data);

// uint8_t I2C_ReadOneByte(uint8_t DeviceAddress, uint8_t RegisterAddress);
// void I2C_WriteOneByte(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data);


// void I2Cx_Init(void);



#endif // !myi2ch   

