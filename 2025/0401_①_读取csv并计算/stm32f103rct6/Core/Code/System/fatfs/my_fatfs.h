#ifndef __MY_FATFS_H
#define __MY_FATFS_H

#include "code.h"

#include "ff.h"

typedef struct __FATFS_Result_Struct
{
    int File_Size;
}FATFS_Result_Struct;

extern FATFS_Result_Struct FATFS_Result_default;
extern long FATFS_Index_Current;

#define MAX_ROWS 100
#define MAX_COLS 20
#define MAX_BUF 1024

extern float data[MAX_ROWS][MAX_COLS];  // 存放解析后的数据

FATFS_Result_Struct FATFS_Read(char* path, uint8_t is_echo);
void FATFS_Write(char* path, char* write_buffer, int write_position);
void FATFS_Delete(char* path);

void FATFS_Init(void);
void FATFS_Deinit(void);
void FATFS_Scan(void);
void FATFS_Muti_Test(void);
FATFS_Result_Struct FATFS_Index_Check(void);
void FATFS_Convert_Array(char *input_Path);

FRESULT get_last_line_from_file(const char *ipath, char *ibuffer, size_t ibuffer_size);
void convert_text_to_array(const char *input);


#endif
