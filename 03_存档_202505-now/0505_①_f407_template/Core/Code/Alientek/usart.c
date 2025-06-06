#include "sys.h"
#include "usart.h"	


// 串口
#if 1
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//__use_no_semihosting was requested, but _ttywrch was
void _ttywrch(int ch)
{
	ch = ch;
} 
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART1_Handler; //UART句柄
  
//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
		
	#if defined(__STM32F1XX_H)
	// F103

		if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
		{
			__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
			__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
			__HAL_RCC_AFIO_CLK_ENABLE();
		
			GPIO_Initure.Pin=USART1_TX_PA9_Pin;			//PA9
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
			GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
			GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
			HAL_GPIO_Init(USART1_TX_PA9_GPIO_Port,&GPIO_Initure);	   	//初始化PA9

			GPIO_Initure.Pin=USART1_RX_PA10_Pin;			//PA10
			GPIO_Initure.Mode=GPIO_MODE_INPUT;	//模式要设置为复用输入模式！	
			HAL_GPIO_Init(USART1_RX_PA10_GPIO_Port,&GPIO_Initure);	   	//初始化PA10
			
		}

	#elif defined(__STM32L1XX_H)
	// L151

	#elif defined(__STM32F4xx_H)
	// F407

		#ifndef USART1_TX_PA9_Pin
			#define USART1_TX_PA9_Pin GPIO_PIN_9
			#define USART1_TX_PA9_GPIO_Port GPIOA
			#define USART1_RX_PA10_Pin GPIO_PIN_10
			#define USART1_RX_PA10_GPIO_Port GPIOA
		#endif

		__USART1_CLK_ENABLE();
		__GPIOA_CLK_ENABLE();

		/**USART1 GPIO Configuration    
		PA9     ------> USART1_TX
		PA10    ------> USART1_RX 
		*/
		/* 配置Tx引脚为复用功能  */
		GPIO_Initure.Pin = USART1_TX_PA9_Pin;
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_Initure.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(USART1_TX_PA9_GPIO_Port, &GPIO_Initure);

		/* 配置Rx引脚为复用功能 */
		GPIO_Initure.Pin = USART1_RX_PA10_Pin;
		GPIO_Initure.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(USART1_RX_PA10_GPIO_Port, &GPIO_Initure); 

	#else
		#error "啊? MCU不是F103, L151或者F407吗? 这..."
	#endif


	#if EN_USART1_RX
			HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
			HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
	#endif	

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}

	}
}
 
//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>HAL_MAX_DELAY) break;		
	
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>HAL_MAX_DELAY) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
} 

#endif	

#endif
// 串口

