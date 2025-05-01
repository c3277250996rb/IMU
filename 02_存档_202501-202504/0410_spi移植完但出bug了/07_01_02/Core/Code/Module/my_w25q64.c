/*�����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ�����*/
#include "my_w25q64.h"


uint16_t g_norflash_type = W25Q64;     /* Ĭ����W25Q64 */


uint8_t SPI_RW_W25Q64(uint8_t input_Data){
    return SPI_1_RW(input_Data);
}
// #define W25Q64_CS_1 SPI_GPIO_Write_CS_1(&SPI_Class_1)
// #define W25Q64_CS_0 SPI_GPIO_Write_CS_0(&SPI_Class_1)

#define W25Q64_CS(x) SPI_GPIO_Write_CS(&SPI_Class_1, x)






void W25Q64_Init(void){
    GPIO_InitTypeDef gpio_init_struct;
    uint8_t temp;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* CS����ģʽ����(�������) */
    gpio_init_struct.Pin = SPI1_CS_PA4_Pin;        
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; 
    gpio_init_struct.Pull = GPIO_PULLUP; 
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI1_CS_PA4_GPIO_Port, &gpio_init_struct);


    W25Q64_CS(1);

    SPI_APP_Init(&g_spi1_handler);
    SPI_Set_Speed(&g_spi1_handler, SPI_SPEED_2);

    g_norflash_type = W25Q64_Read_ID();   /* ��ȡFLASH ID. */
    
    if (g_norflash_type == W25Q256)     /* SPI FLASHΪW25Q256, ����ʹ��4�ֽڵ�ַģʽ */
    {
        temp = W25Q64_Read_Status_Register(3);     /* ��ȡ״̬�Ĵ���3���жϵ�ַģʽ */

        if ((temp & 0X01) == 0)         /* �������4�ֽڵ�ַģʽ,�����4�ֽڵ�ַģʽ */
        {
            W25Q64_Write_ENABLE();    /* дʹ�� */
            temp |= 1 << 1;             /* ADP=1, �ϵ�4λ��ַģʽ */
            W25Q64_Write_Status_Register(3, temp); /* дSR3 */
            
            W25Q64_CS(0);
            SPI_RW_W25Q64(FLASH_Enable4ByteAddr);    /* ʹ��4�ֽڵ�ַָ�� */
            W25Q64_CS(1);
        }
    }

    printf("ID:%x\r\n", g_norflash_type);
}

static void W25Q64_Wait_Busy(void)
{
    while ((W25Q64_Read_Status_Register(1) & 0x01) == 0x01);   /*  �ȴ�BUSYλ��� */
}

static void W25Q64_Send_Address(uint32_t address)
{
    if (g_norflash_type == W25Q256) /*  ֻ��W25Q256֧��4�ֽڵ�ַģʽ */
    {
        SPI_RW_W25Q64((uint8_t)((address)>>24)); /* ���� bit31 ~ bit24 ��ַ */
    } 
    SPI_RW_W25Q64((uint8_t)((address)>>16));     /* ���� bit23 ~ bit16 ��ַ */
    SPI_RW_W25Q64((uint8_t)((address)>>8));      /* ���� bit15 ~ bit8  ��ַ */
    SPI_RW_W25Q64((uint8_t)address);             /* ���� bit7  ~ bit0  ��ַ */
}




