#include "atk_sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//修改日期:2019/9/17
//版本：V1.4
//Copyright(C) 正点原子 2009-2019
//********************************************************************************  

#ifdef USE_MY_ALIENTEK

//时钟系统配置函数
//PLL:选择的倍频数，RCC_PLL_MUL2~RCC_PLL_MUL16
//返回值:0,成功;1,失败
void Stm32_Clock_Init(u32 PLL)
{
    /**
     * 给 STM32F103 用的
     */

    // HAL_StatusTypeDef ret = HAL_OK;
    // RCC_OscInitTypeDef RCC_OscInitStructure; 
    // RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    // RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    	//时钟源为HSE
    // RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      	//打开HSE
	// RCC_OscInitStructure.HSEPredivValue=RCC_HSE_PREDIV_DIV1;		//HSE预分频
    // RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;					//打开PLL
    // RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;			//PLL时钟源选择HSE
    // RCC_OscInitStructure.PLL.PLLMUL=PLL; 							//主PLL倍频因子
    // ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
	
    // if(ret!=HAL_OK) while(1);
    
    // //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    // RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    // RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;		//设置系统时钟时钟源为PLL
    // RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;				//AHB分频系数为1
    // RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV2; 				//APB1分频系数为2
    // RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV1; 				//APB2分频系数为1
    // ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_2);	//同时设置FLASH延时周期为2WS，也就是3个CPU周期。
		
    // if(ret!=HAL_OK) while(1);
}

#ifdef  USE_FULL_ASSERT
//当编译提示出错的时候此函数用来报告错误的文件和所在行
//file：指向源文件
//line：指向在文件中的行数
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

#endif
