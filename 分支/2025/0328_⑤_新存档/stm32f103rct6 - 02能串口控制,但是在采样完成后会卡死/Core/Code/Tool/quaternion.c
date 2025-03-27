#include "quaternion.h"

float euler_angle[3];
float buffer_9axes[9];
volatile float q0=1, q1=0, q2=0, q3=0; 

int16_t *quater_accel = lsm6ds3_accel;
int16_t *quater_gyro = lsm6ds3_gyro;
int16_t *quater_mag = qmc5883_mag;

#define debug_quaternion
char* debug_quaternion_char;

#define use_DM_Write
float DM_angle_record[Cache_Max_Row][Info_Column] = {0};  // 给 data management 用的二维数组
uint8_t Q_current_index = 0;
uint8_t is_Q_stop = enum_Stop;

// #define Q_switch_mode
// enum
// {
//     enum_Static = 0,
//     enum_Dynamic
// };
// int Q_mode_flag = enum_Static;
// int Q_threshold[3] = {1,1,1};  // threshold
// int16_t mag_zero[3] = {0,0,0};


float invSqrt(float x);
void imu_get_quater(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);


/**
 * @brief  From quaternion to euler angles.
 * @param  None
 * @retval None
 */
void imu_get_euler(void){
  imu_convert_9axes();

/* X - roll - 横滚角 */
    euler_angle[0] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
/* ---------------- */

/* Y - pitch - 仰俯角 */
    euler_angle[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
/* ----------------- */

/* Z - yaw - 偏航角 */
    euler_angle[2] = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3;   // yaw
/* --------------- */
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float generate_random_float(float min, float max) {
    return min + (float)rand() / RAND_MAX * (max - min);
}

/**
 * @brief  Convert 9 axes data and pass to imu_get_quater().
 * @param  None
 * @retval None
 */
void imu_convert_9axes(void){

    int i;
    for(i = 0; i < 3; i++){
        buffer_9axes[i] = (float) (quater_accel[i] * QUATRE_LSB_ACCEL);
    }
    for(i = 0; i < 3; i++){
        buffer_9axes[i + 3] = (float) (quater_gyro[i] * QUATER_LSB_GYRO);
    }
    for(i = 0; i < 3; i++){
        buffer_9axes[i + 6] = (float) (quater_mag[i] * QUATER_LSB_MAG);
    }

    #ifdef use_DM_Write
        for(i = 0; i < 3; i++){
            // 生成 [1.0, 90.0) 范围内的随机浮点数
            // euler_angle[i] = generate_random_float(1.0f, 90.0f);
        }
        if(is_Q_stop == enum_Ready){
            is_Q_stop = eunm_Continuous;
        }else if((is_Q_stop == eunm_Continuous) && (Q_current_index < Cache_Max_Row)){
            DM_angle_record[Q_current_index][0] = (((float) (FATFS_Index_Current - 10000))) * 100000.0F + (float) Q_current_index;
            DM_angle_record[Q_current_index][1] = euler_angle[0];
            DM_angle_record[Q_current_index][2] = euler_angle[1];
            DM_angle_record[Q_current_index][3] = euler_angle[2];
            // DM_angle_record[Q_current_index][4] = // 时间戳;
            Q_current_index++;
        }else{
            for(i = 0; i < Cache_Max_Row; i++){
                DM_Write(DM_angle_record[i], i);
            }
            is_Q_stop = enum_Stop;
            memset(DM_angle_record, 0, sizeof(DM_angle_record));
            Q_current_index = 0;
        }
    #else
        #ifdef debug_quaternion
        // debug_quaternion_char = muti_printf_arrays(2, ",", "%f",
        //     euler_angle, 3,
        //     buffer_9axes, 9
        // );
        debug_quaternion_char = muti_printf_arrays(1, ",", "%f",
            &(buffer_9axes[3]), 3
        );
        printf("%s\r\n", debug_quaternion_char);
        #endif
    #endif

    #ifdef Q_switch_mode

    if(Q_mode_flag == enum_Static){
        quater_mag = qmc5883_mag;
    }
    if(Q_mode_flag == enum_Dynamic){
        quater_mag = mag_zero;
    }

    #endif

    imu_get_quater(
        buffer_9axes[0], buffer_9axes[1], buffer_9axes[2], 
        buffer_9axes[3], buffer_9axes[4], buffer_9axes[5], 
        buffer_9axes[6], buffer_9axes[7], buffer_9axes[8]
    );
}

/**
 * @brief  From 9 axes data to quaternion.
 * @param  ax: accel X
 * @param  ay: accel Y
 * @param  az: accel Z
 * @param  gx: gyro X
 * @param  gy: gyro Y
 * @param  gz: gyro Z
 * @param  mx: mag Z
 * @param  my: mag Y
 * @param  mz: mag Z
 * @retval None 
 */
void imu_get_quater(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz){

    float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float exInt = 0.0, eyInt = 0.0, ezInt = 0.0;
    float ex, ey, ez, halfT = 0.024f;

    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;   
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;          

/* 归一化 */
  /* 求模 */
    norm = invSqrt(ax * ax + ay * ay + az * az);       
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
  /* 求模 */
    norm = invSqrt(mx * mx + my * my + mz * mz);          
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm;

/* 磁场水平 垂直分量计算 */
  /* 旋转矩阵 */
    hx = 2 * mx * (0.5f - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
    hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5f - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
    hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5f - q1q1 - q2q2);         
  /* 计算水平和垂直分量 */
    bx = sqrt((hx * hx) + (hy * hy));
    bz = hz;     

/* 将重力从物体坐标系转换到世界坐标系 */
  /* 计算分量 */
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

/* 将磁力从物体坐标系转换到世界坐标系 */
  /* 计算分量 */
    wx = 2 * bx * (0.5 - q2q2 - q3q3) + 2 * bz * (q1q3 - q0q2);
    wy = 2 * bx * (q1q2 - q0q3) + 2 * bz * (q0q1 + q2q3);
    wz = 2 * bx * (q0q2 + q1q3) + 2 * bz * (0.5 - q1q1 - q2q2);  

/* 计算误差 叉积*/
  /* 通过加速计观测值 世界磁力计 世界重力 */
    ex = (ay * vz - az * vy) + (my * wz - mz * wy);
    ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
    ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

    if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
/* 积分 累加误差 Ki*/
        exInt = exInt + ex * Ki * halfT;
        eyInt = eyInt + ey * Ki * halfT;	
        ezInt = ezInt + ez * Ki * halfT;

/* 消除误差 */
        gx = gx + Kp * ex + exInt;
        gy = gy + Kp * ey + eyInt;
        gz = gz + Kp * ez + ezInt;
    }

/* 更新四元数 */
    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;  

    norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 = q0 * norm;
    q1 = q1 * norm;
    q2 = q2 * norm;
    q3 = q3 * norm;
}

/**
  * @brief  fast inverse square root (快速计算倒平方根)
  * @param  x: Float
  * @retval Float
  */
float invSqrt(float x) 
{
    float halfx = 0.5f * x;
    float y = x;
    
    long i = *(long*)&y;                //get bits for floating value
    i = 0x5f3759df - (i >> 1);          //gives initial guss you
    y = *(float*)&i;                    //convert bits back to float
    y = y * (1.5f - (halfx * y * y));   //newtop step, repeating increases accuracy
    
    return y;
}


void quaternion_self_test(void){

    QMC5883_Update_Data();
    LSM6DS3_Update_Data();

    imu_get_euler();
    #ifndef debug_quaternion
    debug_quaternion_char = muti_printf_arrays(2, ",", "%f",
        euler_angle, 3,
        buffer_9axes, 9
    );
    printf("%s\r\n", debug_quaternion_char);
    #endif
    delay_us(10);
}


/**
 * WB
 * 1.开始时使用静态算法
 * 2.开启窗口滤波, 计算窗口均值, 若遇到突变点则切换为动态算法
 * 3.动态算法最多持续?s, 并在持续期间关闭窗口滤波, 事件结束后开启窗口滤波
 */

