#include "my_usart.h"


uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA =0;       //接收状态标记	  

uint8_t Buffer_Receive_IT[RXBUFFERSIZE];//HAL库使用的串口接收缓冲



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
    //  /* 发送一个字节数据到串口USARTx */
     HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
     return (ch);

    
	// while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    // USART1->DR = (uint8_t) ch;      
	// return ch;
}

 //重定向c库函数scanf到串口USARTx，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f) {
     int ch;
     /* 等待串口输入数据 */
     while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == RESET);
     HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
     return (ch);
}


#if 1
void Open_UART1_Receive_IT(void){
    HAL_UART_Receive_IT(&huart1, (uint8_t *)Buffer_Receive_IT, RXBUFFERSIZE);
}

void re_init_buffer(void){
    USART_RX_STA = 0;
    
    // 重新初始化数组为0
    memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));  // 将数组所有字节设置为0
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

    if (huart->Instance == USART1) {  // 如果是串口1

        if ((USART_RX_STA & 0x8000) == 0) {  // 接收未完成

            if (USART_RX_STA & 0x4000) {  // 接收到了0x0d
			
                if (Buffer_Receive_IT[0] != 0x0a) {
                    USART_RX_STA = 0;  // 接收错误, 重新开始
                } else {
                    USART_RX_STA |= 0x8000;  // 接收完成了
                    printf("recived: %s\r\n", USART_RX_BUF);
                    re_init_buffer();
                    BLUE_Toggle();
                }

            } else {  // 还没收到0x0D

                if (Buffer_Receive_IT[0] == 0x0d) { // 刚刚接收到0x0d, 设置标志位
                    USART_RX_STA |= 0x4000;
                } else { // 没接收到0x0D, 将接收到的填入buffer

                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Buffer_Receive_IT[0];
                    USART_RX_STA++;

                    if (USART_RX_STA > (USART_REC_LEN - 1)) { // 接收数据错误, 重新开始接收
                        USART_RX_STA = 0;  
                    }

                }
            }
        }
    }
}


//串口1中断服务程序
void USART1_IRQHandler(void){
	uint32_t timeout=0;
	
	HAL_UART_IRQHandler(&huart1);	//调用HAL库中断处理公用函数
	
	// timeout=0;
    // while(HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY){ //等待就绪
    //     timeout++; //超时处理

    //     if(timeout > HAL_MAX_DELAY){
    //         break;
    //     }
	// }

	// timeout=0;
	// while(HAL_UART_Receive_IT(&huart1, (uint8_t *)Buffer_Receive_IT, RXBUFFERSIZE) != HAL_OK){ //一次处理完成之后，重新开启中断并设置RxXferCount为1
    //     timeout++; //超时处理
        
    //     if(timeout > HAL_MAX_DELAY){
    //         break;
    //     }
	// }
    
    Open_UART1_Receive_IT();
} 

#endif
