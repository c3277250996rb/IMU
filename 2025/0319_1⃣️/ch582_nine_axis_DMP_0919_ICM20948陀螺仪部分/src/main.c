/********************************** (C) COPYRIGHT *******************************
 * File Name          : Main.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : 串口1收发演示
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "main.h"
#include "icm949_driver.h"
#include "imu_app.h"
#include "user_uart.h"
#include "user_shell.h"

#include "stm32_icm20948.h"

#define RAD2DEG     57.2957795131

uint8_t TxBuff[] = "This is a tx exam\r\n";
uint8_t RxBuff[10];
uint8_t trigB;
extern imu_t imu_9;
extern uart_rev_str uart_info;

static uint32_t system_timer_tick=0;
uint32_t get_system_timer_tick_ms(void)
{
    return system_timer_tick;
}
bool do_delay_millis_time(uint32_t interval, uint32_t *previousMillis, bool state)
{
    uint32_t currentMillis = get_system_timer_tick_ms();
    if (currentMillis - *previousMillis >= interval)
    {
        *previousMillis = currentMillis;
        state = !state;
    }
    return state;
}

STM32ICM20948Settings icmSettings =
{
  .mode = 1,                     // 0 = low power mode, 1 = high performance mode
  .enable_gyroscope = true,      // Enables gyroscope output
  .enable_accelerometer = true,  // Enables accelerometer output
  .enable_magnetometer = true,   // Enables magnetometer output
  .enable_quaternion = true,     // Enables quaternion output
  .gyroscope_frequency = 100,      // Max frequency = 225, min frequency = 1
  .accelerometer_frequency = 100,  // Max frequency = 225, min frequency = 1
  .magnetometer_frequency = 100,   // Max frequency = 70, min frequency = 1
  .quaternion_frequency = 100     // Max frequency = 225, min frequency = 50
};

void quat2euler(float q0, float q1, float q2, float q3, float *r, float *p, float *y)
{
    // Roll (x-axis rotation)
    float sinr_cosp = +2.0 * (q0 * q1 + q2 * q3);
    float cosr_cosp = +1.0 - 2.0 * (q1 * q1 + q2 * q2);
    *r = atan2(sinr_cosp, cosr_cosp);

    // Pitch (y-axis rotation)
    float sinp = +2.0 * (q0 * q2 - q3 * q1);
    if (fabs(sinp) >= 1)
        *p = copysign(33.14159265358979323846 / 2, sinp); // use 90 degrees if out of range
    else
        *p = asin(sinp);

    // Yaw (z-axis rotation)
    float siny_cosp = +2.0 * (q0 * q3 + q1 * q2);
    float cosy_cosp = +1.0 - 2.0 * (q2 * q2 + q3 * q3);
    *y = atan2(siny_cosp, cosy_cosp);
}


/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
bool imu_stats=false;
int main()
{

    uint32_t run_timer=0;

    SetSysClock(CLK_SOURCE_PLL_80MHz);
    /* 配置串口1：先配置IO口模式，再配置串口 */
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);      // RXD-配置上拉输入
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
    UART1_DefInit(500000);

#if 1 // uart irq enable
    UART1_ByteTrigCfg(UART_1BYTE_TRIG);
    trigB = 1;
    UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
    PFIC_EnableIRQ(UART1_IRQn);
#endif
    GPIOB_SetBits(GPIO_Pin_4);//GPIOA_InverseBits(GPIO_Pin_4);
    GPIOB_ModeCfg(GPIO_Pin_4, GPIO_ModeOut_PP_5mA); //led

    /********************imu init********************************/
#if 1
    imu_stats= icm20948_init();//icm20948 init,六轴，地磁初始化
   // imu_app_init();//融合算法初始化
#endif
    /********************************************************/

    printf("Ch58x Nine Axis %s %s\r\n",__DATE__,__TIME__);

    user_uart_init();
    user_shell_init();//uart debug cmd

#if 1 //定时器干扰iic,需要设定10ms
    {
        TMR0_TimerInit(FREQ_SYS / 200);         // 设置定时时间 10ms,100 ,,5ms 200
        TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END); // 开启中断
        PFIC_EnableIRQ(TMR0_IRQn);
    }
