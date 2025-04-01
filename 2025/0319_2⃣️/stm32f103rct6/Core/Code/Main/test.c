#include "test.h"

#if !0314

// head
#if 1
#include "ff.h"

FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
char fpath[100];                  /* 保存当前扫描路径 */
char readbuffer[512];    

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

void my_fatfs_init(void){
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

void my_fatfs_deinit(void){
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

void my_read(char* path){
    f_close(&fnew);
    res_flash = f_open(&fnew, path, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
    if(FR_OK == res_flash){

        /*  文件定位到文件起始位置 */
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
}

void my_write(char* path){
    f_close(&fnew);
    char str2write[] = "1,-2,3.1\n";
    res_flash = f_open(&fnew, path, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
    if(FR_OK == res_flash){
        
        /*  文件定位到文件起始位置 */
        res_flash = f_lseek(&fnew,0);
        if(FR_OK == res_flash){
            
            /* 读取文件所有内容到缓存区 */
            f_printf(&fnew, str2write);
            // f_printf(&fnew, "4,-5,6.2\n");
            if(FR_OK == res_flash){
                f_close(&fnew);
                printf("write is succeed, now read it ->\r\n");
                my_read(path);
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

void my_scan(void){
    printf("scaning files...\r\n");
    strcpy(fpath,"1:");
    scan_files(fpath);
}

void my_delete(char* path){
    f_close(&fnew);
    printf("try to delete %s\r\n", path);

    res_flash = f_unlink(path);
    if(FR_OK == res_flash){
        printf("succeed to delete %s, now scan files again\r\n", path);
        my_scan();
    }else{
        printf("fail to delete %s\r\n", path);
        while(1){
            // pause
        }
    }
}

void my_fatfs_muti_test(void){
    my_fatfs_init();

    // char my_path[] = "1:新建 AutoHotkey Script.ahk";
    char my_path[] = "1:/TestDir/testdir.txt";
    my_scan();
    my_delete(my_path);
    my_read(my_path);
    my_write(my_path);

    my_fatfs_deinit();
    while(1){
        // pause
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 10    // 最大行数
#define MAX_COLS 5     // 最大列数
#define MAX_BUF 4096   // 缓冲区大小

float data[MAX_ROWS][MAX_COLS];  // 存放解析后的数据

void parse_csv(const char *input) {
    char buffer[MAX_BUF];  
    strncpy(buffer, input, MAX_BUF - 1); // 复制字符串，避免修改原始数据
    buffer[MAX_BUF - 1] = '\0'; // 确保字符串以 `\0` 结尾

    char *line, *token;
    int row = 0, col;
    
    line = strtok(buffer, "\n"); // 按行分割
    while (line && row < MAX_ROWS) {
        col = 0;
        token = strtok(line, ","); // 按 `,` 分割列
        while (token && col < MAX_COLS) {
            data[row][col] = atof(token); // 转换为 float 并存入数组
            col++;
            token = strtok(NULL, ",");
        }
        row++;
        line = strtok(NULL, "\n");
    }
}




#define my_fatfs_test
#ifndef my_fatfs_test
void my_test(void){
    /* 配置系统时钟为72 MHz */ 
    /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
    
     printf("\r\n******** 这是一个SPI FLASH 文件系统实验 *******\r\n");
    
    //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
    res_flash = f_mount(&fs,"1:",1);
    if(res_flash!=FR_OK)
    {
        printf("\r\n！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
        printf("\r\n！！可能原因：SPI Flash初始化不成功。\r\n");
        while(1);
    }
    else
    {
        printf("\r\n》文件系统挂载成功，可以进行测试\r\n");    
    }
    
    /* FatFs多项功能测试 */
    res_flash = miscellaneous();

    
    printf("\r\n*************** 文件信息获取测试 **************\r\n");
    res_flash = file_check();

    
    printf("\r\n***************** 文件扫描测试 ****************\r\n");
    strcpy(fpath,"1:");
    scan_files(fpath);
    
    
    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,"1:",1);
    
    /* 操作完成，停机 */
    while(1)
    {
    }

}
#else
void my_test(void){
    // my_fatfs_init();

    // char my_path[] = "1:/TestDir/testdir.txt";
    // my_scan();
    // my_read(my_path);

    // my_fatfs_deinit();
    
    // 示例字符串，每行是数据，每列用 `,` 分隔
    const char *csv_data = "1.1,2.2,3.3\n4.4,5.5,6.6\n7.7,8.8,9.9";

    parse_csv(csv_data);
    float result = 0;
    // 输出结果
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            printf("%f ", data[i][j]);
            // if(i == MAX_ROWS && j == MAX_COLS){
            //     printf("sum: %f, average: %f", result, (result / ()))
            // }
        }
        printf("\r\n");
    }

    while(1){
        // pause
        // printf("111");
        // HAL_Delay(1000);
    }
}
#endif

// #endif

#if 0319

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROWS 100
#define MAX_COLS 100
#define MAX_BUF 1024

float data[MAX_ROWS][MAX_COLS];  // 存放解析后的数据

void parse_csv(const char *input) {
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

int my_test() {
    const char *csv_data = "1.1,2.2,3.3\n4.4,5.5,6.6\n7.7,8.8,9.9";
    parse_csv(csv_data);

    // 打印解析后的数据
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", data[i][j]);
        }
        printf("\n");
    }

    return 0;
}

#endif
