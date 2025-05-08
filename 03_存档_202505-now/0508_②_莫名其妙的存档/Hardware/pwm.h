#ifndef __PWM_H
#define __PWM_H

void pwm_init(void);
void motor_pwm(uint8_t l_pwm, uint8_t r_pwm);

#define pwm_gpio_clk_enable             RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#define pwm_tim_clk_enable              RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

#define pwm_tim                         TIM2

#define pwm_gpio                        GPIOA
#define pwm_pin_1                       GPIO_Pin_0
#define pwm_pin_2                       GPIO_Pin_1

#endif
