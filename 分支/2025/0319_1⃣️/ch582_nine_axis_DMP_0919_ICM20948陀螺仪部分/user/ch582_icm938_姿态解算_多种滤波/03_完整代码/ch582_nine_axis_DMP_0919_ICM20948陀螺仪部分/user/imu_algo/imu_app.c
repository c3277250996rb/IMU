/*
 * imu_app.c
 *
 *  Created on: Jul 29, 2024
 *      Author: liu
 */
#include "imu_app.h"
#include "attitude.h"
#include "data_filter.h"
#include "math.h"
#include "icm949_driver.h"

#define MAG_OFFSET_TIMES    5000

ATT_Module attitude_user={0};

imu_t imu_9;


int16_t window_ax[WIN_NUM];
int16_t window_ay[WIN_NUM];
int16_t window_az[WIN_NUM];

int16_t window_gx[WIN_NUM];
int16_t window_gy[WIN_NUM];
int16_t window_gz[WIN_NUM];

int16_t window_mx[WIN_NUM];
int16_t window_my[WIN_NUM];
int16_t window_mz[WIN_NUM];

void imu_app_timer_1ms_callback(void)
{
    static float data1=0.0f;
    static uint16_t TimerCount = 0;
    static int algo_mode=1,calc_att_flag=0,calc_att_timer_cnt=0;
    TimerCount++;
    if(TimerCount >= imu_9.output_freq)  //1ms采集一次传感器数据  1KHz
            {
                if(imu_9.cali_flag==0) //校准时不采集输出
                {
//                    HAL_GPIO_TogglePin(TEST_GPIO_Port, TEST_Pin); //测试引脚PA7，可使用示波器测量，确定采样时间是否准确
                    calc_att_flag=1;
#if 0
                        data1=sqrt(imu_9.f_acc[0]*imu_9.f_acc[0]+imu_9.f_acc[1]*imu_9.f_acc[1]+imu_9.f_acc[2]*imu_9.f_acc[2]);
                        if((data1<0.95)||(data1>1.05))
                        {
                            calc_att_flag=1;
                            calc_att_timer_cnt=0;
                        }
                        else {
                            calc_att_timer_cnt++;
                            if(calc_att_timer_cnt>50)//500ms 稳定不计算
                            {
                                calc_att_timer_cnt=51;
                                calc_att_flag=0;
                            }
                        }
#endif
                        if(calc_att_flag)
                        {
                            calculate_attitude(&attitude, 0.001*10) ; //姿态解算
                        }

                    imu_9.mag_yaw_test = atan2(imu_9.f_mag[1],imu_9.f_mag[0])*57.295f;
                    imu_9.time_tick++;
                    data_output_mode(imu_9.output_mode);  //数据输出,vofa+查看曲线图形
                }

                TimerCount = 0;
            }

}
bool imu_sample_data(void)
{
    bool ret;
    ret= readSensor();
    getAccRawValuesInt16(imu_9.i_acc);
    getGyrRawValuesInt16(imu_9.i_gyro);
    getMagValuesInt16(imu_9.i_mag);


    return ret;
}
//将九轴数据转换为实际物理数据
void imu_data_transition(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,int16_t mx,int16_t my,int16_t mz)
{

    imu_9.f_acc[0] = (float)(ax-imu_9.acc_zero[0])/ 16393.0f;  // 加速度量程为:±2G      获取到的加速度计数据 除以 16393 ，可以转化为带物理单位的数据，单位：g(m/s^2)
    imu_9.f_acc[1] = (float)(ay-imu_9.acc_zero[1]) / 16393.0f;
    imu_9.f_acc[2] = (float)(az-imu_9.acc_zero[2]) / 16393.0f;

    imu_9.f_gyro[0] = (float)(gx-imu_9.gyro_zero[0]) / 57.1f; //  陀螺仪量程为:±500dps  获取到的陀螺仪数据除以 57.1，    可以转化为带物理单位的数据，单位为：°/s
    imu_9.f_gyro[1] = (float)(gy-imu_9.gyro_zero[1]) / 57.1f;
    imu_9.f_gyro[2] = (float)(gz-imu_9.gyro_zero[2]) / 57.1f;
#if 1
    imu_9.f_mag[0] = (float)(imu_9.mag_xsf*mx+imu_9.mag_zero[0]);
    imu_9.f_mag[1] = (float)(imu_9.mag_ysf*my+imu_9.mag_zero[1]);
    imu_9.f_mag[2] = (float)(mz+imu_9.mag_zero[2]) * 0.1495f;
#elif 1
    imu_9.f_mag[0] = (float)(imu_9.mag_xsf*mx) * 0.1495f;
        imu_9.f_mag[1] = (float)(imu_9.mag_ysf*my) * 0.1495f;//单位转为高斯G
        imu_9.f_mag[2] = (float)(mz) * 0.1495f;

#endif



}
bool imu_final_data_get(void)
{
    int16_t filter_ax ,filter_ay,filter_az ;
    int16_t filter_gx ,filter_gy,filter_gz ;
    int16_t filter_mx ,filter_my,filter_mz ;

    bool ret;

    ret= imu_sample_data(); //采样得到九轴原始数据

      //原始数据窗口滤波
      filter_ax = window_filter(imu_9.i_acc[0],window_ax,WIN_NUM);
      filter_ay = window_filter(imu_9.i_acc[1],window_ay,WIN_NUM);
      filter_az = window_filter(imu_9.i_acc[2],window_az,WIN_NUM);

      filter_gx = window_filter(imu_9.i_gyro[0],window_gx,WIN_NUM);
      filter_gy = window_filter(imu_9.i_gyro[1],window_gy,WIN_NUM);
      filter_gz = window_filter(imu_9.i_gyro[2],window_gz,WIN_NUM);

      filter_mx = window_filter(imu_9.i_mag[0],window_mx,WIN_NUM);
      filter_my = window_filter(imu_9.i_mag[1],window_my,WIN_NUM);
      filter_mz = window_filter(imu_9.i_mag[2],window_mz,WIN_NUM);
      //转换成实际物理量
      imu_data_transition(filter_ax,filter_ay,filter_az,filter_gx,filter_gy,filter_gz,filter_mx,filter_my,filter_mz);

      return ret;

}
void set_acc_gyro_offset(void)
{
    uint8_t i;
    int16_t ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
    int32_t ax_offset_sum, ay_offset_sum, az_offset_sum, gx_offset_sum, gy_offset_sum, gz_offset_sum;
    int16_t *gyro_off,*acc_off;

    ax_offset_sum = 0;
    ay_offset_sum = 0;
    az_offset_sum = 0;
    gx_offset_sum = 0;
    gy_offset_sum = 0;
    gz_offset_sum = 0;

    for (i = 0; i < 20; i++)
   {
        imu_sample_data();
        gyro_off=imu_9.i_gyro;
        acc_off=imu_9.i_acc;


       ax_offset = acc_off[0];
       ay_offset = acc_off[1];
       az_offset = acc_off[2];
       ax_offset_sum = ax_offset_sum + ax_offset;
       ay_offset_sum = ay_offset_sum + ay_offset;
       az_offset_sum = az_offset_sum + az_offset;

       gx_offset = gyro_off[0];
       gy_offset = gyro_off[1];
       gz_offset = gyro_off[2];
       gx_offset_sum = gx_offset_sum + gx_offset;
       gy_offset_sum = gy_offset_sum + gy_offset;
       gz_offset_sum = gz_offset_sum + gz_offset;
   }
    imu_9.acc_zero[0] = ax_offset_sum/20;
    imu_9.acc_zero[1] = ay_offset_sum/20;
    imu_9.acc_zero[2] = az_offset_sum/20 - 16384;
    imu_9.gyro_zero[0] = gx_offset_sum/20;
    imu_9.gyro_zero[1] = gy_offset_sum/20;
    imu_9.gyro_zero[2] = gz_offset_sum/20;

    printf("set_acc_gyro_offset acc %d ,%d,%d ,gyro:%d,%d,%d \r\n",imu_9.acc_zero[0],imu_9.acc_zero[1],imu_9.acc_zero[2],
            imu_9.gyro_zero[0],imu_9.gyro_zero[1],imu_9.gyro_zero[2] );

    //存入flash
//    STMFLASH_Write(ACC_ZERO_ADDR,(uint8_t*)&imu_9.acc_zero,6);
//    STMFLASH_Write(GYRO_ZERO_ADDR,(uint8_t*)&imu_9.gyro_zero,6);

}

