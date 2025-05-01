/**
  ******************************************************************************
  * @file    invMotionSensor.c
  * @author  cer
  * @version V1.0
  * @date    October-2018
  * @brief   This file includes the invsense motion sensor driver functions
  
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
  * <h2><center>&copy; COPYRIGHT 2018 Waveshare</center></h2>
  ******************************************************************************
  */
#include "invMotionSensor.h"
#include "MPU9250.h"
#include "I2C.h"
#include "LED.h"


static INVMS_ST_SEN_FUN_INFO gsstSensorList[INVMS_EN_SENSOR_TYPY_MAX] =
{
    {INVMS_EN_SENSOR_TYPY_MPU9255,  invmsMPU9255Check,  invmsMPU9250Init,   invmsMPU9250GyroRead,   invmsMPU9250AccelRead,  invmsMPU9250MagRead},
    {INVMS_EN_SENSOR_TYPY_MPU9250,  invmsMPU9250Check,  invmsMPU9250Init,   invmsMPU9250GyroRead,   invmsMPU9250AccelRead,  invmsMPU9250MagRead},
};
INVMS_EN_SENSOR_TYPY genCurrentSensorType = INVMS_EN_SENSOR_TYPY_MAX;
char * gstring[INVMS_EN_SENSOR_TYPY_MAX] = 
{
    "Motion sersor is MPU9255",
    "Motion sersor is MPU9250",
};

extern void invMSInit()
{
    INVMS_EN_SENSOR_TYPY i;
    Delay_ms(10);
	for(i=INVMS_EN_SENSOR_TYPY_MPU9255; i<INVMS_EN_SENSOR_TYPY_MAX; i++)
	{
        if(gsstSensorList[i].pFunCheck != NULL)
        {   
            if( true == gsstSensorList[i].pFunCheck())
            {
                genCurrentSensorType = i;
                printf("\r\n%s\r\n", gstring[genCurrentSensorType]);
                break;
            }
        }
    }
    
    if( genCurrentSensorType!= INVMS_EN_SENSOR_TYPY_MAX)
    {
        gsstSensorList[genCurrentSensorType].pFunInit();
    }
    else
    {
         printf("\r\n invsense motion sensor have not connect. \r\n");
    }
    
    return;
}
extern void invMSAccelRead(int16_t* ps16AccelX, int16_t* ps16AccelY, int16_t* ps16AccelZ)
{
    if( (genCurrentSensorType != INVMS_EN_SENSOR_TYPY_MAX) &&
        (ps16AccelX != NULL) &&
        (ps16AccelY != NULL) &&
        (ps16AccelZ != NULL) )
    {
        gsstSensorList[genCurrentSensorType].pFunAccelRead(ps16AccelX, ps16AccelY, ps16AccelZ);
    }
    return;
}
extern void invMSGyroRead(int16_t* ps16GyroX, int16_t* ps16GyroY, int16_t* ps16GyroZ)
{
    if( (genCurrentSensorType != INVMS_EN_SENSOR_TYPY_MAX) &&
        (ps16GyroX != NULL) &&
        (ps16GyroY != NULL) &&
        (ps16GyroZ != NULL) )
    {
        gsstSensorList[genCurrentSensorType].pFunGyroRead(ps16GyroX, ps16GyroY, ps16GyroZ);
    }
    return;
}
extern void invMSMagRead(int16_t* ps16MagnX, int16_t* ps16MagnY, int16_t* ps16MagnZ)
{
    if( (genCurrentSensorType != INVMS_EN_SENSOR_TYPY_MAX) &&
        (ps16MagnX != NULL) &&
        (ps16MagnY != NULL) &&
        (ps16MagnZ != NULL) )
    {
        gsstSensorList[genCurrentSensorType].pFunMagRead(ps16MagnX, ps16MagnY, ps16MagnZ);
    }
    return;
}
