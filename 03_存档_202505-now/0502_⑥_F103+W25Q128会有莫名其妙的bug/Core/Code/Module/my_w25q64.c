#include "my_w25q64.h"



#ifndef USE_EMBEDFIRE_SPI


  /*********** */
 /* MY_W25Q64 */
/*********** */

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



#ifndef USE_E_S_Test
#define USE_E_S_Test


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

   
/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\nhttp://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];

//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int my_w25q64_self_test(void)
{   
  /* 设定系统时钟为72MHz */
  // SystemClock_Config();
	
	// LED_GPIO_Config();
	
	/* 配置串口1为：115200 8-N-1 */
	// DEBUG_USART_Config();
  
	printf("\r\n这是一个8M串行flash(W25Q64)实验(SPI驱动) \r\n");
	
  /* 16M串行flash W25Q64初始化 */
    SPI_APP_Init(SPI_Class_1);
		/* 获取 Flash Device ID */
	DeviceID = W25Q64_Read_Device_ID();
	
	delay_ms( 200 );
	
	/* 获取 SPI Flash ID */
	FlashID = W25Q64_Read_JEDEC_ID();
	
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID) 
	{	
		printf("\r\n检测到SPI FLASH W25Q64 !\r\n");
		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		W25Q64_Erase_Sector(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		SPI_APP_Buffer_Write(SPI_Class_1, Tx_Buffer, BufferSize, FLASH_WriteAddress);
		printf("\r\n写入的数据为：\r\n%s", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		SPI_APP_Buffer_Read(SPI_Class_1, Rx_Buffer, BufferSize, FLASH_ReadAddress);
		printf("\r\n读出的数据为：\r\n%s", Rx_Buffer);
		
		/* 检查写入的数据与读出的数据是否相等 */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			// LED2_ON;
			printf("\r\n8M串行flash(W25Q64)测试成功!\n\r");
		}
		else
		{        
			// LED1_ON;
			printf("\r\n8M串行flash(W25Q64)测试失败!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{    
		// LED1_ON;
		printf("\r\n获取不到 W25Q64 ID!\n\r");
	}
	
	W25Q64_PowerDown();  
	// while(1);  
}

/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
 */
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



#endif



