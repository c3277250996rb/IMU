#include "stm32f10x.h"
#include "my_i2c.h"
#include "stdlib.h"
#include "motor.h"
// ???? i2c ????
static void soft_i2c_delay(void) {
    Delay_us(SOFT_I2C_DELAY_US);
}

//  ??SLC
static void i2c_Write_SCL(I2C_DEPLOY* i2c_deploy, uint8_t BitValue) {
    GPIO_WriteBit(i2c_deploy->Private->GPIOx, i2c_deploy->Private->SCL_PIN, (BitAction)BitValue);
}

//  ??SDA
static void i2c_Write_SDA(I2C_DEPLOY* i2c_deploy, uint8_t BitValue) {
    GPIO_WriteBit(i2c_deploy->Private->GPIOx, i2c_deploy->Private->SDA_PIN, (BitAction)BitValue);
}

// ??SCL???ACK
static uint8_t i2c_Read_SDA(I2C_DEPLOY* i2c_deploy) {
    return GPIO_ReadInputDataBit(i2c_deploy->Private->GPIOx, i2c_deploy->Private->SDA_PIN);
}

// ???????i2c
static void Start(I2C_DEPLOY* i2c_deploy) {
    i2c_Write_SDA(i2c_deploy, 1);
    i2c_Write_SCL(i2c_deploy, 1);
    soft_i2c_delay();
    i2c_Write_SDA(i2c_deploy, 0);
    soft_i2c_delay();
    i2c_Write_SCL(i2c_deploy, 0);
    soft_i2c_delay();
}

// ????? ??i2c
static void Stop(I2C_DEPLOY* i2c_deploy) {
    i2c_Write_SDA(i2c_deploy, 0);
    soft_i2c_delay();
    i2c_Write_SCL(i2c_deploy, 1);
    soft_i2c_delay();
    i2c_Write_SDA(i2c_deploy, 1);
    soft_i2c_delay();
}

// ??????
static void SendByte(I2C_DEPLOY* i2c_deploy, uint8_t Byte) {
    for (int i = 0; i < 8; ++i) {
        i2c_Write_SDA(i2c_deploy, (Byte & (0x80 >> i)) != 0);
        soft_i2c_delay();
        i2c_Write_SCL(i2c_deploy, 1);
        soft_i2c_delay();
        i2c_Write_SCL(i2c_deploy, 0);
        soft_i2c_delay();
    }
}

// ??????
static uint8_t ReceiveByte(I2C_DEPLOY* i2c_deploy) {
    uint8_t Byte = 0;
    i2c_Write_SDA(i2c_deploy, 1);
    soft_i2c_delay();
    for (int i = 0; i < 8; ++i) {
        i2c_Write_SCL(i2c_deploy, 1);
        soft_i2c_delay();
        if (i2c_Read_SDA(i2c_deploy)) {
            Byte |= (0x80 >> i);
        }
        i2c_Write_SCL(i2c_deploy, 0);
        soft_i2c_delay();
    }
    return Byte;
}

// ??ack???
static void SendAck(I2C_DEPLOY* i2c_deploy, uint8_t AckBit) {
    i2c_Write_SDA(i2c_deploy, AckBit);
    soft_i2c_delay();
    i2c_Write_SCL(i2c_deploy, 1);
    soft_i2c_delay();
    i2c_Write_SCL(i2c_deploy, 0);
    soft_i2c_delay();
}

static uint8_t ReceiveAck(I2C_DEPLOY* i2c_deploy) {
    uint8_t ack;

	// GPIO_InitTypeDef GPIO_InitStruct;
    // GPIO_InitStruct.GPIO_Pin = i2c_deploy->Private->SDA_PIN;
    // GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU
    // GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(i2c_deploy->Private->GPIOx, &GPIO_InitStruct);

    i2c_Write_SDA(i2c_deploy, 1);
    soft_i2c_delay();
    i2c_Write_SCL(i2c_deploy, 1);
    soft_i2c_delay();
    ack = i2c_Read_SDA(i2c_deploy);
    i2c_Write_SCL(i2c_deploy, 0);
    soft_i2c_delay();

	// GPIO_InitStruct.GPIO_Pin = i2c_deploy->Private->SDA_PIN;
    // GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    // GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(i2c_deploy->Private->GPIOx, &GPIO_InitStruct);

    return ack;
}

