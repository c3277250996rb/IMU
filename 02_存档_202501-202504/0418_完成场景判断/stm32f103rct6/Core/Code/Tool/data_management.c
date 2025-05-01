/*金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮*/

#include "data_management.h"

// int Cache_Max_Row = 30;
// int Cache_Max_Row = 80;
int Cache_Max_Row = 160;

DM_Result_Struct DM_Result_default = {
    .Status = 0
};


// app
void DM_Write(float *input_Array, uint8_t previous_index){
    char path[20];
    sprintf(path, "1:/%d.csv", FATFS_Index_Current);
    // int temp_row = Cache_Max_Row;
    int temp_column = Info_Column;
    int i, j = 0;
    char temp_DM_Write_Buffer[100] = 0;
    // float temp_Array[5] = &input_Array;

    printf("写入: %3d\r\n", previous_index);
    
    for(j = 0; j < temp_column; j++){
        if(0 == j){
            snprintf(temp_DM_Write_Buffer, sizeof(temp_DM_Write_Buffer), "\r\n%f", input_Array[j]);
        }else{
            snprintf(temp_DM_Write_Buffer, sizeof(temp_DM_Write_Buffer), "%s,%f", &temp_DM_Write_Buffer, input_Array[j]);
        }
    }     


    // uint8_t fast_mode = 0;
    // if(previous_index == 0){
    //     FATFS_Write(path, "index,roll,pitch,yaw,date\r\n", 0, 0, 3);
    //     fast_mode = 2;
    // }
    // else if(0 < previous_index < Cache_Max_Row - 1){
    //     fast_mode = 2;
    // }
    // else if(previous_index == Cache_Max_Row - 1){
    //     fast_mode = 1;
    // }
    // FATFS_Write(path, temp_DM_Write_Buffer, -1, 0, fast_mode);

    uint8_t fast_mode = 0;
    if(previous_index == 0){
        FATFS_Write(path, "index,roll,pitch,yaw,date", 0, 0, 3);
        fast_mode = 2;
    }
    if(0 < previous_index < Cache_Max_Row - 1){
        fast_mode = 2;
    }
    if(previous_index == Cache_Max_Row - 1){
        fast_mode = 1;
    }
    FATFS_Write(path, temp_DM_Write_Buffer, -1, 0, fast_mode);

    if(previous_index == Cache_Max_Row - 1){
        FATFS_Read(path, 1);
        #include "scene_judgement.h"
        search_min_max();
        judge_scene();
    }

}


void set_DM_row(int input_row){
    #ifdef USE_ATK_MALLOC
    if(input_row > 87){
        printf("row cannot larger than 87.\r\n");
        input_row = Cache_Max_Row;
    }
    #else 
    if(input_row > 150){
        printf("row cannot larger than 150.\r\n");
        input_row = Cache_Max_Row;
    }
    #endif
    Cache_Max_Row = input_row;
    printf("the Cache_Max_Row is set to be [%d].\r\n", Cache_Max_Row);
}

// driver


//
int get_row(float arr[][5]){
    return sizeof(arr) / sizeof(arr[0]);
}
int get_column(float arr[][5]){
    return sizeof(arr[0]) / sizeof(arr[0][0]);
}

