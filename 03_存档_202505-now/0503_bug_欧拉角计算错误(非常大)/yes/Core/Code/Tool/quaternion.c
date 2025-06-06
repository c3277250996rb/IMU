#include "quaternion.h"
#include <math.h>

float euler_angle[3];
float buffer_9axes[9];
volatile float q0=1, q1=0, q2=0, q3=0; 

#if LSM_for_1_and_MPU_for_0
    int16_t *quater_accel = lsm6ds3_accel;
    int16_t *quater_gyro = lsm6ds3_gyro;
    int16_t *quater_mag = qmc5883_mag;
#else
    int16_t *quater_accel = mpu9250_accel;
    int16_t *quater_gyro = mpu9250_gyro;
    int16_t *quater_mag = mpu9250_mag;
#endif

char* debug_quaternion_char;

#ifdef use_DM_Write
float Q_Record_Array[Max_Q_Row][Max_Q_Column] = {0};  // 给 data management 用的二维数组
uint8_t Q_Record_Index = 0;
uint8_t Q_Record_Flag = F_Q_Stop;
#endif

// #define Debug_The_Fucking_Index_With_Float_Int_And_Long
#ifdef Debug_The_Fucking_Index_With_Float_Int_And_Long
float f_index = 0.0F;
#endif

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
        // buffer_9axes[i + 6] = (float) (quater_mag[i] * QUATER_LSB_MAG);
        buffer_9axes[i + 6] = 0.0F;
    }

    #ifdef use_DM_Write
        write_imu_array_to_file();
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

    // mx = 0.0F; my = 0.0F; mz = 0.0F; 

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

/**
 * @brief  将IMU数组写入文件
 * @param  None
 * @retval None
 */
#ifdef use_DM_Write
void write_imu_array_to_file(void){
    /* (调试用) 生成 [1.0, 90.0) 范围内的随机浮点数 */
        // for(i = 0; i < 3; i++){
        //     euler_angle[i] = generate_random_float(1.0f, 90.0f);
        // }
    /* ------------------------------------------- */

    // [b.1] 如果是静止
    if(F_WF_Reset == WF_Status){
        /* 判断开始: 静止 -> 运动 */
        WF_Judge_From_Static_To_Dynamic(buffer_9axes[0]);
    }

    // [b.2] 如果是运动
    if(F_WF_Trigger == WF_Status || F_WF_AddUp == WF_Status){
        /* 判断结束: 运动 -> 静止 */
        WF_Judge_From_Dynamic_To_Static(buffer_9axes[0]);

    // [b.3] 如果运动结束了
        if(F_WF_Finish == WF_Status){
            Q_Record_Flag = F_Q_Stop;
            WF_Status = F_WF_Reset;
        }

    // [b.4] 如果正在运动 或者 被强制停止了
        if(F_Q_Continuous == Q_Record_Flag || F_Q_Stop == Q_Record_Flag){
        // [a.1] 如果赋值赋满了就写进去 (或者被强制停止了)
            if(Max_Q_Row <= Q_Record_Index || F_Q_Stop == Q_Record_Flag){
                for(int i = 0; i < Max_Q_Row; i++){
                    DM_Write(Q_Record_Array[i], i);
                }
                Q_Record_Flag = F_Q_Continuous;
                memset(Q_Record_Array, 0, sizeof(Q_Record_Array));
                Q_Record_Index = 0;
                    #ifdef Debug_The_Fucking_Index_With_Float_Int_And_Long
                    f_index = 0.0F;
                    #endif
                return;
            }

        // [a.2] 数组赋值
                #ifdef Debug_The_Fucking_Index_With_Float_Int_And_Long
                float temp_f = float_fs_index_current * 100000.0F + f_index;
                printf("float_fs_index_current: %f\r\n", float_fs_index_current);
                printf("f_index: %f\r\n", f_index);
                printf("temp_f: %f\r\n", temp_f);
                Q_Record_Array[Q_Record_Index][0] = temp_f;
                #else
            Q_Record_Array[Q_Record_Index][0] = (((float) (FATFS_Index_Current - 10000))) * 100000.0F + (float) Q_Record_Index;
            #endif
            Q_Record_Array[Q_Record_Index][1] = euler_angle[0];
            Q_Record_Array[Q_Record_Index][2] = euler_angle[1];
            Q_Record_Array[Q_Record_Index][3] = euler_angle[2];
            // Q_Record_Array[Q_Record_Index][4] = // 时间戳;

        // [a.3] index自增 (取余 防止索引越界)
            Q_Record_Index = (Q_Record_Index + 1) % Max_Q_Row;
                #ifdef Debug_The_Fucking_Index_With_Float_Int_And_Long
                f_index = (float) (((int)f_index + 1) % Max_Q_Row);
                #endif
        }

        // [a.4] 打印信息
        /* info */ char *temp_string = multi_printf_arrays(1, ",", "%f",
        /* info */     euler_angle, 3
        /* info */ );
        /* info */ printf("采集: %3d %s\r\n", Q_Record_Index, temp_string);
    }
}
#endif


void quaternion_self_test(void){

    if(LSM_for_1_and_MPU_for_0){
        QMC5883_Update_Data();
        LSM6DS3_Update_Data();
    }else{
        MPU9250_Update_Data();
    }

    imu_get_euler();
    #ifdef debug_quaternion
    debug_quaternion_char = multi_printf_arrays(2, ",", "%f",
        euler_angle, 3,
        buffer_9axes, 9
    );
    printf("%s\r\n", debug_quaternion_char);
    #endif
    delay_us(100 * 1000);
}


/**
 * WB
 * 1.开始时使用静态算法
 * 2.开启窗口滤波, 计算窗口均值, 若遇到突变点则切换为动态算法
 * 3.动态算法最多持续?s, 并在持续期间关闭窗口滤波, 事件结束后开启窗口滤波
 */

