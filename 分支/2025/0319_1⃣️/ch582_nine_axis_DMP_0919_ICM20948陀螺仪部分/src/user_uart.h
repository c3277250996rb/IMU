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
    uint8_t Slave_addr;      // 从机地址
    uint8_t ReceiveBuff[RX_SIZE]; // 接收缓存数组
    uint8_t ReceiveCount;    // 计算接收到的数据有多少字节
    uint8_t ReceiveNum;  // 计算接收到的数据有多少字节
    uint8_t timecount;       // 有多久没有接收到字节，数据断续的时间
    uint8_t timerun;         // 断续时间是否开始累加
    uint8_t ReceiveComplete; // 一帧数据接收完成标志

    uint16_t packet_time;
    uint16_t reg_addr;      // 寄存器地址
    uint16_t write_data;    // 写入的数据
    uint32_t write_data32;  // 写入的数据
    uint16_t read_data_num; // 写入的数据

} uart_rev_str;

void user_uart_init(void);
void user_uart_receive_byte_handle(uint8_t value);
void user_uart_data_timeout(void);


#endif /* USER_UART_H_ */
