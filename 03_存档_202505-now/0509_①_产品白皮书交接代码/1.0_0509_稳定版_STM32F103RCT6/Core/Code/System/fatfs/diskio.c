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


/* Ϊÿ���豸����һ�������� */
#define ATA			    0     // Ԥ��SD��ʹ��
#define SPI_FLASH		1     // �ⲿSPI Flash

/*-----------------------------------------------------------------------*/
/* ��ȡ�豸״̬                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
    BYTE pdrv		/* ������ */
)
{

    DSTATUS status = STA_NOINIT;
    
    switch (pdrv) {
        case ATA:	/* SD CARD */
            break;
    
        case SPI_FLASH:      
      /* SPI Flash״̬��⣺��ȡSPI Flash �豸ID */
    #ifndef USE_EMBEDFIRE_SPI
        if(sFLASH_ID == W25Q64_Read_JEDEC_ID())
    #else
        if(sFLASH_ID == SPI_FLASH_ReadID())
    #endif

      {
        /* �豸ID��ȡ�����ȷ */
        status &= ~STA_NOINIT;
      }
      else
      {
        /* �豸ID��ȡ������� */
        status = STA_NOINIT;;
      }
            break;

        default:
            status = STA_NOINIT;
    }
    return status;
}

/*-----------------------------------------------------------------------*/
/* �豸��ʼ��                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
    BYTE pdrv				/* ������ */
)
{
  uint16_t i;
    DSTATUS status = STA_NOINIT;	
    switch (pdrv) {
        case ATA:	         /* SD CARD */
            break;
    
        case SPI_FLASH:    /* SPI Flash */ 
      /* ��ʼ��SPI Flash */
            #ifndef USE_EMBEDFIRE_SPI
                SPI_APP_Init(SPI_Class_1);
            #else
			    SPI_FLASH_Init();
            #endif
            
      /* ��ʱһС��ʱ�� */
      i=500;
        while(--i);	
      /* ����SPI Flash */
        #ifndef USE_EMBEDFIRE_SPI
            W25Q64_ReleasePowerDown();
        #else
	        SPI_Flash_WAKEUP();
        #endif
        
      /* ��ȡSPI FlashоƬ״̬ */
      status=disk_status(SPI_FLASH);
            break;
      
        default:
            status = STA_NOINIT;
    }
    return status;
}


/*-----------------------------------------------------------------------*/
/* ����������ȡ�������ݵ�ָ���洢��                                              */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
    BYTE pdrv,		/* �豸������(0..) */
    BYTE *buff,		/* ���ݻ����� */
    DWORD sector,	/* �����׵�ַ */
    UINT count		/* ��������(1..128) */
)
{
    DRESULT status = RES_PARERR;
    switch (pdrv) {
        case ATA:	/* SD CARD */
            break;
    
        case SPI_FLASH:
      /* ����ƫ��2MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����6MB�ռ� */
      #ifndef USE_EMBEDFIRE_SPI
        sector+=514;      
        SPI_APP_Buffer_Read(SPI_Class_1, buff, count<<12, sector <<12);
      #else
        sector+=512;      
        SPI_FLASH_BufferRead(buff, sector<<12, count<<12);
        /* ����ƫ�� */
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
/* д������������д��ָ�������ռ���                                      */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    BYTE pdrv,			  /* �豸������(0..) */
    const BYTE *buff,	/* ��д�����ݵĻ����� */
    DWORD sector,		  /* �����׵�ַ */
    UINT count			  /* ��������(1..128) */
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
      /* ����ƫ��2MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����6MB�ռ� */
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
            
            /* ����ƫ�� */
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
/* ��������                                                              */
/*-----------------------------------------------------------------------*/

// #if _USE_IOCTL
DRESULT disk_ioctl (
    BYTE pdrv,		/* ������ */
    BYTE cmd,		  /* ����ָ�� */
    void *buff		/* д����߶�ȡ���ݵ�ַָ�� */
)
{
    DRESULT status = RES_PARERR;
    switch (pdrv) {
        case ATA:	/* SD CARD */
            break;
    
        case SPI_FLASH:
            switch (cmd) {
        /* ����������1536*4096/1024/1024=6(MB) */
        case GET_SECTOR_COUNT:	
            #ifndef USE_EMBEDFIRE_SPI
                *(DWORD * )buff = 1534;
            #else
                *(DWORD * )buff = 1536;	
            #endif
        break;
        /* ������С  */
        case GET_SECTOR_SIZE :
          *(WORD * )buff = 4096;
        break;
        /* ͬʱ������������ */
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
    /* ���ص�ǰʱ��� */
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
  /* ���������ʵ�ʵĻ�ȡʱ��ķ���... */
  
  
  /* ��Ӧ��Ҫ�޸ķ���ֵ */
  return ((2021-1980)<<25) | ((1)<<21) | ((1)<<16) | ((1)<<11) | ((1)<<5) | ((1)<<0);
}

#endif
