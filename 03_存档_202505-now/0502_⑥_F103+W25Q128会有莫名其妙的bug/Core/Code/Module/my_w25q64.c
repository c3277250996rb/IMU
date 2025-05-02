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
    SPI_APP_Init(SPI_Class_1);
		/* ��ȡ Flash Device ID */
	DeviceID = W25Q64_Read_Device_ID();
	
	delay_ms( 200 );
	
	/* ��ȡ SPI Flash ID */
	FlashID = W25Q64_Read_JEDEC_ID();
	
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	
	/* ���� SPI Flash ID */
	if (FlashID == sFLASH_ID) 
	{	
		printf("\r\n��⵽SPI FLASH W25Q64 !\r\n");
		
		/* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
		W25Q64_Erase_Sector(FLASH_SectorToErase);	 	 
		
		/* �����ͻ�����������д��flash�� */
		SPI_APP_Buffer_Write(SPI_Class_1, Tx_Buffer, BufferSize, FLASH_WriteAddress);
		printf("\r\nд�������Ϊ��\r\n%s", Tx_Buffer);
		
		/* ���ո�д������ݶ������ŵ����ջ������� */
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



