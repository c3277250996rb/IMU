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

        /* ѡ�� FLASH: CS �� */
        SPI_GPIO_Write_CS_0(SPI_Class_1);

        /* ���� ��״̬�Ĵ��� ���� */
        SPI_Register_Write_Read(SPI_Class_1, W25X_ReadStatusReg);

       SPITimeout = SPIT_FLAG_TIMEOUT;
		SPITimeout = 0x2000;
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
        SPI_GPIO_Write_CS_1(SPI_Class_1);
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

        uint32_t SPITimeout = SPIT_FLAG_TIMEOUT;

        /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
        while (__HAL_SPI_GET_FLAG( &temp_Handle, SPI_FLAG_TXE ) == RESET){
            if((SPITimeout--) == 0){
				
				return SPI_Register_Timeout(input_SPI_Class, 0);
			}
        }

        /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
        WRITE_REG(temp_Handle.Instance->DR, input_Data);

        SPITimeout = SPIT_FLAG_TIMEOUT;

        /* �ȴ����ջ������ǿգ�RXNE�¼� */
        while (__HAL_SPI_GET_FLAG( &temp_Handle, SPI_FLAG_RXNE ) == RESET){
            if((SPITimeout--) == 0){
				
				return SPI_Register_Timeout(input_SPI_Class, 1);
			}
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


  /*********** */
 /* MY_W25Q64 */
/*********** */


#endif