void vofa_FireWater_USB_output(float s1, float s2, float s3,float s4)
{
    printf("simples:%f,%f,%f,%f\n", s1, s2,s3,s4);
}
#define acc_gravity   0.0098//1000/9.8
void data_output_mode(uint8_t mode)  //数据输出模式,vofa+查看曲线图形
{
    float shake_data;
    switch(mode)
    {
    case 0:  //欧拉角输出
        vofa_FireWater_USB_output(attitude.data.rol, attitude.data.pitch, attitude.data.yaw,imu_9.mag_yaw_test);
//      vofa_FireWater_output(attitude.data.rol, attitude.data.pitch, attitude.data.yaw);
        break;
    case 1: //四元数输出
        vofa_FireWater_USB_output(attitude.process.quaternion[0],attitude.process.quaternion[1],attitude.process.quaternion[2],attitude.process.quaternion[3]);

        break;
    case 2://线性加速度输出
        vofa_FireWater_USB_output((attitude.data.acc_correct.x)*acc_gravity,attitude.data.acc_correct.y*acc_gravity,(attitude.data.acc_correct.z-1000.0f)*acc_gravity,2.0f);
        break;
    case 3://加速度 数据输出  单位：g(m/s^2)
        vofa_FireWater_USB_output(imu_9.f_acc[0],imu_9.f_acc[1],imu_9.f_acc[2],3.0f);
        break;
    case 4://陀螺仪数据输出  单位为：°/s
        vofa_FireWater_USB_output(imu_9.f_gyro[0],imu_9.f_gyro[1],imu_9.f_gyro[2],4.0f);
        break;
    case 5://地磁数据输出
        vofa_FireWater_USB_output(imu_9.f_mag[0],imu_9.f_mag[1],imu_9.f_mag[2],5.0f);
        break;
    case 6://地磁数据输出
        printf("simples:%f,%f,%f,%f,%f,%f,%f,%f,%f\n",imu_9.f_acc[0],imu_9.f_acc[1],imu_9.f_acc[2],imu_9.f_gyro[0],imu_9.f_gyro[1],imu_9.f_gyro[2],imu_9.f_mag[0],imu_9.f_mag[1],imu_9.f_mag[2]);
        break;
//      usb_printf("%f,%f,%f,%f,%f,%f,%f,%f,%f\n",imu_9.f_acc[0],imu_9.f_acc[1],imu_9.f_acc[2],imu_9.f_gyro[0],imu_9.f_gyro[1],imu_9.f_gyro[2],imu_9.f_mag[0],imu_9.f_mag[1],imu_9.f_mag[2]);
    case 7://加速度原始 数据输出
         vofa_FireWater_USB_output(imu_9.i_acc[0],imu_9.i_acc[1],imu_9.i_acc[2],7.0f);
        break;
    case 8://加速度原始 数据输出
             vofa_FireWater_USB_output(imu_9.i_gyro[0],imu_9.i_gyro[1],imu_9.i_gyro[2],8.0f);
         break;
    case 9://加速度原始 数据输出
           vofa_FireWater_USB_output(imu_9.f_acc[0],imu_9.f_acc[1],imu_9.f_acc[2],9.0f);
            break;
    case 10://加速度原始 数据输出
            shake_data=sqrt(imu_9.f_acc[0]*imu_9.f_acc[0]+imu_9.f_acc[1]*imu_9.f_acc[1]+imu_9.f_acc[2]*imu_9.f_acc[2]);
            printf("simples:%f,%f \r\n",shake_data,10.0f);
              break;

    default:

        break;
    }

}

