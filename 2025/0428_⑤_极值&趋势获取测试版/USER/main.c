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
int my_test(void);

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
	
	my_test();
	while(1);
	
}
//

//



#include <stdio.h>

int my_test(){
    // ��1�����帡������
    // float arr[] = {3.3, -2.2, 1.1, 2.2, 3.3, 4.4, 2.2, 1.1, 2.2};
	// float arr[] = {5.0, 6.0, 7.5, 6.5, 5.5, 4.0, 4.5, 5.0, 3.0, 2.0};
	float arr[] = {1.1, -2.2, 3.3, -4.4, 5.5, 6.6, 9.9, 7, 7, 8, 8, 11, 7};
    // ��2����������Ԫ�ظ���
    int n = sizeof(arr) / sizeof(arr[0]);

    // ��3�����屣������ε�����
    int sections[100]; // ����100������
    int sec_idx = 0;    // sections����ĵ�ǰ����

    float extremum[100];
    int ext_idx = 0;

    int i = 0;          // ��4����ǰ�����������±�

    // ��5����ʼ��������
    while (i < n - 1) { // ��i��û�����鵹����2��Ԫ��ʱ
        int dir = 0;    // dir��¼��ǰ���Ʒ���1Ϊ������-1Ϊ�½�

        // ��5.1���жϵ�ǰ����һ��Ԫ�صĹ�ϵ��ȷ�����Ʒ���
        if (arr[i] < arr[i+1]) dir = 1;      // ����
        else if (arr[i] > arr[i+1]) dir = -1; // �½�
        else {
            // ��5.2�������ȣ�����������������
            i++;
            continue;
        }

        // ��5.3����¼��ǰ������ƿ�ʼ��λ��
        int start = i;
        i++; // ����ƶ�һ��λ�ã�׼�������������

        // ��5.4����������ͬһ��������
        while (i < n) {
            if ((dir == 1 && arr[i] < arr[i+1]) ||
                (dir == -1 && arr[i] > arr[i+1]) ||
                arr[i] == arr[i+1]) {
                if(n-1 == i){
                    break; // ���һ��������+1��
                }
                i++; // �������ͬһ�����ϣ�������
            } else {
                break; // �������ƶ��ˣ�����
            }
        }

        // ��5.5����������������Ƶ�Ԫ������
        int count = (i - start + 1); // ע��+1����ΪҪ����startλ��

        // ��5.6������γ��ȣ��������ţ���ʾ���Ʒ���
        sections[sec_idx++] = dir * count;
        extremum[ext_idx++] = arr[start];
    }
    if(extremum[ext_idx-1] != arr[n-1]){
        extremum[ext_idx++] = arr[n-1];
    }

    // ��6�����������������
    printf("�����: [");
    for (int j = 0; j < sec_idx; j++) {
        printf("%d", sections[j]);
        if (j != sec_idx - 1) printf(", "); // ����������һ�����Ӷ���
    }
    printf("]\n");

    printf("��ֵ: [");
    for (int j = 0; j < ext_idx; j++) {
        printf("%.1f", extremum[j]);
        if (j != ext_idx - 1) printf(", "); // ����������һ�����Ӷ���
    }
    printf("]\n");

    return 0; // ��7���������
}












