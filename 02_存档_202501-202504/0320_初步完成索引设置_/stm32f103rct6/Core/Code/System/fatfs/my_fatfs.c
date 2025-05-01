#include "my_fatfs.h"


FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
char fpath[100];                  /* 保存当前扫描路径 */
char readbuffer[512];    
char default_write_buffer[] = "\r\n100001";
uint8_t FATFS_Index_Current = 0;

FATFS_Result_Struct FATFS_Result_a = {
    .File_Size = -1,
};

// head
#if 1
/* FatFs多项功能测试 */
static FRESULT miscellaneous(void)
{
    DIR dir;
    FATFS *pfs;
    DWORD fre_clust, fre_sect, tot_sect;
    
    printf("\r\n*************** 设备信息获取 ***************\r\n");
    /* 获取设备信息和空簇大小 */
    res_flash = f_getfree("1:", &fre_clust, &pfs);

    /* 计算得到总的扇区个数和空扇区个数 */
    tot_sect = (pfs->n_fatent - 2) * pfs->csize;
    fre_sect = fre_clust * pfs->csize;

    /* 打印信息(4096 字节/扇区) */
    printf("\r\n》设备总空间：%10lu KB。\n》可用空间：  %10lu KB。\n", tot_sect *4, fre_sect *4);
    
    printf("\r\n******** 文件定位和格式化写入功能测试 ********\r\n");
    res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",
                                                        FA_OPEN_ALWAYS|FA_WRITE|FA_READ );
    if ( res_flash == FR_OK )
    {
        /*  文件定位 */
        res_flash = f_lseek(&fnew,f_size(&fnew));
        if (res_flash == FR_OK)
        {
            /* 格式化写入，参数格式类似printf函数 */
            f_printf(&fnew,"\r\n在原来文件新添加一行内容\n");
            f_printf(&fnew,"\r\n》设备总空间：%10lu KB。\n》可用空间：  %10lu KB。\n", tot_sect *4, fre_sect *4);
            /*  文件定位到文件起始位置 */
            res_flash = f_lseek(&fnew,0);
            /* 读取文件所有内容到缓存区 */
            res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum);
            if(res_flash == FR_OK)
            {
                printf("》文件内容：\n%s\n",readbuffer);
            }
        }
        f_close(&fnew);    
        
        printf("\r\n********** 目录创建和重命名功能测试 **********\r\n");
        /* 尝试打开目录 */
        res_flash=f_opendir(&dir,"1:TestDir");
        if(res_flash!=FR_OK)
        {
            /* 打开目录失败，就创建目录 */
            res_flash=f_mkdir("1:TestDir");
        }
        else
        {
            /* 如果目录已经存在，关闭它 */
            res_flash=f_closedir(&dir);
            /* 删除文件 */
            f_unlink("1:TestDir/testdir.txt");
        }
        if(res_flash==FR_OK)
        {
            /* 重命名并移动文件 */
            res_flash=f_rename("1:FatFs读写测试文件.txt","1:TestDir/testdir.txt");      
        } 
    }
    else
    {
        printf("\r\n!! 打开文件失败：%d\n",res_flash);
        printf("\r\n!! 或许需要再次运行“FatFs移植与读写测试”工程\n");
    }
    return res_flash;
}

    FILINFO fno;
static FRESULT file_check(void)
{

    
    /* 获取文件信息 */
    res_flash=f_stat("1:TestDir/testdir.txt",&fno);
    if(res_flash==FR_OK)
    {
        printf("\r\n“testdir.txt”文件信息：\n");
        printf("\r\n》文件大小: %ld(字节)\n", fno.fsize);
        printf("\r\n》时间戳: %u/%02u/%02u, %02u:%02u\n",
                     (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,fno.ftime >> 11, fno.ftime >> 5 & 63);
        printf("\r\n》属性: %c%c%c%c%c\n\n",
                     (fno.fattrib & AM_DIR) ? 'D' : '-',      // 是一个目录
                     (fno.fattrib & AM_RDO) ? 'R' : '-',      // 只读文件
                     (fno.fattrib & AM_HID) ? 'H' : '-',      // 隐藏文件
                     (fno.fattrib & AM_SYS) ? 'S' : '-',      // 系统文件
                     (fno.fattrib & AM_ARC) ? 'A' : '-');     // 档案文件
    }
    return res_flash;
}

static FRESULT scan_files (char* path) 
{ 
    FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
    FILINFO fno; 
    DIR dir; 
    int i;            
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
                printf("%s/%s\r\n", path, fn);								//输出文件名	
                /* 可以在这里提取特定格式的文件路径 */        
            }//else
        } //for
    } 
    return res; 
}
#endif

// base
#if 1
FATFS_Result_Struct FATFS_Read(char* path){
    FATFS_Result_Struct temp_FATFS_Result = FATFS_Result_a;
    f_close(&fnew);
    res_flash = f_open(&fnew, path, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
    if(FR_OK == res_flash){

        /*  文件定位到文件起始位置 */
        temp_FATFS_Result.File_Size = f_size(&fnew);
        res_flash = f_lseek(&fnew,0);
        if(FR_OK == res_flash){
            
            /* 读取文件所有内容到缓存区 */
            res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum);
            if(FR_OK == res_flash){
                printf("file content are as follow: \r\n>>>\r\n%s", readbuffer);
                printf("\r\n<<<\r\n");
            }else{
                printf("fail to f_read [%s]\r\n", &path);
            }

        }else{
            printf("fail to f_lseek [%s]\r\n", &path);
        }

    }else{
        printf("fail to f_open [%s]\r\n", &path);
    }

    f_close(&fnew);
    return temp_FATFS_Result;
}

