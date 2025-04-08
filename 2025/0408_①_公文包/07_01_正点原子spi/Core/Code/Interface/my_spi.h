#include "__interface.h"


#ifndef __Struct__SPI__Definition__
#define __Struct__SPI__Definition__

    typedef struct __SPI_Class_Struct
    {
        uint16_t CS_Pin;
        GPIO_TypeDef* CS_Port;
        uint16_t SCK_Pin;
        GPIO_TypeDef* SCK_Port;
        uint16_t MISO_Pin;
        GPIO_TypeDef* MISO_Port;
        uint16_t MOSI_Pin;
        GPIO_TypeDef* MOSI_Port;
        SPI_HandleTypeDef SPI_Handle;
    }SPI_Class_Struct;

    // extern SPI_Class_Struct SPI_Class_1;
    // extern SPI_Class_Struct SPI_Class_2;

#endif














