
#include "System.h"
#include <stdlib.h>

int16_t TestmagnBuff[9] = {0};
int16_t an[2][3];
float gx, gy, gz;
int i = 0;
int j = 0;
int k = 0;
int while_i = 0;

int timeout_count = 0, timeout_standard = 50, flag = 1;
int32_t calc_o_error[6];
int32_t ce[6];	// ?因为az已经达到了-16000, 所以最低得int32_t, 又因为变成int64_t后ax莫名出错, 所以只能选int32_t
int32_t ce_5;	// ?因为az已经达到了-16000, 所以最低得int32_t, 又因为变成int64_t后ax莫名出错, 所以只能选int32_t
float dd[3];
double wa[6];
double woe[3];
double sv[3][2];
double sd[4];
int32_t global_times = 1;
double gT = 0.002;


bool Flag_Check_Magn = false;
extern uint32_t KeyValue;
extern MPU9250_TypeDef_Off MPU9250_Magn_Offset;
void TestMagn(void);

void mag();
void mag_init();
void lsm();
void lsm_init();
void release_gravity();
void update_data();
void is_move();
void calc_error();
void demo_data();
void rotate_acceleration();
void calc_velocity();
void calc_sum_distance();

int main(void)
{

	System_Init(); // 系统初始化 --- 串口波特率115200 I2C初始化 传感器初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	LedGpioConfiguration();
	KEYGpioConfiguration();
	GPIO_SetBits(IO_LED_ARRAY, IO_RS);
	GPIO_SetBits(IO_LED_ARRAY, IO_sysLED); // IO_LED1 输出高电平 LED1灭

	mag_init();
	lsm_init();
	Delay_ms(10);

#ifndef lsm
	a = 0.061 / 1000.0 * 9.8066500286389;
	g = 1.0 / 32.8;
#else
	a = 0.061 * 1.000 / 1000.000;
	g = 4.375 * 2.000 / 1000.000;
#endif

	while (1)
	{		
			update_data();
			// demo_data();

			if(i > 90){		
				wa[3] = wa[0];
				wa[4] = wa[1];
				wa[5] = wa[2];
				rotate_acceleration((double)an[1][0], (double)an[1][1], (double)an[1][2], (double)angles[2], (double)angles[1], (double)angles[0], &wa[0], &wa[1], &wa[2]);
				calc_sum_distance();
			}

			if (((abs(an[1][0] - an[0][0]) > 50) ||
				(abs(an[1][1] - an[0][1]) > 50) ||
				(abs(an[1][2] - an[0][2]) > 50)))
				{
					flag = 3;
				}

			if(flag != 3){
				calc_error();
			}

			i++;
			
			// calc_error();

			IMU_SampleFlag = 0;
		
			// if(flag != 3){
				printf("channels:欧拉角,%.6f,%.6f,%.6f,加速度,%8d,%8d,%8d,", angles[2], angles[1], angles[0], an[1][0], an[1][1], an[1][2]);
				// printf("世界坐标,%8f,%8f,%8f,", wa[0] - calc_o_error[3], wa[1] - calc_o_error[4], -wa[2] - calc_o_error[5]);
				printf("世界坐标,%8f,%8f,%8f,", wa[0], wa[1], wa[2]);
			// }
			// printf("timeout_count: %d, flag: %d\r\n\r\n", timeout_count, flag);
				// printf("零飘,%d,%d,%d,", calc_o_error[3], calc_o_error[4], calc_o_error[5]);
				printf("世界零飘,%d,%d,%d,", (int)ce[3], (int)ce[4], (int)ce[5]);
				printf("位移,%.6f,%.6f,%.6f,%.6f\r\n", sd[0] * 100, sd[1] * 100, sd[2] * 100, sd[3] * 100);


			UART_UpdataFlag = 0;

		Delay_ms(10);
		Delay_us(1962);
	}
}

#define __ZAT_20241018
#if 20241018

/**
 * @brief  check magn
 * @param  None
 * @retval  none
 */
void TestMagn(void)
{
	TestmagnBuff[(KeyValue - 1) * 3] = magn[0];
	TestmagnBuff[(KeyValue - 1) * 3 + 1] = magn[1];
	TestmagnBuff[(KeyValue - 1) * 3 + 2] = magn[2];

	// TestmagnBuff[3]=magn[0];
	// TestmagnBuff[3+3]=magn[1];
	// TestmagnBuff[3+6]=magn[2];

	// if(KeyValue>=3)
	{
		gstMagOffset.s16X = (TestmagnBuff[0] + TestmagnBuff[3]) / 2;
		gstMagOffset.s16Y = (TestmagnBuff[1] + TestmagnBuff[4]) / 2;
		gstMagOffset.s16Z = (TestmagnBuff[5] + TestmagnBuff[8]) / 2;
		Flag_Check_Magn = true;
	}
}
/******************* (C) COPYRIGHT 2014 Waveshare *****END OF FILE*******************/

