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

// head
#if 1
void Delay(__IO uint32_t nCount);
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
// #define  BufferSize (countof(Tx_Buffer)-1)
#define  BufferSize 3

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

   
/* 发送缓冲区初始化 */
// uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\nhttp://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];
uint8_t temp_Buffer[BufferSize];
//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}
#endif
void my_w25q64_self_test(void){

    char* w25q64_test_status = "failed?";

	DeviceID = W25Q64_Read_Device_ID();
    HAL_Delay(100);
	FlashID = W25Q64_Read_JEDEC_ID();
    printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);

    if(FlashID == sFLASH_ID){
        printf("\r\n检测到SPI FLASH W25Q64 !\r\n");
		W25Q64_Erase_Sector(FLASH_SectorToErase);	 	 

		SPI_APP_Buffer_Read(SPI_Class_1, Rx_Buffer, BufferSize, FLASH_ReadAddress);
		// printf("\r\n读出的数据为：\r\n%s", Rx_Buffer);
		printf("\r\n读出的数据为：\r\n%x,%x,%x,", Rx_Buffer[0], Rx_Buffer[1], Rx_Buffer[2]);
		
        memcpy(temp_Buffer, Rx_Buffer, BufferSize);

		SPI_APP_Buffer_Write(SPI_Class_1, Rx_Buffer, BufferSize, FLASH_WriteAddress);
		// printf("\r\n写入的数据为：\r\n%s", Rx_Buffer);
		printf("\r\n写入的数据为：\r\n%x,%x,%x,", Rx_Buffer[0], Rx_Buffer[1], Rx_Buffer[2]);
		
		SPI_APP_Buffer_Read(SPI_Class_1, Rx_Buffer, BufferSize, FLASH_ReadAddress);
		// printf("\r\n读出的数据为：\r\n%s", Rx_Buffer);
		printf("\r\n读出的数据为：\r\n%x,%x,%x,", Rx_Buffer[0], Rx_Buffer[1], Rx_Buffer[2]);
		
		TransferStatus1 = Buffercmp(temp_Buffer, Rx_Buffer, BufferSize);
        if(PASSED == TransferStatus1){
            w25q64_test_status = "succeed!";
        }
    }else{
        printf("\r\n获取不到 W25Q64 ID!\n\r");
    }

    W25Q64_PowerDown();  
    printf("\r\nw25q64 test [%s]\r\n", w25q64_test_status);
	// while(1){
    //     // just wait.
    // }
}

