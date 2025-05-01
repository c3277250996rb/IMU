/*�����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ����ݽ�����*/
#include "code.h"


void CODE_MAIN(void){
    CODE_INIT();
    CODE_SELF_TEST();
    CODE_WHILE();
}


void CODE_INIT(void){
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
    uart_init(115200);                  //��ʼ������
    usmart_init(84);                    //��ʼ��USMART
    LED_Init();							//��ʼ��LED	
}

void CODE_SELF_TEST(void){
    printf("�����ɹ�!\r\n");
    printf("����-F103-��Ӣ\r\n");
    while(1){
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED0��Ӧ����PB5���ͣ�������ͬ��LED0(0)
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);   	//LED1��Ӧ����PE5���ߣ��𣬵�ͬ��LED1(1)
        delay_ms(500);											//��ʱ500ms
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);   	//LED0��Ӧ����PB5���ߣ��𣬵�ͬ��LED0(1)
        HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET); 	//LED1��Ӧ����PE5���ͣ�������ͬ��LED1(0)
        delay_ms(500);                                      	//��ʱ500ms 
    }
}

void CODE_WHILE(void){

    while(1){

    }
}










