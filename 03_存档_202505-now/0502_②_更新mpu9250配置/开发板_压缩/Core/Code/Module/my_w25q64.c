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

// head
#if 1
void Delay(__IO uint32_t nCount);
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
// #define  BufferSize (countof(Tx_Buffer)-1)
#define  BufferSize 3

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

   
/* ���ͻ�������ʼ�� */
// uint8_t Tx_Buffer[] = "��л��ѡ��Ұ��stm32������\r\nhttp://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];
uint8_t temp_Buffer[BufferSize];
//��ȡ��ID�洢λ��
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
        printf("\r\n��⵽SPI FLASH W25Q64 !\r\n");
		W25Q64_Erase_Sector(FLASH_SectorToErase);	 	 

		SPI_APP_Buffer_Read(SPI_Class_1, Rx_Buffer, BufferSize, FLASH_ReadAddress);
		// printf("\r\n����������Ϊ��\r\n%s", Rx_Buffer);
		printf("\r\n����������Ϊ��\r\n%x,%x,%x,", Rx_Buffer[0], Rx_Buffer[1], Rx_Buffer[2]);
		
        memcpy(temp_Buffer, Rx_Buffer, BufferSize);

		SPI_APP_Buffer_Write(SPI_Class_1, Rx_Buffer, BufferSize, FLASH_WriteAddress);
		// printf("\r\nд�������Ϊ��\r\n%s", Rx_Buffer);
		printf("\r\nд�������Ϊ��\r\n%x,%x,%x,", Rx_Buffer[0], Rx_Buffer[1], Rx_Buffer[2]);
		
		SPI_APP_Buffer_Read(SPI_Class_1, Rx_Buffer, BufferSize, FLASH_ReadAddress);
		// printf("\r\n����������Ϊ��\r\n%s", Rx_Buffer);
		printf("\r\n����������Ϊ��\r\n%x,%x,%x,", Rx_Buffer[0], Rx_Buffer[1], Rx_Buffer[2]);
		
		TransferStatus1 = Buffercmp(temp_Buffer, Rx_Buffer, BufferSize);
        if(PASSED == TransferStatus1){
            w25q64_test_status = "succeed!";
        }
    }else{
        printf("\r\n��ȡ���� W25Q64 ID!\n\r");
    }

    W25Q64_PowerDown();  
    printf("\r\nw25q64 test [%s]\r\n", w25q64_test_status);
	// while(1){
    //     // just wait.
    // }
}

