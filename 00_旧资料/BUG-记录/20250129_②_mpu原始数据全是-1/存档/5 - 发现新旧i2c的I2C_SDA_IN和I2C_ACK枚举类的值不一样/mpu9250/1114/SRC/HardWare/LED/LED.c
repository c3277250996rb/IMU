
#include "LED.h"

uint32_t KeyValue=0;

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

int CORREC_KEYSCAN(void)
{	
	if(!GPIO_ReadInputDataBit( IO_KEY_ARRAY, IO_CORREC_KEY))  
	{
		KeyValue ++;
	 	Delay(0xaffff); 
		Delay(0xaffff);
		return 1;
	}
	else
		return 0;
}

void LedShow(void)
{
	if(KeyValue>3)
		 KeyValue=4;
	switch(KeyValue)
	{
		case 1:  {
							GPIO_WriteBit( IO_LED_ARRAY, IO_LED1,Bit_RESET); 
							break;		}
		case 2: {
							GPIO_WriteBit( IO_LED_ARRAY, IO_LED2,Bit_RESET); 
							GPIO_WriteBit( IO_LED_ARRAY, IO_LED1,Bit_SET); 
							break;		}
		case 3: {
							GPIO_WriteBit( IO_LED_ARRAY, IO_LED3,Bit_RESET); 
							GPIO_WriteBit( IO_LED_ARRAY, IO_LED2,Bit_SET);
							break;		}
		default: break;
	}
	
	Delay(0x5ffff);
	GPIO_WriteBit( IO_LED_ARRAY, IO_LED1 | IO_LED2  ,Bit_RESET);
	Delay(0x5ffff);

}

void LedGpioConfiguration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_IO_LED, ENABLE);

  GPIO_InitStructure.GPIO_Pin = IO_LED1|IO_LED2|IO_LED3|IO_sysLED|IO_RS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init( IO_LED_ARRAY, &GPIO_InitStructure);
	GPIO_SetBits(IO_LED_ARRAY, IO_LED1);//IO_LED1 输出高电平 LED1灭
	GPIO_SetBits(IO_LED_ARRAY, IO_LED2);//IO_LED2 输出高电平 LED1灭
	GPIO_SetBits(IO_LED_ARRAY, IO_LED3);//IO_LED3 输出高电平 LED1灭
	GPIO_SetBits(IO_LED_ARRAY, IO_sysLED);//IO_sysLED 输出高电平 LED1灭
}



void KEYGpioConfiguration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_IO_KEY, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IO_CORREC_KEY;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init( IO_KEY_ARRAY, &GPIO_InitStructure);
}
