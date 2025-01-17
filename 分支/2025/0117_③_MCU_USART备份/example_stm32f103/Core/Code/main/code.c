#include "code.h"
#include "usart.h"
void CODE_MAIN();
void CODE_WHILE();

//加入以下代码,而不需要选择use MicroLIB	  
#if 1
    #pragma import(__use_no_semihosting)             
    //标准库需要的支持函数                 
    struct __FILE { int handle; }; 
    FILE __stdout;       
    //定义_sys_exit()以避免使用半主机模式    
    void _sys_exit(int x) { x = x; } 
#endif 

 //重定向c库函数printf到串口USARTx，重定向后可使用printf函数
 int fputc(int ch, FILE *f) {
     /* 发送一个字节数据到串口USARTx */
     HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
     return (ch);
 }

 //重定向c库函数scanf到串口USARTx，重写向后可使用scanf、getchar等函数
 int fgetc(FILE *f) {
     int ch;
     /* 等待串口输入数据 */
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



