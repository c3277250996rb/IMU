#include "scene_judgement.h"

float roll_min_max[2];
float pitch_min_max[2];

float moved_angle;
float start_angle;
float end_angle;

void search_min_max(void){
    memset(roll_min_max, 0, sizeof(roll_min_max));
    memset(pitch_min_max, 0, sizeof(pitch_min_max));
    int i;
    for(i = 0; i < Cache_Max_Row; i++){
        float temp = DM_angle_record[i][1];
        if(0 == temp){
            // continue;
            i--;
            break;
        }
        roll_min_max[0] = temp < roll_min_max[0] ? temp : roll_min_max[0];
        roll_min_max[1] = temp > roll_min_max[1] ? temp : roll_min_max[1];
     
        temp = DM_angle_record[i][2];
        pitch_min_max[0] = temp < pitch_min_max[0] ? temp : pitch_min_max[0];
        pitch_min_max[1] = temp > pitch_min_max[1] ? temp : pitch_min_max[1];
        
    }
    moved_angle = sqrt(
        (roll_min_max[1] - roll_min_max[0]) * (roll_min_max[1] - roll_min_max[0]) + 
        (pitch_min_max[1] - pitch_min_max[0]) * (pitch_min_max[1] - pitch_min_max[0])
    );

    start_angle = sqrt(DM_angle_record[0][1]*DM_angle_record[0][1] + DM_angle_record[0][2]*DM_angle_record[0][2]);
    end_angle   = sqrt(DM_angle_record[i][1]*DM_angle_record[i][1] + DM_angle_record[i][2]*DM_angle_record[i][2]);
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