void mag()
{
	int i, BUF[6], X, Y, Z;
	switch_lq(1);
	for (i = 0; i < 6; i++)
	{
		BUF[i] = I2C_ReadOneByte(0x1A, i);
		printf("[%d]: 0x%x  \r", i, BUF[i]);
	}

	X = (BUF[1] << 8) | BUF[0];
	Y = (BUF[3] << 8) | BUF[2];
	Z = (BUF[5] << 8) | BUF[4];
	// printf("X: %f  Y: %f  Z: %f\r\n", X, Y, Z);
}

void mag_init()
{
	switch_lq(1);
	I2C_WriteOneByte(0x1A, 0x09, 0x1D);
	I2C_WriteOneByte(0x1A, 0x0B, 0x01);
	I2C_WriteOneByte(0x1A, 0x20, 0x40);
	I2C_WriteOneByte(0x1A, 0x21, 0x01);
}

void lsm()
{
	int i;
	switch_lq(0);
	for (i = 0x22; i < 0x22 + 12; i++)
	{
		printf("[%d]: 0x%x  \r", i, I2C_ReadOneByte(0xD6, i));
	}
	printf("\n");
	switch_lq(1);
}

void lsm_init()
{
	// MyI2C_Init();
	switch_lq(0);
	I2C_WriteOneByte(0xD6, 0x18, 0x38);

	I2C_WriteOneByte(0xD6, 0x19, 0x38);

	I2C_WriteOneByte(0xD6, 0x10, 0x60);
	// I2C_WriteOneByte(0xD6, 0x10, 0x62);

	I2C_WriteOneByte(0xD6, 0x11, 0x60);
	// I2C_WriteOneByte(0xD6, 0x11, 0x62);

	switch_lq(1);
}

void release_gravity()
{
	// float accX = imuIn->linear_acceleration.y - sin(roll)*cos(pitch)*9.81;
	// float accY = imuIn->linear_acceleration.z - cos(roll)*cos(pitch)*9.81;
	// float accZ = imuIn->linear_acceleration.x + sin(pitch)*9.81;
}

void update_data(){
	IMU_GetYawPitchRoll(angles);

	for (j = 0; j < 3; j++)
	{
		an[0][j] = an[1][j];
	}

	// an[1][0] = (float)accel[0] * 10.0 / 1000.0 * 0.061;
	// an[1][1] = (float)accel[1] * 10.0 / 1000.0 * 0.061;
	// an[1][2] = (float)accel[2] * 10.0 / 1000.0 * 0.061;
	an[1][0] = accel[0];
	an[1][1] = accel[1];
	an[1][2] = accel[2];

	gx = angles[2];
	gy = angles[1];
	gz = angles[0];

}

void is_move()
{
	if ((flag != 3)  &&
		((abs(an[1][0] - an[0][0]) > 50) ||
		(abs(an[1][1] - an[0][1]) > 50) ||
		(abs(an[1][2] - an[0][2]) > 50)))
	{
		// 有晃动, 标记运动
		timeout_count = 0; // 清除超时
		flag = 1;		   // 标记运动

		calc_o_error[0] = 0;
		calc_o_error[1] = 0;
		calc_o_error[2] = 0;
	}
	else{
		if ((timeout_count < timeout_standard) && flag != 3)
		{
			timeout_count++; // 累积超时
		}
		else if((timeout_standard <= timeout_count) && (timeout_count < 2 * timeout_standard))
		{
			flag = 2;	// 标记计算零飘
			timeout_count++;
			calc_o_error[0] += an[1][0];
			calc_o_error[1] += an[1][1];
			calc_o_error[2] += an[1][2];
		}
		else if(timeout_count == 2 * timeout_standard)
		{
			calc_o_error[3] = calc_o_error[0] / timeout_standard;
			calc_o_error[4] = calc_o_error[1] / timeout_standard;
			calc_o_error[5] = calc_o_error[2] / timeout_standard;

			calc_o_error[0] = 0;
			calc_o_error[1] = 0;
			calc_o_error[2] = 0;

			timeout_count = 0;
			flag = 3; // 已经超时, 标记静止
		}else{

		}
	} 
}