int16_t mag_x_max=0,mag_x_min=0;
int16_t mag_y_max=0,mag_y_min=0;
int16_t mag_z_max=0,mag_z_min=0;

void mag_set_offset(void)
{

    uint32_t cnt = 0;
    uint8_t r_data[6];
    int16_t mag_off[3];
    int16_t Xoffset,Yoffset,Zoffset;

    static float Xsf;
    static float Ysf;

    int16_t xsf_buf,ysf_buf;

    while(cnt<MAG_OFFSET_TIMES)
    {
        cnt++;
//        BSP_I2C1_ReadReg(LIS2MDL_I2C_ADD, LIS2MDL_OUTX_L_REG, r_data, 6);  //读地磁原始数据
//        mag_off[0] = (int16_t)(r_data[0] + (r_data[1] << 8));
//        mag_off[1] = (int16_t)(r_data[2] + (r_data[3] << 8));
//        mag_off[2] = (int16_t)(r_data[4] + (r_data[5] << 8));
        readSensor();
        getMagValuesInt16(mag_off);


        if(mag_off[0]<mag_x_min)
            mag_x_min = mag_off[0];
        else if(mag_off[0]>mag_x_max)
            mag_x_max = mag_off[0];


        if(mag_off[1]<mag_y_min)
            mag_y_min = mag_off[1];
        else if(mag_off[1]>mag_y_max)
            mag_y_max = mag_off[1];

            if(mag_off[2]<mag_z_min)
                mag_z_min = mag_off[2];
        else if(mag_off[2]>mag_z_max)
            mag_z_max = mag_off[2];

        //1ms
        mDelaymS(2);
        printf("mag_set_offset cnt %d \r\n",cnt);
    }
    cnt = 0;

    Xsf = (mag_y_max - mag_y_min) / (mag_x_max - mag_x_min);
    Ysf = (mag_x_max - mag_x_min) / (mag_y_max - mag_y_min);

    if (Xsf < 1)
    {
        Xsf = 1;
    }

    if (Ysf < 1)
    {
        Ysf = 1;
    }

    Xoffset = ( (mag_x_max - mag_x_min)/2 - mag_x_max) *Xsf;
    Yoffset = ( (mag_y_max - mag_y_min)/2 - mag_y_max) *Ysf;
//    Zoffset = ( (mag_z_max-mag_z_min)/2 - mag_z_max) *Xsf;

    imu_9.mag_xsf = Xsf;
    imu_9.mag_ysf = Ysf;

    imu_9.mag_zero[0] = Xoffset;
    imu_9.mag_zero[1] = Yoffset;
//    imu_9.mag_zero[2] = Zoffset;
    imu_9.mag_zero[2] = 0.0f;

    xsf_buf =(uint16_t) (Xsf*1000);
    ysf_buf =(uint16_t) (Ysf*1000);

        //存入
    printf("mag_set_offset offect zero %d,%d \r\n",Xoffset,Yoffset);

//    STMFLASH_Write(MAG_ZERO_ADDR,(uint8_t*)&imu_9.mag_zero,6);
//    STMFLASH_Write(MAG_OFFSET_XSF_ADDR,(uint8_t*)&xsf_buf,2);
//    STMFLASH_Write(MAG_OFFSET_YSF_ADDR,(uint8_t*)&ysf_buf,2);

}

void imu_9_shell_cmd_to_do(void)
{
    if(imu_9.shell_cmd_ok==1)
    {
        imu_9.shell_cmd_ok = 0;
        if(imu_9.cali_flag!=0)  //校准
        {
            if(imu_9.cali_flag==1)
            {
                set_acc_gyro_offset();
                imu_9.cali_flag = 0;
            }
            else if(imu_9.cali_flag==2)
            {
                mag_set_offset();
                imu_9.cali_flag = 0;
            }
        }
        if(imu_9.output_flag!=0)  //数据输出
        {
//            STMFLASH_Write(OUTPUT_ADDR,&imu_9.output_mode,1);
            imu_9.output_flag = 0;
        }

    }

}

int imu_app_init(void)
{

  init_attitude(&attitude);

  imu_9.output_freq = 1;
  imu_9.output_mode = 0; //默认欧拉角输出

  imu_9.mag_xsf = 1; //读取磁力计校准系数
  imu_9.mag_ysf = 1;
  imu_9.use_mag=attitude.flag.use_mag;

  return 0;
}
