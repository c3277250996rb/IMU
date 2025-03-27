#include "data_management.h"

DM_Result_Struct DM_Result_default = {
    .Status = 0
};

// // app
// DM_Result_Struct DM_Write(float input_Array[][5]){
//     char path[20];
//     sprintf(path, "1:/%d.csv", FATFS_Index_Current);
//     int temp_row = get_row(input_Array);
//     int temp_column = 5;
//     int i, j = 0;
//     char temp_DM_Write_Buffer[100] = 0;
    
//     for(i = 0; i < temp_row; i++){
//         snprintf(temp_DM_Write_Buffer, sizeof(temp_DM_Write_Buffer), "\r\n%d", input_Array[i][1]);
//         for(j = 0; j < temp_column; j++){
//             snprintf(temp_DM_Write_Buffer, sizeof(temp_DM_Write_Buffer), "%s,%f", &temp_DM_Write_Buffer, input_Array[i][j]);
//         }     

//         if(i == 1){
//             // 去掉第一行的\r\n
//             FATFS_Write(path, temp_DM_Write_Buffer + 2, -1);
//         }else{
//             // 一行一行写入
//             FATFS_Write(path, temp_DM_Write_Buffer, -1);
//         }

//     }

//     FATFS_Read(path);
// }

// app
DM_Result_Struct DM_Write(float *input_Array, uint8_t previous_index){
    char path[20];
    sprintf(path, "1:/%d.csv", FATFS_Index_Current);
    int temp_row = 10;
    int temp_column = 5;
    int i, j = 0;
    char temp_DM_Write_Buffer[100] = 0;
    // float temp_Array[5] = &input_Array;

    printf("cloumn: %3d\r\n", previous_index);
    
        for(j = 0; j < temp_column; j++){
            if(0 == j){
                snprintf(temp_DM_Write_Buffer, sizeof(temp_DM_Write_Buffer), "\r\n%f", input_Array[j]);
            }else{
                snprintf(temp_DM_Write_Buffer, sizeof(temp_DM_Write_Buffer), "%s,%f", &temp_DM_Write_Buffer, input_Array[j]);
            }
            
        }     

        // if(0 == previous_index){
        //     FATFS_Write(path, temp_DM_Write_Buffer + 2, -1);
        // }else{
            // 一行一行写入
            // FATFS_Write(path, temp_DM_Write_Buffer, -1);
        // }

        FATFS_Write(path, temp_DM_Write_Buffer, -1);
        if(previous_index == 0){
            FATFS_Write(path, "index,roll,pitch,yaw,date\r\n", 0);
        }
        

    // FATFS_Read(path);
}

// driver


//
int get_row(float arr[][5]){
    return sizeof(arr) / sizeof(arr[0]);
}
int get_column(float arr[][5]){
    return sizeof(arr[0]) / sizeof(arr[0][0]);
}

