#include "scene_judgement.h"

float roll_min_max[2];
float pitch_min_max[2];

float moved_angle;
float start_angle;
float end_angle;

float SJ_Array[Max_Q_Row];
int SJ_Counter = 0;

void search_min_max(void){
    memset(roll_min_max, 0, sizeof(roll_min_max));
    memset(pitch_min_max, 0, sizeof(pitch_min_max));
    int i;
    for(i = 0; i < Max_Q_Row; i++){
        float temp_roll = Q_Record_Array[i][1];
        float temp_pitch = Q_Record_Array[i][2];
        if(0 == temp_roll){
            // continue;
            i--;
            break;
        }

        SJ_Array[SJ_Counter] = sqrt(temp_roll * temp_roll + temp_pitch * temp_pitch);
        SJ_Counter++;

        roll_min_max[0] = temp_roll < roll_min_max[0] ? temp_roll : roll_min_max[0];
        roll_min_max[1] = temp_roll > roll_min_max[1] ? temp_roll : roll_min_max[1];
     
        pitch_min_max[0] = temp_pitch < pitch_min_max[0] ? temp_pitch : pitch_min_max[0];
        pitch_min_max[1] = temp_pitch > pitch_min_max[1] ? temp_pitch : pitch_min_max[1];
        
    }
    moved_angle = sqrt(
        (roll_min_max[1] - roll_min_max[0]) * (roll_min_max[1] - roll_min_max[0]) + 
        (pitch_min_max[1] - pitch_min_max[0]) * (pitch_min_max[1] - pitch_min_max[0])
    );

    start_angle = sqrt(Q_Record_Array[0][1]*Q_Record_Array[0][1] + Q_Record_Array[0][2]*Q_Record_Array[0][2]);
    end_angle   = sqrt(Q_Record_Array[i][1]*Q_Record_Array[i][1] + Q_Record_Array[i][2]*Q_Record_Array[i][2]);
}

void judge_scene(void){
    // do{
    //     if(0.0f <= moved_angle < 15.0f){
    //         printf("小幅运动");
    //     }
    //     if(15.0f <= moved_angle < 30.0f){
    //         printf("中福运动");
    //     }
    //     if(30.0f <= moved_angle){
    //         printf("大幅运动");
    //     }
    // }while(0);

    char p1[] = "小幅运动";
    char p2[] = "中福运动";
    char p3[] = "大幅运动";
    char *temp = 15 > moved_angle ? p1 : 30 > moved_angle ? p2 : p3;
    printf(temp);

    printf(": %f\r\n", moved_angle);
    printf("%s: %f\r\n", abs(end_angle-start_angle)<5 ? "归位" : "移动", (end_angle-start_angle));
}

void ReInit_SJ_Array_And_Counter(void){
    memcpy(SJ_Array, 0, sizeof(SJ_Array));
    SJ_Counter = 0;
}

