/*金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮*/
#include "my_w25q64.h"


uint16_t g_norflash_type = W25Q64;     /* 默认是W25Q64 */


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

    /* CS引脚模式设置(复用输出) */
    gpio_init_struct.Pin = SPI1_CS_PA4_Pin;        
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; 
    gpio_init_struct.Pull = GPIO_PULLUP; 
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI1_CS_PA4_GPIO_Port, &gpio_init_struct);


    W25Q64_CS(1);

    SPI_APP_Init(&g_spi1_handler);
    SPI_Set_Speed(&g_spi1_handler, SPI_SPEED_2);

    g_norflash_type = W25Q64_Read_ID();   /* 读取FLASH ID. */
    
    if (g_norflash_type == W25Q256)     /* SPI FLASH为W25Q256, 必须使能4字节地址模式 */
    {
        temp = W25Q64_Read_Status_Register(3);     /* 读取状态寄存器3，判断地址模式 */

        if ((temp & 0X01) == 0)         /* 如果不是4字节地址模式,则进入4字节地址模式 */
        {
            W25Q64_Write_ENABLE();    /* 写使能 */
            temp |= 1 << 1;             /* ADP=1, 上电4位地址模式 */
            W25Q64_Write_Status_Register(3, temp); /* 写SR3 */
            
            W25Q64_CS(0);
            SPI_RW_W25Q64(FLASH_Enable4ByteAddr);    /* 使能4字节地址指令 */
            W25Q64_CS(1);
        }
    }

    printf("ID:%x\r\n", g_norflash_type);
}

static void W25Q64_Wait_Busy(void)
{
    while ((W25Q64_Read_Status_Register(1) & 0x01) == 0x01);   /*  等待BUSY位清空 */
}

static void W25Q64_Send_Address(uint32_t address)
{
    if (g_norflash_type == W25Q256) /*  只有W25Q256支持4字节地址模式 */
    {
        SPI_RW_W25Q64((uint8_t)((address)>>24)); /* 发送 bit31 ~ bit24 地址 */
    } 
    SPI_RW_W25Q64((uint8_t)((address)>>16));     /* 发送 bit23 ~ bit16 地址 */
    SPI_RW_W25Q64((uint8_t)((address)>>8));      /* 发送 bit15 ~ bit8  地址 */
    SPI_RW_W25Q64((uint8_t)address);             /* 发送 bit7  ~ bit0  地址 */
}




#ifndef MEM1_ALLOC_TABLE_SIZE   /* 如果没有定义 MEM1_ALLOC_TABLE_SIZE 则说明没用到内存管理 */
uint8_t g_norflash_buf[4096];   /* 扇区缓存 */
#endif 
void W25Q64_Write(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *norflash_buf;
    
#ifdef MEM1_ALLOC_TABLE_SIZE
    norflash_buf = mymalloc(SRAMIN, 4096);  /* 使用内存管理 申请内存 */
    if(norflash_buf == NULL)
    {
        return;                     /* 申请失败, 直接退出 */
    }
#else
    norflash_buf = g_norflash_buf;  /* 不使用内存管理, 直接指向 g_norflash_buf 数组 */
#endif

    secpos = addr / 4096;       /* 扇区地址 */
    secoff = addr % 4096;       /* 在扇区内的偏移 */
    secremain = 4096 - secoff;  /* 扇区剩余空间大小 */

    //printf("ad:%X,nb:%X\r\n", addr, datalen); /* 测试用 */
    if (datalen <= secremain)
    {
        secremain = datalen;    /* 不大于4096个字节 */
    }

    while (1)
    {
        W25Q64_Read(norflash_buf, secpos * 4096, 4096);   /* 读出整个扇区的内容 */

        for (i = 0; i < secremain; i++)   /* 校验数据 */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;      /* 需要擦除, 直接退出for循环 */
            }
        }

        if (i < secremain)   /* 需要擦除 */
        {
            W25Q64_Erase_Sector(secpos);  /* 擦除这个扇区 */

            for (i = 0; i < secremain; i++)   /* 复制 */
            {
                norflash_buf[i + secoff] = pbuf[i];
            }

            W25Q64_Write_NoCheck(norflash_buf, secpos * 4096, 4096);  /* 写入整个扇区 */
        }
        else        /* 写已经擦除了的,直接写入扇区剩余区间. */
        {
            W25Q64_Write_NoCheck(pbuf, addr, secremain);  /* 直接写扇区 */
        }

        if (datalen == secremain)
        {
            break;  /* 写入结束了 */
        }
        else        /* 写入未结束 */
        {
            secpos++;               /* 扇区地址增1 */
            secoff = 0;             /* 偏移位置为0 */

            pbuf += secremain;      /* 指针偏移 */
            addr += secremain;      /* 写地址偏移 */
            datalen -= secremain;   /* 字节数递减 */

            if (datalen > 4096)
            {
                secremain = 4096;   /* 下一个扇区还是写不完 */
            }
            else
            {
                secremain = datalen;/* 下一个扇区可以写完了 */
            }
        }
    }
#ifdef MEM1_ALLOC_TABLE_SIZE        /* 使用了内存管理 */
    myfree(SRAMIN, norflash_buf);   /* 释放申请的内存 */
#endif
}


