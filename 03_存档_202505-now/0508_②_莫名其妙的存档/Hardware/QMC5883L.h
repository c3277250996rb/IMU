#ifndef __QMC5883L_H
#define __QMC5883L_H
#include "stm32f10x.h"
#include "my_i2c.h"

#define MAG_XOUT_L		0x00
#define MAG_XOUT_H		0x01
#define MAG_YOUT_L		0x02
#define MAG_YOUT_H		0x03
#define MAG_ZOUT_L		0x04
#define MAG_ZOUT_H		0x05

#define QMC5883_REG_CTRL   0x09
#define QMC5883_MODE_CONT  0x01
#define QMC5883_ODR_200HZ  0x0C
#define QMC5883_RNG_8G     0x10
#define QMC5883_OSR_512    0x40

extern int16_t qmc5883_mag[3];
uint8_t qmc5883_i2c_read(I2C_DEPLOY *bus, uint8_t RegisterAddress);

uint8_t QMC5883_Init(I2C_DEPLOY *bus);

void QMC5883_Update_Data(I2C_DEPLOY *bus);

void QMC5883_Read_Mag(I2C_DEPLOY *bus);

// void my_qmc5883_self_test(void);

#endif

