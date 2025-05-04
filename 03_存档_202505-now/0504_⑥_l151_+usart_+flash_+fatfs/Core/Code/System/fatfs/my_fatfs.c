#include "my_fatfs.h"
#include "my_flash.h"


#ifndef __Map__Diskio__Reference__
#define __Map__Diskio__Reference__


    DSTATUS FLASH_Disk_Status(BYTE input_Device){
        if(SPI_FLASH != input_Device) return STA_NOINIT;

        DSTATUS status = STA_NOINIT;

        if(sFLASH_ID == MAP_FLASH_ReadID()){
            /* �豸ID��ȡ�����ȷ */
            status &= ~STA_NOINIT;
        }else{
            /* �豸ID��ȡ������� */
            status = STA_NOINIT;
        }

        return status;
    }

    DSTATUS FLASH_Disk_Initialize(BYTE input_Device){
        if(SPI_FLASH != input_Device) return STA_NOINIT;

        DSTATUS status = STA_NOINIT;
        
        /* ��ʼ��SPI Flash */
        MAP_FLASH_Init();
        /* ��ʼ��SPI Flash */
        for(int i = 0; i < 500; i++);
        /* ����SPI Flash */
        MAP_FLASH_WakeUp();
        /* ��ȡSPI FlashоƬ״̬ */
        status = FLASH_Disk_Status(input_Device);

        return status;
    }

    DSTATUS FLASH_Disk_Read(BYTE input_Device, BYTE *buff, DWORD sector, UINT count){
        if(SPI_FLASH != input_Device) return RES_PARERR;

        /* ����ƫ��?MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����?MB�ռ� */
        sector += FATFS_SECTOR_OFFSET;
        MAP_FLASH_BufferRead(buff, sector <<12, count<<12);

        return RES_OK;
    }

    DSTATUS FLASH_Disk_Write(BYTE input_Device, const BYTE *buff, DWORD sector, UINT count){
        if(SPI_FLASH != input_Device) return RES_PARERR;
        if (!count) return RES_PARERR;		/* Check parameter */
        
        /* ����ƫ��?MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����?MB�ռ� */
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
            /* ����������FATFS_SECTOR_COUNT*4096/1024/1024=?(MB) */
                *(DWORD * )buff = FATFS_SECTOR_COUNT;
            break;

            case GET_SECTOR_SIZE :
            /* ������С  */
                *(WORD * )buff = 4096;
            break;

            case GET_BLOCK_SIZE :
            /* ͬʱ������������ */
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
  *                              �������
  ******************************************************************************
  */
    FATFS fs;													/* FatFs�ļ�ϵͳ���� */
    FIL fnew;													/* �ļ����� */
    FRESULT res_flash;                /* �ļ�������� */
    UINT fnum;            					  /* �ļ��ɹ���д���� */
    BYTE ReadBuffer[1024]={0};        /* �������� */
    BYTE WriteBuffer[] =              /* д������*/
    "��ӭʹ��Ұ��STM32 F407������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  
 
/**
  ******************************************************************************
  *                                ������
  ******************************************************************************
  */

void my_fatfs_self_test(void)
{
//   SystemClock_Config();
// 	/* ��ʼ��LED */
// 	LED_GPIO_Config();	
// 	LED_BLUE;
  
// 	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
// 	 DEBUG_USART_Config();	
  printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
  
  //���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
  res_flash = f_mount(&fs,"1:",1);
  
/*----------------------- ��ʽ������ ---------------------------*/  
  /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
  if(res_flash == FR_NO_FILESYSTEM)
  {
    printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
    /* ��ʽ�� */
    res_flash=f_mkfs("1:",0,0);							
    
    if(res_flash == FR_OK)
    {
      printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
      /* ��ʽ������ȡ������ */
      res_flash = f_mount(NULL,"1:",1);			
      /* ���¹���	*/			
      res_flash = f_mount(&fs,"1:",1);
    }
    else
    {
      // LED_RED;
      printf("������ʽ��ʧ�ܡ�����\r\n");
      while(1);
    }
  }
  else if(res_flash!=FR_OK)
  {
    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
    while(1);
  }
  else
  {
    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
  
/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
  /* ���ļ�������ļ��������򴴽��� */
  printf("\r\n****** ���������ļ�д�����... ******\r\n");	
  res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
  if ( res_flash == FR_OK )
  {
    printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
    /* ��ָ���洢������д�뵽�ļ��� */
    res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
    }    
    /* ���ٶ�д���ر��ļ� */
    f_close(&fnew);
  }
  else
  {	
    // LED_RED;
    printf("������/�����ļ�ʧ�ܡ�\r\n");
  }
  
/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
  printf("****** ���������ļ���ȡ����... ******\r\n");
  res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ); 	 
  if(res_flash == FR_OK)
  {
    // LED_GREEN;
    printf("�����ļ��ɹ���\r\n");
    res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
    }		
  }
  else
  {
    // LED_RED;
    printf("�������ļ�ʧ�ܡ�\r\n");
  }
  /* ���ٶ�д���ر��ļ� */
  f_close(&fnew);	
  
  /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
  f_mount(NULL,"1:",1);
  
  /* ������ɣ�ͣ�� */
  while(1)
  {
  }
}


#endif
// fatfs self test


