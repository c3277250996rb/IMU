
#include "System.h"
#include <stdlib.h>

int16_t TestmagnBuff[9] = {0};
// float ax_n, ay_n, az_n;
// float ax_n_plus_1, ay_n_plus_1, az_n_plus_1;
int16_t an[2][3];
float gx, gy, gz;
int i = 0;
int j = 0;
int k = 0;

int timeout_count = 0, timeout_standard = 3, flag = 1;
int16_t calc_o_error[6];
int32_t ce[6];	// ?因为az已经达到了-16000, 所以最低得int32_t, 又因为变成int64_t后ax莫名出错, 所以只能选int32_t
int32_t ce_5;	// ?因为az已经达到了-16000, 所以最低得int32_t, 又因为变成int64_t后ax莫名出错, 所以只能选int32_t
float dd[3];


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
	a = 0.061 / 1000.0;
	g = 1.0 / 32.8;
#else
	a = 0.061 * 1.000 / 1000.000;
	g = 4.375 * 2.000 / 1000.000;
#endif

	while (1)
	{		
			update_data();
			// demo_data();

			// is_move();
			calc_error();

			IMU_SampleFlag = 0;
		
			// printf("channels:%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\r\n", angles[2], angles[1], angles[0] , ((float)accel[0]) * a, ((float)accel[1]) * a, ((float)accel[2]) * a, ((float)gyro[0]) * g, ((float)gyro[1]) * g, ((float)gyro[2]) * g);
			// printf("channels:%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\r\n", angles[2], angles[1], angles[0] , an[1][0] * 10.0 / 1000.0 * 0.061, an[1][1] * 10.0 / 1000.0 * 0.061, an[1][2] * 10.0 / 1000.0 * 0.061);
			// if (flag == 1)
			printf("channels:%.6f,%.6f,%.6f,%8d,%8d,%8d\r\n", angles[2], angles[1], angles[0], an[1][0], an[1][1], an[1][2]);
			printf("timeout_count: %d, flag: %d\r\n\r\n", timeout_count, flag);
			printf("channels:%d,%d,%d\r\n", ce[3], ce[4], ce_5);

			UART_UpdataFlag = 0;

		// Delay_ms(50);
		Delay_ms(10);
		// Delay_ms(3);
	}
}
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
	if ((abs(an[1][0] - an[0][0]) > 100) ||
		(abs(an[1][1] - an[0][1]) > 200) ||
		(abs(an[1][2] - an[0][2]) > 100))
	{
		// 有晃动, 标记运动
		timeout_count = 0; // 清除超时
		flag = 1;		   // 标记运动
	}
	else{
		if (timeout_count < timeout_standard)
		{
			timeout_count++; // 累积超时
		}
		else if(timeout_standard <= timeout_count < 2 * timeout_standard)
		{
			flag = 2;	// 标记计算零飘
			timeout_count++;
			calc_o_error[0] += an[1][0];
			calc_o_error[1] += an[1][1];
			calc_o_error[2] += an[1][2];
		}
		else if(timeout_count = 2 * timeout_standard)
		{
			calc_o_error[3] = calc_o_error[0] / timeout_standard;
			calc_o_error[4] = calc_o_error[1] / timeout_standard;
			calc_o_error[5] = calc_o_error[2] / timeout_standard;
		}else
		{
			flag = 3; // 已经超时, 标记静止
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
	ce[0] += an[1][0];
	ce[1] += an[1][1];
	ce[2] += an[1][2];

	if(k == 29){
		ce[3] = ce[0] / 30;
		ce[4] = ce[1] / 30;
		ce_5 = ce[2] / 30;

		ce[0] = 0;
		ce[1] = 0;
		ce[2] = 0;
	}
}

float generate_random_float(float min, float max) {
    float scale = rand() / (float) RAND_MAX; // 生成 [0.0, 1.0) 的随机数
    return min + scale * (max - min); // 转换到 [min, max) 范围
}

void demo_data(){
	// static int k = 0;
	dd[0] = generate_random_float(600, 700);
	dd[1] = generate_random_float(800, 900);
	dd[2] = generate_random_float(-16000, -16200);

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




