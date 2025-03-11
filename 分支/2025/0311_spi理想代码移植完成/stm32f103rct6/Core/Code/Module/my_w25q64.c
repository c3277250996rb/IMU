#include "my_w25q64.h"


uint32_t W25Q64_Read_Device_ID(void){
    uint32_t Temp = 0;

    /* Select the FLASH: Chip Select low */
    SPI_GPIO_Write_CS_0(SPI_Class_1);

    /* Send "RDID " instruction */
    SPI_Register_Write_Read(SPI_Class_1, W25X_DeviceID);
    SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);
    SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);
    SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp = SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    SPI_GPIO_Write_CS_1(SPI_Class_1);

    return Temp;
}

uint32_t W25Q64_Read_JEDEC_ID(void){
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* 开始通讯：CS低电平 */
    SPI_GPIO_Write_CS_0(SPI_Class_1);

    /* 发送JEDEC指令，读取ID */
    SPI_Register_Write_Read(SPI_Class_1, W25X_JedecDeviceID);

    /* 读取一个字节数据 */
    Temp0 = SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);

    /* 读取一个字节数据 */
    Temp1 = SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);

    /* 读取一个字节数据 */
    Temp2 = SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);

    /* 停止通讯：CS高电平 */
    SPI_GPIO_Write_CS_1(SPI_Class_1);

        /*把数据组合起来，作为函数的返回值*/
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

void W25Q64_Erase_Sector(uint32_t SectorAddress){
    /* 发送FLASH写使能命令 */
    W25Q64_WriteEnable();
    W25Q64_WaitForWriteFinish();
    /* 擦除扇区 */
    /* 选择FLASH: CS低电平 */
    SPI_GPIO_Write_CS_0(SPI_Class_1);
    /* 发送扇区擦除指令*/
    SPI_Register_Write_Read(SPI_Class_1, W25X_SectorErase);
    /*发送擦除扇区地址的高位*/
    SPI_Register_Write_Read(SPI_Class_1, (SectorAddress & 0xFF0000) >> 16);
    /* 发送擦除扇区地址的中位 */
    SPI_Register_Write_Read(SPI_Class_1, (SectorAddress & 0xFF00) >> 8);
    /* 发送擦除扇区地址的低位 */
    SPI_Register_Write_Read(SPI_Class_1, SectorAddress & 0xFF);
    /* 停止信号 FLASH: CS 高电平 */
    SPI_GPIO_Write_CS_1(SPI_Class_1);
    /* 等待擦除完毕*/
    W25Q64_WaitForWriteFinish();
}

void W25Q64_Erase_Chip(void){
  /* 发送FLASH写使能命令 */
  W25Q64_WriteEnable();

  /* 整块 Erase */
  /* 选择FLASH: CS低电平 */
  SPI_GPIO_Write_CS_0(SPI_Class_1);
  /* 发送整块擦除指令*/
  SPI_Register_Write_Read(SPI_Class_1, W25X_ChipErase);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_GPIO_Write_CS_1(SPI_Class_1);

  /* 等待擦除完毕*/
  W25Q64_WaitForWriteFinish();
}

