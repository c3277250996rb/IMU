#include "my_fatfs.h"
#include "my_flash.h"

#include "my_string.h"

#ifndef __Map__Diskio__Reference__
#define __Map__Diskio__Reference__


    DSTATUS FLASH_Disk_Status(BYTE input_Device){
        if(SPI_FLASH != input_Device) return STA_NOINIT;

        DSTATUS status = STA_NOINIT;

        if(sFLASH_ID == MAP_FLASH_ReadID()){
            /* 设备ID读取结果正确 */
            status &= ~STA_NOINIT;
        }else{
            /* 设备ID读取结果错误 */
            status = STA_NOINIT;
        }

        return status;
    }

    DSTATUS FLASH_Disk_Initialize(BYTE input_Device){
        if(SPI_FLASH != input_Device) return STA_NOINIT;

        DSTATUS status = STA_NOINIT;
        
        /* 初始化SPI Flash */
        MAP_FLASH_Init();
        /* 初始化SPI Flash */
        for(int i = 0; i < 500; i++);
        /* 唤醒SPI Flash */
        MAP_FLASH_WakeUp();
        /* 获取SPI Flash芯片状态 */
        status = FLASH_Disk_Status(input_Device);

        return status;
    }

    DSTATUS FLASH_Disk_Read(BYTE input_Device, BYTE *buff, DWORD sector, UINT count){
        if(SPI_FLASH != input_Device) return RES_PARERR;

        /* 扇区偏移?MB，外部Flash文件系统空间放在SPI Flash后面?MB空间 */
        sector += FATFS_SECTOR_OFFSET;
        MAP_FLASH_BufferRead(buff, sector <<12, count<<12);

        return RES_OK;
    }

    DSTATUS FLASH_Disk_Write(BYTE input_Device, const BYTE *buff, DWORD sector, UINT count){
        if(SPI_FLASH != input_Device) return RES_PARERR;
        if (!count) return RES_PARERR;		/* Check parameter */
        
        /* 扇区偏移?MB，外部Flash文件系统空间放在SPI Flash后面?MB空间 */
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
            /* 扇区数量：FATFS_SECTOR_COUNT*4096/1024/1024=?(MB) */
                *(DWORD * )buff = FATFS_SECTOR_COUNT;
            break;

            case GET_SECTOR_SIZE :
            /* 扇区大小  */
                *(WORD * )buff = 4096;
            break;

            case GET_BLOCK_SIZE :
            /* 同时擦除扇区个数 */
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
  *                              定义变量
  ******************************************************************************
  */
    FATFS fs;													/* FatFs文件系统对象 */
    FIL fnew;													/* 文件对象 */
    FRESULT res_flash;                /* 文件操作结果 */
    UINT fnum;            					  /* 文件成功读写数量 */
    BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
    BYTE WriteBuffer[] =              /* 写缓冲区*/
    "欢迎使用野火STM32 F407开发板 今天是个好日子，新建文件系统测试文件\r\n";  
 
/**
  ******************************************************************************
  *                                任务函数
  ******************************************************************************
  */

void my_fatfs_self_test(void)
{
//   SystemClock_Config();
// 	/* 初始化LED */
// 	LED_GPIO_Config();	
// 	LED_BLUE;
  
// 	/* 初始化调试串口，一般为串口1 */
// 	 DEBUG_USART_Config();	
  printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
  
  //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
  res_flash = f_mount(&fs,"1:",1);
  
/*----------------------- 格式化测试 ---------------------------*/  
  /* 如果没有文件系统就格式化创建创建文件系统 */
  if(res_flash == FR_NO_FILESYSTEM)
  {
    printf("》FLASH还没有文件系统，即将进行格式化...\r\n");
    /* 格式化 */
    res_flash=f_mkfs("1:",0,0);							
    
    if(res_flash == FR_OK)
    {
      printf("》FLASH已成功格式化文件系统。\r\n");
      /* 格式化后，先取消挂载 */
      res_flash = f_mount(NULL,"1:",1);			
      /* 重新挂载	*/			
      res_flash = f_mount(&fs,"1:",1);
    }
    else
    {
      // LED_RED;
      printf("《《格式化失败。》》\r\n");
      while(1);
    }
  }
  else if(res_flash!=FR_OK)
  {
    printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
    printf("！！可能原因：SPI Flash初始化不成功。\r\n");
    while(1);
  }
  else
  {
    printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
  
/*----------------------- 文件系统测试：写测试 -----------------------------*/
  /* 打开文件，如果文件不存在则创建它 */
  printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
  res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
  if ( res_flash == FR_OK )
  {
    printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
    /* 将指定存储区内容写入到文件内 */
    res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf("》文件写入成功，写入字节数据：%d\n",fnum);
      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("！！文件写入失败：(%d)\n",res_flash);
    }    
    /* 不再读写，关闭文件 */
    f_close(&fnew);
  }
  else
  {	
    // LED_RED;
    printf("！！打开/创建文件失败。\r\n");
  }
  
/*------------------- 文件系统测试：读测试 ------------------------------------*/
  printf("****** 即将进行文件读取测试... ******\r\n");
  res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ); 	 
  if(res_flash == FR_OK)
  {
    // LED_GREEN;
    printf("》打开文件成功。\r\n");
    res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
      printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("！！文件读取失败：(%d)\n",res_flash);
    }		
  }
  else
  {
    // LED_RED;
    printf("！！打开文件失败。\r\n");
  }
  /* 不再读写，关闭文件 */
  f_close(&fnew);	
  
  /* 不再使用文件系统，取消挂载文件系统 */
