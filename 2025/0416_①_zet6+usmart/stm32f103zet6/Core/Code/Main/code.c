/*金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮金米妮*/
#include "code.h"


void CODE_MAIN(void){
    CODE_INIT();
    CODE_SELF_TEST();
    CODE_WHILE();
}


void CODE_INIT(void){
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
    uart_init(115200);                  //初始化串口
    usmart_init(84);                    //初始化USMART
    LED_Init();							//初始化LED	
}

void CODE_SELF_TEST(void){
    printf("启动成功!\r\n");
    printf("正点-F103-精英\r\n");
    while(1){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED0对应引脚PB5拉低，亮，等同于LED0(0)
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);   	//LED1对应引脚PE5拉高，灭，等同于LED1(1)
        delay_ms(500);											//延时500ms
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);   	//LED0对应引脚PB5拉高，灭，等同于LED0(1)
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED1对应引脚PE5拉低，亮，等同于LED1(0)
        delay_ms(500);                                      	//延时500ms 
    }
}

void CODE_WHILE(void){

    while(1){

    }
}










