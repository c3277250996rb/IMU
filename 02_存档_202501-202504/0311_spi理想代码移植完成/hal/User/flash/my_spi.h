#include "main.h"

// 后期
#if 0
/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

#define  sFLASH_ID              0XEF4017    //W25Q64


//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/*信息输出*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
#endif

// 前期
#if 1
//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
//#define  sFLASH_ID              0XEF4018   //W25Q128
#define  sFLASH_ID              0XEF4017    //W25Q64


//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/


#define SPIx                             SPI1
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE() 

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN                     GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT               GPIOA

#define SPIx_MISO_PIN                    GPIO_PIN_6
#define SPIx_MISO_GPIO_PORT              GPIOA

#define SPIx_MOSI_PIN                    GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT              GPIOA

// #define FLASH_CS_PIN                     GPIO_PIN_0              
// #define FLASH_CS_GPIO_PORT               GPIOC                     

#define FLASH_CS_PIN                     GPIO_PIN_4              
#define FLASH_CS_GPIO_PORT               GPIOA                     


#define	digitalHi(p,i)			    {p->BSRR=i;}			  //设置为高电平		
#define digitalLo(p,i)			    {p->BSRR=(uint32_t)i << 16;}				//输出低电平
#define SPI_FLASH_CS_LOW()      digitalLo(FLASH_CS_GPIO_PORT,FLASH_CS_PIN )
#define SPI_FLASH_CS_HIGH()     digitalHi(FLASH_CS_GPIO_PORT,FLASH_CS_PIN )
/*SPI接口定义-结尾****************************/

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
#endif

#if 1
#define SPI1_CS_PA4_Pin GPIO_PIN_4
#define SPI1_CS_PA4_GPIO_Port GPIOA
#define SPI1_SCK_PA5_Pin GPIO_PIN_5
#define SPI1_SCK_PA5_GPIO_Port GPIOA
#define SPI1_MISO_PA6_Pin GPIO_PIN_6
#define SPI1_MISO_PA6_GPIO_Port GPIOA
#define SPI1_MOSI_PA7_Pin GPIO_PIN_7
#define SPI1_MOSI_PA7_GPIO_Port GPIOA
#define I2C2_SCL_PB10_Pin GPIO_PIN_10
#define I2C2_SCL_PB10_GPIO_Port GPIOB
#define I2C2_SDA_PB11_Pin GPIO_PIN_11
#define I2C2_SDA_PB11_GPIO_Port GPIOB
#define USART1_PA9_Pin GPIO_PIN_9
#define USART1_PA9_GPIO_Port GPIOA
#define USART1_PA10_Pin GPIO_PIN_10
#define USART1_PA10_GPIO_Port GPIOA
#define I2C1_SCL_PB6_Pin GPIO_PIN_6
#define I2C1_SCL_PB6_GPIO_Port GPIOB
#define I2C1_SDA_PB7_Pin GPIO_PIN_7
#define I2C1_SDA_PB7_GPIO_Port GPIOB
#endif


#ifndef __Struct__SPI__Definition
#define __Struct__SPI__Definition

    // extern SPI_HandleTypeDef SPI_1_Handle;
    // extern SPI_HandleTypeDef SPI_2_Handle;

    
    SPI_HandleTypeDef SPI_1_Handle = {
        .Instance               = SPI1,
        .Init.Mode              = SPI_MODE_MASTER,
        .Init.Direction         = SPI_DIRECTION_2LINES,
        .Init.DataSize          = SPI_DATASIZE_8BIT,
        .Init.CLKPolarity       = SPI_POLARITY_HIGH,
        .Init.CLKPhase          = SPI_PHASE_2EDGE,
        .Init.NSS               = SPI_NSS_SOFT,
        .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4,
        .Init.FirstBit          = SPI_FIRSTBIT_MSB,
        .Init.TIMode            = SPI_TIMODE_DISABLE,
        .Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE,
        .Init.CRCPolynomial     = 7,
    };

    SPI_HandleTypeDef SPI_2_Handle = {
        .Instance               = SPI2
        // .Init.Mode              = 
        // .Init.Direction         = 
        // .Init.DataSize          = 
        // .Init.CLKPolarity       = 
        // .Init.CLKPhase          = 
        // .Init.NSS               = 
        // .Init.BaudRatePrescaler = 
        // .Init.FirstBit          = 
        // .Init.TIMode            = 
        // .Init.CRCCalculation    = 
        // .Init.CRCPolynomial     = 
    };


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
        SPI_HandleTypeDef* SPI_Handle;
    }SPI_Class_Struct;

    extern SPI_Class_Struct SPI_Class_1;
    extern SPI_Class_Struct SPI_Class_2;

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

    void SPI_APP_Buffer_Read(   SPI_Class_Struct input_SPI_Class,
                                uint8_t* input_Pointer, 
                                uint16_t ReadSize, 
                                uint32_t ReadAddress);

    void SPI_APP_Buffer_Write(  SPI_Class_Struct input_SPI_Class,
                                uint8_t* input_Pointer, 
                                uint16_t WriteSize, 
                                uint32_t WriteAddress);

#endif

#ifndef __SPI__Driver__Definition__
#define __SPI__Driver__Definition__

    void SPI_Driver_Write(  SPI_Class_Struct input_SPI_Class,
                            uint8_t* input_Pointer, 
                            uint16_t WriteSize, 
                            uint32_t WriteAddress);

#endif

#ifndef __SPI__Register__Definition__
#define __SPI__Register__Definition__

    uint8_t SPI_Register_Write_Read(SPI_Class_Struct input_SPI_Class, uint8_t input_Data);

    uint16_t SPI_Register_Timeout(SPI_Class_Struct input_SPI_Class, uint8_t input_Message);

    void SPI_GPIO_Write_CS(SPI_Class_Struct input_SPI_Class, GPIO_PinState input_PinState);

    #define SPI_GPIO_Write_CS_1(input_SPI_Class) SPI_GPIO_Write_CS(input_SPI_Class, 1)
    #define SPI_GPIO_Write_CS_0(input_SPI_Class) SPI_GPIO_Write_CS(input_SPI_Class, 0)

#endif











