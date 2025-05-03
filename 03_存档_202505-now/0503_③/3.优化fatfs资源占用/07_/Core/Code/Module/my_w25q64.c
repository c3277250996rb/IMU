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

    /* ��ʼͨѶ��CS�͵�ƽ */
    SPI_GPIO_Write_CS_0(SPI_Class_1);

    /* ����JEDECָ���ȡID */
    SPI_Register_Write_Read(SPI_Class_1, W25X_JedecDeviceID);

    /* ��ȡһ���ֽ����� */
    Temp0 = SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);

    /* ��ȡһ���ֽ����� */
    Temp1 = SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);

    /* ��ȡһ���ֽ����� */
    Temp2 = SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);

    /* ֹͣͨѶ��CS�ߵ�ƽ */
    SPI_GPIO_Write_CS_1(SPI_Class_1);

        /*�����������������Ϊ�����ķ���ֵ*/
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

void W25Q64_Erase_Sector(uint32_t SectorAddress){
    /* ����FLASHдʹ������ */
    W25Q64_WriteEnable();
    
    W25Q64_WaitForWriteFinish();
    /* �������� */
    /* ѡ��FLASH: CS�͵�ƽ */
    SPI_GPIO_Write_CS_0(SPI_Class_1);
    /* ������������ָ��*/
    SPI_Register_Write_Read(SPI_Class_1, W25X_SectorErase);
    /*���Ͳ���������ַ�ĸ�λ*/
    SPI_Register_Write_Read(SPI_Class_1, (SectorAddress & 0xFF0000) >> 16);
    /* ���Ͳ���������ַ����λ */
    SPI_Register_Write_Read(SPI_Class_1, (SectorAddress & 0xFF00) >> 8);
    /* ���Ͳ���������ַ�ĵ�λ */
    SPI_Register_Write_Read(SPI_Class_1, SectorAddress & 0xFF);
    /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
    SPI_GPIO_Write_CS_1(SPI_Class_1);
    /* �ȴ��������*/
    
    W25Q64_WaitForWriteFinish();
}

void W25Q64_Erase_Chip(void){
  /* ����FLASHдʹ������ */
  W25Q64_WriteEnable();

  /* ���� Erase */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_GPIO_Write_CS_0(SPI_Class_1);
  /* �����������ָ��*/
  SPI_Register_Write_Read(SPI_Class_1, W25X_ChipErase);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_GPIO_Write_CS_1(SPI_Class_1);

  /* �ȴ��������*/
  
  W25Q64_WaitForWriteFinish();
}











#ifndef USE_E_S_Test
#define USE_E_S_Test


#if 1


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

   
/* ���ͻ�������ʼ�� */
uint8_t Tx_Buffer[] = "��л��ѡ��Ұ��stm32������\r\nhttp://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];

//��ȡ��ID�洢λ��
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// ����ԭ������
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
int my_w25q64_self_test(void)
{   
  /* �趨ϵͳʱ��Ϊ72MHz */
  // SystemClock_Config();
	
	// LED_GPIO_Config();
	
	/* ���ô���1Ϊ��115200 8-N-1 */
	// DEBUG_USART_Config();
  
	printf("\r\n����һ��8M����flash(W25Q64)ʵ��(SPI����) \r\n");
	
  /* 16M����flash W25Q64��ʼ�� */
	// SPI_FLASH_Init();
		/* ��ȡ Flash Device ID */
	// DeviceID = SPI_FLASH_ReadDeviceID();
	DeviceID = W25Q64_Read_Device_ID();
	
	delay_ms( 200 );
	
	/* ��ȡ SPI Flash ID */
	// FlashID = SPI_FLASH_ReadID();
	FlashID = W25Q64_Read_JEDEC_ID();
	
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* ���� SPI Flash ID */
	if (FlashID == sFLASH_ID) 
	{	
		printf("\r\n��⵽SPI FLASH W25Q64 !\r\n");
		
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		// SPI_FLASH_SectorErase(FLASH_SectorToErase);	 	 
		W25Q64_Erase_Sector(FLASH_SectorToErase);	 	 
		
		/* �����ͻ�����������д��flash�� */
		// SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
		SPI_APP_Buffer_Write(SPI_Class_1, Tx_Buffer, BufferSize, FLASH_WriteAddress);
		printf("\r\nд�������Ϊ��\r\n%s", Tx_Buffer);
		
		/* ���ո�д������ݶ������ŵ����ջ������� */
		// SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		SPI_APP_Buffer_Read(SPI_Class_1, Rx_Buffer, BufferSize, FLASH_ReadAddress);
		printf("\r\n����������Ϊ��\r\n%s", Rx_Buffer);
		
		/* ���д�������������������Ƿ���� */
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		
		if( PASSED == TransferStatus1 )
		{    
			// LED2_ON;
			printf("\r\n8M����flash(W25Q64)���Գɹ�!\n\r");
		}
		else
		{        
			// LED1_ON;
			printf("\r\n8M����flash(W25Q64)����ʧ��!\n\r");
		}
	}// if (FlashID == sFLASH_ID)
	else
	{    
		// LED1_ON;
		printf("\r\n��ȡ���� W25Q64 ID!\n\r");
	}
	
	// SPI_Flash_PowerDown();  
    W25Q64_PowerDown();  
	// while(1);  
}

/*
 * ��������Buffercmp
 * ����  ���Ƚ������������е������Ƿ����
 * ����  ��-pBuffer1     src������ָ��
 *         -pBuffer2     dst������ָ��
 *         -BufferLength ����������
 * ���  ����
 * ����  ��-PASSED pBuffer1 ����   pBuffer2
 *         -FAILED pBuffer1 ��ͬ�� pBuffer2
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

BYTE work[4096];//��ʽ���豸������

FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] = "��ӭʹ��Ұ��STM32������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n"; 
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int embed_fire_fatfs_self_test(void)
{
  // HAL_Init();        
  // /* ����ϵͳʱ��Ϊ72 MHz */ 
  // SystemClock_Config();
	// /* ��ʼ��RGB�ʵ� */ 
  // LED_GPIO_Config(); 
  // /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
  // DEBUG_USART_Config();
  
  
	printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
  
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	//��ʼ������������������
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);
	
/*----------------------- ��ʽ������ -----------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
    /* ��ʽ�� */
		res_flash=f_mkfs("1:", 0, work, sizeof(work));						
		
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
			// LED1(ON);
			printf("������ʽ��ʧ�ܡ�����\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
    printf("������ SPI����д����FLASH ���̲��ԣ�����������ڸ�����f_mount�����if���ǰ��ʱ�����һ�� res_flash = FR_NO_FILESYSTEM; ������ֱ��ִ�и�ʽ������\r\n");
		while(1);
  }
  else
  {
    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
  
/*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
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
		// LED1(ON);;
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	
/*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		// LED2(ON);
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
		// LED1(ON);
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"1:",1);

  printf("\r\n���Գɹ�\r\n");
  
  /* ������ɣ�ͣ�� */
	while(1)
	{
	}
}


#endif

#endif
















