/*�����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ�����*/
#include "code.h"

void CODE_MAIN(void){
    CODE_INIT();
    POWER_ON();
    CODE_SELF_TEST();
    CODE_WHILE();
}


void CODE_INIT(void){
	delay_init(di_sysclk);      //��ʼ����ʱ����
    uart_init(115200);          //��ʼ������
    usmart_init(ui_sysclk);     //��ʼ��USMART
}

void CODE_SELF_TEST(void){
    while(1){
        my_flash_self_test();
        // muti_printf_self_test();
        while(1);
    }
}

void CODE_WHILE(void){

    while(1){

    }
}











