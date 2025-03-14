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

#endif


// ��ֲ
#if 1
  
#ifndef __Struct__SPI__Reference
#define __Struct__SPI__Reference

    SPI_Class_Struct SPI_Class_1 = {
        .CS_Pin      = SPI1_CS_PA4_Pin,
        .CS_Port     = SPI1_CS_PA4_GPIO_Port,
        .SCK_Pin     = SPI1_SCK_PA5_Pin,
        .SCK_Port    = SPI1_SCK_PA5_GPIO_Port,
        .MISO_Pin    = SPI1_MISO_PA6_Pin,
        .MISO_Port   = SPI1_MISO_PA6_GPIO_Port,
        .MOSI_Pin    = SPI1_MOSI_PA7_Pin,
        .MOSI_Port   = SPI1_MOSI_PA7_GPIO_Port,

        .SPI_Handle  = {
            .Instance               = SPI1,
            .Init.Mode              = SPI_MODE_MASTER,
            .Init.Direction         = SPI_DIRECTION_2LINES,
            .Init.DataSize          = SPI_DATASIZE_8BIT,
            .Init.CLKPolarity       = SPI_POLARITY_HIGH,
            .Init.CLKPhase          = SPI_PHASE_2EDGE,
            .Init.NSS               = SPI_NSS_SOFT,
            .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4,
            .Init.FirstBit          = SPI_FIRSTBIT_MSB,
            .Init.TIMode            = SPI_TIMODE_DISABLE,
            .Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE,
            .Init.CRCPolynomial     = 7
        }

    };

    SPI_Class_Struct SPI_Class_2 = {
        // .CS_Pin      = 
        // .CS_Port     = 
        // .SCK_Pin     = 
        // .SCK_Port    = 
        // .MISO_Pin    = 
        // .MISO_Port   = 
        // .MOSI_Pin    = 
        // .MOSI_Port   = 

        .SPI_Handle  = {
            .Instance               = SPI2,
            .Init.Mode              = SPI_MODE_MASTER,
            .Init.Direction         = SPI_DIRECTION_2LINES,
            .Init.DataSize          = SPI_DATASIZE_8BIT,
            .Init.CLKPolarity       = SPI_POLARITY_HIGH,
            .Init.CLKPhase          = SPI_PHASE_2EDGE,
            .Init.NSS               = SPI_NSS_SOFT,
            .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4,
            .Init.FirstBit          = SPI_FIRSTBIT_MSB,
            .Init.TIMode            = SPI_TIMODE_DISABLE,
            .Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE,
            .Init.CRCPolynomial     = 7
        }

    };

#endif

