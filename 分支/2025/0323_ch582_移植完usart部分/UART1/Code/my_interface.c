#include "my_interface.h"

#ifndef __USART__APP__Reference__
#define __USART__APP__Reference__

    // head
    uint8_t TxBuff[] = "This is a tx exam\r\n";
    uint8_t RxBuff[100];
    uint8_t trigB;
    // head

    void USART_APP_Init(void){
        /* ���ô���1��������IO��ģʽ�������ô��� */
        GPIOA_SetBits(GPIO_Pin_9);
        GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);      // RXD-������������
        GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA); // TXD-�������������ע������IO������ߵ�ƽ
        UART1_DefInit();

        // �жϷ�ʽ���������ݺ��ͳ�ȥ
        UART1_ByteTrigCfg(UART_7BYTE_TRIG);
        trigB = 7;
        UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
        PFIC_EnableIRQ(UART1_IRQn);
    }

    __INTERRUPT
    __HIGH_CODE
    void UART1_IRQHandler(void){
        volatile uint8_t i;

        switch(UART1_GetITFlag()){

            case UART_II_LINE_STAT: // ��·״̬����
                {
                    UART1_GetLinSTA();
                    break;
                }

            case UART_II_RECV_RDY: // ���ݴﵽ���ô�����
                for(i = 0; i != trigB; i++){
                    RxBuff[i] = UART1_RecvByte();
                    UART1_SendByte(RxBuff[i]);
                }
                break;

            case UART_II_RECV_TOUT: // ���ճ�ʱ����ʱһ֡���ݽ������
                i = UART1_RecvString(RxBuff);
                UART1_SendString(RxBuff, i);
                break;

            case UART_II_THR_EMPTY: // ���ͻ������գ��ɼ�������
                break;

            case UART_II_MODEM_CHG: // ֻ֧�ִ���0
                break;

            default:
                break;
        }
    }

#endif

