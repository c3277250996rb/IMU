#include "my_i2c.h"


// #define MyI2C_W_SDA(bit) I2C_GPIO_SDA_Write(I2C_Class_2, bit)
// #define MyI2C_W_SCL(bit) I2C_GPIO_SCL_Write(I2C_Class_2, bit)
// #define MyI2C_R_SDA() I2C_GPIO_SDA_Read(I2C_Class_2)


void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);


void MPU6050_WriteReg(uint8_t DevAddress, uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t DevAddress, uint8_t RegAddress);

