#include "main.h"
#include "usart.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "my_led.h"


#define USART_REC_LEN  			200  		//定义最大接收字节数 200
	
#define RXBUFFERSIZE   1 					//缓存大小


// extern uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
// //接收状态
// //bit15，	接收完成标志
// //bit14，	接收到0x0d
// //bit13~0，	接收到的有效字节数目

// extern uint8_t Buffer_Receive_IT[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

extern uint16_t USART_RX_STA;       //接收状态标记	  

