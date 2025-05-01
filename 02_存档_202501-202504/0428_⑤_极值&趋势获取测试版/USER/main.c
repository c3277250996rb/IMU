#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
/************************************************
 ALIENTEK精英STM32开发板实验4
 串口 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
int my_test(void);

 int main(void)
 {		
//#if 1
 	u16 t;  
	u16 len;	
	u16 times=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
//#endif
/* 这里开始 */
	 
	printf("Simulink-启动成功\r\n");
	
	my_test();
	while(1);
	
}
//

//



#include <stdio.h>

int my_test(){
    // 【1】定义浮点数组
    // float arr[] = {3.3, -2.2, 1.1, 2.2, 3.3, 4.4, 2.2, 1.1, 2.2};
	// float arr[] = {5.0, 6.0, 7.5, 6.5, 5.5, 4.0, 4.5, 5.0, 3.0, 2.0};
	float arr[] = {1.1, -2.2, 3.3, -4.4, 5.5, 6.6, 9.9, 7, 7, 8, 8, 11, 7};
    // 【2】计算数组元素个数
    int n = sizeof(arr) / sizeof(arr[0]);

    // 【3】定义保存区间段的数组
    int sections[100]; // 最多存100个区间
    int sec_idx = 0;    // sections数组的当前索引

    float extremum[100];
    int ext_idx = 0;

    int i = 0;          // 【4】当前处理到的数组下标

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
                break; // 否则趋势断了，跳出
            }
        }

        // 【5.5】计算这段连续趋势的元素数量
        int count = (i - start + 1); // 注意+1，因为要包含start位置

        // 【5.6】保存段长度（带正负号，表示趋势方向）
        sections[sec_idx++] = dir * count;
        extremum[ext_idx++] = arr[start];
    }
    if(extremum[ext_idx-1] != arr[n-1]){
        extremum[ext_idx++] = arr[n-1];
    }

    // 【6】最后输出区间段数组
    printf("区间段: [");
    for (int j = 0; j < sec_idx; j++) {
        printf("%d", sections[j]);
        if (j != sec_idx - 1) printf(", "); // 如果不是最后一个，加逗号
    }
    printf("]\n");

    printf("极值: [");
    for (int j = 0; j < ext_idx; j++) {
        printf("%.1f", extremum[j]);
        if (j != ext_idx - 1) printf(", "); // 如果不是最后一个，加逗号
    }
    printf("]\n");

    return 0; // 【7】程序结束
}












