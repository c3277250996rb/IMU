#include "my_fatfs.h"
#include "my_flash.h"


#ifndef __Map__Diskio__Reference__
#define __Map__Diskio__Reference__


    DSTATUS FLASH_Disk_Status(BYTE input_Device){
        if(SPI_FLASH != input_Device) return STA_NOINIT;

        DSTATUS status = STA_NOINIT;

        if(sFLASH_ID == MAP_FLASH_ReadID()){
            /* 设备ID读取结果正确 */
            status &= ~STA_NOINIT;
        }else{
            /* 设备ID读取结果错误 */
            status = STA_NOINIT;
        }

        return status;
    }

    DSTATUS FLASH_Disk_Initialize(BYTE input_Device){
        if(SPI_FLASH != input_Device) return STA_NOINIT;

        DSTATUS status = STA_NOINIT;
        
        /* 初始化SPI Flash */
        MAP_FLASH_Init();
        /* 初始化SPI Flash */
        for(int i = 0; i < 500; i++);
        /* 唤醒SPI Flash */
        MAP_FLASH_WakeUp();
        /* 获取SPI Flash芯片状态 */
        status = FLASH_Disk_Status(input_Device);

        return status;
    }

    DSTATUS FLASH_Disk_Read(BYTE input_Device, BYTE *buff, DWORD sector, UINT count){
        if(SPI_FLASH != input_Device) return RES_PARERR;

        /* 扇区偏移?MB，外部Flash文件系统空间放在SPI Flash后面?MB空间 */
        sector += FATFS_SECTOR_OFFSET;
        MAP_FLASH_BufferRead(buff, sector <<12, count<<12);

        return RES_OK;
    }

    DSTATUS FLASH_Disk_Write(BYTE input_Device, const BYTE *buff, DWORD sector, UINT count){
        if(SPI_FLASH != input_Device) return RES_PARERR;
        if (!count) return RES_PARERR;		/* Check parameter */
        
        /* 扇区偏移?MB，外部Flash文件系统空间放在SPI Flash后面?MB空间 */
        sector += FATFS_SECTOR_OFFSET;
        sector = sector << 12;
        MAP_FLASH_SectorErase(sector);
        MAP_FLASH_BufferWrite((uint8_t *)buff, sector, count<<12);

        return RES_OK;
    }

    DSTATUS FLASH_Disk_IOCtl(BYTE input_Device, BYTE cmd, void *buff){
        if(SPI_FLASH != input_Device) return RES_PARERR;

        switch (cmd) {
            case GET_SECTOR_COUNT:
            /* 扇区数量：FATFS_SECTOR_COUNT*4096/1024/1024=?(MB) */
                *(DWORD * )buff = FATFS_SECTOR_COUNT;
            break;

            case GET_SECTOR_SIZE :
            /* 扇区大小  */
                *(WORD * )buff = 4096;
            break;

            case GET_BLOCK_SIZE :
            /* 同时擦除扇区个数 */
                *(DWORD * )buff = 1;
            break;

            default:
                return RES_PARERR;
          }

      return RES_OK;
    }


#endif // __Map__Diskio__Reference__



// fatfs self test
#if 1

/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
    FATFS fs;													/* FatFs文件系统对象 */
    FIL fnew;													/* 文件对象 */
    FRESULT res_flash;                /* 文件操作结果 */
    UINT fnum;            					  /* 文件成功读写数量 */
    BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
    BYTE WriteBuffer[] =              /* 写缓冲区*/
    "欢迎使用野火STM32 F407开发板 今天是个好日子，新建文件系统测试文件\r\n";  
 
/**
  ******************************************************************************
  *                                任务函数
  ******************************************************************************
  */

void my_fatfs_self_test(void)
{
//   SystemClock_Config();
// 	/* 初始化LED */
// 	LED_GPIO_Config();	
// 	LED_BLUE;
  
// 	/* 初始化调试串口，一般为串口1 */
// 	 DEBUG_USART_Config();	
  printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
  
  //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
  res_flash = f_mount(&fs,"1:",1);
  
/*----------------------- 格式化测试 ---------------------------*/  
  /* 如果没有文件系统就格式化创建创建文件系统 */
  if(res_flash == FR_NO_FILESYSTEM)
  {
    printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
    /* 格式化 */
    res_flash=f_mkfs("1:",0,0);							
    
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
      // LED_RED;
      printf("《《格式化失败。》》\r\n");
      while(1);
    }
  }
  else if(res_flash!=FR_OK)
  {
    printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
    printf("！！可能原因：SPI Flash初始化不成功。\r\n");
    while(1);
  }
  else
  {
    printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
  
/*----------------------- 文件系统测试：写测试 -----------------------------*/
  /* 打开文件，如果文件不存在则创建它 */
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
    // LED_RED;
    printf("！！打开/创建文件失败。\r\n");
  }
  
/*------------------- 文件系统测试：读测试 ------------------------------------*/
  printf("****** 即将进行文件读取测试... ******\r\n");
  res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ); 	 
  if(res_flash == FR_OK)
  {
    // LED_GREEN;
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
    // LED_RED;
    printf("！！打开文件失败。\r\n");
  }
  /* 不再读写，关闭文件 */
  f_close(&fnew);	
  
  /* 不再使用文件系统，取消挂载文件系统 */
  f_mount(NULL,"1:",1);
  
  /* 操作完成，停机 */
  while(1)
  {
  }
}


#endif
// fatfs self test


