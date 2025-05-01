#include "windows_filter.h"

int WF_Status = F_WF_Reset;

float WF_Window[WF_WINDOWS_SIZE];
int WF_W_Current_Index = 0;
float WF_W_Average = 0;

int WF_Counter_AddUp = 0;
int WF_Counter_Init = 0;
int WF_Counter_Dynamic = 0;

void WF_Judge_From_Static_To_Dynamic(float input_Float){
    /* info */ printf("1: input_Float: %f\r\n", input_Float);

    /* 1.初始化 & 检测标记 */

        /* 1.1 正在初始化 */
            if(WF_WINDOWS_SIZE > WF_Counter_Init){
                /* info */ printf("[%d]开始初始化\r\n", WF_Counter_Init);

        /* 1.2 完成初始化 */
            }else if(WF_WINDOWS_SIZE == WF_Counter_Init){
                /* info */ printf("完毕\r\n");

        /* 1.3 检测标记 (|当前值 - 窗口均值| > 阈值吗? 如果是则标记触发) */
            }else if(WF_WINDOWS_SIZE < WF_Counter_Init){
                int temp_different = (int)input_Float - (int)WF_W_Average;
                /* info */ printf("temp_different: %d\r\n", temp_different);
                if(abs(temp_different) > WF_TRIGGER_THRESHOLD){
                    /* 标记触发 */
                    WF_Status = F_WF_Trigger;
                    /* 初始化索引 */
                    WF_W_Current_Index = 0;
                    return;
                }
            }
        
        /* 1.4 更新初始化计数器 */
            WF_Counter_Init++;

    /* ----------------- */


    /* 2.更新数组 & 均值 */
        WF_update_average_index_and_array(input_Float);
    /* ----------------- */

}

void WF_Judge_From_Dynamic_To_Static(float input_Float){
    int temp_different = (int)input_Float - (int)WF_W_Average;
    /* info */ printf("temp_different: %d\r\n", temp_different);
    /* info */ printf("2: input_Float: %f\r\n", input_Float);


    /* 1.等待达到最小时长 */            /* [0, 2W] */
        if(WF_WINDOWS_SIZE * 2 > WF_Counter_Dynamic){
            /* 运动计数器 自增 */
            WF_Counter_Dynamic++;
            return;
        }
    /* ----------------- */


    /* 2.重置窗口 */                    /* {3W} */
        if(WF_WINDOWS_SIZE * 3 == WF_Counter_Dynamic){
            /* info */ printf("\r\n重置窗口.\r\n");
            /* 重置数组 */
            memset(WF_Window, 0, sizeof(WF_Window));
            /* 重置均值 */
            WF_W_Average = 0;
        }
    /* --------- */


    /* 3.更新窗口, 均值和索引 */        /* [3W, +∞] */
        if(WF_WINDOWS_SIZE * 3 < WF_Counter_Dynamic){
            WF_update_average_index_and_array(input_Float);
        }
    /* --------------------- */


    /* 4.等待静止 (结束) */             /* [4W, +∞] */
        if(WF_WINDOWS_SIZE * 4 < WF_Counter_Dynamic){
            /* info */ printf("\r\n可以静止了.\r\n");

            /* 4.1 正在累计 -> (|当前值 - 窗口均值| < 阈值) -> 波动小 */
                if(abs(temp_different) < WF_TRIGGER_THRESHOLD){
                    /* info */ printf("[%d]正在累计\r\n", WF_Counter_AddUp);
                    /* 标记累计 */
                    WF_Status = F_WF_AddUp;
                    /* 累计计数器 自增 */
                    WF_Counter_AddUp++;
                }
        
            /* 4.2 取消累计 -> (|当前值 - 窗口均值| > 阈值) -> 波动大 */
                if(abs(temp_different) > WF_TRIGGER_THRESHOLD){
                    /* info */ printf("\r\n动辣!取消累计.\r\n");
                    /* 标记运动 */
                    WF_Status = F_WF_Trigger;
                    /* 累计计数器 重置 */
                    WF_Counter_AddUp = 0;
                }
        }
    /* ---------------- */


    /* 5.如果累计完成 */
        if(F_WF_AddUp == WF_Status && WF_WINDOWS_SIZE <= WF_Counter_AddUp){
            /* 标记完成 */
            WF_Status = F_WF_Finish;
            /* 重置 运动计数器 */
            WF_Counter_Dynamic = 0;
            /* 重置 累计计数器 */
            WF_Counter_AddUp = 0;
            return;
        }
    /* ------------- */


    /* 6.运动计数器 自增 */
        WF_Counter_Dynamic++;
    /* ---------------- */

}

void WF_update_average_index_and_array(float input_Float){
    /* 更新平均值 */
    WF_W_Average += (input_Float - WF_Window[WF_W_Current_Index]) / WF_WINDOWS_SIZE;

    /* 更新数组 */
    WF_Window[WF_W_Current_Index] = input_Float;

    /* 更新索引 */
    WF_W_Current_Index = (WF_W_Current_Index + 1) % WF_WINDOWS_SIZE;
}


