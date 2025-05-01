#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
/************************************************
 ALIENTEK��ӢSTM32������ʵ��4
 ���� ʵ��   
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
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
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
//#endif
/* ���￪ʼ */
	 
	printf("Simulink-�����ɹ�\r\n");
	test();
//   TE_Get_Extremum_And_Trend();
	while(1);
	
}
//

//



#include <stdio.h>

#define Max_Q_Row 10

// ��3�����屣������ε�����
int trend[Max_Q_Row]; // ����100������
int trd_idx = 0;    // trend����ĵ�ǰ����

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
        // 1.���Ͽ�ʼ����
        start_index = end_index;
        // 2.�жϷ���
        dir = arr[start_index + 1] > arr[start_index] ? TE_UP : TE_DOWN;
        // 3.����ڶ�ѭ��, �ж���һ���Ƿ�������� (3.1 ��������:i_2nd++ ; 3.2 ����������:break,������ѭ��)
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

        // 4.���²���
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