#ifndef __W25Q64__SPI__Reference__
#define __W25Q64__SPI__Reference__

    void W25Q64_WriteEnable(void){

        SPI_GPIO_Write_CS_0(SPI_Class_1);
        SPI_Register_Write_Read(SPI_Class_1, W25X_WriteEnable);
        SPI_GPIO_Write_CS_1(SPI_Class_1);
    }

    void W25Q64_WaitForWriteFinish(void){
        uint8_t FLASH_Status = 0;

        /* ѡ�� FLASH: CS �� */
        SPI_GPIO_Write_CS_0(SPI_Class_1);

        /* ���� ��״̬�Ĵ��� ���� */
        SPI_Register_Write_Read(SPI_Class_1, W25X_ReadStatusReg);

        SPITimeout = SPIT_FLAG_TIMEOUT;
        /* ��FLASHæµ����ȴ� */
        do{
            /* ��ȡFLASHоƬ��״̬�Ĵ��� */
            FLASH_Status = SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);	 

            {
                if((SPITimeout--) == 0){
                    SPI_Register_Timeout(SPI_Class_1, 4);
                    return;
                }
            } 
        }while((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */

        /* ֹͣ�ź�  FLASH: CS �� */
        SPI_GPIO_Write_CS_1(SPI_Class_1);;
    }

    void W25Q64_PowerDown(void){
        /* ѡ�� FLASH: CS �� */
        SPI_GPIO_Write_CS_0(SPI_Class_1);

        /* ���� ���� ���� */
        SPI_Register_Write_Read(SPI_Class_1, W25X_PowerDown);

        /* ֹͣ�ź�  FLASH: CS �� */
        SPI_GPIO_Write_CS_1(SPI_Class_1);;
    }

    void W25Q64_ReleasePowerDown(void){
        /*ѡ�� FLASH: CS �� */
        SPI_GPIO_Write_CS_0(SPI_Class_1);

        /* ���� �ϵ� ���� */
        SPI_Register_Write_Read(SPI_Class_1, W25X_ReleasePowerDown);

        /* ֹͣ�ź� FLASH: CS �� */
        SPI_GPIO_Write_CS_1(SPI_Class_1);;                   //�ȴ�TRES1
    }

#endif

#ifndef __Map__SPI__Reference__
#define __Map__SPI__Reference__

    void SPI_1_Read(uint8_t* input_Pointer, uint16_t ReadSize, uint32_t ReadAddress){
        SPI_APP_Buffer_Read(SPI_Class_1,  input_Pointer,  ReadSize,  ReadAddress);
    }
    void SPI_1_Write(uint8_t* input_Pointer, uint16_t WriteSize, uint32_t WriteAddress){
        SPI_APP_Buffer_Write(SPI_Class_1,  input_Pointer,  WriteSize,  WriteAddress);
    }

    void SPI_2_Read(uint8_t* input_Pointer, uint16_t ReadSize, uint32_t ReadAddress){
        // SPI_APP_Buffer_Read(SPI_Class_2,  input_Pointer,  ReadSize,  ReadAddress);
    }
    void SPI_2_Write(uint8_t* input_Pointer, uint16_t WriteSize, uint32_t WriteAddress){
        // SPI_APP_Buffer_Write(SPI_Class_2,  input_Pointer,  WriteSize,  WriteAddress);
    }

#endif

#ifndef __SPI__APP__Reference__
#define __SPI__APP__Reference__

    void SPI_APP_Buffer_Read(   SPI_Class_Struct input_SPI_Class,
                                uint8_t* input_Pointer, 
                                uint16_t ReadSize, 
                                uint32_t ReadAddress)
    {
        /* ѡ��FLASH: CS�͵�ƽ */
        SPI_GPIO_Write_CS_0(input_SPI_Class);

        /* ���� �� ָ�� */
        SPI_Register_Write_Read(input_SPI_Class, W25X_ReadData);

        /* ���� �� ��ַ��λ */
        SPI_Register_Write_Read(input_SPI_Class, (ReadAddress & 0xFF0000) >> 16);
        /* ���� �� ��ַ��λ */
        SPI_Register_Write_Read(input_SPI_Class, (ReadAddress& 0xFF00) >> 8);
        /* ���� �� ��ַ��λ */
        SPI_Register_Write_Read(input_SPI_Class, ReadAddress & 0xFF);

        /* ��ȡ���� */
        while (ReadSize--)
        {
        /* ��ȡһ���ֽ�*/
        *input_Pointer = SPI_Register_Write_Read(input_SPI_Class, Dummy_Byte);
        /* ָ����һ���ֽڻ����� */
        input_Pointer++;
        }

        /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
        SPI_GPIO_Write_CS_1(input_SPI_Class);;
    }

    void SPI_APP_Buffer_Write(  SPI_Class_Struct input_SPI_Class,
                                uint8_t* input_Pointer, 
                                uint16_t WriteSize, 
                                uint32_t WriteAddress)
    {
        uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
            
            /*mod�������࣬��WriteAddress��SPI_FLASH_PageSize��������������AddrֵΪ0*/
        Addr = WriteAddress % SPI_FLASH_PageSize;
            
            /*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
        count = SPI_FLASH_PageSize - Addr;	
            /*�����Ҫд��������ҳ*/
        NumOfPage =  WriteSize / SPI_FLASH_PageSize;
            /*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
        NumOfSingle = WriteSize % SPI_FLASH_PageSize;

            /* Addr=0,��WriteAddress �պð�ҳ���� aligned  */
        if (Addr == 0) 
        {
                /* WriteSize < SPI_FLASH_PageSize */
            if (NumOfPage == 0) 
            {
            SPI_Driver_Write(input_SPI_Class, input_Pointer, WriteSize, WriteAddress);
            }
            else /* WriteSize > SPI_FLASH_PageSize */
            {
                    /*�Ȱ�����ҳ��д��*/
            while (NumOfPage--)
            {
                SPI_Driver_Write(input_SPI_Class, input_Pointer, SPI_FLASH_PageSize, WriteAddress);
                WriteAddress +=  SPI_FLASH_PageSize;
                input_Pointer += SPI_FLASH_PageSize;
            }
                    
                    /*���ж���Ĳ���һҳ�����ݣ�����д��*/
            SPI_Driver_Write(input_SPI_Class, input_Pointer, NumOfSingle, WriteAddress);
            }
        }
            /* ����ַ�� SPI_FLASH_PageSize ������  */
        else 
        {
                /* WriteSize < SPI_FLASH_PageSize */
            if (NumOfPage == 0) 
            {
                    /*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
            if (NumOfSingle > count) 
            {
                temp = NumOfSingle - count;
                        
                        /*��д����ǰҳ*/
                SPI_Driver_Write(input_SPI_Class, input_Pointer, count, WriteAddress);
                WriteAddress +=  count;
                input_Pointer += count;
                        
                        /*��дʣ�������*/
                SPI_Driver_Write(input_SPI_Class, input_Pointer, temp, WriteAddress);
            }
            else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
            {				
                SPI_Driver_Write(input_SPI_Class, input_Pointer, WriteSize, WriteAddress);
            }
            }
            else /* WriteSize > SPI_FLASH_PageSize */
            {
                    /*��ַ����������count�ֿ������������������*/
            WriteSize -= count;
            NumOfPage =  WriteSize / SPI_FLASH_PageSize;
            NumOfSingle = WriteSize % SPI_FLASH_PageSize;

            SPI_Driver_Write(input_SPI_Class, input_Pointer, count, WriteAddress);
            WriteAddress +=  count;
            input_Pointer += count;
                    
                    /*������ҳ��д��*/
            while (NumOfPage--)
            {
                SPI_Driver_Write(input_SPI_Class, input_Pointer, SPI_FLASH_PageSize, WriteAddress);
                WriteAddress +=  SPI_FLASH_PageSize;
                input_Pointer += SPI_FLASH_PageSize;
            }
                    /*���ж���Ĳ���һҳ�����ݣ�����д��*/
            if (NumOfSingle != 0)
            {
                SPI_Driver_Write(input_SPI_Class, input_Pointer, NumOfSingle, WriteAddress);
            }
            }
        }
    }
    
