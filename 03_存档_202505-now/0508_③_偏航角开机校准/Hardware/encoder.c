#include "stm32f10x.h"
#include "encoder.h"

void encoder_init(void)
{               //tim3  PA6 and PA7
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = encoder_pin_1 | encoder_pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    TIM_TimeBaseInitTypeDef tim3_base;
    tim3_base.TIM_ClockDivision = TIM_CKD_DIV1;
    tim3_base.TIM_CounterMode = TIM_CounterMode_Up;
    tim3_base.TIM_Period = 65535;
    tim3_base.TIM_Prescaler = 0;
    tim3_base.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &tim3_base);

    TIM_ICInitTypeDef tim3_ic;
    TIM_ICStructInit(&tim3_ic);
    tim3_ic.TIM_Channel = TIM_Channel_1;
    tim3_ic.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM3, &tim3_ic);
    tim3_ic.TIM_Channel = TIM_Channel_2;
    tim3_ic.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM3, &tim3_ic);
    //极性改这里，小心电机进行pid调节时反转
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_Cmd(TIM3, ENABLE);
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
        
    TIM_TimeBaseInitTypeDef tim4_base;
    tim4_base.TIM_ClockDivision = TIM_CKD_DIV1;
    tim4_base.TIM_CounterMode = TIM_CounterMode_Up;
    tim4_base.TIM_Period = 65535;
    tim4_base.TIM_Prescaler = 0;
    tim4_base.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &tim4_base);

    TIM_ICInitTypeDef tim4_ic;
    TIM_ICStructInit(&tim4_ic);
    tim4_ic.TIM_Channel = TIM_Channel_1;
    tim4_ic.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM4, &tim4_ic);
    tim4_ic.TIM_Channel = TIM_Channel_2;
    tim4_ic.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM4, &tim4_ic);
    //极性改这里，小心电机进行pid调节时反转
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_Cmd(TIM4, ENABLE);
}

int16_t encoder1_get(void)
{
    int16_t temp;
    temp = TIM_GetCounter(TIM3);
    TIM_SetCounter(TIM3, 0);
    return temp;
}

int16_t encoder2_get(void)
{
    int16_t m;
    m = TIM_GetCounter(TIM4);
    TIM_SetCounter(TIM4, 0);
    return m;
}
