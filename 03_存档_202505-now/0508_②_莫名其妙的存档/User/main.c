#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "motor.h"
#include "encoder.h"
#include "serial.h"
#include "pwm.h"
#include "key.h"
#include "test.h"
#include "my_i2c.h"
// #include "MPU6050.h"
// #include "_I2C_.h"

int main(void)
{
    tim_1_base_init();                          //基本定时器中断
    pwm_init();                                 //pwm变速
    motor_init();                               //电机
    encoder_init();                             //编码器
    Serial1_Init();                              //串口
    Serial3_Init();
    
    test();

    while (1)
    {
        
    }
}
// 定时器中断
void TIM1_UP_IRQHandler(void)
{
    static uint16_t count;
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        car_state_machine_run();
        count++;
        if (count >= 40)
        {
            count = 0;
            // 获取编码器速度，并进行 PID 更新
            pid_update(&motor_left_pid, encoder1_get());
            pid_update(&motor_right_pid, encoder2_get());
            // 根据 PID 输出控制 PWM, 并且取绝对值，因为电机正反转已经体现在了电机两边的 1 - 0 上取反了，不知道可爱的你有没有 get 到；
            motor_pwm(MY_FABSF( motor_left_pid.out), MY_FABSF( motor_right_pid.out));
            
        }
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

