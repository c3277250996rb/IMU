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
void test(void);
int compress_tail();
void TE_Extremum_Compress(void);
void TE_Get_Extremum_And_Trend(void);

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
	test();
//   TE_Get_Extremum_And_Trend();
	while(1);
	
}
//

//



#include <stdio.h>

#define Max_Q_Row 10

// 【3】定义保存区间段的数组
int trend[Max_Q_Row]; // 最多存100个区间
int trd_idx = 0;    // trend数组的当前索引

float extremum[Max_Q_Row];
int ext_idx = 0;


#define TE_UP 1
#define TE_DOWN -1

void test(){

    int start_index = 0;
    int end_index = 0;
    int dir = 0;
    int i_1st = 0;
    int i_2nd = 0;

    float arr[] = {1.1, 2.2, 3.3, 2.2, 4.4, 1.1};
    int length = sizeof(arr) / sizeof(arr[0]);

    trend[0] = 0;
    extremum[0] = arr[0];

    while(1){
        // 1.给上开始索引
        start_index = end_index;
        // 2.判断方向
        dir = arr[start_index + 1] > arr[start_index] ? TE_UP : TE_DOWN;
        // 3.进入第二循环, 判断下一个是否符合趋势 (3.1 符合趋势:i_2nd++ ; 3.2 不符合趋势:break,返回主循环)
        while(1){
						
						if(end_index == length - 1){
								break;
						}
				
            end_index++;
            if(
                (TE_UP == dir && arr[end_index] >= arr[end_index - 1]) ||
                (TE_DOWN == dir && arr[end_index] <= arr[end_index - 1])
            ){
                continue;
            }else{
                end_index--;
                break;
            }
        }

        // 4.更新参数
				int temp = dir * end_index;
				printf("%d, ", temp);
        trend[i_1st + 1] = temp;
        extremum[i_1st + 1] = arr[end_index];

        i_1st++;

        if(end_index == length - 1){
            break;
        }
				
				
    }


    printf("trend: [");
    for(int i = 0; i < Max_Q_Row; i++){
			printf("%d, ", trend[i]);
    }
    printf("]\r\n");


    printf("extremum: [");
    for(int i = 0; i < Max_Q_Row; i++){
        printf("%.1f, ", extremum[i]);
    }
    printf("]\r\n");


}





