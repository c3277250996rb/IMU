/*�����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ�����*/
#include "code.h"


void CODE_MAIN(void){
    CODE_INIT();
    CODE_SELF_TEST();
    CODE_WHILE();
}


void CODE_INIT(void){
	delay_init(150);               		//��ʼ����ʱ����
    uart_init(115200);                  //��ʼ������
    usmart_init(150);                    //��ʼ��USMART
}

void CODE_SELF_TEST(void){
	printf("111");
    POWER_ON();
    while(1){
        
        while(1);
    }
}

void CODE_WHILE(void){

    while(1){

    }
}