static void W25Q64_Write_NoCheck(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t pageremain;
    pageremain = 256 - addr % 256;  /* 单页剩余的字节数 */

    if (datalen <= pageremain)      /* 不大于256个字节 */
    {
        pageremain = datalen;
    }

    while (1)
    {
        /* 当写入字节比页内剩余地址还少的时候, 一次性写完
         * 当写入直接比页内剩余地址还多的时候, 先写完整个页内剩余地址, 然后根据剩余长度进行不同处理
         */
        W25Q64_Write_Page(pbuf, addr, pageremain);

        if (datalen == pageremain)   /* 写入结束了 */
        {
            break;
        }
        else     /* datalen > pageremain */
        {
            pbuf += pageremain;         /* pbuf指针地址偏移,前面已经写了pageremain字节 */
            addr += pageremain;         /* 写地址偏移,前面已经写了pageremain字节 */
            datalen -= pageremain;      /* 写入总长度减去已经写入了的字节数 */

            if (datalen > 256)          /* 剩余数据还大于一页,可以一次写一页 */
            {
                pageremain = 256;       /* 一次可以写入256个字节 */
            }
            else     /* 剩余数据小于一页,可以一次写完 */
            {
                pageremain = datalen;   /* 不够256个字节了 */
            }
        }
    }
}

static void W25Q64_Write_Page(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t i;

    W25Q64_Write_ENABLE();    /* 写使能 */

    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_PageProgram);    /* 发送写页命令 */
    W25Q64_Send_Address(addr);                /* 发送地址 */

    for(i=0;i<datalen;i++)
    {
        SPI_RW_W25Q64(pbuf[i]);          /* 循环写入 */
    }
    
    W25Q64_CS(1);
    W25Q64_Wait_Busy();       /* 等待写入结束 */
}

void W25Q64_Read(uint8_t *pbuf, uint32_t addr, uint16_t datalen)
{
    uint16_t i;

    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_ReadData);       /* 发送读取命令 */
    W25Q64_Send_Address(addr);                /* 发送地址 */
    
    for(i=0;i<datalen;i++)
    {
        pbuf[i] = SPI_RW_W25Q64(0XFF);   /* 循环读取 */
    }
    
    W25Q64_CS(1);
}




void W25Q64_Erase_Chip(void)
{
    W25Q64_Write_ENABLE();    /* 写使能 */
    W25Q64_Wait_Busy();       /* 等待空闲 */
    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_ChipErase);  /* 发送读寄存器命令 */ 
    W25Q64_CS(1);
    W25Q64_Wait_Busy();       /* 等待芯片擦除结束 */
}

void W25Q64_Erase_Sector(uint32_t saddr)
{
    //printf("fe:%x\r\n", saddr);   /* 监视falsh擦除情况,测试用 */
    saddr *= 4096;
    W25Q64_Write_ENABLE();        /* 写使能 */
    W25Q64_Wait_Busy();           /* 等待空闲 */

    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_SectorErase);    /* 发送写页命令 */
    W25Q64_Send_Address(saddr);   /* 发送地址 */
    W25Q64_CS(1);
    W25Q64_Wait_Busy();           /* 等待扇区擦除完成 */
}

uint16_t W25Q64_Read_ID(void)
{
    uint16_t deviceid;

    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_ManufactDeviceID);   /* 发送读 ID 命令 */
    SPI_RW_W25Q64(0);    /* 写入一个字节 */
    SPI_RW_W25Q64(0);
    SPI_RW_W25Q64(0);
    deviceid = SPI_RW_W25Q64(0xFF) << 8;     /* 读取高8位字节 */
    deviceid |= SPI_RW_W25Q64(0xFF);         /* 读取低8位字节 */
    W25Q64_CS(1);

    return deviceid;
}


uint8_t W25Q64_Read_Status_Register(uint8_t regno)
{
    uint8_t byte = 0, command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_ReadStatusReg1;  /* 读状态寄存器1指令 */
            break;

        case 2:
            command = FLASH_ReadStatusReg2;  /* 读状态寄存器2指令 */
            break;

        case 3:
            command = FLASH_ReadStatusReg3;  /* 读状态寄存器3指令 */
            break;

        default:
            command = FLASH_ReadStatusReg1;
            break;
    }

    W25Q64_CS(0);
    SPI_RW_W25Q64(command);      /* 发送读寄存器命令 */
    byte = SPI_RW_W25Q64(0Xff);  /* 读取一个字节 */
    W25Q64_CS(1);
    
    return byte;
}

void W25Q64_Write_Status_Register(uint8_t regno, uint8_t sr)
{
    uint8_t command = 0;

    switch (regno)
    {
        case 1:
            command = FLASH_WriteStatusReg1;  /* 写状态寄存器1指令 */
            break;

        case 2:
            command = FLASH_WriteStatusReg2;  /* 写状态寄存器2指令 */
            break;

        case 3:
            command = FLASH_WriteStatusReg3;  /* 写状态寄存器3指令 */
            break;

        default:
            command = FLASH_WriteStatusReg1;
            break;
    }

    W25Q64_CS(0);
    SPI_RW_W25Q64(command);  /* 发送读寄存器命令 */
    SPI_RW_W25Q64(sr);       /* 写入一个字节 */
    W25Q64_CS(1);
}

void W25Q64_Write_ENABLE(void)
{
    W25Q64_CS(0);
    SPI_RW_W25Q64(FLASH_WriteEnable);   /* 发送写使能 */
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



















