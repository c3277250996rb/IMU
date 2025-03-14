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


// 移植
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

        /* 选择 FLASH: CS 低 */
        SPI_GPIO_Write_CS_0(SPI_Class_1);

        /* 发送 读状态寄存器 命令 */
        SPI_Register_Write_Read(SPI_Class_1, W25X_ReadStatusReg);

        SPITimeout = SPIT_FLAG_TIMEOUT;
        /* 若FLASH忙碌，则等待 */
        do{
            /* 读取FLASH芯片的状态寄存器 */
            FLASH_Status = SPI_Register_Write_Read(SPI_Class_1, Dummy_Byte);	 

            {
                if((SPITimeout--) == 0){
                    SPI_Register_Timeout(SPI_Class_1, 4);
                    return;
                }
            } 
        }while((FLASH_Status & WIP_Flag) == SET); /* 正在写入标志 */

        /* 停止信号  FLASH: CS 高 */
        SPI_GPIO_Write_CS_1(SPI_Class_1);;
    }

    void W25Q64_PowerDown(void){
        /* 选择 FLASH: CS 低 */
        SPI_GPIO_Write_CS_0(SPI_Class_1);

        /* 发送 掉电 命令 */
        SPI_Register_Write_Read(SPI_Class_1, W25X_PowerDown);

        /* 停止信号  FLASH: CS 高 */
        SPI_GPIO_Write_CS_1(SPI_Class_1);;
    }

    void W25Q64_ReleasePowerDown(void){
        /*选择 FLASH: CS 低 */
        SPI_GPIO_Write_CS_0(SPI_Class_1);

        /* 发上 上电 命令 */
        SPI_Register_Write_Read(SPI_Class_1, W25X_ReleasePowerDown);

        /* 停止信号 FLASH: CS 高 */
        SPI_GPIO_Write_CS_1(SPI_Class_1);;                   //等待TRES1
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
        /* 选择FLASH: CS低电平 */
        SPI_GPIO_Write_CS_0(input_SPI_Class);

        /* 发送 读 指令 */
        SPI_Register_Write_Read(input_SPI_Class, W25X_ReadData);

        /* 发送 读 地址高位 */
        SPI_Register_Write_Read(input_SPI_Class, (ReadAddress & 0xFF0000) >> 16);
        /* 发送 读 地址中位 */
        SPI_Register_Write_Read(input_SPI_Class, (ReadAddress& 0xFF00) >> 8);
        /* 发送 读 地址低位 */
        SPI_Register_Write_Read(input_SPI_Class, ReadAddress & 0xFF);

        /* 读取数据 */
        while (ReadSize--)
        {
        /* 读取一个字节*/
        *input_Pointer = SPI_Register_Write_Read(input_SPI_Class, Dummy_Byte);
        /* 指向下一个字节缓冲区 */
        input_Pointer++;
        }

        /* 停止信号 FLASH: CS 高电平 */
        SPI_GPIO_Write_CS_1(input_SPI_Class);;
    }

    void SPI_APP_Buffer_Write(  SPI_Class_Struct input_SPI_Class,
                                uint8_t* input_Pointer, 
                                uint16_t WriteSize, 
                                uint32_t WriteAddress)
    {
        uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
            
            /*mod运算求余，若WriteAddress是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
        Addr = WriteAddress % SPI_FLASH_PageSize;
            
            /*差count个数据值，刚好可以对齐到页地址*/
        count = SPI_FLASH_PageSize - Addr;	
            /*计算出要写多少整数页*/
        NumOfPage =  WriteSize / SPI_FLASH_PageSize;
            /*mod运算求余，计算出剩余不满一页的字节数*/
        NumOfSingle = WriteSize % SPI_FLASH_PageSize;

            /* Addr=0,则WriteAddress 刚好按页对齐 aligned  */
        if (Addr == 0) 
        {
                /* WriteSize < SPI_FLASH_PageSize */
            if (NumOfPage == 0) 
            {
            SPI_Driver_Write(input_SPI_Class, input_Pointer, WriteSize, WriteAddress);
            }
            else /* WriteSize > SPI_FLASH_PageSize */
            {
                    /*先把整数页都写了*/
            while (NumOfPage--)
            {
                SPI_Driver_Write(input_SPI_Class, input_Pointer, SPI_FLASH_PageSize, WriteAddress);
                WriteAddress +=  SPI_FLASH_PageSize;
                input_Pointer += SPI_FLASH_PageSize;
            }
                    
                    /*若有多余的不满一页的数据，把它写完*/
            SPI_Driver_Write(input_SPI_Class, input_Pointer, NumOfSingle, WriteAddress);
            }
        }
            /* 若地址与 SPI_FLASH_PageSize 不对齐  */
        else 
        {
                /* WriteSize < SPI_FLASH_PageSize */
            if (NumOfPage == 0) 
            {
                    /*当前页剩余的count个位置比NumOfSingle小，写不完*/
            if (NumOfSingle > count) 
            {
                temp = NumOfSingle - count;
                        
                        /*先写满当前页*/
                SPI_Driver_Write(input_SPI_Class, input_Pointer, count, WriteAddress);
                WriteAddress +=  count;
                input_Pointer += count;
                        
                        /*再写剩余的数据*/
                SPI_Driver_Write(input_SPI_Class, input_Pointer, temp, WriteAddress);
            }
            else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
            {				
                SPI_Driver_Write(input_SPI_Class, input_Pointer, WriteSize, WriteAddress);
            }
            }
            else /* WriteSize > SPI_FLASH_PageSize */
            {
                    /*地址不对齐多出的count分开处理，不加入这个运算*/
            WriteSize -= count;
            NumOfPage =  WriteSize / SPI_FLASH_PageSize;
            NumOfSingle = WriteSize % SPI_FLASH_PageSize;

            SPI_Driver_Write(input_SPI_Class, input_Pointer, count, WriteAddress);
            WriteAddress +=  count;
            input_Pointer += count;
                    
                    /*把整数页都写了*/
            while (NumOfPage--)
            {
                SPI_Driver_Write(input_SPI_Class, input_Pointer, SPI_FLASH_PageSize, WriteAddress);
                WriteAddress +=  SPI_FLASH_PageSize;
                input_Pointer += SPI_FLASH_PageSize;
            }
                    /*若有多余的不满一页的数据，把它写完*/
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
        /* 发送FLASH写使能命令 */
        W25Q64_WriteEnable();

        /* 选择FLASH: CS低电平 */
        SPI_GPIO_Write_CS_0(input_SPI_Class);
        /* 写页写指令*/
        SPI_Register_Write_Read(input_SPI_Class, W25X_PageProgram);
        /*发送写地址的高位*/
        SPI_Register_Write_Read(input_SPI_Class, (WriteAddress & 0xFF0000) >> 16);
        /*发送写地址的中位*/
        SPI_Register_Write_Read(input_SPI_Class, (WriteAddress & 0xFF00) >> 8);
        /*发送写地址的低位*/
        SPI_Register_Write_Read(input_SPI_Class, WriteAddress & 0xFF);

        if(WriteSize > SPI_FLASH_PerWritePageSize)
        {
            WriteSize = SPI_FLASH_PerWritePageSize;
            FLASH_ERROR("SPI_FLASH_PageWrite too large!");
        }

        /* 写入数据*/
        while (WriteSize--)
        {
            /* 发送当前要写入的字节数据 */
            SPI_Register_Write_Read(input_SPI_Class, *input_Pointer);
            /* 指向下一字节数据 */
            input_Pointer++;
        }

        /* 停止信号 FLASH: CS 高电平 */
        SPI_GPIO_Write_CS_1(input_SPI_Class);;

        /* 等待写入完毕*/
        W25Q64_WaitForWriteFinish();
    }

#endif

#ifndef __SPI__Register__Reference__
#define __SPI__Register__Reference__

    uint8_t SPI_Register_Write_Read(SPI_Class_Struct input_SPI_Class, uint8_t input_Data){
        
        SPI_HandleTypeDef temp_Handle = input_SPI_Class.SPI_Handle;
        // SPI_HandleTypeDef temp_Handle = SpiHandle;

        uint32_t SPITimeout = SPIT_FLAG_TIMEOUT;

        /* 等待发送缓冲区为空，TXE事件 */
        while (__HAL_SPI_GET_FLAG( &temp_Handle, SPI_FLAG_TXE ) == RESET){
            if((SPITimeout--) == 0) return SPI_Register_Timeout(input_SPI_Class, 0);
        }

        /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
        WRITE_REG(temp_Handle.Instance->DR, input_Data);

        SPITimeout = SPIT_FLAG_TIMEOUT;

        /* 等待接收缓冲区非空，RXNE事件 */
        while (__HAL_SPI_GET_FLAG( &temp_Handle, SPI_FLAG_RXNE ) == RESET){
            if((SPITimeout--) == 0) return SPI_Register_Timeout(input_SPI_Class, 1);
        }

        /* 读取数据寄存器，获取接收缓冲区数据 */
        return READ_REG(temp_Handle.Instance->DR);
    }

    static uint8_t SPI_Register_Timeout(SPI_Class_Struct input_SPI_Class, uint8_t input_Message){
        /* 等待超时后的处理,输出错误信息 */
        FLASH_ERROR("SPI 等待超时!errorCode = %d",input_Message);
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
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();
//  SPI_FLASH_WaitForWriteEnd();
	W25Q64_WaitForWriteFinish();
  /* 擦除扇区 */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送扇区擦除指令*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*发送擦除扇区地址的高位*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* 发送擦除扇区地址的中位 */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* 发送擦除扇区地址的低位 */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();
  /* 等待擦除完毕*/
//  SPI_FLASH_WaitForWriteEnd();
	W25Q64_WaitForWriteFinish();
}

void SPI_FLASH_BulkErase(void)
{
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 整块 Erase */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送整块擦除指令*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待擦除完毕*/
  SPI_FLASH_WaitForWriteEnd();
}

uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 开始通讯：CS低电平 */
  SPI_FLASH_CS_LOW();

  /* 发送JEDEC指令，读取ID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* 读取一个字节数据 */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 读取一个字节数据 */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 停止通讯：CS高电平 */
  SPI_FLASH_CS_HIGH();

	/*把数据组合起来，作为函数的返回值*/
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
//   /* 通讯开始：CS低 */
//   SPI_FLASH_CS_LOW();

//   /* 发送写使能命令*/
//   SPI_FLASH_SendByte(W25X_WriteEnable);

//   /*通讯结束：CS高 */
//   SPI_FLASH_CS_HIGH();
}

void SPI_FLASH_WaitForWriteEnd(void)
{
    W25Q64_WaitForWriteFinish();
//   uint8_t FLASH_Status = 0;

//   /* 选择 FLASH: CS 低 */
//   SPI_FLASH_CS_LOW();

//   /* 发送 读状态寄存器 命令 */
//   SPI_FLASH_SendByte(W25X_ReadStatusReg);

//   SPITimeout = SPIT_FLAG_TIMEOUT;
//   /* 若FLASH忙碌，则等待 */
//   do
//   {
//     /* 读取FLASH芯片的状态寄存器 */
//     FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 

//     {
//       if((SPITimeout--) == 0) 
//       {
//         SPI_TIMEOUT_UserCallback(4);
//         return;
//       }
//     } 
//   }
//   while ((FLASH_Status & WIP_Flag) == SET); /* 正在写入标志 */

//   /* 停止信号  FLASH: CS 高 */
//   SPI_FLASH_CS_HIGH();
}

void SPI_Flash_PowerDown(void)   
{ 
    W25Q64_PowerDown();
//   /* 选择 FLASH: CS 低 */
//   SPI_FLASH_CS_LOW();

//   /* 发送 掉电 命令 */
//   SPI_FLASH_SendByte(W25X_PowerDown);

//   /* 停止信号  FLASH: CS 高 */
//   SPI_FLASH_CS_HIGH();
}   

void SPI_Flash_WAKEUP(void)   
{
    W25Q64_ReleasePowerDown();
//   /*选择 FLASH: CS 低 */
//   SPI_FLASH_CS_LOW();

//   /* 发上 上电 命令 */
//   SPI_FLASH_SendByte(W25X_ReleasePowerDown);

//   /* 停止信号 FLASH: CS 高 */
//   SPI_FLASH_CS_HIGH();                   //等待TRES1
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
            /*mod运算求余，若WriteAddress是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
        Addr = WriteAddr % SPI_FLASH_PageSize;

            /*差count个数据值，刚好可以对齐到页地址*/
        count = SPI_FLASH_PageSize - Addr;	
            /*计算出要写多少整数页*/
        NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            /*mod运算求余，计算出剩余不满一页的字节数*/
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
      /*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
      Addr = WriteAddr % SPI_FLASH_PageSize;
      
      /*差count个数据值，刚好可以对齐到页地址*/
      count = SPI_FLASH_PageSize - Addr;	
      /*计算出要写多少整数页*/
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      /*mod运算求余，计算出剩余不满一页的字节数*/
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
    #endif
    
    if(Addr == 0) {
        printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
        if(NumOfPage == 0) {
            printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }else{
            // printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
            /*先把整数页都写了*/
            while (NumOfPage--) {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            
            /*若有多余的不满一页的数据，把它写完*/
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }else{
        // printf("\r\n[%s]: %d\r\n", __FILE__, __LINE__);
        if(NumOfPage == 0) {
            /*当前页剩余的count个位置比NumOfSingle小，写不完*/
            if(NumOfSingle > count) {
                temp = NumOfSingle - count;
                /*先写满当前页*/
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;
                /*再写剩余的数据*/
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }else{				
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }else{
            /*地址不对齐多出的count分开处理，不加入这个运算*/
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;
            
            /*把整数页都写了*/
            while (NumOfPage--) {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
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
        /* 选择FLASH: CS低电平 */
        SPI_FLASH_CS_LOW();

        /* 发送 读 指令 */
        SPI_FLASH_SendByte(W25X_ReadData);

        /* 发送 读 地址高位 */
        SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
        /* 发送 读 地址中位 */
        SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
        /* 发送 读 地址低位 */
        SPI_FLASH_SendByte(ReadAddr & 0xFF);

        /* 读取数据 */
        while (NumByteToRead--)
        {
        /* 读取一个字节*/
        *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
        /* 指向下一个字节缓冲区 */
        pBuffer++;
        }

        /* 停止信号 FLASH: CS 高电平 */
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
  /* 发送FLASH写使能命令 */
  SPI_FLASH_WriteEnable();

  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 写页写指令*/
  SPI_FLASH_SendByte(W25X_PageProgram);
  /*发送写地址的高位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*发送写地址的中位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*发送写地址的低位*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* 写入数据*/
  while (NumByteToWrite--)
  {
    /* 发送当前要写入的字节数据 */
    SPI_FLASH_SendByte(*pBuffer);
    /* 指向下一字节数据 */
    pBuffer++;
  }

  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待写入完毕*/
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

  /* 等待发送缓冲区为空，TXE事件 */
  while (__HAL_SPI_GET_FLAG( &SpiHandle, SPI_FLAG_TXE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  WRITE_REG(SpiHandle.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待接收缓冲区非空，RXNE事件 */
  while (__HAL_SPI_GET_FLAG( &SpiHandle, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  return READ_REG(SpiHandle.Instance->DR);
}

uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}
#endif

