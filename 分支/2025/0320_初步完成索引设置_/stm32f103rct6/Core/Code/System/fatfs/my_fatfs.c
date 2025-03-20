#include "my_fatfs.h"


FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
char fpath[100];                  /* ���浱ǰɨ��·�� */
char readbuffer[512];    
char default_write_buffer[] = "\r\n100001";
uint8_t FATFS_Index_Current = 0;

FATFS_Result_Struct FATFS_Result_a = {
    .File_Size = -1,
};

// head
#if 1
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

// base
#if 1
FATFS_Result_Struct FATFS_Read(char* path){
    FATFS_Result_Struct temp_FATFS_Result = FATFS_Result_a;
    f_close(&fnew);
    res_flash = f_open(&fnew, path, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
    if(FR_OK == res_flash){

        /*  �ļ���λ���ļ���ʼλ�� */
        temp_FATFS_Result.File_Size = f_size(&fnew);
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
        
        /* �ļ���λ��write_positionλ�� */
        if(write_position < -1){
            // ���<-1���ǷǷ���ַ
            res_flash = FR_INVALID_PARAMETER;
        }else if(write_position == -1){
            // ���==-1��λ��β��
            res_flash = f_lseek(&fnew, f_size(&fnew));
        }else{
            // ���!==-1��λ��ָ��λ��
            res_flash = f_lseek(&fnew, write_position);
        }
        
        if(FR_OK == res_flash){
            
            /* ��write_bufferд�� */
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

void FATFS_Scan(void){
    printf("scaning files...\r\n");
    strcpy(fpath,"1:");
    scan_files(fpath);
}

void FATFS_Muti_Test(void){
    FATFS_Init();

    // char my_path[] = "1:�½� AutoHotkey Script.ahk";
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
    // 1.�ж��ļ��Ƿ�Ϊ��(��Ϊ�������)
    if(temp_FATFS_Result.File_Size > 0){
        get_last_line_from_file(index_check_path, temp_buffer, index_check_buffer_size);
        printf("get last line from file:\r\n>>>\r\n%s\r\n<<<\r\n", temp_buffer);
        // 2.�ж����һ�е������Ƿ���ڵ���10001, ���������д��Ĭ������:10001
        is_write_next_index = is_number_greater_than_or_equal_to_target(temp_buffer, 10001);
    }

    if(is_write_next_index){
        // 3.1 д����һ������, ��ʱstrtol(temp_buffer)+1Ϊ��һ�ε�����
        FATFS_Index_Current = atoi(temp_buffer)+1;
        sprintf(temp_to_write, "\r\n%d", FATFS_Index_Current);
        FATFS_Write(index_check_path, temp_to_write, -1);
    }else{
        // 3.2 ����Ҫ׷������, ֱ��д��Ĭ������default_write_buffer
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

float data[MAX_ROWS][MAX_COLS];  // ��Ž����������

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
            data[row][col] = atof(token); // ת��Ϊ float ����������
            col++;
            token = strtok_r(NULL, ",", &saveptr_col);
        }
        row++;
        line = strtok_r(NULL, "\n", &saveptr_row);
    }
}

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
