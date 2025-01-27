#include "my_usart.h"


uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA =0;       //����״̬���	  

uint8_t Buffer_Receive_IT[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���



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
    //  /* ����һ���ֽ����ݵ�����USARTx */
     HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
     return (ch);

    
	// while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    // USART1->DR = (uint8_t) ch;      
	// return ch;
}

 //�ض���c�⺯��scanf������USARTx����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f) {
     int ch;
     /* �ȴ������������� */
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
    
    // ���³�ʼ������Ϊ0
    memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));  // �����������ֽ�����Ϊ0
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

    if (huart->Instance == USART1) {  // ����Ǵ���1

        if ((USART_RX_STA & 0x8000) == 0) {  // ����δ���

            if (USART_RX_STA & 0x4000) {  // ���յ���0x0d
			
                if (Buffer_Receive_IT[0] != 0x0a) {
                    USART_RX_STA = 0;  // ���մ���, ���¿�ʼ
                } else {
                    USART_RX_STA |= 0x8000;  // ���������
                    printf("recived: %s\r\n", USART_RX_BUF);
                    re_init_buffer();
                    BLUE_Toggle();
                }

            } else {  // ��û�յ�0x0D

                if (Buffer_Receive_IT[0] == 0x0d) { // �ոս��յ�0x0d, ���ñ�־λ
                    USART_RX_STA |= 0x4000;
                } else { // û���յ�0x0D, �����յ�������buffer

                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Buffer_Receive_IT[0];
                    USART_RX_STA++;

                    if (USART_RX_STA > (USART_REC_LEN - 1)) { // �������ݴ���, ���¿�ʼ����
                        USART_RX_STA = 0;  
                    }

                }
            }
        }
    }
}


//����1�жϷ������
void USART1_IRQHandler(void){
	uint32_t timeout=0;
	
	HAL_UART_IRQHandler(&huart1);	//����HAL���жϴ����ú���
	
	// timeout=0;
    // while(HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY){ //�ȴ�����
    //     timeout++; //��ʱ����

    //     if(timeout > HAL_MAX_DELAY){
    //         break;
    //     }
	// }

	// timeout=0;
	// while(HAL_UART_Receive_IT(&huart1, (uint8_t *)Buffer_Receive_IT, RXBUFFERSIZE) != HAL_OK){ //һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
    //     timeout++; //��ʱ����
        
    //     if(timeout > HAL_MAX_DELAY){
    //         break;
    //     }
	// }
    
    Open_UART1_Receive_IT();
} 

#endif
