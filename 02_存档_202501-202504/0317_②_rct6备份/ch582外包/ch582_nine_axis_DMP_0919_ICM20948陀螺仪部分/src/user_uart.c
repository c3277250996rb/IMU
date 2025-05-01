/*
 * user_uart.c
 *
 *  Created on: Jul 30, 2024
 *      Author: liu
 */

#include "user_uart.h"



uart_rev_str uart_info={0};

void user_uart_receive_byte_handle(uint8_t value)
{
    uart_info.ReceiveBuff[uart_info.ReceiveCount] = value;
    uart_info.ReceiveCount++;
    if(uart_info.ReceiveCount >=RX_SIZE)
    {
        uart_info.ReceiveCount = 0;
    }
    if (uart_info.ReceiveCount == 1)
    {
        uart_info.timerun = 1;
    }
}

void user_uart_data_timeout(void)
{
    if (uart_info.timerun == 1)
    {
        uart_info.timecount++;
    }
    if (uart_info.timecount >= uart_info.packet_time) // 大于3.5个字符时间,接受到一包数据
    {
        uart_info.timerun = 0;
        uart_info.timecount = 0;
        uart_info.ReceiveComplete = 1;
//        modbus_receive_packet(uart_info.ReceiveBuff); //解析数据
        uart_info.ReceiveNum=uart_info.ReceiveCount;
        uart_info.ReceiveCount = 0;

    //          modbus.ReceiveComplete = 0;
    }

}
void user_uart_init(void)
{
    uart_info.packet_time=1;

}
