#include "my_usart.h"

#ifndef USE_ATK_SYSTEM
uint8_t Buffer_Receive_Storage[Storage_Length];     //接收缓冲,最大Storage_Length个字节.

// +-------------------------------------------------------------+
// |   16-bit Register                                           |
// |-------------------------------------------------------------|
// |  Bit  |    15   |   14    |             13~0                |
// |-------------------------------------------------------------|
// |  Flags| 0a flag | 0d flag |       Data field                |
// +-------------------------------------------------------------+

// - 0a flag (Bit 16):     Represents the status of the "0a" flag, 1 means enabled, 0 means disabled.  
// - 0d flag (Bit 15):     Represents the status of the "0d" flag, 1 means enabled, 0 means disabled.  
// - Data field (Bits 14-0): Represents the actual data, with a width of 15 bits to store the value or other data. 
uint16_t Buffer_Receive_Register = 0;       //接收状态标记	  

uint8_t Buffer_Receive_IT[IT_Length];//HAL库使用的串口接收缓冲



// 加入以下代码,而不需要选择use MicroLIB	  
#if 1
    #pragma import(__use_no_semihosting)             
    //标准库需要的支持函数                 
    struct __FILE { int handle; }; 
    FILE __stdout;       
    //定义_sys_exit()以避免使用半主机模式    
    void _sys_exit(int x) { x = x; } 
#endif 

// 重定向c库函数printf到串口USARTx，重定向后可使用printf函数
int fputc(int ch, FILE *f) {
    
    /* 推荐 - 完美通过压力测试 */
        while((USART1->SR & 0X40) == 0){
            // 循环发送,直到发送完毕
        }  // 注意,读取USARTx->SR能避免莫名其妙的错误   	
        USART1->DR = (uint8_t) ch;  
        return ch;
    /* --------------------- */

    /* 不推荐 - 无法通过压力测试 */
        //  /* 发送一个字节数据到串口USARTx */
        //  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
        //  return (ch);
    /* ----------------------- */

}

// 重定向c库函数scanf到串口USARTx，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f) {
     int ch;
     /* 等待串口输入数据 */
     while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == RESET);
     HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
     return (ch);
}

// open recive interrupt
void Open_UART1_Receive_IT(void){
    HAL_UART_Receive_IT(&huart1, (uint8_t *)Buffer_Receive_IT, IT_Length);
}

// initial again
void re_init_buffer(void){
    Buffer_Receive_Register = 0;
    
    // 重新初始化数组为0
    memset(Buffer_Receive_Storage, 0, sizeof(Buffer_Receive_Storage));  // 将数组所有字节设置为0
}

// Recive Complete Callback Function
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if (huart->Instance == USART1) {  // 如果是串口1
        if (0 == (0x8000 & Buffer_Receive_Register)) {  // 上一个byte不是0a, 未完成接收, 但上上一个byte可能是od.

            if (1 <= (0x4000 & Buffer_Receive_Register)) {  // 上一个byte是0d, 半完成接收.
			
                if (0x0a == Buffer_Receive_IT[0]){  // 上一个byte是0d, 现在接收到了0a, 设置0a标志位, 已完成接收.
                    Buffer_Receive_Register |= 0x8000;
                    my_gpio_self_test();
                    printf("recived: %s\r\n", Buffer_Receive_Storage);
                    re_init_buffer();
                } else {  // 上一个byte是0d, 现在没接收到0a, 接收错误, 重置Register, 重新开始.
                    Buffer_Receive_Register = 0;
                }

            } else {  // 上一个byte不是od, 未完成接收

                if (0x0d == Buffer_Receive_IT[0]) {  // 上一个byte不是0d, 现在接收到了0d, 设置0d标志位, 半完成接收.
                    Buffer_Receive_Register |= 0x4000;
                } else {  // 上一个byte不是0d, 现在没接收到0d, 将接收到的byte存入Storage, 自增Register.

                    Buffer_Receive_Storage[Buffer_Receive_Register & 0X3FFF] = Buffer_Receive_IT[0];
                    Buffer_Receive_Register++;

                    if (Buffer_Receive_Register > (Storage_Length - 1)) { // 接收数据错误, 重新开始接收
                        Buffer_Receive_Register = 0;  
                    }

                }
            }

        }
    }
}

// USART1 Interrupt Request Handler
void USART1_IRQHandler(void){
	HAL_UART_IRQHandler(&huart1);	//调用HAL库中断处理公用函数
	
    Open_UART1_Receive_IT();
} 

void UART1_APP_Init(void){
    MX_USART1_UART_Init();
    Open_UART1_Receive_IT();
    printf("(no) 不用正点原子的函数.\r\n");
}

#else
// init uart
void UART1_APP_Init(void){
    /**
     * 给 STM32F103 用的
     */
    // delay_init(72);               		//初始化延时函数
    // uart_init(115200);					//初始化串口

    delay_init(8);  // stm32l151rct6
    MX_USART1_UART_Init();
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量

    #ifdef USE_ATK_USMART
    usmart_init(84);
    #endif
    printf("(yes) 在用正点原子的函数.\r\n");
}
#endif


void my_usart_self_test(void){
    printf("just send and see the echo.\r\n");
    #ifdef USE_ATK_SYSTEM
        delay_ms(1000);
    #else
        for(int i = 1000*1000; i--; i > 0);
    #endif
}


#ifdef USE_USMART_GIDLE
/**
 * @brief Alientek USMART Quick Function.
 */
char *my_func_list[] = {
    "minnie",
    "yuqi",
    "shuhua"
};

int usmart_minnie(void){
    int result = 1;

    return result;
}

int usmart_yuqi(void){
    int result = 2;

    
    return result;
}

int usmart_shuhua(void){
    int result = 3;

    
    return result;
}

int ug(char *idle){
    printf("\r\nit is GIDLE!\r\n");
    int result = 0;
    if(*idle == *my_func_list[0]){
        printf("it is minnie!\r\n");
        result = usmart_minnie();
    }
    if(idle == my_func_list[1]){
        printf("it is yuqi!\r\n");
        result = usmart_yuqi();
    }
    if(idle == my_func_list[2]){
        printf("it is shuhua!\r\n");
        result = usmart_shuhua();
    }
    // if(idle == my_func_list[3]){
    //     printf("it is ");
    //     result = 
    // }

    printf("end of it\r\n");
    return result;
}
#endif