#ifndef MEM1_ALLOC_TABLE_SIZE   /* ���û�ж��� MEM1_ALLOC_TABLE_SIZE ��˵��û�õ��ڴ���� */
uint8_t g_norflash_buf[4096];   /* �������� */
#endif 
void W25Q64_Write(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *norflash_buf;
    
#ifdef MEM1_ALLOC_TABLE_SIZE
    norflash_buf = mymalloc(SRAMIN, 4096);  /* ʹ���ڴ���� �����ڴ� */
    if(norflash_buf == NULL)
    {
        return;                     /* ����ʧ��, ֱ���˳� */
    }
#else
    norflash_buf = g_norflash_buf;  /* ��ʹ���ڴ����, ֱ��ָ�� g_norflash_buf ���� */
#endif

    secpos = addr / 4096;       /* ������ַ */
    secoff = addr % 4096;       /* �������ڵ�ƫ�� */
    secremain = 4096 - secoff;  /* ����ʣ��ռ��С */

    //printf("ad:%X,nb:%X\r\n", addr, datalen); /* ������ */
    if (datalen <= secremain)
    {
        secremain = datalen;    /* ������4096���ֽ� */
    }

    while (1)
    {
        W25Q64_Read(norflash_buf, secpos * 4096, 4096);   /* ������������������ */

        for (i = 0; i < secremain; i++)   /* У������ */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;      /* ��Ҫ����, ֱ���˳�forѭ�� */
            }
        }

        if (i < secremain)   /* ��Ҫ���� */
        {
            W25Q64_Erase_Sector(secpos);  /* ����������� */

            for (i = 0; i < secremain; i++)   /* ���� */
            {
                norflash_buf[i + secoff] = pbuf[i];
            }

            W25Q64_Write_NoCheck(norflash_buf, secpos * 4096, 4096);  /* д���������� */
        }
        else        /* д�Ѿ������˵�,ֱ��д������ʣ������. */
        {
            W25Q64_Write_NoCheck(pbuf, addr, secremain);  /* ֱ��д���� */
        }

        if (datalen == secremain)
        {
            break;  /* д������� */
        }
        else        /* д��δ���� */
        {
            secpos++;               /* ������ַ��1 */
            secoff = 0;             /* ƫ��λ��Ϊ0 */

            pbuf += secremain;      /* ָ��ƫ�� */
            addr += secremain;      /* д��ַƫ�� */
            datalen -= secremain;   /* �ֽ����ݼ� */

            if (datalen > 4096)
            {
                secremain = 4096;   /* ��һ����������д���� */
            }
            else
            {
                secremain = datalen;/* ��һ����������д���� */
            }
        }
    }
#ifdef MEM1_ALLOC_TABLE_SIZE        /* ʹ�����ڴ���� */
    myfree(SRAMIN, norflash_buf);   /* �ͷ�������ڴ� */
#endif
}


static void W25Q64_Write_NoCheck(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t pageremain;
    pageremain = 256 - addr % 256;  /* ��ҳʣ����ֽ��� */

    if (datalen <= pageremain)      /* ������256���ֽ� */
    {
        pageremain = datalen;
    }

    while (1)
    {
        /* ��д���ֽڱ�ҳ��ʣ���ַ���ٵ�ʱ��, һ����д��
         * ��д��ֱ�ӱ�ҳ��ʣ���ַ�����ʱ��, ��д������ҳ��ʣ���ַ, Ȼ�����ʣ�೤�Ƚ��в�ͬ����
         */
        W25Q64_Write_Page(pbuf, addr, pageremain);

        if (datalen == pageremain)   /* д������� */
        {
            break;
        }
        else     /* datalen > pageremain */
        {
            pbuf += pageremain;         /* pbufָ���ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
            addr += pageremain;         /* д��ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
            datalen -= pageremain;      /* д���ܳ��ȼ�ȥ�Ѿ�д���˵��ֽ��� */

            if (datalen > 256)          /* ʣ�����ݻ�����һҳ,����һ��дһҳ */
            {
                pageremain = 256;       /* һ�ο���д��256���ֽ� */
            }
            else     /* ʣ������С��һҳ,����һ��д�� */
            {
                pageremain = datalen;   /* ����256���ֽ��� */
            }
        }
    }
}

static void W25Q64_Write_Page(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t i;

    W25Q64_Write_ENABLE();    /* дʹ�� */

    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_PageProgram);    /* ����дҳ���� */
    W25Q64_Send_Address(addr);                /* ���͵�ַ */

    for(i=0;i<datalen;i++)
    {
        SPI_RW_W25Q64(pbuf[i]);          /* ѭ��д�� */
    }
    
    W25Q64_CS(1);
    W25Q64_Wait_Busy();       /* �ȴ�д����� */
}

void W25Q64_Read(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t i;

    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_ReadData);       /* ���Ͷ�ȡ���� */
    W25Q64_Send_Address(addr);                /* ���͵�ַ */
    
    for(i=0;i<datalen;i++)
    {
        pbuf[i] = SPI_RW_W25Q64(0XFF);   /* ѭ����ȡ */
    }
    
    W25Q64_CS(1);
}




