#include "my_fatfs.h"
#include "my_flash.h"

#include "my_string.h"

#ifndef __Map__Diskio__Reference__
#define __Map__Diskio__Reference__


    DSTATUS FLASH_Disk_Status(BYTE input_Device){
        if(SPI_FLASH != input_Device) return STA_NOINIT;

        DSTATUS status = STA_NOINIT;

        if(sFLASH_ID == MAP_FLASH_ReadID()){
            /* �豸ID��ȡ�����ȷ */
            status &= ~STA_NOINIT;
        }else{
            /* �豸ID��ȡ������� */
            status = STA_NOINIT;
        }

        return status;
    }

    DSTATUS FLASH_Disk_Initialize(BYTE input_Device){
        if(SPI_FLASH != input_Device) return STA_NOINIT;

        DSTATUS status = STA_NOINIT;
        
        /* ��ʼ��SPI Flash */
        MAP_FLASH_Init();
        /* ��ʼ��SPI Flash */
        for(int i = 0; i < 500; i++);
        /* ����SPI Flash */
        MAP_FLASH_WakeUp();
        /* ��ȡSPI FlashоƬ״̬ */
        status = FLASH_Disk_Status(input_Device);

        return status;
    }

    DSTATUS FLASH_Disk_Read(BYTE input_Device, BYTE *buff, DWORD sector, UINT count){
        if(SPI_FLASH != input_Device) return RES_PARERR;

        /* ����ƫ��?MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����?MB�ռ� */
        sector += FATFS_SECTOR_OFFSET;
        MAP_FLASH_BufferRead(buff, sector <<12, count<<12);

        return RES_OK;
    }

    DSTATUS FLASH_Disk_Write(BYTE input_Device, const BYTE *buff, DWORD sector, UINT count){
        if(SPI_FLASH != input_Device) return RES_PARERR;
        if (!count) return RES_PARERR;		/* Check parameter */
        
        /* ����ƫ��?MB���ⲿFlash�ļ�ϵͳ�ռ����SPI Flash����?MB�ռ� */
        sector += FATFS_SECTOR_OFFSET;
        sector = sector << 12;
        MAP_FLASH_SectorErase(sector);
        MAP_FLASH_BufferWrite((uint8_t *)buff, sector, count<<12);

        return RES_OK;
    }

    DSTATUS FLASH_Disk_IOCtl(BYTE input_Device, BYTE cmd, void *buff){
        if(SPI_FLASH != input_Device) return RES_PARERR;

        switch (cmd) {
            case GET_SECTOR_COUNT:
            /* ����������FATFS_SECTOR_COUNT*4096/1024/1024=?(MB) */
                *(DWORD * )buff = FATFS_SECTOR_COUNT;
            break;

            case GET_SECTOR_SIZE :
            /* ������С  */
                *(WORD * )buff = 4096;
            break;

            case GET_BLOCK_SIZE :
            /* ͬʱ������������ */
                *(DWORD * )buff = 1;
            break;

            default:
                return RES_PARERR;
          }

      return RES_OK;
    }


#endif // __Map__Diskio__Reference__



// fatfs self test
#if 1

/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
    FATFS fs;													/* FatFs�ļ�ϵͳ���� */
    FIL fnew;													/* �ļ����� */
    FRESULT res_flash;                /* �ļ�������� */
    UINT fnum;            					  /* �ļ��ɹ���д���� */
    BYTE ReadBuffer[1024]={0};        /* �������� */
    BYTE WriteBuffer[] =              /* д������*/
    "��ӭʹ��Ұ��STM32 F407������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  
 
/**
  ******************************************************************************
  *                                ������
  ******************************************************************************
  */

