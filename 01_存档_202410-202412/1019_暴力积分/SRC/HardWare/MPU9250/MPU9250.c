/**
  ******************************************************************************
  * @file    MPU9250.c
  * @author  cer
  * @version V1.0
  * @date    27-January-2015
  * @brief   This file includes the MPU9250 driver functions
  
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 Waveshare</center></h2>
  ******************************************************************************
  */


#include "MPU9250.h"
#include "I2C.h"

int16_t magn[3];
int16_t accel[3], gyro[3];
unsigned char BUF[10];
MPU9250_TypeDef MPU9250_Offset={0};

//-----------------------------------------------------------------------------
extern bool invmsMPU9250Check(void);
extern void invmsMPU9250Init(void);
extern void invmsMPU9250GyroRead(int16_t* ps16X, int16_t* ps16Y, int16_t* ps16Z);
extern void invmsMPU9250AccelRead(int16_t* ps16X, int16_t* ps16Y, int16_t* ps16Z);
extern void invmsMPU9250MagRead(int16_t* ps16X, int16_t* ps16Y, int16_t* ps16Z);
int16_t convert(int16_t t);


int16_t convert(int16_t t){
    return t > 0 ? t - 65536 : t + 65535;
}

extern bool invmsMPU9255Check(void)
{
    bool bRet = false;
    if(WHO_AM_I_VAL_9255 == I2C_ReadOneByte(DEFAULT_ADDRESS, WHO_AM_I))
    {
        bRet = true;
    }
    return bRet;
}

extern bool invmsMPU9250Check(void)
{
    bool bRet = false;
    // if(WHO_AM_I_VAL == I2C_ReadOneByte(DEFAULT_ADDRESS, WHO_AM_I))
    {
        bRet = true;
    }
    return bRet;
}

extern void invmsMPU9250Init(void)
{
    // I2C_WriteOneByte(GYRO_ADDRESS,PWR_MGMT_1, 0x00);
    // I2C_WriteOneByte(GYRO_ADDRESS,SMPLRT_DIV, 0x07);
    // I2C_WriteOneByte(GYRO_ADDRESS,CONFIG, 0x06);
    // I2C_WriteOneByte(GYRO_ADDRESS,GYRO_CONFIG, 0x10);
    // I2C_WriteOneByte(GYRO_ADDRESS,ACCEL_CONFIG, 0x01);

    Delay_ms(10);

    MPU9250_InitGyrOffset();
    return;
}

extern void invmsMPU9250GyroRead(int16_t* ps16X, int16_t* ps16Y, int16_t* ps16Z)
{
    uint8_t i;
    int16_t InBuffer[3] = {0}; 
    static int32_t OutBuffer[3] = {0};
    static MPU9250_AvgTypeDef MPU9250_Filter[3];

    switch_lq(0);

    BUF[0]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_L); 
    BUF[1]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_XOUT_H);
    InBuffer[0]=	(BUF[1]<<8)|BUF[0];
    // InBuffer[1]=	-(BUF[1]<<8)|BUF[0];

    BUF[2]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_L);
    BUF[3]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_YOUT_H);
    InBuffer[1] = (BUF[3]<<8)|BUF[2];
    // InBuffer[0] = -(BUF[3]<<8)|BUF[2];

    BUF[4]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_L);
    BUF[5]=I2C_ReadOneByte(GYRO_ADDRESS,GYRO_ZOUT_H);
    InBuffer[2] = ((BUF[5]<<8)|BUF[4]);	
    // InBuffer[2] = -((BUF[5]<<8)|BUF[4]);
    

    for(i = 0; i < 3; i ++)	
    {
        MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
    }
#if 0  
    gyro[0] = *(OutBuffer + 0) - MPU9250_Offset.X;
    gyro[1] = *(OutBuffer + 1) - MPU9250_Offset.Y;
    gyro[2] = *(OutBuffer + 2) - MPU9250_Offset.Z;
#else
    *ps16X = *(OutBuffer + 0) - MPU9250_Offset.X;
    *ps16Y = *(OutBuffer + 1) - MPU9250_Offset.Y;
    *ps16Z = *(OutBuffer + 2) - MPU9250_Offset.Z;
#endif    
    return;
}

