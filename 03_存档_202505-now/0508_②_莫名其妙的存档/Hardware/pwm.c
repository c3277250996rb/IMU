#include "stm32f10x.h"
#include "pwm.h"

void pwm_init(void)
{                                               //共设置了2路PWM，分别是PA0-PA1，对应TIM2的CH1-2
    pwm_gpio_clk_enable;                        //使能tim
    pwm_tim_clk_enable;                         //使能gpio

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = pwm_pin_1|pwm_pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(pwm_gpio, &GPIO_InitStructure);

    TIM_InternalClockConfig(pwm_tim);

    TIM_TimeBaseInitTypeDef tim_base;
    tim_base.TIM_ClockDivision = TIM_CKD_DIV1;
    tim_base.TIM_CounterMode = TIM_CounterMode_Up;
    tim_base.TIM_Period = 100 - 1;
    tim_base.TIM_Prescaler = 36 - 1;
    tim_base.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(pwm_tim, &tim_base);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(pwm_tim, &TIM_OCInitStructure);
    TIM_OC2Init(pwm_tim, &TIM_OCInitStructure);

    TIM_Cmd(pwm_tim, ENABLE);
}

void motor_pwm(uint8_t l_pwm, uint8_t r_pwm)
{
    TIM_SetCompare1(pwm_tim, l_pwm);                  //改变占空比变速
    TIM_SetCompare2(pwm_tim, r_pwm);
}

