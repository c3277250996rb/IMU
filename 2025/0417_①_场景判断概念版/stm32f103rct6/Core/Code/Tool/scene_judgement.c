#include "scene_judgement.h"

float roll_min_max[2];
float pitch_min_max[2];

float moved_angle;

void search_min_max(void){
    roll_min_max = {0};
    pitch_min_max = {0};
    for(int i = 0; i < Cache_Max_Row; i++){
        float temp = DM_angle_record[i][1];
        if(0 == temp){
            continue;
        }
        roll_min_max[0] = temp < roll_min_max[0] ? temp : roll_min_max[0];
        roll_min_max[1] = temp > roll_min_max[0] ? temp : roll_min_max[0];
     
        float temp = DM_angle_record[i][2];
        pitch_min_max[0] = temp < pitch_min_max[0] ? temp : pitch_min_max[0];
        pitch_min_max[1] = temp > pitch_min_max[0] ? temp : pitch_min_max[0];
        
    }
}

void judge_scene(void){
    moved_angle = sqrt(
        (roll_min_max[1] - roll_min_max[0]) * (roll_min_max[1] - roll_min_max[0]) + 
        (pitch_min_max[1] - pitch_min_max[0]) * (pitch_min_max[1] - pitch_min_max[0])
    );
    if(0 <= moved_angle < 15){
        printf("С���˶�");
    }
    if(15 < moved_angle <= 30){
        printf("�и��˶�");
    }
    if(30 < moved_angle){
        printf("����˶�");
    }

    printf(": %f\r\n", moved_angle);
}



