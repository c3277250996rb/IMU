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


void my_fatfs_self_test(void);


// fatfs functions
#if 1

    typedef struct __FATFS_Result_Struct
    {
        int File_Size;
    }FATFS_Result_Struct;

    extern FATFS_Result_Struct FATFS_Result_default;
    extern long FATFS_Index_Current;
    extern float float_fs_index_current;

    #define MAX_ROWS 100
    #define MAX_COLS 20
    #define MAX_BUF 1024

    extern float sensor_storage_data[MAX_ROWS][MAX_COLS];  // 存放解析后的数据

    int FATFS_Read(char* path, uint8_t is_echo);
    void FATFS_Write(char* path, char* write_buffer, int write_position, uint8_t is_echo, uint8_t fast_mode);
    void FATFS_Delete(char* path, uint8_t is_scan, uint8_t is_secure);
    void i_file_read(char* path, uint8_t is_echo);

    void FATFS_Init(uint8_t is_secure);
    void FATFS_Deinit(uint8_t is_secure);
    void FATFS_Scan(void);
    void FATFS_Muti_Test(void);
    void FATFS_Index_Check(uint8_t is_append);
    void FATFS_Convert_Array(char *input_Path);

    FRESULT get_last_line_from_file(const char *ipath, char *ibuffer, size_t ibuffer_size);
    void convert_text_to_array(const char *input);

    float conver_from_long_to_float(long input_Long);


#endif
// fatfs functions


// 给到usmart的函数
#if 0

	(void*)my_fatfs_self_test,"void my_fatfs_self_test(void)",
	(void*)i_file_read,"void i_file_read(char* path, uint8_t is_echo)",
	(void*)FATFS_Write,"void FATFS_Write(char* path, char* write_buffer, int write_position, uint8_t is_echo)",
	(void*)FATFS_Delete,"void FATFS_Delete(char* path, uint8_t is_scan, uint8_t is_secure)",
	(void*)FATFS_Scan,"void FATFS_Scan(void)",
	(void*)FATFS_Index_Check,"void FATFS_Index_Check(uint8_t is_append)",
	(void*)FATFS_Deinit,"void FATFS_Deinit(uint8_t is_secure)",
	(void*)FATFS_Init,"void FATFS_Init(uint8_t is_secure)",
	(void*)FATFS_Muti_Test,"void FATFS_Muti_Test(void)",

	(void*)FATFS_Convert_Array,"void FATFS_Convert_Array(char *input_Path)",

#endif
// 给到usmart的函数



#endif // __MY_FATFS_H


