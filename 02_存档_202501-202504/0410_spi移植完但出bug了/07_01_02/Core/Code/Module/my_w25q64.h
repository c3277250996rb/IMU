/*金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮*/
#ifndef __norflash_H
#define __norflash_H

#include "atk_sys.h"
#include "my_spi.h"

// head
#if 1
/* FLASH芯片列表 */
#define W25Q80      0XEF13          /* W25Q80   芯片ID */
#define W25Q16      0XEF14          /* W25Q16   芯片ID */
#define W25Q32      0XEF15          /* W25Q32   芯片ID */
#define W25Q64      0XEF16          /* W25Q64   芯片ID */
#define W25Q128     0XEF17          /* W25Q128  芯片ID */
#define W25Q256     0XEF18          /* W25Q256  芯片ID */
#define BY25Q64     0X6816          /* BY25Q64  芯片ID */
#define BY25Q128    0X6817          /* BY25Q128 芯片ID */
#define NM25Q64     0X5216          /* NM25Q64  芯片ID */
#define NM25Q128    0X5217          /* NM25Q128 芯片ID */

extern uint16_t norflash_TYPE;      /* 定义FLASH芯片型号 */

/* 指令表 */
#define FLASH_WriteEnable           0x06 
#define FLASH_WriteDisable          0x04 
#define FLASH_ReadStatusReg1        0x05 
#define FLASH_ReadStatusReg2        0x35 
#define FLASH_ReadStatusReg3        0x15 
#define FLASH_WriteStatusReg1       0x01 
#define FLASH_WriteStatusReg2       0x31 
#define FLASH_WriteStatusReg3       0x11 
#define FLASH_ReadData              0x03 
#define FLASH_FastReadData          0x0B 
#define FLASH_FastReadDual          0x3B 
#define FLASH_FastReadQuad          0xEB  
#define FLASH_PageProgram           0x02 
#define FLASH_PageProgramQuad       0x32 
#define FLASH_BlockErase            0xD8 
#define FLASH_SectorErase           0x20 
#define FLASH_ChipErase             0xC7 
#define FLASH_PowerDown             0xB9 
#define FLASH_ReleasePowerDown      0xAB 
#define FLASH_DeviceID              0xAB 
#define FLASH_ManufactDeviceID      0x90 
#define FLASH_JedecDeviceID         0x9F 
#define FLASH_Enable4ByteAddr       0xB7
#define FLASH_Exit4ByteAddr         0xE9
#define FLASH_SetReadParam          0xC0 
#define FLASH_EnterQPIMode          0x38
#define FLASH_ExitQPIMode           0xFF

#endif


void W25Q64_Init(void);

static void W25Q64_Wait_Busy(void);

static void W25Q64_Send_Address(uint32_t address);

void W25Q64_Write(uint8_t *pbuf, uint32_t addr, uint16_t datalen);

static void W25Q64_Write_NoCheck(uint8_t *pbuf, uint32_t addr, uint16_t datalen);

static void W25Q64_Write_Page(uint8_t *pbuf, uint32_t addr, uint16_t datalen);

void W25Q64_Read(uint8_t *pbuf, uint32_t addr, uint16_t datalen);

void W25Q64_Erase_Chip(void);

void W25Q64_Erase_Sector(uint32_t saddr);

uint16_t W25Q64_Read_ID(void);

uint8_t W25Q64_Read_Status_Register(uint8_t regno);

void W25Q64_Write_Status_Register(uint8_t regno, uint8_t sr);

void W25Q64_Write_ENABLE(void);



#endif


