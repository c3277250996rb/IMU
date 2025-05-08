#ifndef __MOTOR_H
#define __MOTOR_H

void motor_init(void);
void motor_go(void);
void car_go_slow(void);
void pid_right(void);
void pid_left(void);
void car_go(void);
void car_right(void);
void car_left(void);
void car_stop(void);
void car_zhuan_right(void);
void car_zhuan_left(void);

#define motor_gpio_clk_enable           RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define motor_gpio                      GPIOA
#define motor1_pin_1                    GPIO_Pin_2
#define motor1_pin_2                    GPIO_Pin_3
#define motor2_pin_1                    GPIO_Pin_4
#define motor2_pin_2                    GPIO_Pin_5

#endif
