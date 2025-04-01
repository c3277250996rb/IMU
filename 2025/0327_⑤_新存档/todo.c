
/**
 * 1.gyro[3]丢进func()
 * 2.func()更新窗口数组
 * 3.根据窗口均值和当前值判断是否触发运动
 * 4.静止->运动 - 运动中: 1
 * 5.运动中->关闭触发器
 * 6.达到最大计数 - 静止: 0
 */

// #define WINDOW_SIZE 20
// int16_t window_array[3][WINDOW_SIZE] = {0};
// int16_t window_threshold = ?;
// uint8_t window_i = 0;
// uint8_t window_j = 0;

// void window_filter(){
//     int16_t *current_gyro = quater_gyro;
//     uint8_t is_larger_than = 0;
//     for(int i = 0; i < 3; i++){
//         is_larger_than = abs(current_gyro[i] - avg_i16(window_array[i], WINDOW_SIZE)) > 2*window_threshold;
//         if(is_larger_than){
//             break;
//         }

//         window_array[i][window_j] = current_gyro[i];
//         window_j = (window_j + 1) % WINDOW_SIZE;
//     }
// }


// int16_t avg_i16(int16_t* input_P, uint8_t size){
//     int16_t sum;
//     for(int i = 0; i < size; i++){
//         sum = sum + input_P[i];
//     }

//     return sum / size;
// }
