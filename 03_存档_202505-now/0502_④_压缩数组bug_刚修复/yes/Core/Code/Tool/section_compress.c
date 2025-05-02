#include "section_compress.h"


void calculate_coefficient(float* input_array, int length){
    int direction = input_array[1] > input_array[0] ? 1 : -1;
    int add_up = direction;
    for(int i = 2; i < length; i++){
        direction = input_array[i] > input_array[i - 1] ? 1 : -1;

        if(1 == direction){
            if(0 < add_up){
                add_up++;
            }
            if(0 > add_up){
                input_array[i - 1][5] = add_up;
                add_up = -1;
            }
            
        }
        if(-1 == direction){
            if(0 < add_up){
                input_array[i - 1][5] = add_up;
                add_up = 1;
            }
            if(0 > add_up){
                add_up--;
            }
        }
    }
}

void test_cc(float* input_array, int length){
    for(int i = 0; i < length; i++){
        if(0 != input_array[i][5]){
            printf("input_array\r\n");
        }
    }
}