#endif

#ifndef __SPI__Driver__Reference__
#define __SPI__Driver__Reference__

    void SPI_Driver_Write(  SPI_Class_Struct input_SPI_Class,
                            uint8_t* input_Pointer, 
                            uint16_t WriteSize, 
                            uint32_t WriteAddress)
    {
        /* ����FLASHдʹ������ */
        W25Q64_WriteEnable();

        /* ѡ��FLASH: CS�͵�ƽ */
        SPI_GPIO_Write_CS_0(input_SPI_Class);
        /* дҳдָ��*/
        SPI_Register_Write_Read(input_SPI_Class, W25X_PageProgram);
        /*����д��ַ�ĸ�λ*/
        SPI_Register_Write_Read(input_SPI_Class, (WriteAddress & 0xFF0000) >> 16);
        /*����д��ַ����λ*/
        SPI_Register_Write_Read(input_SPI_Class, (WriteAddress & 0xFF00) >> 8);
        /*����д��ַ�ĵ�λ*/
        SPI_Register_Write_Read(input_SPI_Class, WriteAddress & 0xFF);

        if(WriteSize > SPI_FLASH_PerWritePageSize)
        {
            WriteSize = SPI_FLASH_PerWritePageSize;
            FLASH_ERROR("SPI_FLASH_PageWrite too large!");
        }

        /* д������*/
        while (WriteSize--)
        {
            /* ���͵�ǰҪд����ֽ����� */
            SPI_Register_Write_Read(input_SPI_Class, *input_Pointer);
            /* ָ����һ�ֽ����� */
            input_Pointer++;
        }

        /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
        SPI_GPIO_Write_CS_1(input_SPI_Class);;

        /* �ȴ�д�����*/
        W25Q64_WaitForWriteFinish();
    }

