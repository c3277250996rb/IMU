#include "my_usart.h"


uint8_t Buffer_Receive_Storage[Storage_Length];     //���ջ���,���Storage_Length���ֽ�.

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
uint16_t Buffer_Receive_Register = 0;       //����״̬���	  

uint8_t Buffer_Receive_IT[IT_Length];//HAL��ʹ�õĴ��ڽ��ջ���



// �������´���,������Ҫѡ��use MicroLIB	  
#if 1
    #pragma import(__use_no_semihosting)             
    //��׼����Ҫ��֧�ֺ���                 
    struct __FILE { int handle; }; 
    FILE __stdout;       
    //����_sys_exit()�Ա���ʹ�ð�����ģʽ    
    void _sys_exit(int x) { x = x; } 
#endif 

// �ض���c�⺯��printf������USARTx���ض�����ʹ��printf����
int fputc(int ch, FILE *f) {
    
    /* �Ƽ� - ����ͨ��ѹ������ */
        while((USART1->SR & 0X40) == 0){
            // ѭ������,ֱ���������
        }  // ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
        USART1->DR = (uint8_t) ch;  
        return ch;
    /* --------------------- */

    /* ���Ƽ� - �޷�ͨ��ѹ������ */
        //  /* ����һ���ֽ����ݵ�����USARTx */
        //  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
        //  return (ch);
    /* ----------------------- */

}

// �ض���c�⺯��scanf������USARTx����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f) {
     int ch;
     /* �ȴ������������� */
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
    
    // ���³�ʼ������Ϊ0
    memset(Buffer_Receive_Storage, 0, sizeof(Buffer_Receive_Storage));  // �����������ֽ�����Ϊ0
}

// Recive Complete Callback Function
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if (huart->Instance == USART1) {  // ����Ǵ���1
        if (0 == (0x8000 & Buffer_Receive_Register)) {  // ��һ��byte����0a, δ��ɽ���, ������һ��byte������od.

            if (1 <= (0x4000 & Buffer_Receive_Register)) {  // ��һ��byte��0d, ����ɽ���.
			
                if (0x0a == Buffer_Receive_IT[0]){  // ��һ��byte��0d, ���ڽ��յ���0a, ����0a��־λ, ����ɽ���.
                    Buffer_Receive_Register |= 0x8000;
                    printf("recived: %s\r\n", Buffer_Receive_Storage);
                    re_init_buffer();
                    // BLUE_Toggle();
                } else {  // ��һ��byte��0d, ����û���յ�0a, ���մ���, ����Register, ���¿�ʼ.
                    Buffer_Receive_Register = 0;
                }

            } else {  // ��һ��byte����od, δ��ɽ���

                if (0x0d == Buffer_Receive_IT[0]) {  // ��һ��byte����0d, ���ڽ��յ���0d, ����0d��־λ, ����ɽ���.
                    Buffer_Receive_Register |= 0x4000;
                } else {  // ��һ��byte����0d, ����û���յ�0d, �����յ���byte����Storage, ����Register.

                    Buffer_Receive_Storage[Buffer_Receive_Register & 0X3FFF] = Buffer_Receive_IT[0];
                    Buffer_Receive_Register++;

                    if (Buffer_Receive_Register > (Storage_Length - 1)) { // �������ݴ���, ���¿�ʼ����
                        Buffer_Receive_Register = 0;  
                    }

                }
            }

        }
    }
}

// USART1 Interrupt Request Handler
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
	// while(HAL_UART_Receive_IT(&huart1, (uint8_t *)Buffer_Receive_IT, IT_Length) != HAL_OK){ //һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
    //     timeout++; //��ʱ����
        
    //     if(timeout > HAL_MAX_DELAY){
    //         break;
    //     }
	// }
    
    Open_UART1_Receive_IT();
} 


void my_usart_self_test(void){
    printf("just send and see the echo.\r\n");
    HAL_Delay(1000);
}

