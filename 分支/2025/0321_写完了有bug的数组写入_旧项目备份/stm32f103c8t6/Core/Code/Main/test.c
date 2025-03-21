#include "test.h"

#if !0210
#define MAX_STR_LEN 1024  // 结果字符串最大长度

// 拼接多个数组的元素
// char* concat_arrays(int num_arrays, const char* delimiter, const char* format, ...) {
char* concat_arrays(int num_arrays, const char* delimiter, const char* format, ...) {
        static char result[MAX_STR_LEN];  // 静态缓冲区存储结果
        result[0] = '\0';  // 初始化为空字符串

        va_list args;
        va_start(args, format);

        char buffer[32];  // 临时存储格式化后的数据
        int first = 1;    // 用于避免首个元素前的分隔符

        for (int i = 0; i < num_arrays; i++) {
                int* array = va_arg(args, int*);  // 获取数组指针
                int size = va_arg(args, int);     // 获取数组大小

                for (int j = 0; j < size; j++) {
                        if (!first) {
                                strcat(result, delimiter);  // 添加分隔符
                        }
                        first = 0;
                        snprintf(buffer, sizeof(buffer), format, array[j]);  // 根据传入的格式字符串格式化整数
                        strcat(result, buffer);  // 追加到结果字符串
                }
        }

        strcat(result, "\r\n");  // 追加到结果字符串
        va_end(args);
        return result;
}
#endif

#if !0307
float angles[3];
volatile float q0=1, q1=0, q2=0, q3=0; 