#endif

#ifndef __SPI__Register__Reference__
#define __SPI__Register__Reference__

    uint8_t SPI_Register_Write_Read(SPI_Class_Struct input_SPI_Class, uint8_t input_Data){
        
        SPI_HandleTypeDef temp_Handle = input_SPI_Class.SPI_Handle;
        // SPI_HandleTypeDef temp_Handle = SpiHandle;

        uint32_t SPITimeout = SPIT_FLAG_TIMEOUT;

        /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
        while (__HAL_SPI_GET_FLAG( &temp_Handle, SPI_FLAG_TXE ) == RESET){
            if((SPITimeout--) == 0) return SPI_Register_Timeout(input_SPI_Class, 0);
        }

        /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
        WRITE_REG(temp_Handle.Instance->DR, input_Data);

        SPITimeout = SPIT_FLAG_TIMEOUT;

        /* �ȴ����ջ������ǿգ�RXNE�¼� */
        while (__HAL_SPI_GET_FLAG( &temp_Handle, SPI_FLAG_RXNE ) == RESET){
            if((SPITimeout--) == 0) return SPI_Register_Timeout(input_SPI_Class, 1);
        }

        /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
        return READ_REG(temp_Handle.Instance->DR);
    }

    static uint8_t SPI_Register_Timeout(SPI_Class_Struct input_SPI_Class, uint8_t input_Message){
        /* �ȴ���ʱ��Ĵ���,���������Ϣ */
        FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",input_Message);
        return 0;
    }

    void SPI_GPIO_Write_CS(SPI_Class_Struct input_SPI_Class, GPIO_PinState input_PinState){
        GPIO_TypeDef* temp_Port  = input_SPI_Class.CS_Port;
        uint16_t temp_Pin       = input_SPI_Class.CS_Pin;

        HAL_GPIO_WritePin(temp_Port, temp_Pin, input_PinState);
    }

#endif

#endif


// other
#if 1
void SPI_FLASH_Init(void)
{
  // SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  // SpiHandle.Init.Mode              = SPI_MODE_MASTER;
  // SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  // SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  // SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
  // SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  // SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  // SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  // SpiHandle.Init.CRCPolynomial     = 7;

  // SpiHandle.Instance               = SPIx;
  // SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  // SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  
  SpiHandle = SPI_Class_1.SPI_Handle;

  HAL_SPI_Init(&SpiHandle); 
  
  __HAL_SPI_ENABLE(&SpiHandle);     
}

void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  /* ����FLASHдʹ������ */
  SPI_FLASH_WriteEnable();
//  SPI_FLASH_WaitForWriteEnd();
	W25Q64_WaitForWriteFinish();
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
//  SPI_FLASH_WaitForWriteEnd();
	W25Q64_WaitForWriteFinish();
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
#endif