extern void invmsMPU9250AccelRead(int16_t* ps16X, int16_t* ps16Y, int16_t* ps16Z)
{
    uint8_t i;
    int16_t InBuffer[3] = {0}; 
    static int32_t OutBuffer[3] = {0};
    static MPU9250_AvgTypeDef MPU9250_Filter[3];

    switch_lq(0);

    BUF[0]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_L); 
    BUF[1]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_XOUT_H);
    InBuffer[0]=	(BUF[1]<<8)|BUF[0];

    BUF[2]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_L);
    BUF[3]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_YOUT_H);
    InBuffer[1]=	(BUF[3]<<8)|BUF[2];
    // InBuffer[1]=	-((BUF[3]<<8)|BUF[2]);
                   
    BUF[4]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_L);
    BUF[5]=I2C_ReadOneByte(ACCEL_ADDRESS,ACCEL_ZOUT_H);
    InBuffer[2]=	(BUF[5]<<8)|BUF[4];			       
    // InBuffer[2]=	-((BUF[5]<<8)|BUF[4]);
    // InBuffer[2] = convert(InBuffer[2]);

    for(i = 0; i < 3; i ++)	
    {
        MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
    }
#if 1
    *ps16X = *(OutBuffer + 0);
    *ps16Y = *(OutBuffer + 1);
    *ps16Z = *(OutBuffer + 2); 
#else    
    accel[0] = *(OutBuffer + 0);
    accel[1] = *(OutBuffer + 1);
    accel[2] = *(OutBuffer + 2); 
#endif    
    return;
}

extern void invmsMPU9250MagRead(int16_t* ps16X, int16_t* ps16Y, int16_t* ps16Z)
{
    uint8_t i;
    int16_t InBuffer[3] = {0}; 
    static int32_t OutBuffer[3] = {0};
    static MPU9250_AvgTypeDef MPU9250_Filter[3];

    switch_lq(1);

    // I2C_WriteOneByte(GYRO_ADDRESS,0x37,0x02);//turn on Bypass Mode 
    // Delay_ms(10);
    // I2C_WriteOneByte(MAG_ADDRESS,0x0A,0x01);	
    // Delay_ms(10);
    BUF[0]=I2C_ReadOneByte (MAG_ADDRESS,MAG_XOUT_L);
    BUF[1]=I2C_ReadOneByte (MAG_ADDRESS,MAG_XOUT_H);
    InBuffer[0] =(BUF[1]<<8)|BUF[0];

    BUF[2]=I2C_ReadOneByte(MAG_ADDRESS,MAG_YOUT_L);
    BUF[3]=I2C_ReadOneByte(MAG_ADDRESS,MAG_YOUT_H);
    InBuffer[1] =	(BUF[3]<<8)|BUF[2];
    // InBuffer[0] = -InBuffer[0];

    BUF[4]=I2C_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_L);
    BUF[5]=I2C_ReadOneByte(MAG_ADDRESS,MAG_ZOUT_H);
    InBuffer[2] =	(BUF[5]<<8)|BUF[4];	
    // InBuffer[2] = -InBuffer[2];

    for(i = 0; i < 3; i ++)	
    {
        MPU9250_CalAvgValue(&MPU9250_Filter[i].Index, MPU9250_Filter[i].AvgBuffer, InBuffer[i], OutBuffer + i);
    }
#if 0
    magn[0] = *(OutBuffer + 0);
    magn[1] = *(OutBuffer + 1);
    magn[2] = *(OutBuffer + 2);    
#else
    *ps16X = *(OutBuffer + 0);
    *ps16Y = *(OutBuffer + 1);
    *ps16Z = *(OutBuffer + 2);    
#endif    
    return;
}

/**
  * @brief  Digital filter
  * @param *pIndex:
  * @param *pAvgBuffer:
  * @param InVal:
  * @param pOutVal:
  *
  * @retval None
  *               
  */
void MPU9250_CalAvgValue(uint8_t *pIndex, int16_t *pAvgBuffer, int16_t InVal, int32_t *pOutVal)
{	
	uint8_t i;
	
	*(pAvgBuffer + ((*pIndex) ++)) = InVal;
  	*pIndex &= 0x07;
  	
  	*pOutVal = 0;
	for(i = 0; i < 8; i ++) 
  	{
    	*pOutVal += *(pAvgBuffer + i);
  	}
  	*pOutVal >>= 3;
}
/**
  * @brief  Initializes gyroscopes offset
  * @param  None
  * @retval None
  */
void MPU9250_InitGyrOffset(void)
{
	uint8_t i;
	int32_t	TempGx = 0, TempGy = 0, TempGz = 0;
	int16_t s16Data[3];
    
 	for(i = 0; i < 32; i ++)
 	{
		invmsMPU9250GyroRead(&s16Data[0],&s16Data[1],&s16Data[2]);
		
		TempGx += s16Data[0];
		TempGy += s16Data[1];
		TempGz += s16Data[2];

		Delay_us(100);
	}

	MPU9250_Offset.X = TempGx >> 5;
	MPU9250_Offset.Y = TempGy >> 5;
	MPU9250_Offset.Z = TempGz >> 5;

}
