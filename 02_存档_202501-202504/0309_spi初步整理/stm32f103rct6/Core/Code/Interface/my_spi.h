#include "__interface.h"


#ifndef __Struct__SPI__Definition
#define __Struct__SPI__Definition

    extern SPI_Class_1;
    extern SPI_Class_2;

    extern SPI_HandleTypeDef SPI_1_Handle;
    extern SPI_HandleTypeDef SPI_2_Handle;

#endif


#ifndef __W25Q64__SPI__Definition__
#define __W25Q64__SPI__Definition__

    void W25Q64_WriteEnable(void);

    void W25Q64_WaitForWriteFinish(void);

    void W25Q64_PowerDown(void);

    void W25Q64_ReleasePowerDown(void);

#endif


#ifndef __Map__SPI__Definition__
#define __Map__SPI__Definition__

    void SPI_1_Read(uint8_t* input_Pointer, uint16_t ReadSize, uint32_t ReadAddress);
    void SPI_1_Write(uint8_t* input_Pointer, uint16_t WriteSize, uint32_t WriteAddress);

    void SPI_2_Read(uint8_t* input_Pointer, uint16_t ReadSize, uint32_t ReadAddress);
    void SPI_2_Write(uint8_t* input_Pointer, uint16_t WriteSize, uint32_t WriteAddress);

#endif


#ifndef __SPI__APP__Definition__
#define __SPI__APP__Definition__

    void SPI_APP_Buffer_Read(   SPI_HandleTypeDef input_SPI_Handle,
                                uint8_t* input_Pointer, 
                                uint16_t ReadSize, 
                                uint32_t ReadAddress);

    void SPI_APP_Buffer_Write(  SPI_HandleTypeDef input_SPI_Handle,
                                uint8_t* input_Pointer, 
                                uint16_t WriteSize, 
                                uint32_t WriteAddress);

#endif

#ifndef __SPI__Driver__Definition__
#define __SPI__Driver__Definition__

    void SPI_Driver_Write(  SPI_HandleTypeDef input_SPI_Handle,
                            uint8_t* input_Pointer, 
                            uint16_t WriteSize, 
                            uint32_t WriteAddress);

#endif

#ifndef __SPI__Register__Definition__
#define __SPI__Register__Definition__

    void SPI_Register_Write_Read(SPI_HandleTypeDef input_SPI_Handle, uint8_t input_Data);

    static void SPI_Register_Timeout(SPI_HandleTypeDef input_SPI_Handle, uint8_t input_Message);


    #define SPI_Register_CS_1() MY_GPIO_1()
    #define SPI_Register_CS_0() MY_GPIO_0()

#endif











