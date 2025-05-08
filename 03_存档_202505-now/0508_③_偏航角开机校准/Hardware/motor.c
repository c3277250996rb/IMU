#include "stm32f10x.h"
#include "pwm.h"
#include "motor.h"
#include "encoder.h"

void motor_init(void)
{
                                                        //选用PA2----PA5
    motor_gpio_clk_enable;                              //使能对应io口

    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = motor1_pin_1|motor1_pin_2|motor2_pin_1|motor2_pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(motor_gpio, &GPIO_InitStructure);
}


void motor_go()
{
        GPIO_ResetBits(motor_gpio, motor1_pin_1);          //电机1
        GPIO_SetBits(motor_gpio, motor1_pin_2);
        
        GPIO_ResetBits(motor_gpio, motor2_pin_1);          //电机2
        GPIO_SetBits(motor_gpio, motor2_pin_2);

    
    
}

//左电机pid
float l_pwm, l_speed, l_out;
float l_Kp = 0.2, l_Ki = 0.22, l_Kd = 0.23;
float l_error0, l_error1, l_errorint;
//右电机pid
float r_pwm, r_speed, r_out;
float r_Kp = 0.2, r_Ki = 0.22, r_Kd = 0.23;
float r_error0, r_error1, r_errorint;

void pid_left()
{
    l_speed = encoder1_get();

    l_error1 = l_error0;
    l_error0 = l_pwm - l_speed;
    
    if (l_Ki != 0)
    {
        l_errorint += l_error0;
    }
    else
    {
        l_errorint = 0;
    }
    
    l_out = l_Kp * l_error0 + l_Ki * l_errorint + l_Kd * (l_error0 - l_error1);
    
    if (l_out > 100) {l_out = 100;}
    if (l_out < -100) {l_out = -100;}
}


void pid_right()
{
    r_speed = encoder2_get();
    
    r_error1 = r_error0;
    r_error0 = r_pwm - r_speed;
    
    if (r_Ki != 0)
    {
        r_errorint += r_error0;
    }
    else
    {
        r_errorint = 0;
    }
    
    r_out = r_Kp * r_error0 + r_Ki * r_errorint + r_Kd * (r_error0 - r_error1);
    
    if (r_out > 100) {r_out = 100;}
    if (r_out < -100) {r_out = -100;}
}


void car_go_slow()
{
    l_pwm = r_pwm = 50;
}

void car_go()
{
//李工提醒：千万不要超过280，出事了李工不负责任,但是贴心的李工怕你们叛逆特意加了car_limit以限制
    l_pwm = r_pwm = 100;
}

void car_right()
{
    l_pwm = 90;
    r_pwm = 45;
}

void car_left()
{
    l_pwm = 45;
    r_pwm = 90;
}

void car_stop()
{
    l_pwm = 0;
    r_pwm = 0;
}

void car_zhuan_left()
{
    l_pwm = 0;
    r_pwm = 20;
}

void car_zhuan_right()
{
    l_pwm = 20;
    r_pwm = 0;
}