static uint8_t CheckDevice(I2C_DEPLOY* i2c_deploy) {
    uint8_t ack;
    i2c_deploy->Start(i2c_deploy);
    i2c_deploy->SendByte(i2c_deploy, i2c_deploy->Private->I2C_Add);
    ack = i2c_deploy->ReceiveAck(i2c_deploy);
    i2c_deploy->Stop(i2c_deploy);
    return ack;
}

static void WriteReg(I2C_DEPLOY* i2c_deploy, uint8_t RegAddress, uint8_t Data) {
    i2c_deploy->Start(i2c_deploy);
    i2c_deploy->SendByte(i2c_deploy, i2c_deploy->Private->I2C_Add);
    i2c_deploy->ReceiveAck(i2c_deploy);
    i2c_deploy->SendByte(i2c_deploy, RegAddress);
    i2c_deploy->ReceiveAck(i2c_deploy);
    i2c_deploy->SendByte(i2c_deploy, Data);
    i2c_deploy->ReceiveAck(i2c_deploy);
    i2c_deploy->Stop(i2c_deploy);
}

static uint8_t ReadReg(I2C_DEPLOY* i2c_deploy, uint8_t RegAddress) {
    uint8_t data;
    i2c_deploy->Start(i2c_deploy);
    i2c_deploy->SendByte(i2c_deploy, i2c_deploy->Private->I2C_Add);
    i2c_deploy->ReceiveAck(i2c_deploy);
    i2c_deploy->SendByte(i2c_deploy, RegAddress);
    i2c_deploy->ReceiveAck(i2c_deploy);
    i2c_deploy->Start(i2c_deploy);
    i2c_deploy->SendByte(i2c_deploy, i2c_deploy->Private->I2C_Add | 0x01);
    i2c_deploy->ReceiveAck(i2c_deploy);
    data = i2c_deploy->ReceiveByte(i2c_deploy);
    i2c_deploy->SendAck(i2c_deploy, 1);
    i2c_deploy->Stop(i2c_deploy);
    return data;
}

// ?????? ??
static uint8_t WriteRegCont(I2C_DEPLOY* i2c_deploy, uint8_t DevAddr, uint8_t Ctrl, uint8_t Cnt, uint8_t* Data) {
    i2c_deploy->Start(i2c_deploy);
    i2c_deploy->SendByte(i2c_deploy, DevAddr);
    i2c_deploy->ReceiveAck(i2c_deploy);
    i2c_deploy->SendByte(i2c_deploy, Ctrl);
    i2c_deploy->ReceiveAck(i2c_deploy);
    for (uint8_t i = 0; i < Cnt; i++) {
        i2c_deploy->SendByte(i2c_deploy, Data[i]);
        i2c_deploy->ReceiveAck(i2c_deploy);
    }
    i2c_deploy->Stop(i2c_deploy);
    return 0;
}

// ??????i2c???
I2C_DEPLOY soft_i2c_create(GPIO_TypeDef* GPIOx, uint16_t SCL_PIN, uint16_t SDA_PIN, uint8_t Address) {
    I2C_DEPLOY i2c_deploy;
    i2c_deploy.Private = malloc(sizeof(I2C_Private));
    i2c_deploy.Private->GPIOx = GPIOx;
    i2c_deploy.Private->SCL_PIN = SCL_PIN;
    i2c_deploy.Private->SDA_PIN = SDA_PIN;
    i2c_deploy.Private->I2C_Add = Address;

    i2c_deploy.Start = Start;
    i2c_deploy.Stop = Stop;
    i2c_deploy.SendByte = SendByte;
    i2c_deploy.ReceiveByte = ReceiveByte;
    i2c_deploy.SendAck = SendAck;
    i2c_deploy.ReceiveAck = ReceiveAck;

    i2c_deploy.CheckDevice = CheckDevice;
    i2c_deploy.WriteReg = WriteReg;
    i2c_deploy.ReadReg = ReadReg;
    i2c_deploy.WriteRegCont = WriteRegCont;

    //  ±÷”≥ı ºªØ
    if(GPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(GPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(GPIOx == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Mode = GPIO_Mode_Out_OD;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_Pin = SCL_PIN | SDA_PIN;
    GPIO_Init(GPIOx, &gpioInit);

    GPIO_WriteBit(GPIOx, SCL_PIN, Bit_SET);
    GPIO_WriteBit(GPIOx, SDA_PIN, Bit_SET);

    return i2c_deploy;
}
