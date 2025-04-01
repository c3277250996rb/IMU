#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "my_lsm6ds3.h"
#include "my_qmc5883.h"

#if !0210
#define MAX_STR_LEN 1024  // ����ַ�����󳤶�

// ƴ�Ӷ�������Ԫ��
// char* concat_arrays(int num_arrays, const char* delimiter, const char* format, ...) {
char* concat_arrays(int num_arrays, const char* delimiter, const char* format, ...) {
    static char result[MAX_STR_LEN];  // ��̬�������洢���
    result[0] = '\0';  // ��ʼ��Ϊ���ַ���

    va_list args;
    va_start(args, format);

    char buffer[32];  // ��ʱ�洢��ʽ���������
    int first = 1;    // ���ڱ����׸�Ԫ��ǰ�ķָ���

    for (int i = 0; i < num_arrays; i++) {
        int* array = va_arg(args, int*);  // ��ȡ����ָ��
        int size = va_arg(args, int);     // ��ȡ�����С

        for (int j = 0; j < size; j++) {
            if (!first) {
                strcat(result, delimiter);  // ��ӷָ���
            }
            first = 0;
            snprintf(buffer, sizeof(buffer), format, array[j]);  // ���ݴ���ĸ�ʽ�ַ�����ʽ������
            strcat(result, buffer);  // ׷�ӵ�����ַ���
        }
    }

    strcat(result, "\r\n");  // ׷�ӵ�����ַ���
    va_end(args);
    return result;
}
#endif

#if !0307
float angles[3];
volatile float q0=1, q1=0, q2=0, q3=0; 

void get_euler(void);
void get_guater(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
float invSqrt(float x);

void get_euler(void){
    
    int i;
    float gf[3], af[3], mf[3];
    printf("[%s]: %d\r\n", __FILE__, __LINE__);
    QMC5883_Update_Data();
    LSM6DS3_Update_Data();
    // for(i = 0; i < 3; i++ ){        gf[i] = (float) lsm6ds3_gyro[i] /2 *2;printf("[%s]: %d\r\n", __FILE__, __LINE__);    }
    // for(i = 0; i < 3; i++ ){        gf[i] = (float) lsm6ds3_gyro[i] /2 *2;printf("%f,\r\n", gf[i]);    }
    for(i = 0; i < 3; i++ ){        gf[i] = (float) lsm6ds3_gyro[i] /2 *2;    }
    for(i = 0; i < 3; i++ ){        af[i] = (float) lsm6ds3_accel[i] /2 *2;    }
    for(i = 0; i < 3; i++ ){        mf[i] = (float) qmc5883_mag[i] /2 *2;    }

    get_guater(
        gf[0], gf[1], gf[2],
        af[0], af[1], af[2], 
        mf[0], mf[1], mf[2]
    );
    // printf("%f,%f,%f,%f\r\n", q1, q2, q3, q0);

    angles[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
    angles[2] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
    angles[0] = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3;   // yaw
    printf("%f,%f,%f\r\n", angles[0], angles[1], angles[2]);

}

#define Kp 4.50f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 1.0f    // integral gain governs rate of convergence of gyroscope biases

void get_guater(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz){
    
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

/* ��һ�� */
  /* ��ģ */
	norm = invSqrt(ax * ax + ay * ay + az * az);       
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;
  /* ��ģ */
	norm = invSqrt(mx * mx + my * my + mz * mz);          
	mx = mx * norm;
	my = my * norm;
	mz = mz * norm;

    printf("%f\r\n", norm);
    printf("%f\r\n", norm);
    printf("%f\r\n", norm);
/* �ų�ˮƽ ��ֱ�������� */
  /* ��ת���� */
	hx = 2 * mx * (0.5f - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
	hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5f - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
	hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5f - q1q1 - q2q2);         
  /* ����ˮƽ�ʹ�ֱ���� */
	bx = sqrt((hx * hx) + (hy * hy));
	bz = hz;     

/* ����������������ϵת������������ϵ */
  /* ������� */
	vx = 2 * (q1q3 - q0q2);
	vy = 2 * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;

/* ����������������ϵת������������ϵ */
  /* ������� */
	wx = 2 * bx * (0.5 - q2q2 - q3q3) + 2 * bz * (q1q3 - q0q2);
	wy = 2 * bx * (q1q2 - q0q3) + 2 * bz * (q0q1 + q2q3);
	wz = 2 * bx * (q0q2 + q1q3) + 2 * bz * (0.5 - q1q1 - q2q2);  

/* ������� ���*/
  /* ͨ�����ټƹ۲�ֵ ��������� �������� */
	ex = (ay * vz - az * vy) + (my * wz - mz * wy);
	ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
	ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

	if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
	{
/* ���� �ۼ���� Ki*/
		exInt = exInt + ex * Ki * halfT;
		eyInt = eyInt + ey * Ki * halfT;	
		ezInt = ezInt + ez * Ki * halfT;

/* ������� */
		gx = gx + Kp * ex + exInt;
		gy = gy + Kp * ey + eyInt;
		gz = gz + Kp * ez + ezInt;
	}

    printf("%f,%f\r\n", gx, gy);
    printf("%f,%f\r\n", q0, q1);
    printf("[%s]: %d\r\n", __FILE__, __LINE__);
    
/* ������Ԫ�� */
	q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    printf("(%f * %f - %f * %f - %f * %f) * %f;\r\n", -q1 , gx , q2 , gy , q3 , gz , halfT);
	q1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
	q2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
	q3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;  

	norm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;

}

float invSqrt(float x){

	float halfx = 0.5f * x;
	float y = x;
	
	long i = *(long*)&y;                //get bits for floating value
	i = 0x5f3759df - (i >> 1);          //gives initial guss you
	y = *(float*)&i;                    //convert bits back to float
	y = y * (1.5f - (halfx * y * y));   //newtop step, repeating increases accuracy
	
	return y;
}

int my_test() {

    printf("[%s]: %d\r\n", __FILE__, __LINE__);
    get_euler();
    HAL_Delay(500);
    return 0;
}

#endif










