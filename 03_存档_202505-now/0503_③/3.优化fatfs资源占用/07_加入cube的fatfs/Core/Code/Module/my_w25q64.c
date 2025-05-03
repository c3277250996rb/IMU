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


#if 1


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
	// SPI_FLASH_Init();
		/* 获取 Flash Device ID */
	// DeviceID = SPI_FLASH_ReadDeviceID();
	DeviceID = W25Q64_Read_Device_ID();
	
	delay_ms( 200 );
	
	/* 获取 SPI Flash ID */
	// FlashID = SPI_FLASH_ReadID();
	FlashID = W25Q64_Read_JEDEC_ID();
	
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* 检验 SPI Flash ID */
	if (FlashID == sFLASH_ID) 
	{	
		printf("\r\n检测到SPI FLASH W25Q64 !\r\n");
		
		/* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
		// SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		W25Q64_Erase_Sector(FLASH_SectorToErase);	 	 
		
		/* 将发送缓冲区的数据写到flash中 */
		// SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		SPI_APP_Buffer_Write(SPI_Class_1, Tx_Buffer, BufferSize, FLASH_WriteAddress);
		printf("\r\n写入的数据为：\r\n%s", Tx_Buffer);
		
		/* 将刚刚写入的数据读出来放到接收缓冲区中 */
		// SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
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
	
	// SPI_Flash_PowerDown();  
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




#ifndef USE_E_F_Test
#define USE_E_F_Test


#if 0


#include "main.h"
#include "stm32l1xx.h"
#include "ff.h"

BYTE work[4096];//格式化设备工作区

FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] = "欢迎使用野火STM32开发板 今天是个好日子，新建文件系统测试文件\r\n"; 
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int embed_fire_fatfs_self_test(void)
{
  // HAL_Init();        
  // /* 配置系统时钟为72 MHz */ 
  // SystemClock_Config();
	// /* 初始化RGB彩灯 */ 
  // LED_GPIO_Config(); 
  // /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  // DEBUG_USART_Config();
  
  
	printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
  
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	//初始化函数调用流程如下
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);
	
/*----------------------- 格式化测试 -----------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
    /* 格式化 */
		res_flash=f_mkfs("1:", 0, work, sizeof(work));						
		
		if(res_flash == FR_OK)
		{
			printf("》FLASH已成功格式化文件系统。\r\n");
      /* 格式化后，先取消挂载 */
			res_flash = f_mount(NULL,"1:",1);			
      /* 重新挂载	*/			
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			// LED1(ON);
			printf("《《格式化失败。》》\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
    printf("请下载 SPI—读写串行FLASH 例程测试，如果正常，在该例程f_mount语句下if语句前临时多添加一句 res_flash = FR_NO_FILESYSTEM; 让重新直接执行格式化流程\r\n");
		while(1);
  }
  else
  {
    printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
  
/*----------------------- 文件系统测试：写测试 -------------------*/
	/* 打开文件，每次都以新建的形式打开，属性为可写 */
	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
	res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_flash == FR_OK )
	{
		printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
    /* 将指定存储区内容写入到文件内 */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf("》文件写入成功，写入字节数据：%d\n",fnum);
      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("！！文件写入失败：(%d)\n",res_flash);
    }    
		/* 不再读写，关闭文件 */
    f_close(&fnew);
	}
	else
	{	
		// LED1(ON);;
		printf("！！打开/创建文件失败。\r\n");
	}
	
/*------------------- 文件系统测试：读测试 --------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		// LED2(ON);
		printf("》打开文件成功。\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
      printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("！！文件读取失败：(%d)\n",res_flash);
    }		
	}
	else
	{
		// LED1(ON);
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"1:",1);

  printf("\r\n测试成功\r\n");
  
  /* 操作完成，停机 */
	while(1)
	{
	}
}


#endif

#endif
















