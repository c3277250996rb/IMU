#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "motor.h"
#include "encoder.h"
#include "serial.h"
#include "pwm.h"
#include "key.h"
#include "_I2C_.h"
#include "MPU6050.h"
#include "Kalman_filter.h"
#include "Sensor.h"
MPU6050 MM;
void oled_debug(void);
void task_1(void);
void task_2(void);
void task_3(void);
//左电机pid
extern float l_pwm, l_speed, l_out;
//右电机pid
extern float r_pwm, r_speed, r_out;



int main(void)
{
    static uint16_t flag1 = 0;
    float newroll = 0;
    float newpitch = 0;
    KalmanFilter KK1;
    KalmanFilter KK2;
    KalmanFilter_Init(&KK2,0,0.01,0.1);
    KalmanFilter_Init(&KK1,0,0.01,0.1);
    
    tim_base_init();                            //基本定时器中断
    OLED_Init();                                //oled调式
    motor_init();                               //电机
    encoder_init();                             //编码器
    Serial_Init();                              //串口
    pwm_init();                                 //pwm变速
    key_init();
    MPU6050_init(GPIOB,GPIO_Pin_10,GPIO_Pin_11);
    Sensor_Init();

    while (1)
    {

        MPU6050_Get_Angle(&MM);
        newroll = KalmanFilter_Update(&KK1,MM.roll);
        newpitch = KalmanFilter_Update(&KK2,MM.pitch);
        Serial_Printf("%f,%f,%f\r\n", newroll, newpitch, MM.yaw);
        motor_go();
        oled_debug();
//        task_1();
//        task_2();
        task_3();
    }
}


void TIM1_UP_IRQHandler(void)
{
    static uint16_t count;
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        count ++;
        if(count >= 40)//这里不要改，改了后面限制也要改
        {
            count = 0;
            pid_left();
            pid_right();
            motor_pwm(l_out, r_out);
        }
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}



void task_1()
{
    car_go();
    if((L1 == 1)||(M0 == 1) || (R1 == 1) || (R2 == 1) || (L2 == 1))
    {
        car_stop();
    }
}



static int flag1 = 0;
void task_2()
{ 
    if(flag1 == 0) 
    {
        car_go_slow();
        if((L1 == 1)||(M0 == 1) || (R1 == 1) || (R2 == 1) || (L2 == 1)) flag1 = 1;
    }
    if(flag1 == 1) car_ggg();
}

void task_3()
{
    if(flag1 == 0)
    {
        car_go_slow();
        if((L1 == 1)||(M0 == 1) || (R1 == 1) || (R2 == 1) || (L2 == 1)) flag1 = 1;
    }
    
    else if(flag1 == 1)
    {
        car_ggg();
        if((L1 == 0&&M0 == 0) && (R1 == 0) && (R2 == 0) && (L2 == 0)) flag1 = 2;
    }
    
    else if(flag1 == 2)
    {
        if(MM.yaw > -62) car_zhuan_left();
        if(MM.yaw < -67) car_zhuan_right();
        if((MM.yaw < -62) && (MM.yaw > -67)) car_go_slow();
        if((L1 == 1)||(M0 == 1) || (R1 == 1) || (R2 == 1) || (L2 == 1)) flag1 = 3;
    }
    else if(flag1 == 3)
    {
        car_ggg();
        if((L1 == 0&&M0 == 0) && (R1 == 0) && (R2 == 0) && (L2 == 0)) flag1 = 4;
    }
    else if(flag1 == 4)
    {
        car_zhuan_right();
        if((MM.yaw < 15) && (MM.yaw > 10)) car_go_slow();
        if(MM.yaw > 15) car_zhuan_left();
        if(MM.yaw < 10) car_zhuan_right();
    }
}

void oled_debug()
{
    OLED_Clear();
    OLED_ShowFloatNum(0, 0, MM.yaw, 2, 2, OLED_8X16);
    OLED_ShowFloatNum(60, 0, MM.yaw, 2, 2, OLED_8X16);
    OLED_ShowFloatNum(0, 40, MM.yaw, 2, 2, OLED_8X16);
    OLED_ShowFloatNum(60, 40, MM.yaw, 2, 2, OLED_8X16);
    OLED_Update();
}

