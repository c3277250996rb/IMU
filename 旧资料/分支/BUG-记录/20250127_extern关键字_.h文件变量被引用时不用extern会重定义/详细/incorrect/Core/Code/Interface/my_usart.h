#include "main.h"
#include "usart.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "my_led.h"


#define USART_REC_LEN  			200  		//�����������ֽ��� 200
	
#define RXBUFFERSIZE   1 					//�����С


// extern uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
// //����״̬
// //bit15��	������ɱ�־
// //bit14��	���յ�0x0d
// //bit13~0��	���յ�����Ч�ֽ���Ŀ

// extern uint8_t Buffer_Receive_IT[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

extern uint16_t USART_RX_STA;       //����״̬���	  