void my_fatfs_self_test(void)
{
//   SystemClock_Config();
// 	/* ��ʼ��LED */
// 	LED_GPIO_Config();	
// 	LED_BLUE;
  
// 	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
// 	 DEBUG_USART_Config();	
  printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
  
  //���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
  res_flash = f_mount(&fs,"1:",1);
  
/*----------------------- ��ʽ������ ---------------------------*/  
  /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
  if(res_flash == FR_NO_FILESYSTEM)
  {
    printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
    /* ��ʽ�� */
    res_flash=f_mkfs("1:",0,0);							
    
    if(res_flash == FR_OK)
    {
      printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
      /* ��ʽ������ȡ������ */
      res_flash = f_mount(NULL,"1:",1);			
      /* ���¹���	*/			
      res_flash = f_mount(&fs,"1:",1);
    }
    else
    {
      // LED_RED;
      printf("������ʽ��ʧ�ܡ�����\r\n");
      while(1);
    }
  }
  else if(res_flash!=FR_OK)
  {
    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
    while(1);
  }
  else
  {
    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
  
/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
  /* ���ļ�������ļ��������򴴽��� */
  printf("\r\n****** ���������ļ�д�����... ******\r\n");	
  res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
  if ( res_flash == FR_OK )
  {
    printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
    /* ��ָ���洢������д�뵽�ļ��� */
    res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
    }    
    /* ���ٶ�д���ر��ļ� */
    f_close(&fnew);
  }
  else
  {	
    // LED_RED;
    printf("������/�����ļ�ʧ�ܡ�\r\n");
  }
  
/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
  printf("****** ���������ļ���ȡ����... ******\r\n");
  res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ); 	 
  if(res_flash == FR_OK)
  {
    // LED_GREEN;
    printf("�����ļ��ɹ���\r\n");
    res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
    }		
  }
  else
  {
    // LED_RED;
    printf("�������ļ�ʧ�ܡ�\r\n");
  }
  /* ���ٶ�д���ر��ļ� */
  f_close(&fnew);	
  
  /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
//   f_mount(NULL,"1:",1);
  
  /* ������ɣ�ͣ�� */
    printf("������ɣ�ͣ��\r\n");
  while(1)
  {
  }
}


#endif
// fatfs self test



