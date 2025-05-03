#include "main.h"

#ifndef USE_EMBEDFIRE_SPI
#include "code.h"
/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/
#else
#include "bsp_spi_flash.h"
/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#endif


#include "diskio.h"		/* FatFs lower layer API */
#include "ff.h"


/* 为每个设备定义一个物理编号 */
#define ATA			    0     // 预留SD卡使用
#define SPI_FLASH		1     // 外部SPI Flash

/*-----------------------------------------------------------------------*/
/* 获取设备状态                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
    BYTE pdrv		/* 物理编号 */
)
{

    DSTATUS status = STA_NOINIT;
    
    switch (pdrv) {
        case ATA:	/* SD CARD */
            break;
    
        case SPI_FLASH:      
      /* SPI Flash状态检测：读取SPI Flash 设备ID */
    #ifndef USE_EMBEDFIRE_SPI
        if(sFLASH_ID == W25Q64_Read_JEDEC_ID())
    #else
        if(sFLASH_ID == SPI_FLASH_ReadID())
    #endif

      {
        /* 设备ID读取结果正确 */
        status &= ~STA_NOINIT;
      }
      else
      {
        /* 设备ID读取结果错误 */
        status = STA_NOINIT;;
      }
            break;

        default:
            status = STA_NOINIT;
    }
    return status;
}

/*-----------------------------------------------------------------------*/
/* 设备初始化                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
    BYTE pdrv				/* 物理编号 */
)
{
  uint16_t i;
    DSTATUS status = STA_NOINIT;	
    switch (pdrv) {
        case ATA:	         /* SD CARD */
            break;
    
        case SPI_FLASH:    /* SPI Flash */ 
      /* 初始化SPI Flash */
            #ifndef USE_EMBEDFIRE_SPI
                SPI_APP_Init(SPI_Class_1);
            #else
			    SPI_FLASH_Init();
            #endif
            
      /* 延时一小段时间 */
      i=500;
        while(--i);	
      /* 唤醒SPI Flash */
        #ifndef USE_EMBEDFIRE_SPI
            W25Q64_ReleasePowerDown();
        #else
	        SPI_Flash_WAKEUP();
        #endif
        
      /* 获取SPI Flash芯片状态 */
      status=disk_status(SPI_FLASH);
            break;
      
        default:
            status = STA_NOINIT;
    }
    return status;
}


/*-----------------------------------------------------------------------*/
/* 读扇区：读取扇区内容到指定存储区                                              */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
    BYTE pdrv,		/* 设备物理编号(0..) */
    BYTE *buff,		/* 数据缓存区 */
    DWORD sector,	/* 扇区首地址 */
    UINT count		/* 扇区个数(1..128) */
)
{
    DRESULT status = RES_PARERR;
    switch (pdrv) {
        case ATA:	/* SD CARD */
            break;
    
        case SPI_FLASH:
      /* 扇区偏移2MB，外部Flash文件系统空间放在SPI Flash后面6MB空间 */
      #ifndef USE_EMBEDFIRE_SPI
        sector+=514;      
        SPI_APP_Buffer_Read(SPI_Class_1, buff, count<<12, sector <<12);
      #else
        sector+=512;      
        SPI_FLASH_BufferRead(buff, sector<<12, count<<12);
        /* 不想偏移 */
        // SPI_FLASH_BufferRead((BYTE *)buff, sector<<12, count<<12);
      #endif
      
      status = RES_OK;
        break;
    
        default:
            status = RES_PARERR;
    }
    return status;
}

/*-----------------------------------------------------------------------*/
/* 写扇区：见数据写入指定扇区空间上                                      */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    BYTE pdrv,			  /* 设备物理编号(0..) */
    const BYTE *buff,	/* 欲写入数据的缓存区 */
    DWORD sector,		  /* 扇区首地址 */
    UINT count			  /* 扇区个数(1..128) */
)
{
  uint32_t write_addr; 
    DRESULT status = RES_PARERR;
    if (!count) {
        return RES_PARERR;		/* Check parameter */
    }

    switch (pdrv) {
        case ATA:	/* SD CARD */      
        break;

        case SPI_FLASH:
      /* 扇区偏移2MB，外部Flash文件系统空间放在SPI Flash后面6MB空间 */
        #ifndef USE_EMBEDFIRE_SPI
            sector+=514;
            write_addr = sector<<12;    
            W25Q64_Erase_Sector(write_addr);
            SPI_APP_Buffer_Write(SPI_Class_1, (uint8_t *)buff,count<<12,write_addr);
            status = RES_OK;
        #else
            sector+=512;
            write_addr = sector<<12;    
            SPI_FLASH_SectorErase(write_addr);
            SPI_FLASH_BufferWrite((uint8_t *)buff,write_addr,count<<12);
            status = RES_OK;
            
            /* 不想偏移 */
            // SPI_FLASH_SectorErase(sector<<12);
            // SPI_FLASH_BufferWrite((BYTE *)buff, sector<<12, count<<12);
            // status = RES_OK;		
        #endif

        break;
    
        default:
            status = RES_PARERR;
    }
    return status;
}

/*-----------------------------------------------------------------------*/
/* 其他控制                                                              */
/*-----------------------------------------------------------------------*/

// #if _USE_IOCTL
DRESULT disk_ioctl (
    BYTE pdrv,		/* 物理编号 */
    BYTE cmd,		  /* 控制指令 */
    void *buff		/* 写入或者读取数据地址指针 */
)
{
    DRESULT status = RES_PARERR;
    switch (pdrv) {
        case ATA:	/* SD CARD */
            break;
    
        case SPI_FLASH:
            switch (cmd) {
        /* 扇区数量：1536*4096/1024/1024=6(MB) */
        case GET_SECTOR_COUNT:	
            #ifndef USE_EMBEDFIRE_SPI
                *(DWORD * )buff = 1534;
            #else
                *(DWORD * )buff = 1536;	
            #endif
        break;
        /* 扇区大小  */
        case GET_SECTOR_SIZE :
          *(WORD * )buff = 4096;
        break;
        /* 同时擦除扇区个数 */
        case GET_BLOCK_SIZE :
          *(DWORD * )buff = 1;
        break;        
      }
      status = RES_OK;
        break;
    
        default:
            status = RES_PARERR;
    }
    return status;
}
// #endif


#ifndef USE_EMBEDFIRE_SPI

__weak DWORD get_fattime(void) {
    /* 返回当前时间戳 */
    return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
            | ((DWORD)1 << 21)				/* Month 1 */
            | ((DWORD)1 << 16)				/* Mday 1 */
            | ((DWORD)0 << 11)				/* Hour 0 */
            | ((DWORD)0 << 5)				  /* Min 0 */
            | ((DWORD)0 >> 1);				/* Sec 0 */
}

#else

DWORD get_fattime (void)
{
  /* 在这里添加实际的获取时间的方法... */
  
  
  /* 对应的要修改返回值 */
  return ((2021-1980)<<25) | ((1)<<21) | ((1)<<16) | ((1)<<11) | ((1)<<5) | ((1)<<0);
}

#endif
