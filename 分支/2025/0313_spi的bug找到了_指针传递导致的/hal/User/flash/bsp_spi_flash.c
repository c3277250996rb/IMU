// head
#if 1
#include "./flash/bsp_spi_flash.h"

SPI_HandleTypeDef SpiHandle;

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  SPIx_SCK_GPIO_CLK_ENABLE();
  SPIx_MISO_GPIO_CLK_ENABLE();
  SPIx_MOSI_GPIO_CLK_ENABLE();
  SPIx_CS_GPIO_CLK_ENABLE();
  /* Enable SPI clock */
  SPIx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

  
  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);//A5
    
  /* SPI MISO GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_MISO_PIN;  
  HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);//A6
  
  /* SPI MOSI GPIO pin configuration  */
  GPIO_InitStruct.Pin = SPIx_MOSI_PIN; 
  HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);   //A7

  // GPIO_InitStruct.Pin = FLASH_CS_PIN ;
  // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  // HAL_GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStruct); //C0

  GPIO_InitStruct.Pin = GPIO_PIN_4 ;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //A4
}

void SPI_FLASH_Init(void)
{
   /*##-1- Configure the SPI peripheral #######################################*/
  /* Set the SPI parameters */
  // SpiHandle.Instance               = SPIx;
  // SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  // SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  // SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
  // SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  // SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  // SpiHandle.Init.CRCPolynomial     = 7;
  // SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  // SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  // SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  // SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  
  // SpiHandle.Init.Mode = SPI_MODE_MASTER;


  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle.Init.Mode              = SPI_MODE_MASTER;
  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.CRCPolynomial     = 7;

  SpiHandle.Instance               = SPIx;
  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  



  HAL_SPI_Init(&SpiHandle); 
  
  __HAL_SPI_ENABLE(&SpiHandle);     
}

#endif

// w25x
#if 1
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* �������� */
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* ������������ָ��*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*���Ͳ���������ַ�ĸ�λ*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* ���Ͳ���������ַ����λ */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* ���Ͳ���������ַ�ĵ�λ */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();
  /* �ȴ��������*/
  SPI_FLASH_WaitForWriteEnd();
}


void SPI_FLASH_BulkErase(void)
{
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
#endif
// w25x


// w25x
#if 1
uint32_t SPI_FLASH_ReadID(void)
{
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

uint32_t SPI_FLASH_ReadDeviceID(void)
{
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

#if 0
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}
#endif
#endif
// w25x


// app
#if 1


#define debug05
#ifdef debug05

#ifdef debug10

#else
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite){
  SPI_APP_Buffer_Write(SPI_Class_1, pBuffer, NumByteToWrite, WriteAddr);
}
#endif

#else


void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{

  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
#if 1	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = SPI_FLASH_PageSize - Addr;	
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
#endif

printf("\r\ndebug07\r\n");
#define debug07
#ifdef debug07

  if (Addr == 0) {
    if (NumOfPage == 0) {
      // SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      SPI_Driver_Write(SPI_Class_1, pBuffer, NumByteToWrite, WriteAddr);

    } else{
      while (NumOfPage--){
        // SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        SPI_Driver_Write(SPI_Class_1, pBuffer, SPI_FLASH_PageSize, WriteAddr);

        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      /*���ж���Ĳ���һҳ�����ݣ�����д��*/
      // SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      SPI_Driver_Write(SPI_Class_1, pBuffer, NumOfSingle, WriteAddr);

    }

  } else { /* ����ַ�� SPI_FLASH_PageSize ������  */

#else

	 /* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*�Ȱ�����ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
  else 
  {



#endif

printf("\r\ndebug08\r\n");
#define debug08
#ifdef debug08

    if (NumOfPage == 0) {
      if (NumOfSingle > count) {

        temp = NumOfSingle - count;
				/*��д����ǰҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;
				/*��дʣ�������*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);

      } else {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    
  

#else

/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*��д����ǰҳ*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*��дʣ�������*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
      {				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    

#endif

printf("\r\ndebug09\r\n");
#define debug09
#ifdef debug09

  } else {
    /*��ַ����������count�ֿ������������������*/
    NumByteToWrite -= count;
    NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
    WriteAddr +=  count;
    pBuffer += count;
    
    /*������ҳ��д��*/
    while (NumOfPage--){
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
      WriteAddr +=  SPI_FLASH_PageSize;
      pBuffer += SPI_FLASH_PageSize;
    }

    if (NumOfSingle != 0){
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }

  }

#else

   } else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ����������count�ֿ������������������*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*������ҳ��д��*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }


#endif


  }
}


#endif


void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();

  /* ���� �� ָ�� */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* ���� �� ��ַ��λ */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  
	/* ��ȡ���� */
  while (NumByteToRead--)
  {
    /* ��ȡһ���ֽ�*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* ָ����һ���ֽڻ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();
}
#endif
// app


// driver
#if 1

#define debug04
#ifdef debug04


void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite){
  SPI_Driver_Write(SPI_Class_1, pBuffer, NumByteToWrite, WriteAddr);
}


#else


void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();

  /* ѡ��FLASH: CS�͵�ƽ */
  SPI_FLASH_CS_LOW();
  /* дҳдָ��*/
  SPI_FLASH_SendByte(W25X_PageProgram);
  /*����д��ַ�ĸ�λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*����д��ַ����λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*����д��ַ�ĵ�λ*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* д������*/
  while (NumByteToWrite--)
  {
    /* ���͵�ǰҪд����ֽ����� */
    SPI_FLASH_SendByte(*pBuffer);
    /* ָ����һ�ֽ����� */
    pBuffer++;
  }

  /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ�д�����*/
  SPI_FLASH_WaitForWriteEnd();
}


#endif

#endif
// driver


// register
#if 1

#define debug03
#ifdef debug03

uint8_t SPI_FLASH_SendByte(uint8_t byte){
  return SPI_Register_Write_Read(SPI_Class_1, byte);
}

#else

uint8_t SPI_FLASH_SendByte(uint8_t byte)
{

#if 1
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (__HAL_SPI_GET_FLAG( &SpiHandle, SPI_FLAG_TXE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  WRITE_REG(SpiHandle.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (__HAL_SPI_GET_FLAG( &SpiHandle, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return READ_REG(SpiHandle.Instance->DR);
#endif
}

#endif


#define debug06
#ifdef debug06


void SPI_FLASH_WriteEnable(void){
  W25Q64_WriteEnable();
}


#else


void SPI_FLASH_WriteEnable(void)
{
  /* ͨѶ��ʼ��CS�� */
  SPI_FLASH_CS_LOW();

  /* ����дʹ������*/
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /*ͨѶ������CS�� */
  SPI_FLASH_CS_HIGH();
}


#endif




void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;

  /* ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� ��״̬�Ĵ��� ���� */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* ��FLASHæµ����ȴ� */
  do
  {
    /* ��ȡFLASHоƬ��״̬�Ĵ��� */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 

    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */

  /* ֹͣ�ź�  FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}


#if 1
//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  /* ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� ���� ���� */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* ֹͣ�ź�  FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}   

//����
void SPI_Flash_WAKEUP(void)   
{
  /*ѡ�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* ���� �ϵ� ���� */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* ֹͣ�ź� FLASH: CS �� */
  SPI_FLASH_CS_HIGH();                   //�ȴ�TRES1
}   


#endif


#define debug02
#ifdef debug02


static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode){
  return SPI_Register_Timeout(SPI_Class_1, errorCode);
}


#else

static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}

#endif


#endif
// register






