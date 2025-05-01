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

