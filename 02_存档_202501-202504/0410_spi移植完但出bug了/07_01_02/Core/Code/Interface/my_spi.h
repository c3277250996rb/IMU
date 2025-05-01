// #include "__interface.h"
#include "main.h"

/* SPI总线速度设置 */
#define SPI_SPEED_2         0
#define SPI_SPEED_4         1
#define SPI_SPEED_8         2
#define SPI_SPEED_16        3
#define SPI_SPEED_32        4
#define SPI_SPEED_64        5
#define SPI_SPEED_128       6
#define SPI_SPEED_256       7


#ifndef __Struct__SPI__Definition__
#define __Struct__SPI__Definition__

    typedef struct __SPI_Class_Struct
    {
        uint16_t CS_Pin;
        GPIO_TypeDef* CS_Port;
        // uint16_t SCK_Pin;
        // GPIO_TypeDef* SCK_Port;
        // uint16_t MISO_Pin;
        // GPIO_TypeDef* MISO_Port;
        // uint16_t MOSI_Pin;
        // GPIO_TypeDef* MOSI_Port;
        // SPI_HandleTypeDef SPI_Handle;
    }SPI_Class_Struct;

    extern SPI_Class_Struct SPI_Class_1;
    // extern SPI_Class_Struct SPI_Class_2;

#endif


extern SPI_HandleTypeDef g_spi1_handler;


uint8_t SPI_1_RW(uint8_t input_Data);


void SPI_APP_Init(SPI_HandleTypeDef *input_SPI_HandleTypeDef);
uint8_t SPI_Read_Write_Byte(SPI_HandleTypeDef *input_SPI_HandleTypeDef, uint8_t input_Data);
void SPI_Set_Speed(SPI_HandleTypeDef *input_SPI_HandleTypeDef, uint8_t input_Speed);


void SPI_GPIO_Write_CS(SPI_Class_Struct *input_SPI_Class, GPIO_PinState input_PinState);
#define SPI_GPIO_Write_CS_1(input_SPI_Class) SPI_GPIO_Write_CS(input_SPI_Class, 1)
#define SPI_GPIO_Write_CS_0(input_SPI_Class) SPI_GPIO_Write_CS(input_SPI_Class, 0)




