#ifndef __MY_FATFS_H
#define __MY_FATFS_H

#include "code.h"

#include "ff.h"

typedef struct __FATFS_Result_Struct
{
    int File_Size;
}FATFS_Result_Struct;

extern FATFS_Result_Struct FATFS_Result_default;
extern uint8_t FATFS_Index_Current;

FATFS_Result_Struct FATFS_Read(char* path);
void FATFS_Write(char* path, char* write_buffer, int write_position);
void FATFS_Delete(char* path);

void FATFS_Init(void);
void FATFS_Deinit(void);
void FATFS_Scan(void);
void FATFS_Muti_Test(void);
FATFS_Result_Struct FATFS_Index_Check(void);

FRESULT get_last_line_from_file(const char *ipath, char *ibuffer, size_t ibuffer_size);


#endif
