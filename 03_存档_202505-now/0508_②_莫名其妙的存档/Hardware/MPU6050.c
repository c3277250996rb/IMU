#include "MPU6050.h"
#include "my_i2c.h"
#include "math.h"
#include "QMC5883L.h"

int16_t mpu6050_accel[3];
int16_t mpu6050_gyro[3];
int8_t mpu6050_temp;
float euler_angle[3];                       // ���������� - ŷ�������飨roll, pitch, yaw��
float buffer_9axes[9];                      // ��ʱ���洫�������� - �洢9�ᴫ����ת���������
volatile float q0=1, q1=0, q2=0, q3=0;      // ��ʼ����Ԫ������λ��Ԫ����


uint8_t mpu6050_i2c_read(I2C_DEPLOY *bus, uint8_t RegisterAddress)
{
    uint8_t data;

    data = bus->ReadReg(bus, RegisterAddress);

    return data;
}

uint8_t MPU6050_Init(I2C_DEPLOY* bus)
{

    // ����豸�Ƿ�Ӧ��
    if (bus->CheckDevice(bus))
        return 1; // ����Ӧ�����ش���

    // д�Ĵ�����ʼ�� MPU6050
    bus->WriteReg(bus, 0x6B, 0x00); // �˳����ߣ�����ʱ��
    bus->WriteReg(bus, 0x19, 0x07); // ���ò����� SMPLRT_DIV
    bus->WriteReg(bus, 0x1A, 0x06); // ����DLPF
    bus->WriteReg(bus, 0x1B, 0x18); // �������������� ��2000��/s
    bus->WriteReg(bus, 0x1C, 0x01); // ���ü��ٶȼ����� ��2g

    return 0; // �ɹ�
}

void mpu6050_read_all(I2C_DEPLOY *bus)
{
    uint8_t high, low;

    // --- ���ٶ� ---
    high = mpu6050_i2c_read(bus, 0x3B); low = mpu6050_i2c_read(bus, 0x3C);
    mpu6050_accel[0] = (int16_t)((high << 8) | low);

    high = mpu6050_i2c_read(bus, 0x3D); low = mpu6050_i2c_read(bus, 0x3E);
    mpu6050_accel[1] = (int16_t)((high << 8) | low);

    high = mpu6050_i2c_read(bus, 0x3F); low = mpu6050_i2c_read(bus, 0x40);
    mpu6050_accel[2] = (int16_t)((high << 8) | low);

    // --- �¶� ---
    high = mpu6050_i2c_read(bus, 0x41); low = mpu6050_i2c_read(bus, 0x42);
    mpu6050_temp = (int16_t)((high << 8) | low);

    // --- ������ ---
    high = mpu6050_i2c_read(bus, 0x43); low = mpu6050_i2c_read(bus, 0x44);
    mpu6050_gyro[0] = (int16_t)((high << 8) | low);

    high = mpu6050_i2c_read(bus, 0x45); low = mpu6050_i2c_read(bus, 0x46);
    mpu6050_gyro[1] = (int16_t)((high << 8) | low);

    high = mpu6050_i2c_read(bus, 0x47); low = mpu6050_i2c_read(bus, 0x48);
    mpu6050_gyro[2] = (int16_t)((high << 8) | low);
}


void imu_get_euler(void){
    imu_convert_9axes();
  
  /* X - roll - ����� */
      euler_angle[0] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
  /* ---------------- */
  
  /* Y - pitch - ������ */
      euler_angle[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
  /* ----------------- */
  
  /* Z - yaw - ƫ���� */
      euler_angle[2] = atan2(-2 * q1 * q2 - 2 * q0 * q3, 2 * q2 * q2 + 2 * q3 * q3 - 1) * 57.3;   // yaw
  /* --------------- */
  }



void imu_convert_9axes(void){

    int i;
    for(i = 0; i < 3; i++){
        buffer_9axes[i] = (float) (mpu6050_accel[i] * QUATRE_LSB_ACCEL);
    }
    for(i = 0; i < 3; i++){
        buffer_9axes[i + 3] = (float) (mpu6050_gyro[i] * QUATER_LSB_GYRO);
    }
    for(i = 0; i < 3; i++){
        buffer_9axes[i + 6] = (float) (qmc5883_mag[i] * QUATER_LSB_MAG);
    }

    imu_get_quater(
        buffer_9axes[0], buffer_9axes[1], buffer_9axes[2], 
        buffer_9axes[3], buffer_9axes[4], buffer_9axes[5], 
        buffer_9axes[6], buffer_9axes[7], buffer_9axes[8]
    );
}

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

/* ������Ԫ�� */
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