// w25q64
#if 1
void SPI_FLASH_WriteEnable(void)
{
    W25Q64_WriteEnable();
//   /* ͨѶ��ʼ��CS�� */
//   SPI_FLASH_CS_LOW();

//   /* ����дʹ������*/
//   SPI_FLASH_SendByte(W25X_WriteEnable);

//   /*ͨѶ������CS�� */
//   SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_WaitForWriteEnd(void)
{
    W25Q64_WaitForWriteFinish();
//   uint8_t FLASH_Status = 0;

//   /* ѡ�� FLASH: CS �� */
//   SPI_FLASH_CS_LOW();

//   /* ���� ��״̬�Ĵ��� ���� */
//   SPI_FLASH_SendByte(W25X_ReadStatusReg);

//   SPITimeout = SPIT_FLAG_TIMEOUT;
//   /* ��FLASHæµ����ȴ� */
//   do
//   {
//     /* ��ȡFLASHоƬ��״̬�Ĵ��� */
//     FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 

//     {
//       if((SPITimeout--) == 0) 
//       {
//         SPI_TIMEOUT_UserCallback(4);
//         return;
//       }
//     } 
//   }
//   while ((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */

//   /* ֹͣ�ź�  FLASH: CS �� */
//   SPI_FLASH_CS_HIGH();
}

void SPI_Flash_PowerDown(void)   
{ 
    W25Q64_PowerDown();
//   /* ѡ�� FLASH: CS �� */
//   SPI_FLASH_CS_LOW();

//   /* ���� ���� ���� */
//   SPI_FLASH_SendByte(W25X_PowerDown);

//   /* ֹͣ�ź�  FLASH: CS �� */
//   SPI_FLASH_CS_HIGH();
}   

void SPI_Flash_WAKEUP(void)   
{
    W25Q64_ReleasePowerDown();
//   /*ѡ�� FLASH: CS �� */
//   SPI_FLASH_CS_LOW();

//   /* ���� �ϵ� ���� */
//   SPI_FLASH_SendByte(W25X_ReleasePowerDown);

//   /* ֹͣ�ź� FLASH: CS �� */
//   SPI_FLASH_CS_HIGH();                   //�ȴ�TRES1
}   
#endif


// app
#define debug03
#ifdef debug03

void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite){
    #define debug04
    #ifdef debug04
        SPI_APP_Buffer_Write(SPI_Class_1, pBuffer, NumByteToWrite, WriteAddr);
    #else
        uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
        #if 1
            /*mod�������࣬��WriteAddress��SPI_FLASH_PageSize��������������AddrֵΪ0*/
        Addr = WriteAddr % SPI_FLASH_PageSize;

            /*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
        count = SPI_FLASH_PageSize - Addr;	
            /*�����Ҫд��������ҳ*/
        NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            /*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
        NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
        #endif

        if (Addr == 0) {
            printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
            if (NumOfPage == 0) {
                printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
                SPI_Driver_Write(SPI_Class_1, pBuffer, NumByteToWrite, WriteAddr);
            }
        }
    #endif

}

#else
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite){
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
    
    if(Addr == 0) {
        printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
        if(NumOfPage == 0) {
            printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }else{
            // printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
            /*�Ȱ�����ҳ��д��*/
            while (NumOfPage--) {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            
            /*���ж���Ĳ���һҳ�����ݣ�����д��*/
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }else{
        // printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
        if(NumOfPage == 0) {
            /*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
            if(NumOfSingle > count) {
                temp = NumOfSingle - count;
                /*��д����ǰҳ*/
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;
                /*��дʣ�������*/
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }else{				
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }else{
            /*��ַ����������count�ֿ������������������*/
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;
            
            /*������ҳ��д��*/
            while (NumOfPage--) {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*���ж���Ĳ���һҳ�����ݣ�����д��*/
            if (NumOfSingle != 0) {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }

        }
    }
}
#endif

void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead){
    #define debug05
    #ifdef debug05
        SPI_APP_Buffer_Read(SPI_Class_1, pBuffer, NumByteToRead, ReadAddr);
    #else
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

    #endif
    
}



// driver
#define debug02
#ifdef debug02
  
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


// register
#define debug01
#ifdef debug01
  uint8_t SPI_FLASH_SendByte(uint8_t byte){
    return SPI_Register_Write_Read(SPI_Class_1, byte);
  }
  
  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode){
    return SPI_Register_Timeout(SPI_Class_1, errorCode);
  }
#else
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
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
}

uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}
#endif

