#include "trend_extremum.h"


// 【3】定义保存区间段的数组
int trend[Max_Q_Row]; // 最多存100个区间
int trd_idx = 0;    // trend数组的当前索引

float extremum[Max_Q_Row];
int ext_idx = 0;

void TE_Get_Extremum_And_Trend(int flag){
    // 【1】定义浮点数组
    // float arr[] = {3.3, -2.2, 1.1, 2.2, 3.3, 4.4, 2.2, 1.1, 2.2};
	// float arr[] = {5.0, 6.0, 7.5, 6.5, 5.5, 4.0, 4.5, 5.0, 3.0, 2.0};
	// float arr[] = {1.1, -2.2, 3.3, -4.4, 5.5, 6.6, 9.9, 7, 7, 8, 8, 11, 7};
    float *arr;
    switch (flag)
    {
        case F_TE_Roll:
            arr = SJ_Array_Roll;
            break;
        
        case F_TE_Pitch:
            arr = SJ_Array_Pitch;
            break;
        
        // case F_TE_Yaw:
        //     arr = SJ_Array_Roll;
        //     break;
        
        default:
            printf("[获取趋势极值.c]: 没有给数组!先退出了.");
            return;
            break;
    }

    // 【2】计算数组元素个数
    // int n = sizeof(arr) / sizeof(arr[0]);
    int n = Max_Q_Row;

    int i = 0;          // 【4】当前处理到的数组下标
    int trd_addup[Max_Q_Row];
    trd_addup[0] = 1;

    // 【5】开始遍历数组
    while (i < n - 1) { // 当i还没到数组倒数第2个元素时
        int dir = 0;    // dir记录当前趋势方向，1为上升，-1为下降

        // 【5.1】判断当前和下一个元素的关系，确定趋势方向
        if (arr[i] < arr[i+1]) dir = 1;      // 上升
        else if (arr[i] > arr[i+1]) dir = -1; // 下降
        else {
            // 【5.2】如果相等，跳过，继续往后找
            i++;
            continue;
        }

        // 【5.3】记录当前这段趋势开始的位置
        int start = i;
        i++; // 向后移动一个位置，准备继续检查趋势

        // 【5.4】继续沿着同一个趋势走
        while (i < n) {
            if ((dir == 1 && arr[i] < arr[i+1]) ||
                (dir == -1 && arr[i] > arr[i+1]) ||
                arr[i] == arr[i+1]) {
                if(n-1 == i){
                    break; // 最后一个不用再+1了
                }
                i++; // 如果还在同一趋势上，继续走
            } else {
                if(0 < trd_idx){
                    trd_addup[trd_idx] = dir * i;
                }
                break; // 否则趋势断了，跳出
            }
        }

        // 【5.5】计算这段连续趋势的元素数量
        int count = (i - start + 1); // 注意+1，因为要包含start位置

        // 【5.6】保存段长度（带正负号，表示趋势方向）
        if(0 < trd_idx){
            // trd_addup[trd_idx] = dir * (count - 1 + abs(trd_addup[trd_idx - 1]));
        }
        trend[trd_idx++] = dir * count;
        extremum[ext_idx++] = arr[start];
    }
    if(extremum[ext_idx-1] != arr[n-1]){
        extremum[ext_idx++] = arr[n-1];
    }

    printf("增长趋势: [");
    for (int j = 0; j < trd_idx; j++) {
        printf("%d", trd_addup[j]);
        if (j != trd_idx - 1) printf(", "); // 如果不是最后一个，加逗号
    }
    printf("]\r\n");
    printf("trd_idx: %d\r\n", trd_idx);

    // 【6】最后输出区间段数组
    printf("区间段: [");
    for (int j = 0; j < trd_idx; j++) {
        printf("%d", trend[j]);
        if (j != trd_idx - 1) printf(", "); // 如果不是最后一个，加逗号
    }
    printf("]\r\n");
    printf("trd_idx: %d\r\n", trd_idx);

    printf("极值: [");
    for (int j = 0; j < ext_idx; j++) {
        printf("%.2f", extremum[j]);
        if (j != ext_idx - 1) printf(", "); // 如果不是最后一个，加逗号
    }
    printf("]\r\n");
    printf("ext_idx: %d\r\n", ext_idx);

    // 【7】程序结束
}

void TE_ReInit(void){
    trd_idx = 0;
    ext_idx = 0;
    memcpy(trend, 0, sizeof(trend));
    memcpy(extremum, 0, sizeof(extremum));
}

#define FLOAT_THRESHOLD 0.5F
void TE_Extremum_Compress(void){
    int length = ext_idx - 2;
    float start = extremum[length - 1];
	
    while(--length){
        float diff = start - extremum[length - 1];
        diff = diff > 0 ? diff : -diff;
        if(FLOAT_THRESHOLD > diff){
            continue;
        }else{
            break;
        }
    }

    printf("极值压缩: [");
    for(int i = 0; i < length; i++){
        printf("%.2f, ", extremum[i]);
    }
    printf("]\r\n");
}



