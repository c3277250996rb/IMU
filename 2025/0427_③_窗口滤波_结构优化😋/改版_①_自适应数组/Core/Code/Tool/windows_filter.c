#include "windows_filter.h"


int is_WF_trigger = enum_RESET;
float WF_array[WINDOWS_SIZE];
int WF_a_current_index = 0;
int WF_a_add_up = 0;

float WF_a_average = 0;
int is_WF_init = 0;
int dynamic_index = 0;

void WF_Judge_From_Static_To_Dynamic(float input_float){
    /* info */ printf("1: input_float: %f\r\n", input_float);

    /* 1.初始化 & 检测标记 */

        /* 1.1 正在初始化 */
            if(WINDOWS_SIZE > is_WF_init){
                /* info */ printf("[%d]开始初始化\r\n", is_WF_init);

        /* 1.2 完成初始化 */
            }else if(WINDOWS_SIZE == is_WF_init){
                /* info */ printf("完毕\r\n");

        /* 1.3 检测标记 (|当前值 - 窗口均值| > 阈值吗? 如果是则标记触发) */
            }else if(WINDOWS_SIZE < is_WF_init){
                int temp_i = (int)input_float - (int)WF_a_average;
                /* info */ printf("temp_i: %d\r\n", temp_i);
                if(abs(temp_i) > TRIGGER_THRESHOLD){
                    /* 标记触发 */
                    is_WF_trigger = enum_TRIGGER;
                    /* 初始化索引 */
                    WF_a_current_index = 0;
                    return;
                }
            }
        
        /* 1.4 更新初始化计数器 */
            is_WF_init++;

    /* ----------------- */


    /* 2.更新数组 & 均值 */
        WF_update_average_index_and_array(input_float);
    /* ----------------- */

}

void WF_Judge_From_Dynamic_To_Static(float input_float){
    int temp_i = (int)input_float - (int)WF_a_average;
    /* info */ printf("temp_i: %d\r\n", temp_i);
    /* info */ printf("2: input_float: %f\r\n", input_float);


    /* 1.等待达到最小时长 */            /* [0, 2W] */
        if(WINDOWS_SIZE * 2 > dynamic_index){
            /* 运动计数器 自增 */
            dynamic_index++;
            return;
        }
    /* ----------------- */


    /* 2.重置窗口 */                    /* {3W} */
        if(WINDOWS_SIZE * 3 == dynamic_index){
            /* info */ printf("\r\n重置窗口.\r\n");
            /* 重置数组 */
            memset(WF_array, 0, sizeof(WF_array));
            /* 重置均值 */
            WF_a_average = 0;
        }
    /* --------- */


    /* 3.更新窗口, 均值和索引 */        /* [3W, +∞] */
        if(WINDOWS_SIZE * 3 < dynamic_index){
            WF_update_average_index_and_array(input_float);
        }
    /* --------------------- */


    /* 4.等待静止 (结束) */             /* [4W, +∞] */
        if(WINDOWS_SIZE * 4 < dynamic_index){
            /* info */ printf("\r\n可以静止了.\r\n");

            /* 4.1 正在累计 -> (|当前值 - 窗口均值| < 阈值) -> 波动小 */
                if(abs(temp_i) < TRIGGER_THRESHOLD){
                    /* info */ printf("[%d]正在累计\r\n", WF_a_add_up);
                    /* 标记累计 */
                    is_WF_trigger = enum_ADDUP;
                    /* 累计计数器 自增 */
                    WF_a_add_up++;
                }
        
            /* 4.2 取消累计 -> (|当前值 - 窗口均值| > 阈值) -> 波动大 */
                if(abs(temp_i) > TRIGGER_THRESHOLD){
                    /* info */ printf("\r\n动辣!取消累计.\r\n");
                    /* 标记运动 */
                    is_WF_trigger = enum_TRIGGER;
                    /* 累计计数器 重置 */
                    WF_a_add_up = 0;
                }
        }
    /* ---------------- */


    /* 5.如果累计完成 */
        if(enum_ADDUP == is_WF_trigger && WINDOWS_SIZE <= WF_a_add_up){
            /* 标记完成 */
            is_WF_trigger = enum_FINISH;
            /* 重置 运动计数器 */
            dynamic_index = 0;
            /* 重置 累计计数器 */
            WF_a_add_up = 0;
            return;
        }
    /* ------------- */


    /* 6.运动计数器 自增 */
        dynamic_index++;
    /* ---------------- */

}

void WF_update_average_index_and_array(float input_float){
    /* 更新平均值 */
    WF_a_average += (input_float - WF_array[WF_a_current_index]) / WINDOWS_SIZE;

    /* 更新数组 */
    WF_array[WF_a_current_index] = input_float;

    /* 更新索引 */
    WF_a_current_index = (WF_a_current_index + 1) % WINDOWS_SIZE;
}


