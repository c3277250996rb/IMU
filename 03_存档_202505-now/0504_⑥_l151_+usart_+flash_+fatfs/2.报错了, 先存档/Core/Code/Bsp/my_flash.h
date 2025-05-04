#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H
#include <stdio.h>


// flash config
#if 1

#include "main.h"
    #if defined(__STM32F1XX_H)
        // F103
    #elif defined(__STM32L1XX_H)
    // L151
        #include "stm32l1xx.h"

        /* Private typedef -----------------------------------------------------------*/
        //#define  sFLASH_ID                       0xEF3015     //W25X16
        //#define  sFLASH_ID                       0xEF4015	    //W25Q16
        //#define  sFLASH_ID                        0XEF4017     //W25Q64
        #define  sFLASH_ID                       0XEF4018     //W25Q128

        //SPI号及时钟初始化函数
        #define SPIx                             SPI1
        #define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
        #define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
        #define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
        #define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
        #define SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE() 

        #define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
        #define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

        #ifndef SPI1_CS_PA4_Pin
            #define SPI1_CS_PA4_Pin GPIO_PIN_4
            #define SPI1_CS_PA4_GPIO_Port GPIOA
            #define SPI1_SCK_PA5_Pin GPIO_PIN_5
            #define SPI1_SCK_PA5_GPIO_Port GPIOA
            #define SPI1_MISO_PA6_Pin GPIO_PIN_6
            #define SPI1_MISO_PA6_GPIO_Port GPIOA
            #define SPI1_MOSI_PA7_Pin GPIO_PIN_7
            #define SPI1_MOSI_PA7_GPIO_Port GPIOA
        #endif

        //SCK引脚
        #define SPIx_SCK_PIN                     SPI1_SCK_PA5_Pin
        #define SPIx_SCK_GPIO_PORT               SPI1_SCK_PA5_GPIO_Port
        #define SPIx_SCK_AF                      GPIO_AF5_SPI1
        //MISO引脚
        #define SPIx_MISO_PIN                    SPI1_MISO_PA6_Pin
        #define SPIx_MISO_GPIO_PORT              SPI1_MISO_PA6_GPIO_Port
        #define SPIx_MISO_AF                     GPIO_AF5_SPI1
        //MOSI引脚
        #define SPIx_MOSI_PIN                    SPI1_MOSI_PA7_Pin
        #define SPIx_MOSI_GPIO_PORT              SPI1_MOSI_PA7_GPIO_Port
        #define SPIx_MOSI_AF                     GPIO_AF5_SPI1
        //CS(NSS)引脚
        #define FLASH_CS_PIN                     SPI1_CS_PA4_Pin               
        #define FLASH_CS_GPIO_PORT               SPI1_CS_PA4_GPIO_Port       
        //GPIO SPEED
        #define SPIx_GPIO_SPEED                  GPIO_SPEED_FREQ_VERY_HIGH
        /* ---------------------------------------------------------------------------*/              

    #elif defined(__STM32F4xx_H)
    // F407
        #include "stm32f4xx.h"

        /* Private typedef -----------------------------------------------------------*/
        //#define  sFLASH_ID                       0xEF3015     //W25X16
        //#define  sFLASH_ID                       0xEF4015	    //W25Q16
        //#define  sFLASH_ID                        0XEF4017     //W25Q64
        #define  sFLASH_ID                       0XEF4018     //W25Q128

        //SPI号及时钟初始化函数
        #define SPIx                             SPI1
        #define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
        #define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
        #define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 
        #define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 
        #define SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOG_CLK_ENABLE() 

        #define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
        #define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

        #ifndef SPI1_CS_PG6_Pin
            #define SPI1_CS_PG6_Pin GPIO_PIN_6
            #define SPI1_CS_PG6_GPIO_Port GPIOG
            #define SPI1_SCK_PB3_Pin GPIO_PIN_3
            #define SPI1_SCK_PB3_GPIO_Port GPIOB
            #define SPI1_MISO_PB4_Pin GPIO_PIN_4
            #define SPI1_MISO_PB4_GPIO_Port GPIOB
            #define SPI1_MOSI_PB5_Pin GPIO_PIN_5
            #define SPI1_MOSI_PB5_GPIO_Port GPIOB
        #endif

        //SCK引脚
        #define SPIx_SCK_PIN                     SPI1_SCK_PB3_Pin
        #define SPIx_SCK_GPIO_PORT               SPI1_SCK_PB3_GPIO_Port
        #define SPIx_SCK_AF                      GPIO_AF5_SPI1
        //MISO引脚
        #define SPIx_MISO_PIN                    SPI1_MISO_PB4_Pin
        #define SPIx_MISO_GPIO_PORT              SPI1_MISO_PB4_GPIO_Port
        #define SPIx_MISO_AF                     GPIO_AF5_SPI1
        //MOSI引脚
        #define SPIx_MOSI_PIN                    SPI1_MOSI_PB5_Pin
        #define SPIx_MOSI_GPIO_PORT              SPI1_MOSI_PB5_GPIO_Port
        #define SPIx_MOSI_AF                     GPIO_AF5_SPI1
        //CS(NSS)引脚
        #define FLASH_CS_PIN                     SPI1_CS_PG6_Pin               
        #define FLASH_CS_GPIO_PORT               SPI1_CS_PG6_GPIO_Port          
        //GPIO SPEED
        #define SPIx_GPIO_SPEED                  GPIO_SPEED_FAST
        /* ---------------------------------------------------------------------------*/              

    #else
        #error "啊? MCU不是F103, L151或者F407吗? 这..."
    #endif

#endif
// flash config


// head
#if 1
//#define SPI_FLASH_PageSize            4096
#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		  0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	  0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/
//设置为高电平	
#define	digitalHi(p,i)			    {p->BSRR=i;}		
//输出低电平
#define digitalLo(p,i)			    {p->BSRR=(uint32_t)i << 16;}				
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



void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif
// head


int my_flash_self_test(void)


// 给到usmart的函数
#if 0

    (void*)my_flash_self_test,"int my_flash_self_test(void)",

#endif
// 给到usmart的函数


#endif /* __SPI_FLASH_H */

