#include "quaternion.h"
#include "scene_judgement.h"
#include "my_string.h"

enum
{
    F_TE_Roll = 0,
    F_TE_Pitch,
    F_TE_Yaw
};


typedef struct __TE_Array_Struct
{
    int *Trend;
    float *Extremum;
}TE_Array_Struct;

typedef struct __TE_Frame_Struct
{
    char Date[20]; // "2025/04/30 17:30:00"
    char Brief[20]; // 备注
    long File_Index; // 文件索引
    TE_Array_Struct Array;
}TE_Frame_Struct;

extern TE_Frame_Struct TE_Frame_default;


void TE_Get_Extremum_And_Trend(int flag);

void TE_ReInit(void);

void TE_Extremum_Compress(void);

void TE_Generate_DataFrame(void);

