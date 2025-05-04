#ifndef __MY_FATFS_H
#define __MY_FATFS_H


#include "main.h"

#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"



// fatfs config
#if 1
    #if defined(__STM32F1XX_H)
        // F103
    #elif defined(__STM32L1XX_H)
    // L151
        /* 扇区偏移6MB，外部Flash文件系统空间放在SPI Flash后面10MB空间 */
        #define FATFS_SECTOR_OFFSET 1024 * 1.5
        /* 扇区数量：2560*4096/1024/1024=10(MB) */
        #define FATFS_SECTOR_COUNT  1024 * 2.5

    /* Map */
        #define MAP_FLASH_ReadID()   SPI_FLASH_ReadID()
        #define MAP_FLASH_Init()     SPI_FLASH_Init()   
        #define MAP_FLASH_WakeUp()   SPI_Flash_WAKEUP() 
        #define MAP_FLASH_BufferRead(pBuffer, ReadAddr, NumByteToRead)   SPI_FLASH_BufferRead(pBuffer, ReadAddr, NumByteToRead) 
        #define MAP_FLASH_SectorErase(SectorAddr)    SPI_FLASH_SectorErase(SectorAddr) 
        #define MAP_FLASH_BufferWrite(pBuffer, WriteAddr, NumByteToWrite)    SPI_FLASH_BufferWrite(pBuffer, WriteAddr, NumByteToWrite) 
    /* --- */
    #elif defined(__STM32F4xx_H)
    // F407
        /* 扇区偏移6MB，外部Flash文件系统空间放在SPI Flash后面10MB空间 */
        #define FATFS_SECTOR_OFFSET 1024 * 1.5
        /* 扇区数量：2560*4096/1024/1024=10(MB) */
        #define FATFS_SECTOR_COUNT  1024 * 2.5

    /* Map */
        #define MAP_FLASH_ReadID()   SPI_FLASH_ReadID()
        #define MAP_FLASH_Init()     SPI_FLASH_Init()   
        #define MAP_FLASH_WakeUp()   SPI_Flash_WAKEUP() 
        #define MAP_FLASH_BufferRead(pBuffer, ReadAddr, NumByteToRead)   SPI_FLASH_BufferRead(pBuffer, ReadAddr, NumByteToRead) 
        #define MAP_FLASH_SectorErase(SectorAddr)    SPI_FLASH_SectorErase(SectorAddr) 
        #define MAP_FLASH_BufferWrite(pBuffer, WriteAddr, NumByteToWrite)    SPI_FLASH_BufferWrite(pBuffer, WriteAddr, NumByteToWrite) 
    /* --- */
    #else
        #error "啊? MCU不是F103, L151或者F407吗? 这..."
    #endif
#endif
// fatfs config


#ifndef __Map__Diskio__Definition__
#define __Map__Diskio__Definition__


    DSTATUS FLASH_Disk_Status(BYTE input_Device);
    DSTATUS FLASH_Disk_Initialize(BYTE input_Device);
    DSTATUS FLASH_Disk_Read(BYTE input_Device, BYTE *buff, DWORD sector, UINT count);
    DSTATUS FLASH_Disk_Write(BYTE input_Device, const BYTE *buff, DWORD sector, UINT count);
    DSTATUS FLASH_Disk_IOCtl(BYTE input_Device, BYTE cmd, void *buff);


#endif // __Map__Diskio__Definition__






#endif // __MY_FATFS_H


