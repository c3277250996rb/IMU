/*
 * user_uart.h
 *
 *  Created on: Jul 30, 2024
 *      Author: liu
 */

#ifndef USER_UART_H_
#define USER_UART_H_

#include  "main.h"

#define RX_SIZE   50
typedef struct
{
    uint8_t Slave_addr;      // �ӻ���ַ
    uint8_t ReceiveBuff[RX_SIZE]; // ���ջ�������
    uint8_t ReceiveCount;    // ������յ��������ж����ֽ�
    uint8_t ReceiveNum;  // ������յ��������ж����ֽ�
    uint8_t timecount;       // �ж��û�н��յ��ֽڣ����ݶ�����ʱ��
    uint8_t timerun;         // ����ʱ���Ƿ�ʼ�ۼ�
    uint8_t ReceiveComplete; // һ֡���ݽ�����ɱ�־

    uint16_t packet_time;
    uint16_t reg_addr;      // �Ĵ�����ַ
    uint16_t write_data;    // д�������
    uint32_t write_data32;  // д�������
    uint16_t read_data_num; // д�������

} uart_rev_str;

void user_uart_init(void);
void user_uart_receive_byte_handle(uint8_t value);
void user_uart_data_timeout(void);


#endif /* USER_UART_H_ */