void W25Q64_Erase_Chip(void)
{
    W25Q64_Write_ENABLE();    /* дʹ�� */
    W25Q64_Wait_Busy();       /* �ȴ����� */
    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_ChipErase);  /* ���Ͷ��Ĵ������� */ 
    W25Q64_CS(1);
    W25Q64_Wait_Busy();       /* �ȴ�оƬ�������� */
}

void W25Q64_Erase_Sector(uint32_t saddr)
{
    //printf("fe:%x\r\n", saddr);   /* ����falsh�������,������ */
    saddr *= 4096;
    W25Q64_Write_ENABLE();        /* дʹ�� */
    W25Q64_Wait_Busy();           /* �ȴ����� */

    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_SectorErase);    /* ����дҳ���� */
    W25Q64_Send_Address(saddr);   /* ���͵�ַ */
    W25Q64_CS(1);
    W25Q64_Wait_Busy();           /* �ȴ������������ */
}

uint16_t W25Q64_Read_ID(void)
{
    uint16_t deviceid;

    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_ManufactDeviceID);   /* ���Ͷ� ID ���� */
    SPI_RW_W25Q64(0);    /* д��һ���ֽ� */
    SPI_RW_W25Q64(0);
    SPI_RW_W25Q64(0);
    deviceid = SPI_RW_W25Q64(0xFF) << 8;     /* ��ȡ��8λ�ֽ� */
    deviceid |= SPI_RW_W25Q64(0xFF);         /* ��ȡ��8λ�ֽ� */
    W25Q64_CS(1);

    return deviceid;
}


uint8_t W25Q64_Read_Status_Register(uint8_t regno)
{
    uint8_t byte = 0, command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_ReadStatusReg1;  /* ��״̬�Ĵ���1ָ�� */
            break;

        case 2:
            command = FLASH_ReadStatusReg2;  /* ��״̬�Ĵ���2ָ�� */
            break;

        case 3:
            command = FLASH_ReadStatusReg3;  /* ��״̬�Ĵ���3ָ�� */
            break;

        default:
            command = FLASH_ReadStatusReg1;
            break;
    }

    W25Q64_CS(0);
    SPI_RW_W25Q64(command);      /* ���Ͷ��Ĵ������� */
    byte = SPI_RW_W25Q64(0Xff);  /* ��ȡһ���ֽ� */
    W25Q64_CS(1);
    
    return byte;
}

void W25Q64_Write_Status_Register(uint8_t regno, uint8_t sr)
{
    uint8_t command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_WriteStatusReg1;  /* д״̬�Ĵ���1ָ�� */
            break;

        case 2:
            command = FLASH_WriteStatusReg2;  /* д״̬�Ĵ���2ָ�� */
            break;

        case 3:
            command = FLASH_WriteStatusReg3;  /* д״̬�Ĵ���3ָ�� */
            break;

        default:
            command = FLASH_WriteStatusReg1;
            break;
    }

    W25Q64_CS(0);
    SPI_RW_W25Q64(command);  /* ���Ͷ��Ĵ������� */
    SPI_RW_W25Q64(sr);       /* д��һ���ֽ� */
    W25Q64_CS(1);
}

void W25Q64_Write_ENABLE(void)
{
    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_WriteEnable);   /* ����дʹ�� */
    W25Q64_CS(1);
}










const uint8_t g_text_buf[] = {"my w25q64 self test"};
#define TEXT_SIZE sizeof(g_text_buf)
uint8_t datatemp[TEXT_SIZE];

void my_w25q64_self_test(void){
    uint16_t id = W25Q64_Read_ID();
    if((id == 0) || (id == 0xFFFF)){
        printf("id is wrong!\r\n");
        while(1);
    }

    uint32_t flashsize = 8 *1024 *1024;
    printf("write: %s\r\n", g_text_buf);
    W25Q64_Write((uint8_t *)g_text_buf, flashsize - 100, TEXT_SIZE);
    W25Q64_Read(datatemp, flashsize - 100, TEXT_SIZE);
    printf("write: %s\r\n", datatemp);
    while(1);
}



















