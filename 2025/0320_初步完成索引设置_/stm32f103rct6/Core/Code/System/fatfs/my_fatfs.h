#include "code.h"

#include "ff.h"

typedef struct __FATFS_Result_Struct
{
    int File_Size;
}FATFS_Result_Struct;


FATFS_Result_Struct FATFS_Read(char* path);
void FATFS_Write(char* path, char* write_buffer, int write_position);
void FATFS_Delete(char* path);

void FATFS_Init(void);
void FATFS_Deinit(void);
void FATFS_Scan(void);
void FATFS_Muti_Test(void);
FATFS_Result_Struct FATFS_Index_Check(void);

FRESULT get_last_line_from_file(const char *ipath, char *ibuffer, size_t ibuffer_size);

