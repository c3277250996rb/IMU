#include "test.h"

#if !314

// head
#if 1
#include "ff.h"

FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
char fpath[100];                  /* ���浱ǰɨ��·�� */
char readbuffer[512];    

/* FatFs����ܲ��� */
static FRESULT miscellaneous(void)
{
    DIR dir;
    FATFS *pfs;
    DWORD fre_clust, fre_sect, tot_sect;
    
    printf("\r\n*************** �豸��Ϣ��ȡ ***************\r\n");
    /* ��ȡ�豸��Ϣ�Ϳմش�С */
    res_flash = f_getfree("1:", &fre_clust, &pfs);

    /* ����õ��ܵ����������Ϳ��������� */
    tot_sect = (pfs->n_fatent - 2) * pfs->csize;
    fre_sect = fre_clust * pfs->csize;

    /* ��ӡ��Ϣ(4096 �ֽ�/����) */
    printf("\r\n���豸�ܿռ䣺%10lu KB��\n�����ÿռ䣺  %10lu KB��\n", tot_sect *4, fre_sect *4);
    
    printf("\r\n******** �ļ���λ�͸�ʽ��д�빦�ܲ��� ********\r\n");
    res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",
                                                        FA_OPEN_ALWAYS|FA_WRITE|FA_READ );
    if ( res_flash == FR_OK )
    {
        /*  �ļ���λ */
        res_flash = f_lseek(&fnew,f_size(&fnew));
        if (res_flash == FR_OK)
        {
            /* ��ʽ��д�룬������ʽ����printf���� */
            f_printf(&fnew,"\r\n��ԭ���ļ������һ������\n");
            f_printf(&fnew,"\r\n���豸�ܿռ䣺%10lu KB��\n�����ÿռ䣺  %10lu KB��\n", tot_sect *4, fre_sect *4);
            /*  �ļ���λ���ļ���ʼλ�� */
            res_flash = f_lseek(&fnew,0);
            /* ��ȡ�ļ��������ݵ������� */
            res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum);
            if(res_flash == FR_OK)
            {
                printf("���ļ����ݣ�\n%s\n",readbuffer);
            }
        }
        f_close(&fnew);    
        
        printf("\r\n********** Ŀ¼���������������ܲ��� **********\r\n");
        /* ���Դ�Ŀ¼ */
        res_flash=f_opendir(&dir,"1:TestDir");
        if(res_flash!=FR_OK)
        {
            /* ��Ŀ¼ʧ�ܣ��ʹ���Ŀ¼ */
            res_flash=f_mkdir("1:TestDir");
        }
        else
        {
            /* ���Ŀ¼�Ѿ����ڣ��ر��� */
            res_flash=f_closedir(&dir);
            /* ɾ���ļ� */
            f_unlink("1:TestDir/testdir.txt");
        }
        if(res_flash==FR_OK)
        {
            /* ���������ƶ��ļ� */
            res_flash=f_rename("1:FatFs��д�����ļ�.txt","1:TestDir/testdir.txt");      
        } 
    }
    else
    {
        printf("\r\n!! ���ļ�ʧ�ܣ�%d\n",res_flash);
        printf("\r\n!! ������Ҫ�ٴ����С�FatFs��ֲ���д���ԡ�����\n");
    }
    return res_flash;
}

    FILINFO fno;
