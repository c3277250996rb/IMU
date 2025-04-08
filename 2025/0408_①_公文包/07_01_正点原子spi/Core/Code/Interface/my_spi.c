#include "my_spi.h"



void SPI_APP_Init(SPI_Class_Struct *input_SPI_Class){
    HAL_SPI_Init(input_SPI_Class);
    __HAL_SPI_ENABLE(input_SPI_Class);
}


uint8_t SPI_Read_Write_Byte(SPI_Class_Struct *input_SPI_Class, uint8_t input_Data){
    uint8_t temp_Data;
    HAL_SPI_TransmitReceive(input_SPI_Class, &input_Data, &temp_Data);
    return temp_Data;
}






