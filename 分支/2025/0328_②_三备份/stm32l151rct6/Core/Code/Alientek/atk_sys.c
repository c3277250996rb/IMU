#include "atk_sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//�޸�����:2019/9/17
//�汾��V1.4
//Copyright(C) ����ԭ�� 2009-2019
//********************************************************************************  

#ifdef USE_MY_ALIENTEK

//ʱ��ϵͳ���ú���
//PLL:ѡ��ı�Ƶ����RCC_PLL_MUL2~RCC_PLL_MUL16
//����ֵ:0,�ɹ�;1,ʧ��
void Stm32_Clock_Init(u32 PLL)
{
    // HAL_StatusTypeDef ret = HAL_OK;
    // RCC_OscInitTypeDef RCC_OscInitStructure; 
    // RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    // RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    	//ʱ��ԴΪHSE
    // RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      	//��HSE
	// RCC_OscInitStructure.HSEPredivValue=RCC_HSE_PREDIV_DIV1;		//HSEԤ��Ƶ
    // RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;					//��PLL
    // RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;			//PLLʱ��Դѡ��HSE
    // RCC_OscInitStructure.PLL.PLLMUL=PLL; 							//��PLL��Ƶ����
    // ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
	
    // if(ret!=HAL_OK) while(1);
    
    // //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    // RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    // RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;		//����ϵͳʱ��ʱ��ԴΪPLL
    // RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;				//AHB��Ƶϵ��Ϊ1
    // RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV2; 				//APB1��Ƶϵ��Ϊ2
    // RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV1; 				//APB2��Ƶϵ��Ϊ1
    // ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_2);	//ͬʱ����FLASH��ʱ����Ϊ2WS��Ҳ����3��CPU���ڡ�
		
    // if(ret!=HAL_OK) while(1);

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  
    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
    RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
      Error_Handler();
    }
}

#ifdef  USE_FULL_ASSERT
//��������ʾ�����ʱ��˺����������������ļ���������
//file��ָ��Դ�ļ�
//line��ָ�����ļ��е�����
void assert_failed(uint8_t* file, uint32_t line)
{ 
	while (1)
	{
	}
}
#endif
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

#endif