void get_euler(void);
void get_guater(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
float invSqrt(float x);

void get_euler(void){
        
        int i;
        float gf[3], af[3], mf[3];
        printf("[%s]: %d\r\n", __FILE__, __LINE__);
        QMC5883_Update_Data();
        LSM6DS3_Update_Data();
        // for(i = 0; i < 3; i++ ){        gf[i] = (float) lsm6ds3_gyro[i] /2 *2;printf("[%s]: %d\r\n", __FILE__, __LINE__);    }
        // for(i = 0; i < 3; i++ ){        gf[i] = (float) lsm6ds3_gyro[i] /2 *2;printf("%f,\r\n", gf[i]);    }
        for(i = 0; i < 3; i++ ){        gf[i] = (float) lsm6ds3_gyro[i] /2 *2;    }
        for(i = 0; i < 3; i++ ){        af[i] = (float) lsm6ds3_accel[i] /2 *2;    }
        for(i = 0; i < 3; i++ ){        mf[i] = (float) qmc5883_mag[i] /2 *2;    }

        get_guater(
                gf[0], gf[1], gf[2],
                af[0], af[1], af[2], 
                mf[0], mf[1], mf[2]
        );
        // printf("%f,%f,%f,%f\r\n", q1, q2, q3, q0);

        angles[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
        angles[2] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
        angles[0] = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3;   // yaw
        printf("%f,%f,%f\r\n", angles[0], angles[1], angles[2]);

}

#define Kp 4.50f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 1.0f    // integral gain governs rate of convergence of gyroscope biases

void get_guater(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz){
        
    float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float exInt = 0.0, eyInt = 0.0, ezInt = 0.0;
    float ex, ey, ez, halfT = 0.024f;

    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;   
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;          

/* 归一化 */
    /* 求模 */
    norm = invSqrt(ax * ax + ay * ay + az * az);       
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    /* 求模 */
    norm = invSqrt(mx * mx + my * my + mz * mz);          
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;

        printf("%f\r\n", norm);
        printf("%f\r\n", norm);
        printf("%f\r\n", norm);
/* 磁场水平 垂直分量计算 */
    /* 旋转矩阵 */
    hx = 2 * mx * (0.5f - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
    hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5f - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
    hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5f - q1q1 - q2q2);         
    /* 计算水平和垂直分量 */
    bx = sqrt((hx * hx) + (hy * hy));
    bz = hz;     

/* 将重力从物体坐标系转换到世界坐标系 */
    /* 计算分量 */
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

/* 将磁力从物体坐标系转换到世界坐标系 */
    /* 计算分量 */
    wx = 2 * bx * (0.5 - q2q2 - q3q3) + 2 * bz * (q1q3 - q0q2);
    wy = 2 * bx * (q1q2 - q0q3) + 2 * bz * (q0q1 + q2q3);
    wz = 2 * bx * (q0q2 + q1q3) + 2 * bz * (0.5 - q1q1 - q2q2);  

/* 计算误差 叉积*/
    /* 通过加速计观测值 世界磁力计 世界重力 */
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

    if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
/* 积分 累加误差 Ki*/
        exInt = exInt + ex * Ki * halfT;
        eyInt = eyInt + ey * Ki * halfT;	
        ezInt = ezInt + ez * Ki * halfT;

/* 消除误差 */
        gx = gx + Kp * ex + exInt;
        gy = gy + Kp * ey + eyInt;
        gz = gz + Kp * ez + ezInt;
    }

        printf("%f,%f\r\n", gx, gy);
        printf("%f,%f\r\n", q0, q1);
        printf("[%s]: %d\r\n", __FILE__, __LINE__);
        
/* 更新四元数 */
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
        printf("(%f * %f - %f * %f - %f * %f) * %f;\r\n", -q1 , gx , q2 , gy , q3 , gz , halfT);
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;  

    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;

}

float invSqrt(float x){

    float halfx = 0.5f * x;
    float y = x;
    
    long i = *(long*)&y;                //get bits for floating value
    i = 0x5f3759df - (i >> 1);          //gives initial guss you
    y = *(float*)&i;                    //convert bits back to float
    y = y * (1.5f - (halfx * y * y));   //newtop step, repeating increases accuracy
    
    return y;
}

int my_test() {

        printf("[%s]: %d\r\n", __FILE__, __LINE__);
        get_euler();
        HAL_Delay(500);
        return 0;
}

#endif

#if !0314

// head
#if 1
void Delay(__IO uint32_t nCount);
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

     
/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\nhttp://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];
//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
#endif

void my_test(void){   
    
    printf("\r\n这是一个8Mbyte串行flash(W25Q64)实验(SPI驱动) \r\n");
    
    /* 16M串行flash W25Q64初始化 */
    
        /* 获取 Flash Device ID */
    DeviceID = W25Q64_Read_Device_ID();
    
    Delay( 200 );
    
    /* 获取 SPI Flash ID */
    FlashID = W25Q64_Read_JEDEC_ID();
    
    printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
    
    /* 检验 SPI Flash ID */
    if (FlashID == sFLASH_ID) 
    {	
        printf("\r\n检测到SPI FLASH W25Q64 !\r\n");
        
        /* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
        W25Q64_Erase_Sector(FLASH_SectorToErase);	 	 

        /* 将发送缓冲区的数据写到flash中 */
        SPI_APP_Buffer_Write(SPI_Class_1, Tx_Buffer, BufferSize, FLASH_WriteAddress);

        printf("\r\n写入的数据为：\r\n%s", Tx_Buffer);
        
        /* 将刚刚写入的数据读出来放到接收缓冲区中 */
        SPI_APP_Buffer_Read(SPI_Class_1, Rx_Buffer, BufferSize, FLASH_ReadAddress);
        printf("\r\n读出的数据为：\r\n%s", Rx_Buffer);
        
        /* 检查写入的数据与读出的数据是否相等 */
        TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
        
        if( PASSED == TransferStatus1 )
        {    
            // LED_GREEN;
            printf("\r\n16M串行flash(W25Q64)测试成功!\n\r");
        }
        else
        {        
            // LED_RED;
            printf("\r\n16M串行flash(W25Q64)测试失败!\n\r");
        }
    }// if (FlashID == sFLASH_ID)
    else
    {    
        // LED_RED;
        printf("\r\n获取不到 W25Q64 ID!\n\r");
    }
    
    W25Q64_PowerDown();  
    while(1);  
}

TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }

        pBuffer1++;
        pBuffer2++;
    }
    return PASSED;
}

void Delay(__IO uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}


#endif

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

#endif


