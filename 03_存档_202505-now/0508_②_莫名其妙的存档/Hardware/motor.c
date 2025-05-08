#include "stm32f10x.h"
#include "pwm.h"
#include "motor.h"
#include "encoder.h"
#include "my_tool.h"


// 左右电机 PID 实例
PID_TypeDef motor_left_pid = { .Kp = 0.22f, .Ki = 0.25f, .Kd = 0.2f };
PID_TypeDef motor_right_pid = { .Kp = 0.18f, .Ki = 0.2f, .Kd = 0.18f };

CarState_t current_state = CAR_STATE_STOP;
uint8_t car_flag = 0;
uint8_t pwm_set = 70;

void motor_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = motor1_pin_1 | motor1_pin_2 | motor2_pin_1 | motor2_pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(motor_gpio, &GPIO_InitStructure);
}

void motor_polirity_set()
{
    if (motor_left_pid.pwm >= 0) {
        GPIO_SetBits(motor_gpio, motor1_pin_1);
        GPIO_ResetBits(motor_gpio, motor1_pin_2);
    } else {
        GPIO_ResetBits(motor_gpio, motor1_pin_1);
        GPIO_SetBits(motor_gpio, motor1_pin_2);
    }

    if (motor_right_pid.pwm >= 0) {
        GPIO_SetBits(motor_gpio, motor2_pin_1);
        GPIO_ResetBits(motor_gpio, motor2_pin_2);
    } else {
        GPIO_ResetBits(motor_gpio, motor2_pin_1);
        GPIO_SetBits(motor_gpio, motor2_pin_2);
    }
}

void pid_update(PID_TypeDef* pid, float speed_encoder)
{
    pid->speed = speed_encoder;
    pid->error1 = pid->error0;
    pid->error0 = pid->pwm - pid->speed;
    if (pid->Ki != 0.0f)
        pid->error_integral += pid->error0;
    else
        pid->error_integral = 0.0f;
        // pid->error_integral = my_constrain(pid->error_integral, -100.0f, 100.0f);//加入限制防止积分爆炸
    pid->out = pid->Kp * pid->error0
             + pid->Ki * pid->error_integral
             + pid->Kd * (pid->error0 - pid->error1);
    pid->out = my_constrain(pid->out, -100.0f, 100.0f); // 输出限幅
    if (MY_FABSF(pid->out) < 5.0f) pid->out = 0.0f; //增加死区避免抖动

    //李工找不到bug，if强制来凑
    if (current_state == CAR_STATE_STOP) pid->out = 0.0f;
    
}

//定义一个函数指针类型 - 用于存放car动作函数于函数指针数组中
typedef void (*StateHandler_t)(void);

void car_pwm_set(float left_pwm, float right_pwm, uint8_t flag)
{
    motor_left_pid.pwm = left_pwm;
    motor_right_pid.pwm = right_pwm;
    car_flag = flag;
}

static void handle_stop()        { car_pwm_set(0.0f, 0.0f, 0); }
static void handle_forward()     { car_pwm_set(pwm_set, pwm_set, 1); }
static void handle_left()        { car_pwm_set(pwm_set / 2.0f, pwm_set, 2); }
static void handle_right()       { car_pwm_set(pwm_set, pwm_set / 2.0f, 3); }
static void handle_turn_left()   { car_pwm_set(-25.0f, 25.0f, 5); }
static void handle_turn_right()  { car_pwm_set(25.0f, -25.0f, 6); }
static void handle_backward()    { car_pwm_set(-pwm_set, -pwm_set, 4); }

static StateHandler_t state_handlers[] = {
    handle_stop,
    handle_forward,
    handle_left,
    handle_right,
    handle_turn_left,
    handle_turn_right,
    handle_backward
};


void car_state_machine_run()
{
    if (current_state < CAR_STATE_MAX) {
        state_handlers[current_state]();
    } else {
        // 错误处理
    }
}

// 状态设置函数
void car_set_state(CarState_t state) {
    if (state < CAR_STATE_MAX) {
        current_state = state;
    } else {
        // 错误处理
    }
}