#endif
    imu_9.output_mode=0;// 默认数据输出模式,,,vofa+查看曲线图形


    ICM20948_init(icmSettings);

    while(1)
    {


        float gyro_x, gyro_y, gyro_z;
        float accel_x, accel_y, accel_z;
        float mag_x, mag_y, mag_z;
        float quat_w, quat_x, quat_y, quat_z;
        float roll, pitch, yaw;
        char sensor_string_buff[128];

        // Must call this often in main loop -- updates the sensor values
        ICM20948_task();

        if (ICM20948_gyroDataIsReady()) {
        ICM20948_readGyroData(&gyro_x, &gyro_y, &gyro_z);
        sprintf(sensor_string_buff, "Gyro (deg/s): [%f,%f,%f]\r\n", gyro_x, gyro_y, gyro_z);
        //HAL_UART_Transmit(&ICM20948_UART, sensor_string_buff, strlen(sensor_string_buff), 100);
        }

        if (ICM20948_accelDataIsReady()) {
        ICM20948_readAccelData(&accel_x, &accel_y, &accel_z);
        sprintf(sensor_string_buff, "Accel (g): [%f,%f,%f]\r\n", accel_x, accel_y, accel_z);
        //HAL_UART_Transmit(&ICM20948_UART, sensor_string_buff, strlen(sensor_string_buff), 100);
        }

        if (ICM20948_magDataIsReady()) {
        ICM20948_readMagData(&mag_x, &mag_y, &mag_z);
        sprintf(sensor_string_buff, "Mag (uT): [%f,%f,%f]\r\n", mag_x, mag_y, mag_z);
        //HAL_UART_Transmit(&ICM20948_UART, sensor_string_buff, strlen(sensor_string_buff), 100);
        }

        if (ICM20948_quatDataIsReady())
        {
        ICM20948_readQuatData(&quat_w, &quat_x, &quat_y, &quat_z);
        // sprintf(sensor_string_buff, "Quat (deg): [%f,%f,%f,%f]\r\n", quat_w, quat_x, quat_y, quat_z);
        // HAL_UART_Transmit(&ICM20948_UART, sensor_string_buff, strlen(sensor_string_buff), 100);

        quat2euler(quat_w, quat_x, quat_y, quat_z, &roll, &pitch, &yaw);
        sprintf(sensor_string_buff, "Attitude (deg): [%f,%f,%f]\r\n", roll*RAD2DEG, pitch*RAD2DEG, yaw*RAD2DEG);
           // vofa_FireWater_USB_output(roll*RAD2DEG, pitch*RAD2DEG, yaw*RAD2DEG,0);
        vofa_FireWater_USB_output(quat_w, quat_x, quat_y,quat_z);
        //usart1_report_imu((int)(roll*RAD2DEG*100),(int)(pitch*RAD2DEG*100),(int)(yaw*RAD2DEG*100),0,0,0);
        // HAL_UART_Transmit(&ICM20948_UART, sensor_string_buff, strlen(sensor_string_buff), 100);
        }

//        if(imu_stats)
//        {
//             if(run_timer!=system_timer_tick)
//            {
//                 run_timer=system_timer_tick;
//                /**************imu thread 10ms,不可放在中断内*****************************/
//                imu_stats= imu_final_data_get(); //原始数据采样
//                imu_app_timer_1ms_callback();//不能放到定时器内 ,此函数需要10ms 运行一次,,
//                /*********************imu thread end***************************/
//                GPIOB_InverseBits(GPIO_Pin_4);//led debug test
//            }
//
//        }
//        else {
//            data_output_mode(0xff);  //iic 通信失败，关闭输出
//            GPIOB_ResetBits(GPIO_Pin_4);//GPIOA_InverseBits(GPIO_Pin_4);
//        }
//        imu_9_shell_cmd_to_do();

        if(uart_info.ReceiveComplete==1)//shell cmd 一包数据解析
        {
            uart_info.ReceiveComplete=0;
            user_shell_recive_bytes(uart_info.ReceiveBuff, uart_info.ReceiveNum);
        }
    }

}

/*********************************************************************
 * @fn      UART1_IRQHandler
 *
 * @brief   UART1中断函数
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void UART1_IRQHandler(void)
{
    volatile uint8_t i;

    switch(UART1_GetITFlag())
    {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART1_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
//            for(i = 0; i != trigB; i++)
//            {
                RxBuff[0] = UART1_RecvByte();
//                UART1_SendByte(RxBuff[i]);
//            }
            user_uart_receive_byte_handle(RxBuff[0]);
            break;

        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
//            i = UART1_RecvString(RxBuff);
//            user_uart_receive_byte_handle(RxBuff[0]);
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;

        case UART_II_MODEM_CHG: // 只支持串口0
            break;

        default:
            break;
    }
}

__INTERRUPT
__HIGH_CODE
void TMR0_IRQHandler(void) // TMR0 定时中断
{
    if(TMR0_GetITFlag(TMR0_3_IT_CYC_END))
    {
        TMR0_ClearITFlag(TMR0_3_IT_CYC_END); // 清除中断标志
        system_timer_tick++;
//        imu_app_timer_1ms_callback();
        if(system_timer_tick%100==0)
        GPIOB_InverseBits(GPIO_Pin_4);

        user_uart_data_timeout();

    }
}