// fatfs functions
#if 1

    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>


    // head
    #if 1

        #define Index_Text_Default "\r\n10001"

        FATFS fs_defualt;													/* FatFs�ļ�ϵͳ���� */
        FIL f_default;													/* �ļ����� */
        FRESULT r_default;                /* �ļ�������� */
        UINT f_read_count;            					  /* �ļ��ɹ���д���� */
        char f_scan_path[100];                  /* ���浱ǰɨ��·�� */

        #ifdef USE_ATK_MALLOC
        char *f_read_buffer = 0;
        #else
        char f_read_buffer[512];
        // char f_read_buffer[2000];
        #endif

        long FATFS_Index_Current = 0;

        FATFS_Result_Struct FATFS_Result_default = {
            .File_Size = 0
        };

        static FRESULT scan_files (char* path) 
        { 
            FRESULT res; 		//�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���	
            FILINFO fno; 
            DIR dir; 
            int i;
            uint8_t j = 1;
            char *fn;        // �ļ���	
            
        #if _USE_LFN 
            /* ���ļ���֧�� */
            /* ����������Ҫ2���ֽڱ���һ�����֡�*/
            static char lfn[_MAX_LFN*2 + 1]; 	
            fno.lfname = lfn; 
            fno.lfsize = sizeof(lfn); 
        #endif 
            //��Ŀ¼
            res = f_opendir(&dir, path); 
            printf("\r\nthe list of files is as follow: \r\n>>>\r\n");
            if (res == FR_OK) 
            { 
                i = strlen(path); 
                for (;;) 
                { 
                    //��ȡĿ¼�µ����ݣ��ٶ����Զ�����һ���ļ�
                    res = f_readdir(&dir, &fno); 								
                    //Ϊ��ʱ��ʾ������Ŀ��ȡ��ϣ�����
                    if (res != FR_OK || fno.fname[0] == 0) break; 	
        #if _USE_LFN 
                    fn = *fno.lfname ? fno.lfname : fno.fname; 
        #else 
                    fn = fno.fname; 
        #endif 
                    //���ʾ��ǰĿ¼������			
                    if (*fn == '.') continue; 	
                    //Ŀ¼���ݹ��ȡ      
                    if (fno.fattrib & AM_DIR)         
                    { 			
                        //�ϳ�����Ŀ¼��        
                        sprintf(&path[i], "/%s", fn); 		
                        //�ݹ����         
                        res = scan_files(path);	
                        path[i] = 0;         
                        //��ʧ�ܣ�����ѭ��        
                        if (res != FR_OK) 
                            break; 
                    } 
                    else 
                    { 
                        printf(" [%d]`%s/%s`, ", j++, path, fn);								//����ļ���	
                        /* ������������ȡ�ض���ʽ���ļ�·�� */        
                    }//else
                } //for
            } 
            printf("\r\n<<<\r\n");
            return res; 
        }

    #endif
    // head


    // base
    #if 1

        /**
         * @brief  Read file.
         * @param  path: File path.
         * @param  is_echo: Is display the content or not.
         * @retval File size
         */
        int FATFS_Read(char* path, uint8_t is_echo){
            int file_size;

            // 1.Ԥ�ȹر� (����)
            // f_close(&f_default);

            // 2.���ļ�
            r_default = f_open(&f_default, path, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
            if(FR_OK == r_default){

            // 3.��ȡ�ļ���С
                file_size = f_size(&f_default);
                printf("[%s] size: %d.\r\n", path, file_size);
                #ifdef USE_ATK_MALLOC
                f_read_buffer = (void*)mymalloc(SRAMIN, file_size);
                printf("used percentage after malloc: %3d%%.\r\n", my_mem_perused(SRAMIN));
                #endif

            // 4.��λ���ļ���ʼλ��
                r_default = f_lseek(&f_default, 0);
                if(FR_OK == r_default){

            // 5.Ԥ�ȸ�ʽ�� (����)
                    #ifndef USE_ATK_MALLOC
                    memset(f_read_buffer, 0, sizeof(f_read_buffer));
                    #endif

            // 6.��ȡ�������ݵ�������
                    r_default = f_read(&f_default, f_read_buffer, file_size, &f_read_count);
                    if(FR_OK == r_default){

            // 7.����    
                        if(is_echo){
                            printf("file content are as follow: \r\n>>>\r\n%s", f_read_buffer);
                            printf("\r\n<<<\r\n");
                        }

                    }else{
                        printf("fail to f_read [%s]\r\n", path);
                    }

                }else{
                    printf("fail to f_lseek [%s], result is [%d].\r\n", path, r_default);
                }

            }else{
                printf("fail to f_open [%s], result is [%d].\r\n", path, r_default);
            }

            // 8.��ɺ�ر�
            #ifdef USE_ATK_MALLOC
            myfree(SRAMIN, f_read_buffer);
            printf("used percentage after free: %3d%%.\r\n", my_mem_perused(SRAMIN));
            #endif
            f_close(&f_default);
            return file_size;
        }

        /**
         * @brief  Write someting to file.
         * @param  path: Path of file.
         * @param  write_buffer: Buffer to write.
         * @param  write_position: Position of the file to write.
         *         @arg  -1: Tail address
         *         @arg   0: Head address
         *         @arg ...: Specified address
         * @param  is_echo: Is display the content or not.
         * @param  fast_mode: Fast to write.
         *         @arg  3: Open
         *         @arg  2: Continue
         *         @arg  1: Close
         *         @arg  0: Disable
         * @retval None
         */
        void FATFS_Write(char* path, char* write_buffer, int write_position, uint8_t is_echo, uint8_t fast_mode){
            // if(1 <= fast_mode <= 2){
            //     f_printf(&f_default, write_buffer);
            //     if(fast_mode == 1){
            //         f_close(&f_default);
            //     }
            //     return;
            // }
            if(fast_mode == 1 || fast_mode == 2){
                f_printf(&f_default, write_buffer);
                if(fast_mode == 1){
                    f_close(&f_default);
                }
                return;
            }
            
            // 1.�ر��ļ� (����)
            if(fast_mode != 3){
                f_close(&f_default);
            }

            // 2.���ļ�
            r_default = f_open(&f_default, path, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
            if(FR_OK == r_default){
                
            // 3.��λд��λ��
                if(write_position < -1){
                    /* ���<-1���ǷǷ���ַ */
                    r_default = FR_INVALID_PARAMETER;
                    printf("the write position is invalid.\r\n");
                }else if(write_position == -1){
                    /* ���==-1��λ��β�� */
                    r_default = f_lseek(&f_default, f_size(&f_default));
                }else{
                    /* ���!=-1��λ��ָ��λ�� */
                    r_default = f_lseek(&f_default, write_position);
                }

            // 4.��write_bufferд��
                if(FR_OK == r_default){
                    f_printf(&f_default, write_buffer);
                    if(FR_OK == r_default){
                        if(fast_mode == 3){
                            return;
                        }
                        f_close(&f_default);
                        printf("write is succeed.\r\n");
            // 5.����    
                        if(is_echo){
                            printf("now read it ->\r\n");
                            FATFS_Read(path, 1);
                        }

                    }else{
                        printf("fail to f_printf [%s]\r\n", &path);
                    }

                }else{
                    printf("fail to f_lseek [%s]\r\n", &path);
                }

            }else{
                printf("fail to f_open [%s]\r\n", &path);
            }

            // 6.��ɺ�ر�
            f_close(&f_default);
            return;
        }

        /**
         * @brief  Delete file.
         * @param  path: File path.
         * @param  is_scan: Is scan the file list or not.
         * @retval None
         */
        void FATFS_Delete(char* path, uint8_t is_scan, uint8_t is_secure){
            // 1.�ر��ļ� (����)
            f_close(&f_default);

            // 2.ɾ���ļ�
            printf("try to delete [%s].\r\n", path);
            r_default = f_unlink(path);
            if(FR_OK == r_default){
                printf("succeed to delete [%s].\r\n", path);

            // 3.ɨ����
                if(is_scan){
                    printf("now scan files to check ->\r\n");
                    FATFS_Scan();
                }
            }else{
                printf("fail to f_unlink [%s]\r\n", path);
                if(is_secure){
                    printf("now it is paused...\r\n");
                    while(1){
                        // pause
                    }
                }
            }
            
            // 4.��ɺ�ر�
            f_close(&f_default);
        }

        void i_file_read(char* path, uint8_t is_echo){
            FATFS_Read(path, is_echo);
        }

    #endif
    // base


    // function
    #if 1

        /**
         * @brief  FAT file system initialize.
         * @param  None
         * @retval None
         */
        void FATFS_Init(uint8_t is_secure){
            r_default = f_mount(&fs_defualt,"1:",1);
            if(FR_OK == r_default){
                printf("succeed to mount fatfs.\r\n");
            }else{
                printf("fail to mount fatfs.\r\n");
                if(is_secure){
                    printf("now it is paused...\r\n");
                    while(1){
                        my_fatfs_self_test();
                        // pause
                    }
                }
            }
        }

        /**
         * @brief  FAT file system deinitialize.
         * @param  None
         * @retval None
         */
        void FATFS_Deinit(uint8_t is_secure){
            r_default = f_mount(NULL,"1:",1);
            if(FR_OK == r_default){
                printf("succeed to unmount fatfs.\r\n");
            }else{
                printf("fail to unmount fatfs.\r\n");
                if(is_secure){
                    printf("now it is paused...\r\n");
                    while(1){
                        // pause
                    }
                }
            }
        }

        /**
         * @brief  FAT file system scan files.
         * @param  None
         * @retval None
         */
        void FATFS_Scan(void){
            printf("scaning files...\r\n");
            strcpy(f_scan_path,"1:");
            scan_files(f_scan_path);
        }

        /**
         * @brief  FAT file system multiple test.
         * @param  None
         * @retval None
         */
        void FATFS_Muti_Test(void){
            // 1.��ʼ��
            FATFS_Init(0);

            // 2.ɨ���ļ�
            FATFS_Scan();

            // 3.ɾ���ļ���ɨ��
            char my_path[] = "1:/TestDir/testfile.txt";
            FATFS_Delete(my_path, 1, 0);

            // 4.��ȡ�ļ�������
            FATFS_Read(my_path, 1);

            // 5.д���ļ�������
            FATFS_Write(my_path, Index_Text_Default, -1, 1, 0);

            // 6.ж��
            FATFS_Deinit(0);
        }

        #define index_check_path "1:/my_index.csv"
        #define index_check_buffer_size 20
        float float_fs_index_current;
        /**
         * @brief  Check index for sensor data storage.
         * @param  is_append: Is append new index or not.
         * @retval None
         */
        void FATFS_Index_Check(uint8_t is_append){
            char temp_buffer[index_check_buffer_size];
            char temp_to_write[5];
            uint8_t is_write_next_index = 0;

            // 1.��ȡ�����ļ�
            int temp_size = FATFS_Read(index_check_path, 0);

            // 2.����ļ���Ϊ�����ж������Ƿ���Ч
            if(temp_size > 0){
                get_last_line_from_file(index_check_path, temp_buffer, index_check_buffer_size);
                /* �����Ƿ�>=10001 */
                is_write_next_index = is_number_greater_than_or_equal_to_target(temp_buffer, 10001);
            }

            // 3.д�뵱ǰ����
            if(is_append){
                if(is_write_next_index){
                    /* ׷����һ������ */
                    long temp_num = strtol(temp_buffer, NULL, 10);
                    FATFS_Index_Current = temp_num + 1;
                    sprintf(temp_to_write, "\r\n%d", FATFS_Index_Current);
                    FATFS_Write(index_check_path, temp_to_write, -1, 0, 0);
                }else{
                    /* ����Ҫ׷������, ֱ��д��Ĭ������10001 */
                    FATFS_Index_Current = 10001;
                    FATFS_Write(index_check_path, Index_Text_Default, -1, 1, 0);
                }

                printf("[FATFS_Index_Current: %d]\r\n", FATFS_Index_Current);
            }else{
                printf("[FATFS_Index_Current: %s]\r\n", temp_buffer);
            }

            float_fs_index_current = (float) FATFS_Index_Current;
        }

        
        void FATFS_Convert_Array(char *input_Path){
            #ifdef USE_ATK_MALLOC
            printf("[%s][%d]: FATFS_Convert_Array��ʱ������\r\n", __FILE__, __LINE__);
            #else
            FATFS_Read(input_Path, 0);
            convert_text_to_array(f_read_buffer);
            printf("\r\n[3][2] + [4][2] = %f\r\n", sensor_storage_data[3][2] + sensor_storage_data[4][2]);
            printf("\r\n[3][2] = %f\r\n", sensor_storage_data[3][2]);
            printf("\r\n[4][2] = %f\r\n", sensor_storage_data[4][2]);
            #endif
        }

    #endif
    // function


    // other
    #if 1

        float sensor_storage_data[MAX_ROWS][MAX_COLS];  // ��Ž����������

        void convert_text_to_array(const char *input) {
            char buffer[MAX_BUF];  
            strncpy(buffer, input, MAX_BUF - 1); // �����ַ����������޸�ԭʼ����
            buffer[MAX_BUF - 1] = '\0'; // ȷ���ַ����� `\0` ��β

            char *line, *token;
            char *saveptr_row, *saveptr_col; // ���� strtok_r �ı���ָ��
            int row = 0, col;
            
            line = strtok_r(buffer, "\n", &saveptr_row); // ���зָ�
            while (line && row < MAX_ROWS) {
                col = 0;
                token = strtok_r(line, ",", &saveptr_col); // �� `,` �ָ���
                while (token && col < MAX_COLS) {
                    sensor_storage_data[row][col] = atof(token); // ת��Ϊ float ����������
                    col++;
                    token = strtok_r(NULL, ",", &saveptr_col);
                }
                row++;
                line = strtok_r(NULL, "\n", &saveptr_row);
            }
        }

        #define debug_get_last_line_from_file
        #ifdef debug_get_last_line_from_file

        FRESULT get_last_line_from_file(const char *ipath, char *ibuffer, size_t ibuffer_size) {
            // FIL f_default;         // ����һ���ļ��������ڲ����ļ�

            FRESULT iresult;      // �洢 FatFs �����Ľ��
            int ichar;           // ���ڴ洢��ȡ�ĵ����ַ�
            size_t iposition = 0;   // �������ĵ�ǰλ��
            UINT ibytes_read;  // ʵ�ʶ�ȡ���ֽ���


            iresult = f_open(&f_default, ipath, FA_OPEN_EXISTING | FA_READ);
            if (iresult != FR_OK) {
                // ������ļ�ʧ�ܣ�ֱ�ӷ��ش�����
                return iresult;
            }

            // ���ļ�ָ�붨λ���ļ�ĩβ��f_size(&f_default) �����ļ��Ĵ�С
            iresult = f_lseek(&f_default, f_size(&f_default));
            if (iresult != FR_OK) {
                // �����λʧ�ܣ��ر��ļ������ش�����
                f_close(&f_default);
                return iresult;
            }

            // �������ֽڶ�ȡ�ļ���ֱ���������з����ļ���ͷ
            while (f_tell(&f_default) > 0) {
                // ���ļ�ָ����ǰ�ƶ�һ���ֽ�
                // ��ǰλ��: n
                iresult = f_lseek(&f_default, f_tell(&f_default) - 1);
                if (iresult != FR_OK) {
                    // ����ƶ�ָ��ʧ�ܣ��ر��ļ������ش�����
                    f_close(&f_default);
                    return iresult;
                }
                // ��ȡһ���ֽڵ�����
                // ��ǰλ��: n-1
                char ichar;
                iresult = f_read(&f_default, &ichar, 1, &ibytes_read);
                if (iresult != FR_OK || ibytes_read == 0) {
                    // �����ȡʧ�ܻ򵽴��ļ���ͷ���ر��ļ������ش�����
                    f_close(&f_default);
                    return iresult;
                }
                if (ichar == '\n') {
                    // ����������з�������ѭ��
                    break;
                }
                // ���ļ�ָ������ǰ�ƶ�һ���ֽڣ����������ȡ
                // ��ǰλ��: n
                iresult = f_lseek(&f_default, f_tell(&f_default) - 1);
                if (iresult != FR_OK) {
                    // ����ƶ�ָ��ʧ�ܣ��ر��ļ������ش�����
                    f_close(&f_default);
                    return iresult;
                }

                // ��ǰλ��: n-1
                // (�´�����ѭ����ʱ��: n-1)
            }

            // ��ȡ���һ������
            while (iposition < ibuffer_size - 1) {
                // ��ȡһ���ֽڵ�����
                iresult = f_read(&f_default, &ichar, 1, &ibytes_read);
                if (iresult != FR_OK || ibytes_read == 0 || ichar == '\n') {
                    // �����ȡʧ�ܡ������ļ�ĩβ���������з�������ѭ��
                    break;
                }
                // ����ȡ���ַ����뻺����
                ibuffer[iposition++] = (char)ichar;
            }
            // �ڻ�����ĩβ����ַ���������
            ibuffer[iposition] = '\0';

            // �ر��ļ�
            f_close(&f_default);
            // ���ز����ɹ��Ľ��
            return iresult;
        }

        #else

        FRESULT get_last_line_from_file(const char *ipath, char *ibuffer, size_t ibuffer_size) {
            FIL ifile;         // ����һ���ļ��������ڲ����ļ�

            FRESULT iresult;      // �洢 FatFs �����Ľ��
            int ichar;           // ���ڴ洢��ȡ�ĵ����ַ�
            size_t iposition = 0;   // �������ĵ�ǰλ��
            UINT ibytes_read;  // ʵ�ʶ�ȡ���ֽ���


            iresult = f_open(&ifile, ipath, FA_OPEN_EXISTING | FA_READ);
            if (iresult != FR_OK) {
                // ������ļ�ʧ�ܣ�ֱ�ӷ��ش�����
                return iresult;
            }

            // ���ļ�ָ�붨λ���ļ�ĩβ��f_size(&ifile) �����ļ��Ĵ�С
            iresult = f_lseek(&ifile, f_size(&ifile));
            if (iresult != FR_OK) {
                // �����λʧ�ܣ��ر��ļ������ش�����
                f_close(&ifile);
                return iresult;
            }

            // �������ֽڶ�ȡ�ļ���ֱ���������з����ļ���ͷ
            while (f_tell(&ifile) > 0) {
                // ���ļ�ָ����ǰ�ƶ�һ���ֽ�
                // ��ǰλ��: n
                iresult = f_lseek(&ifile, f_tell(&ifile) - 1);
                if (iresult != FR_OK) {
                    // ����ƶ�ָ��ʧ�ܣ��ر��ļ������ش�����
                    f_close(&ifile);
                    return iresult;
                }
                // ��ȡһ���ֽڵ�����
                // ��ǰλ��: n-1
                char ichar;
                iresult = f_read(&ifile, &ichar, 1, &ibytes_read);
                if (iresult != FR_OK || ibytes_read == 0) {
                    // �����ȡʧ�ܻ򵽴��ļ���ͷ���ر��ļ������ش�����
                    f_close(&ifile);
                    return iresult;
                }
                if (ichar == '\n') {
                    // ����������з�������ѭ��
                    break;
                }
                // ���ļ�ָ������ǰ�ƶ�һ���ֽڣ����������ȡ
                // ��ǰλ��: n
                iresult = f_lseek(&ifile, f_tell(&ifile) - 1);
                if (iresult != FR_OK) {
                    // ����ƶ�ָ��ʧ�ܣ��ر��ļ������ش�����
                    f_close(&ifile);
                    return iresult;
                }

                // ��ǰλ��: n-1
                // (�´�����ѭ����ʱ��: n-1)
            }

            // ��ȡ���һ������
            while (iposition < ibuffer_size - 1) {
                // ��ȡһ���ֽڵ�����
                iresult = f_read(&ifile, &ichar, 1, &ibytes_read);
                if (iresult != FR_OK || ibytes_read == 0 || ichar == '\n') {
                    // �����ȡʧ�ܡ������ļ�ĩβ���������з�������ѭ��
                    break;
                }
                // ����ȡ���ַ����뻺����
                ibuffer[iposition++] = (char)ichar;
            }
            // �ڻ�����ĩβ����ַ���������
            ibuffer[iposition] = '\0';

            // �ر��ļ�
            f_close(&ifile);
            // ���ز����ɹ��Ľ��
            return iresult;
        }

        #endif

    #endif
    // ohter


#endif
// fatfs functions




