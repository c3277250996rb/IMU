#include "code.h"
#include "usart.h"
void CODE_MAIN();
void CODE_WHILE();

//�������´���,������Ҫѡ��use MicroLIB	  
#if 1
    #pragma import(__use_no_semihosting)             
    //��׼����Ҫ��֧�ֺ���                 
    struct __FILE { int handle; }; 
    FILE __stdout;       
    //����_sys_exit()�Ա���ʹ�ð�����ģʽ    
    void _sys_exit(int x) { x = x; } 
#endif 

 //�ض���c�⺯��printf������USARTx���ض�����ʹ��printf����
 int fputc(int ch, FILE *f) {
     /* ����һ���ֽ����ݵ�����USARTx */
     HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
     return (ch);
 }

 //�ض���c�⺯��scanf������USARTx����д����ʹ��scanf��getchar�Ⱥ���
 int fgetc(FILE *f) {
     int ch;
     /* �ȴ������������� */
     while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == RESET);
     HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
     return (ch);
 }

void CODE_MAIN(){

    
    CODE_WHILE();
}

void CODE_WHILE(){

    while(1){

        LED_PC13_read == LED_PC13_on_status ? LED_PC13_off : LED_PC13_on;
        LED_PA0_read == LED_PA0_on_status ? LED_PA0_off : LED_PA0_on;
        LED_PB13_read == LED_PB13_on_status ? LED_PB13_off : LED_PB13_on;
        printf("1111\r\n");
        HAL_Delay(1000);

    }

}