//   f_mount(NULL,"1:",1);
  
  /* 操作完成，停机 */
    printf("操作完成，停机\r\n");
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

        FATFS fs_defualt;													/* FatFs文件系统对象 */
        FIL f_default;													/* 文件对象 */
        FRESULT r_default;                /* 文件操作结果 */
        UINT f_read_count;            					  /* 文件成功读写数量 */
        char f_scan_path[100];                  /* 保存当前扫描路径 */

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
            FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
            FILINFO fno; 
            DIR dir; 
            int i;
            uint8_t j = 1;
            char *fn;        // 文件名	
            
        #if _USE_LFN 
            /* 长文件名支持 */
            /* 简体中文需要2个字节保存一个“字”*/
            static char lfn[_MAX_LFN*2 + 1]; 	
            fno.lfname = lfn; 
            fno.lfsize = sizeof(lfn); 
        #endif 
            //打开目录
            res = f_opendir(&dir, path); 
            printf("\r\nthe list of files is as follow: \r\n>>>\r\n");
            if (res == FR_OK) 
            { 
                i = strlen(path); 
                for (;;) 
                { 
                    //读取目录下的内容，再读会自动读下一个文件
                    res = f_readdir(&dir, &fno); 								
                    //为空时表示所有项目读取完毕，跳出
                    if (res != FR_OK || fno.fname[0] == 0) break; 	
        #if _USE_LFN 
                    fn = *fno.lfname ? fno.lfname : fno.fname; 
        #else 
                    fn = fno.fname; 
        #endif 
                    //点表示当前目录，跳过			
                    if (*fn == '.') continue; 	
                    //目录，递归读取      
                    if (fno.fattrib & AM_DIR)         
                    { 			
                        //合成完整目录名        
                        sprintf(&path[i], "/%s", fn); 		
                        //递归遍历         
                        res = scan_files(path);	
                        path[i] = 0;         
                        //打开失败，跳出循环        
                        if (res != FR_OK) 
                            break; 
                    } 
                    else 
                    { 
                        printf(" [%d]`%s/%s`, ", j++, path, fn);								//输出文件名	
                        /* 可以在这里提取特定格式的文件路径 */        
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

            // 1.预先关闭 (保险)
            // f_close(&f_default);

            // 2.打开文件
            r_default = f_open(&f_default, path, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
            if(FR_OK == r_default){

            // 3.获取文件大小
                file_size = f_size(&f_default);
                printf("[%s] size: %d.\r\n", path, file_size);
                #ifdef USE_ATK_MALLOC
                f_read_buffer = (void*)mymalloc(SRAMIN, file_size);
                printf("used percentage after malloc: %3d%%.\r\n", my_mem_perused(SRAMIN));
                #endif

            // 4.定位到文件起始位置
                r_default = f_lseek(&f_default, 0);
                if(FR_OK == r_default){

            // 5.预先格式化 (保险)
                    #ifndef USE_ATK_MALLOC
                    memset(f_read_buffer, 0, sizeof(f_read_buffer));
                    #endif

            // 6.读取所有内容到缓存区
                    r_default = f_read(&f_default, f_read_buffer, file_size, &f_read_count);
                    if(FR_OK == r_default){

            // 7.回显    
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

            // 8.完成后关闭
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
            
            // 1.关闭文件 (保险)
            if(fast_mode != 3){
                f_close(&f_default);
            }

            // 2.打开文件
            r_default = f_open(&f_default, path, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
            if(FR_OK == r_default){
                
            // 3.定位写入位置
                if(write_position < -1){
                    /* 如果<-1则是非法地址 */
                    r_default = FR_INVALID_PARAMETER;
                    printf("the write position is invalid.\r\n");
                }else if(write_position == -1){
                    /* 如果==-1则定位到尾部 */
                    r_default = f_lseek(&f_default, f_size(&f_default));
                }else{
                    /* 如果!=-1则定位到指定位置 */
                    r_default = f_lseek(&f_default, write_position);
                }

            // 4.将write_buffer写入
                if(FR_OK == r_default){
                    f_printf(&f_default, write_buffer);
                    if(FR_OK == r_default){
                        if(fast_mode == 3){
                            return;
                        }
                        f_close(&f_default);
                        printf("write is succeed.\r\n");
            // 5.回显    
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

            // 6.完成后关闭
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
            // 1.关闭文件 (保险)
            f_close(&f_default);

            // 2.删除文件
            printf("try to delete [%s].\r\n", path);
            r_default = f_unlink(path);
            if(FR_OK == r_default){
                printf("succeed to delete [%s].\r\n", path);

            // 3.扫描检查
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
            
            // 4.完成后关闭
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
            // 1.初始化
            FATFS_Init(0);

            // 2.扫描文件
            FATFS_Scan();

            // 3.删除文件并扫描
            char my_path[] = "1:/TestDir/testfile.txt";
            FATFS_Delete(my_path, 1, 0);

            // 4.读取文件并回显
            FATFS_Read(my_path, 1);

            // 5.写入文件并回显
            FATFS_Write(my_path, Index_Text_Default, -1, 1, 0);

            // 6.卸载
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

            // 1.读取索引文件
            int temp_size = FATFS_Read(index_check_path, 0);

            // 2.如果文件不为空则判断索引是否有效
            if(temp_size > 0){
                get_last_line_from_file(index_check_path, temp_buffer, index_check_buffer_size);
                /* 索引是否>=10001 */
                is_write_next_index = is_number_greater_than_or_equal_to_target(temp_buffer, 10001);
            }

            // 3.写入当前索引
            if(is_append){
                if(is_write_next_index){
                    /* 追加下一个索引 */
                    long temp_num = strtol(temp_buffer, NULL, 10);
                    FATFS_Index_Current = temp_num + 1;
                    sprintf(temp_to_write, "\r\n%d", FATFS_Index_Current);
                    FATFS_Write(index_check_path, temp_to_write, -1, 0, 0);
                }else{
                    /* 不需要追加索引, 直接写入默认索引10001 */
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
            printf("[%s][%d]: FATFS_Convert_Array暂时不可用\r\n", __FILE__, __LINE__);
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

        float sensor_storage_data[MAX_ROWS][MAX_COLS];  // 存放解析后的数据

        void convert_text_to_array(const char *input) {
            char buffer[MAX_BUF];  
            strncpy(buffer, input, MAX_BUF - 1); // 复制字符串，避免修改原始数据
            buffer[MAX_BUF - 1] = '\0'; // 确保字符串以 `\0` 结尾

            char *line, *token;
            char *saveptr_row, *saveptr_col; // 用于 strtok_r 的保存指针
            int row = 0, col;
            
            line = strtok_r(buffer, "\n", &saveptr_row); // 按行分割
            while (line && row < MAX_ROWS) {
                col = 0;
                token = strtok_r(line, ",", &saveptr_col); // 按 `,` 分割列
                while (token && col < MAX_COLS) {
                    sensor_storage_data[row][col] = atof(token); // 转换为 float 并存入数组
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
            // FIL f_default;         // 定义一个文件对象，用于操作文件

            FRESULT iresult;      // 存储 FatFs 操作的结果
            int ichar;           // 用于存储读取的单个字符
            size_t iposition = 0;   // 缓冲区的当前位置
            UINT ibytes_read;  // 实际读取的字节数


            iresult = f_open(&f_default, ipath, FA_OPEN_EXISTING | FA_READ);
            if (iresult != FR_OK) {
                // 如果打开文件失败，直接返回错误结果
                return iresult;
            }

            // 将文件指针定位到文件末尾，f_size(&f_default) 返回文件的大小
            iresult = f_lseek(&f_default, f_size(&f_default));
            if (iresult != FR_OK) {
                // 如果定位失败，关闭文件并返回错误结果
                f_close(&f_default);
                return iresult;
            }

            // 反向逐字节读取文件，直到遇到换行符或文件开头
            while (f_tell(&f_default) > 0) {
                // 将文件指针向前移动一个字节
                // 当前位置: n
                iresult = f_lseek(&f_default, f_tell(&f_default) - 1);
                if (iresult != FR_OK) {
                    // 如果移动指针失败，关闭文件并返回错误结果
                    f_close(&f_default);
                    return iresult;
                }
                // 读取一个字节的数据
                // 当前位置: n-1
                char ichar;
                iresult = f_read(&f_default, &ichar, 1, &ibytes_read);
                if (iresult != FR_OK || ibytes_read == 0) {
                    // 如果读取失败或到达文件开头，关闭文件并返回错误结果
                    f_close(&f_default);
                    return iresult;
                }
                if (ichar == '\n') {
                    // 如果遇到换行符，跳出循环
                    break;
                }
                // 将文件指针再向前移动一个字节，继续反向读取
                // 当前位置: n
                iresult = f_lseek(&f_default, f_tell(&f_default) - 1);
                if (iresult != FR_OK) {
                    // 如果移动指针失败，关闭文件并返回错误结果
                    f_close(&f_default);
                    return iresult;
                }

                // 当前位置: n-1
                // (下次运行循环的时候: n-1)
            }

            // 读取最后一行内容
            while (iposition < ibuffer_size - 1) {
                // 读取一个字节的数据
                iresult = f_read(&f_default, &ichar, 1, &ibytes_read);
                if (iresult != FR_OK || ibytes_read == 0 || ichar == '\n') {
                    // 如果读取失败、到达文件末尾或遇到换行符，跳出循环
                    break;
                }
                // 将读取的字符存入缓冲区
                ibuffer[iposition++] = (char)ichar;
            }
            // 在缓冲区末尾添加字符串结束符
            ibuffer[iposition] = '\0';

            // 关闭文件
            f_close(&f_default);
            // 返回操作成功的结果
            return iresult;
        }

        #else

        FRESULT get_last_line_from_file(const char *ipath, char *ibuffer, size_t ibuffer_size) {
            FIL ifile;         // 定义一个文件对象，用于操作文件

            FRESULT iresult;      // 存储 FatFs 操作的结果
            int ichar;           // 用于存储读取的单个字符
            size_t iposition = 0;   // 缓冲区的当前位置
            UINT ibytes_read;  // 实际读取的字节数


            iresult = f_open(&ifile, ipath, FA_OPEN_EXISTING | FA_READ);
            if (iresult != FR_OK) {
                // 如果打开文件失败，直接返回错误结果
                return iresult;
            }

            // 将文件指针定位到文件末尾，f_size(&ifile) 返回文件的大小
            iresult = f_lseek(&ifile, f_size(&ifile));
            if (iresult != FR_OK) {
                // 如果定位失败，关闭文件并返回错误结果
                f_close(&ifile);
                return iresult;
            }

            // 反向逐字节读取文件，直到遇到换行符或文件开头
            while (f_tell(&ifile) > 0) {
                // 将文件指针向前移动一个字节
                // 当前位置: n
                iresult = f_lseek(&ifile, f_tell(&ifile) - 1);
                if (iresult != FR_OK) {
                    // 如果移动指针失败，关闭文件并返回错误结果
                    f_close(&ifile);
                    return iresult;
                }
                // 读取一个字节的数据
                // 当前位置: n-1
                char ichar;
                iresult = f_read(&ifile, &ichar, 1, &ibytes_read);
                if (iresult != FR_OK || ibytes_read == 0) {
                    // 如果读取失败或到达文件开头，关闭文件并返回错误结果
                    f_close(&ifile);
                    return iresult;
                }
                if (ichar == '\n') {
                    // 如果遇到换行符，跳出循环
                    break;
                }
                // 将文件指针再向前移动一个字节，继续反向读取
                // 当前位置: n
                iresult = f_lseek(&ifile, f_tell(&ifile) - 1);
                if (iresult != FR_OK) {
                    // 如果移动指针失败，关闭文件并返回错误结果
                    f_close(&ifile);
                    return iresult;
                }

                // 当前位置: n-1
                // (下次运行循环的时候: n-1)
            }

            // 读取最后一行内容
            while (iposition < ibuffer_size - 1) {
                // 读取一个字节的数据
                iresult = f_read(&ifile, &ichar, 1, &ibytes_read);
                if (iresult != FR_OK || ibytes_read == 0 || ichar == '\n') {
                    // 如果读取失败、到达文件末尾或遇到换行符，跳出循环
                    break;
                }
                // 将读取的字符存入缓冲区
                ibuffer[iposition++] = (char)ichar;
            }
            // 在缓冲区末尾添加字符串结束符
            ibuffer[iposition] = '\0';

            // 关闭文件
            f_close(&ifile);
            // 返回操作成功的结果
            return iresult;
        }

        #endif

    #endif
    // ohter


#endif
// fatfs functions