void calc_error(){
	// static 
	if(k < 29){
		k++;
	}else{
		k = 0;
	}
	ce[0] += wa[0];
	ce[1] += wa[1];
	ce[2] += wa[2];

	if(k == 29){
		ce[3] = ce[0] / 30;
		ce[4] = ce[1] / 30;
		ce[5] = ce[2] / 30;

		ce[0] = 0;
		ce[1] = 0;
		ce[2] = 0;

		// k = 30;
	}
}

float generate_random_float(float min, float max) {
    float scale = rand() / (float) RAND_MAX; // 生成 [0.0, 1.0) 的随机数
    return min + scale * (max - min); // 转换到 [min, max) 范围
}

void demo_data(){
	// if(k < timeout_standard){
		dd[0] = generate_random_float(600, 700);
		dd[1] = generate_random_float(800, 900);
		dd[2] = generate_random_float(-16000, -16100);
	// }else if(timeout_standard <= k < timeout_standard * 3){
	// 	dd[0] = generate_random_float(600, 650);
	// 	dd[1] = generate_random_float(800, 850);
	// 	dd[2] = generate_random_float(-16000, -16050);
	// }else if(k = timeout_standard * 3){
	// 	k = 0;
	// }

	// k++;	

	for (j = 0; j < 3; j++)
	{
		an[0][j] = an[1][j];
	}

	// an[1][0] = (float)accel[0] * 10.0 / 1000.0 * 0.061;
	// an[1][1] = (float)accel[1] * 10.0 / 1000.0 * 0.061;
	// an[1][2] = (float)accel[2] * 10.0 / 1000.0 * 0.061;
	an[1][0] = dd[0];
	an[1][1] = dd[1];
	an[1][2] = dd[2];

}


#endif


// 角度转弧度
double deg_to_rad(double deg) {
    return deg * PI / 180.0;
}

// 旋转矩阵和加速度变换
void rotate_acceleration(double ax, double ay, double az, double in_roll, double in_pitch, double in_yaw, double *ax_world, double *ay_world, double *az_world) {
    // 将欧拉角转成弧度
    double roll = deg_to_rad(in_roll);
    double pitch = deg_to_rad(in_pitch);
    double yaw = deg_to_rad(in_yaw);

    // 计算旋转矩阵的元素
    double cos_roll = cos(roll), sin_roll = sin(roll);
    double cos_pitch = cos(pitch), sin_pitch = sin(pitch);
    double cos_yaw = cos(yaw), sin_yaw = sin(yaw);

    // 旋转矩阵 R = Rz * Ry * Rx
    double R[3][3];

	R[0][0] = cos_yaw * cos_pitch;
	R[0][1] = cos_yaw * sin_pitch * sin_roll - sin_yaw * cos_roll;
	R[0][2] = cos_yaw * sin_pitch * cos_roll + sin_yaw * sin_roll;

	R[1][0] = sin_yaw * cos_pitch;
	R[1][1] = sin_yaw * sin_pitch * sin_roll + cos_yaw * cos_roll;
	R[1][2] = sin_yaw * sin_pitch * cos_roll - cos_yaw * sin_roll;

	R[2][0] = -sin_pitch;
	R[2][1] = cos_pitch * sin_roll;
	R[2][2] = cos_pitch * cos_roll;
	

    // // 世界坐标系下的加速度 a' = R * a
    *ax_world = R[0][0] * ax + R[0][1] * ay + R[0][2] * az;
    *ay_world = R[1][0] * ax + R[1][1] * ay + R[1][2] * az;
    *az_world = R[2][0] * ax + R[2][1] * ay + R[2][2] * az;
}


void calc_velocity(){
	int i_cv;
	for(i_cv = 0; i_cv < 3; i_cv++){
		sv[i_cv][0] = sv[i_cv][1];
		sv[i_cv][1] = sv[i_cv][0] + gT * ((wa[i_cv] - ce[i_cv+3]) * a + wa[i_cv+3]) * 0.5;
	}	
}

void calc_sum_distance(){
	int i_csd = 0;
	calc_velocity();
	for(; i_csd < 3; i_csd++){
		sd[i_csd] += (sv[i_csd][0] + sv[i_csd][1]) * 0.5 * gT;
	}
	sd[3] = (double) sqrt((sd[0] * 100) * (sd[0] * 100) + (sd[1] * 100) * (sd[1] * 100) + (sd[2] * 100) * (sd[2] * 100));
}






