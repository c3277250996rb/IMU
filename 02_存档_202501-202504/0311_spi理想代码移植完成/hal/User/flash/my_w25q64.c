#include "my_w25q64.h"


uint32_t W25Q64_Read_Device_ID(void){
    uint32_t Temp = 0;

    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();

    /* Send "RDID " instruction */
    SPI_FLASH_SendByte(W25X_DeviceID);
    SPI_FLASH_SendByte(Dummy_Byte);
    SPI_FLASH_SendByte(Dummy_Byte);
    SPI_FLASH_SendByte(Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp = SPI_FLASH_SendByte(Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();

    return Temp;
}

uint32_t W25Q64_Read_JEDEC_ID(void){
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* ��ʼͨѶ��CS�͵�ƽ */
    SPI_FLASH_CS_LOW();

    /* ����JEDECָ���ȡID */
    SPI_FLASH_SendByte(W25X_JedecDeviceID);

    /* ��ȡһ���ֽ����� */
    Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

    /* ��ȡһ���ֽ����� */
    Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

    /* ��ȡһ���ֽ����� */
    Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

    /* ֹͣͨѶ��CS�ߵ�ƽ */
    SPI_FLASH_CS_HIGH();

        /*�����������������Ϊ�����ķ���ֵ*/
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

void W25Q64_Erase_Sector(uint32_t SectorAddress){
    /* ����FLASHдʹ������ */
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitForWriteEnd();
    /* �������� */
    /* ѡ��FLASH: CS�͵�ƽ */
    SPI_FLASH_CS_LOW();
    /* ������������ָ��*/
    SPI_FLASH_SendByte(W25X_SectorErase);
    /*���Ͳ���������ַ�ĸ�λ*/
    SPI_FLASH_SendByte((SectorAddress & 0xFF0000) >> 16);
    /* ���Ͳ���������ַ����λ */
    SPI_FLASH_SendByte((SectorAddress & 0xFF00) >> 8);
    /* ���Ͳ���������ַ�ĵ�λ */
    SPI_FLASH_SendByte(SectorAddress & 0xFF);
    /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
    SPI_FLASH_CS_HIGH();
    /* �ȴ��������*/
    SPI_FLASH_WaitForWriteEnd();
}

void W25Q64_Erase_Chip(void){
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ���� Erase */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* �����������ָ��*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}

