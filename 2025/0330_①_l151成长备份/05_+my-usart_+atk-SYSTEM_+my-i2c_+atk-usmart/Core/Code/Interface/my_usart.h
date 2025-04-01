#include "__interface.h"

#ifndef USE_ATK_SYSTEM
#include "usart.h"
#include "my_gpio.h"

#define Storage_Length  200     //定义最大接收字节数 200
#define IT_Length   1       //缓存大小

extern uint8_t Buffer_Receive_Storage[Storage_Length];      //接收缓冲,最大Storage_Length个字节.
extern uint8_t Buffer_Receive_IT[IT_Length];        //HAL库使用的串口接收缓冲

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
extern uint16_t Buffer_Receive_Register;       //接收状态标记	  

void Open_UART1_Receive_IT(void);
void re_init_buffer(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void USART1_IRQHandler(void);
#else
#include "atk_delay.h"
#include "atk_usart.h"
// #include "atk_usmart.h"
#endif

void UART1_APP_Init(void);