static FRESULT file_check(void)
{

    
    /* ��ȡ�ļ���Ϣ */
    res_flash=f_stat("1:TestDir/testdir.txt",&fno);
    if(res_flash==FR_OK)
    {
        printf("\r\n��testdir.txt���ļ���Ϣ��\n");
        printf("\r\n���ļ���С: %ld(�ֽ�)\n", fno.fsize);
        printf("\r\n��ʱ���: %u/%02u/%02u, %02u:%02u\n",
                     (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,fno.ftime >> 11, fno.ftime >> 5 & 63);
        printf("\r\n������: %c%c%c%c%c\n\n",
                     (fno.fattrib & AM_DIR) ? 'D' : '-',      // ��һ��Ŀ¼
                     (fno.fattrib & AM_RDO) ? 'R' : '-',      // ֻ���ļ�
                     (fno.fattrib & AM_HID) ? 'H' : '-',      // �����ļ�
                     (fno.fattrib & AM_SYS) ? 'S' : '-',      // ϵͳ�ļ�
                     (fno.fattrib & AM_ARC) ? 'A' : '-');     // �����ļ�
    }
    return res_flash;
}

static FRESULT scan_files (char* path) 
{ 
    FRESULT res; 		//�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���	
    FILINFO fno; 
    DIR dir; 
    int i;            
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
                printf("%s/%s\r\n", path, fn);								//����ļ���	
                /* ������������ȡ�ض���ʽ���ļ�·�� */        
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
    /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
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

        /*  �ļ���λ���ļ���ʼλ�� */
        res_flash = f_lseek(&fnew,0);
        if(FR_OK == res_flash){
            
            /* ��ȡ�ļ��������ݵ������� */
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
        
        /*  �ļ���λ���ļ���ʼλ�� */
        res_flash = f_lseek(&fnew,0);
        if(FR_OK == res_flash){
            
            /* ��ȡ�ļ��������ݵ������� */
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

    // char my_path[] = "1:�½� AutoHotkey Script.ahk";
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

#define MAX_ROWS 10    // �������
#define MAX_COLS 5     // �������
#define MAX_BUF 4096   // ��������С

float data[MAX_ROWS][MAX_COLS];  // ��Ž����������

void parse_csv(const char *input) {
    char buffer[MAX_BUF];  
    strncpy(buffer, input, MAX_BUF - 1); // �����ַ����������޸�ԭʼ����
    buffer[MAX_BUF - 1] = '\0'; // ȷ���ַ����� `\0` ��β

    char *line, *token;
    int row = 0, col;
    
    line = strtok(buffer, "\n"); // ���зָ�
    while (line && row < MAX_ROWS) {
        col = 0;
        token = strtok(line, ","); // �� `,` �ָ���
        while (token && col < MAX_COLS) {
            data[row][col] = atof(token); // ת��Ϊ float ����������
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
    /* ����ϵͳʱ��Ϊ72 MHz */ 
    /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
    
     printf("\r\n******** ����һ��SPI FLASH �ļ�ϵͳʵ�� *******\r\n");
    
    //���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
    res_flash = f_mount(&fs,"1:",1);
    if(res_flash!=FR_OK)
    {
        printf("\r\n�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
        printf("\r\n��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
        while(1);
    }
    else
    {
        printf("\r\n���ļ�ϵͳ���سɹ������Խ��в���\r\n");    
    }
    
    /* FatFs����ܲ��� */
    res_flash = miscellaneous();

    
    printf("\r\n*************** �ļ���Ϣ��ȡ���� **************\r\n");
    res_flash = file_check();

    
    printf("\r\n***************** �ļ�ɨ����� ****************\r\n");
    strcpy(fpath,"1:");
    scan_files(fpath);
    
    
    /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
    f_mount(NULL,"1:",1);
    
    /* ������ɣ�ͣ�� */
    while(1)
    {
    }

}
#else

#include "my_fatfs.h"
int my_test(void){
    my_fatfs_init();

    char my_path[] = "1:/TestDir/testdir.txt";
    my_scan();
    my_read(my_path);

    my_fatfs_deinit();

    while(1){
        // pause
        // printf("111");
        // HAL_Delay(1000);
    }
}
#endif

#endif

#if !319

int my_test() {
    char path[] = "1:/my_index.csv";
    FATFS_Init();
    FATFS_Read(path);

    FATFS_Index_Check();

    FATFS_Deinit();
    while(1){
        // just wait
    }

    return 0;
}

#endif

#if !31902

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>

    // �ж��ַ����Ƿ�Ϊ��Ч������
    int is_valid_number(const char *str) {
        if (str == NULL || *str == '\0') {
            return 0;
        }
        for (int i = 0; str[i] != '\0'; i++) {
            if (!isdigit(str[i])) {
                return 0;
            }
        }
        return 1;
    }

    // �ж��ַ�����ʾ�������Ƿ���ڵ��� 1001
    int is_number_greater_than_or_equal_to_target(const char *str) {
        if (!is_valid_number(str)) {
            return 0;
        }
        long num = strtol(str, NULL, 10);
        return num >= 1001;
    }

    // ʾ������
    void test_string_number_check() {
        char test_str1[] = "1001";
        char test_str2[] = "1000";
        char test_str3[] = "abc";

        printf("'%s' �Ƿ���ڵ��� 1001: %s\n", test_str1, is_number_greater_than_or_equal_to_target(test_str1) ? "��" : "��");
        printf("'%s' �Ƿ���ڵ��� 1001: %s\n", test_str2, is_number_greater_than_or_equal_to_target(test_str2) ? "��" : "��");
        printf("'%s' �Ƿ���ڵ��� 1001: %s\n", test_str3, is_number_greater_than_or_equal_to_target(test_str3) ? "��" : "��");
    }

    int my_test(void){
        test_string_number_check();
        while (1)
        {
            /* code */
        }
        
    }

#endif

#if !325

int my_test(){
    printf("111\r\n");
    HAL_Delay(500);
    return 0;
}

#endif

#if !326
#include "atk_malloc.h"
int my_test(void){
    atk_malloc_self_test();
}
#endif

#if 327

int my_test() {
    char path[] = "1:/my_index.csv";
    FATFS_Init();
    // FATFS_Read(path);


    FATFS_Index_Check();
    int i, j = 0;
    for(i = 0; i < 1; i++){
        // 1.��2��
        printf("group %d.\r\n", i);
        is_Q_stop = enum_Ready;
        while(is_Q_stop != enum_Stop){
            // 2.һ���50��
            quaternion_self_test();
        }
        delay_ms(1000);
    }
    char path2[20];
    sprintf(path2, "1:/%d.csv", FATFS_Index_Current);
    FATFS_Read(path2, 1);

    
    FATFS_Deinit();
    while(1){
        // just wait
    }
    return 0;
}

#endif

#if !32702


#include <stdint.h>

int my_test(void)
{
    long a = 10000;
    uint8_t b = 50;

    // long �� uint8_t ��ӣ����תΪ float
    float result = (float)(a + b);

    // �����ڴ˴���ӡ������������������߼�
    printf("%f\n", result);

    // ����֮ǰ����һ����ѭ��
    while (1);

    return 0; // ���Ը�����Ҫ���ķ���ֵ
}







#endif
