#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

#define motor_gpio                      GPIOA
#define motor1_pin_1                    GPIO_Pin_2
#define motor1_pin_2                    GPIO_Pin_3
#define motor2_pin_1                    GPIO_Pin_4
#define motor2_pin_2                    GPIO_Pin_5
//绝对值
#define MY_FABSF(x) ((x) < 0.0f ? -(x) : (x))


// PID结构体定义
typedef struct {
    float pwm;        // 人为给定目标速度
    float speed;      // 编码器返回实际速度
    float out;        // PID输出
    float Kp, Ki, Kd;
    float error0, error1;
    float error_integral;
} PID_TypeDef;

typedef enum {
    CAR_STATE_STOP = 0,
    CAR_STATE_FORWARD,
    CAR_STATE_LEFT,
    CAR_STATE_RIGHT,
    CAR_STATE_TURN_LEFT,
    CAR_STATE_TURN_RIGHT,
    CAR_STATE_BACKWARD,
    CAR_STATE_MAX
} CarState_t;

extern PID_TypeDef motor_left_pid;
extern PID_TypeDef motor_right_pid;
extern uint8_t car_flag;
extern uint8_t pwm_set;
extern CarState_t current_state;

void motor_init(void);
void motor_polirity_set(void);
void pid_update(PID_TypeDef* pid, float speed_measurement);
void car_pwm_set(float left_pwm, float right_pwm, uint8_t flag);
void car_state_machine_run(void);
void car_set_state(CarState_t state);

#endif