/**
 * @brief  Write someting to file.
 * @param  path: Path of file.
 * @param  write_buffer: Buffer to write.
 * @param  write_position: Position of the file to write.
 *         @arg  -1: Tail address
 *         @arg   0: Head address
 *         @arg ...: Specified address
 * @retval None
 */
void FATFS_Write(char* path, char* write_buffer, int write_position){
    f_close(&fnew);
    
    res_flash = f_open(&fnew, path, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
    if(FR_OK == res_flash){
        
        /* 文件定位到write_position位置 */
        if(write_position < -1){
            // 如果<-1则是非法地址
            res_flash = FR_INVALID_PARAMETER;
        }else if(write_position == -1){
            // 如果==-1则定位到尾部
            res_flash = f_lseek(&fnew, f_size(&fnew));
        }else{
            // 如果!==-1则定位到指定位置
            res_flash = f_lseek(&fnew, write_position);
        }
        
        if(FR_OK == res_flash){
            
            /* 将write_buffer写入 */
            f_printf(&fnew, write_buffer);
            if(FR_OK == res_flash){
                f_close(&fnew);
                printf("write is succeed, now read it ->\r\n");
                FATFS_Read(path);
            }else{
                printf("fail to f_read [%s]\r\n", &path);
            }

        }else{
            printf("fail to f_lseek [%s]\r\n", &path);
        }

    }else{
        printf("fail to f_open [%s]\r\n", &path);
    }
}

void FATFS_Delete(char* path){
    f_close(&fnew);
    printf("try to delete %s\r\n", path);

    res_flash = f_unlink(path);
    if(FR_OK == res_flash){
        printf("succeed to delete %s, now scan files again\r\n", path);
        // FATFS_Scan();
    }else{
        printf("fail to delete %s\r\n", path);
        while(1){
            // pause
        }
    }
}
#endif

// function
#if 1
void FATFS_Init(void){
    printf("my fatfs test.\r\n");

    res_flash = f_mount(&fs,"1:",1);
    if(FR_OK == res_flash){
        printf("succeed to mount fatfs.\r\n");
    }else{
        printf("fail to mount fatfs.\r\n");
        while(1){
            // pause
        }
    }
}

void FATFS_Deinit(void){
    /* 不再使用文件系统，取消挂载文件系统 */
    res_flash = f_mount(NULL,"1:",1);
    if(FR_OK == res_flash){
        printf("succeed to unmount fatfs.\r\n");
    }else{
        printf("fail to unmount fatfs.\r\n");
        while(1){
            // pause
        }
    }
}

void FATFS_Scan(void){
    printf("scaning files...\r\n");
    strcpy(fpath,"1:");
    scan_files(fpath);
}

void FATFS_Muti_Test(void){
    FATFS_Init();

    // char my_path[] = "1:新建 AutoHotkey Script.ahk";
    char my_path[] = "1:/TestDir/testdir.txt";
    FATFS_Scan();
    FATFS_Delete(my_path);
    FATFS_Read(my_path);
    FATFS_Write(my_path, default_write_buffer, -1);

    FATFS_Deinit();
    while(1){
        // pause
    }
}

#define index_check_path "1:/my_index.csv"
#define index_check_buffer_size 20
FATFS_Result_Struct FATFS_Index_Check(void){
    FATFS_Result_Struct temp_FATFS_Result = FATFS_Result_a;
    char temp_buffer[index_check_buffer_size];
    char temp_to_write[5];

    temp_FATFS_Result = FATFS_Read(index_check_path);
    printf("file size is %d\r\n", temp_FATFS_Result.File_Size);

    int is_write_next_index = 0;
    // 1.判断文件是否为空(不为空则继续)
    if(temp_FATFS_Result.File_Size > 0){
        get_last_line_from_file(index_check_path, temp_buffer, index_check_buffer_size);
        printf("get last line from file:\r\n>>>\r\n%s\r\n<<<\r\n", temp_buffer);
        // 2.判断最后一行的内容是否大于等于10001, 如果不是则写入默认索引:10001
        is_write_next_index = is_number_greater_than_or_equal_to_target(temp_buffer, 10001);
    }

    if(is_write_next_index){
        // 3.1 写入下一个索引, 此时strtol(temp_buffer)+1为下一次的索引
        FATFS_Index_Current = atoi(temp_buffer)+1;
        sprintf(temp_to_write, "\r\n%d", FATFS_Index_Current);
        FATFS_Write(index_check_path, temp_to_write, -1);
    }else{
        // 3.2 不需要追加索引, 直接写入默认索引default_write_buffer
        FATFS_Index_Current = 10001;
        FATFS_Write(index_check_path, default_write_buffer, -1);
    }

    return temp_FATFS_Result;
}


#endif


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROWS 100
#define MAX_COLS 20
#define MAX_BUF 1024

float data[MAX_ROWS][MAX_COLS];  // 存放解析后的数据

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
            data[row][col] = atof(token); // 转换为 float 并存入数组
            col++;
            token = strtok_r(NULL, ",", &saveptr_col);
        }
        row++;
        line = strtok_r(NULL, "\n", &saveptr_row);
    }
}

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
