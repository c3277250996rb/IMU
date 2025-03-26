#include "data_management.h"

DM_Result_Struct DM_Result_default = {
    .Status = 0
};

// app
DM_Result_Struct DM_Write(float input_Array[][5]){
    char path[20];
    sprintf(path, "1:/%d.csv", FATFS_Index_Current);
    int temp_row = get_row(input_Array);
    int temp_column = 5;
    int i, j = 0;
    char temp_DM_Write_Buffer[100] = 0;
    
    for(i = 0; i < temp_row; i++){
        snprintf(temp_DM_Write_Buffer, sizeof(temp_DM_Write_Buffer), "\r\n%d", input_Array[i][1]);
        for(j = 0; j < temp_column; j++){
            snprintf(temp_DM_Write_Buffer, sizeof(temp_DM_Write_Buffer), "%s,%f", &temp_DM_Write_Buffer, input_Array[i][j]);
        }     

        if(i == 1){
            // 去掉第一行的\r\n
            FATFS_Write(path, temp_DM_Write_Buffer + 2, -1);
        }else{
            // 一行一行写入
            FATFS_Write(path, temp_DM_Write_Buffer, -1);
        }

    }

    FATFS_Read(path);
}

// driver


//
int get_row(float arr[][5]){
    return sizeof(arr) / sizeof(arr[0]);
}
int get_column(float arr[][5]){
    return sizeof(arr[0]) / sizeof(arr[0][0]);
}

