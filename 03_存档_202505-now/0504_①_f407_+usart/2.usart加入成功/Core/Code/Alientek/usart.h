/**
 ******************************************************************************
 * @file    usart.h
 * @brief   ����.
 ******************************************************************************
 * 
 *-----------------------------------------------------------------------------
 * @brief   1.��ʼ������.
 * @param   uart_init
 *  param   bound: ������
 *                 (����ԭ��һ���� 115200).
 *-----------------------------------------------------------------------------
 * 
 */


#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "main.h"


#if defined(__STM32F1XX_H)
    // F103
    #include "stm32f1xx_hal_uart.h"
#elif defined(__STM32L1XX_H)
    // L151
    #include "stm32l1xx_hal_uart.h"
#elif defined(__STM32F4xx_H)
    // F407
    #include "stm32f4xx_hal_uart.h"
#else
    #error "��? MCU����F103, L151����F407��? ��..."
#endif



// ����
#if 1
#define USART_REC_LEN  			200  		//�����������ֽ��� 200
#define EN_USART1_RX 			1			//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         			//����״̬���	
extern UART_HandleTypeDef UART1_Handler; 	//UART���

#define RXBUFFERSIZE   1 					//�����С
extern u8 aRxBuffer[RXBUFFERSIZE];			//HAL��USART����Buffer

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

#endif
// ����


#endif

