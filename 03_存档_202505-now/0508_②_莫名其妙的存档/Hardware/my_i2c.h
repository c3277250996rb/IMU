// soft_i2c.h
#ifndef SOFT_I2C_H
#define SOFT_I2C_H

#include "stm32f10x.h"
#include "Delay.h"
#include <stdint.h>
#include <stdlib.h>

#define SOFT_I2C_DELAY_US 0 //延迟（oled为0，不需要延迟）

typedef struct I2C_Private {
    GPIO_TypeDef* GPIOx;
    uint16_t SCL_PIN;
    uint16_t SDA_PIN;
    uint8_t I2C_Add;
} I2C_Private;

struct I2C_DEPLOY;

typedef struct I2C_DEPLOY {
    I2C_Private* Private;

    void (*Start)(struct I2C_DEPLOY* this);
    void (*Stop)(struct I2C_DEPLOY* this);
    void (*SendByte)(struct I2C_DEPLOY* this, uint8_t Byte);
    uint8_t (*ReceiveByte)(struct I2C_DEPLOY* this);
    void (*SendAck)(struct I2C_DEPLOY* this, uint8_t AckBit);
    uint8_t (*ReceiveAck)(struct I2C_DEPLOY* this);

    uint8_t (*CheckDevice)(struct I2C_DEPLOY* this);
    void (*WriteReg)(struct I2C_DEPLOY* this, uint8_t RegAddress, uint8_t Data);
    uint8_t (*ReadReg)(struct I2C_DEPLOY* this, uint8_t RegAddress);
    uint8_t (*WriteRegCont)(struct I2C_DEPLOY* this, uint8_t DevAddr, uint8_t Ctrl, uint8_t Cnt, uint8_t* Data);
} I2C_DEPLOY;

I2C_DEPLOY soft_i2c_create(GPIO_TypeDef* GPIOx, uint16_t SCL_PIN, uint16_t SDA_PIN, uint8_t Address);

#endif
