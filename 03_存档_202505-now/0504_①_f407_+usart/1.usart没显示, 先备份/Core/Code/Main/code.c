/*金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮*/
#include "code.h"


void CODE_MAIN(void){
    CODE_INIT();
    CODE_SELF_TEST();
    CODE_WHILE();
}


void CODE_INIT(void){
	delay_init(150);               		//初始化延时函数
    uart_init(115200);                  //初始化串口
    usmart_init(150);                    //初始化USMART
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











