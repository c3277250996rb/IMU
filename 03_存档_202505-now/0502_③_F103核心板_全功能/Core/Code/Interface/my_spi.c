#include "main.h"



#ifndef USE_EMBEDFIRE_SPI


  /*********** */
 /* MY_W25Q64 */
/*********** */



#include "my_spi.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

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
		SPITimeout = 0x2000;
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
        SPI_GPIO_Write_CS_1(SPI_Class_1);
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

    void SPI_APP_Init(SPI_Class_Struct input_SPI_Class){
        SPI_HandleTypeDef temp_SPI_Handle = input_SPI_Class.SPI_Handle;

        HAL_SPI_Init(&temp_SPI_Handle);
        __HAL_SPI_ENABLE(&temp_SPI_Handle);
    }

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

        uint32_t SPITimeout = SPIT_FLAG_TIMEOUT;

        /* 等待发送缓冲区为空，TXE事件 */
        while (__HAL_SPI_GET_FLAG( &temp_Handle, SPI_FLAG_TXE ) == RESET){
            if((SPITimeout--) == 0){
				
				return SPI_Register_Timeout(input_SPI_Class, 0);
			}
        }

        /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
        WRITE_REG(temp_Handle.Instance->DR, input_Data);

        SPITimeout = SPIT_FLAG_TIMEOUT;

        /* 等待接收缓冲区非空，RXNE事件 */
        while (__HAL_SPI_GET_FLAG( &temp_Handle, SPI_FLAG_RXNE ) == RESET){
            if((SPITimeout--) == 0){
				
				return SPI_Register_Timeout(input_SPI_Class, 1);
			}
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


  /*********** */
 /* MY_W25Q64 */
/*********** */


#endif





