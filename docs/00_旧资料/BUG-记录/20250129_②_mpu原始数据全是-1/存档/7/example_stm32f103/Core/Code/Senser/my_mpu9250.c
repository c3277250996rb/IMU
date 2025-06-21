#include "my_mpu9250.h"

int16_t mpu9250_accel[3];
int16_t mpu9250_gyro[3];
int16_t mpu9250_mag[3];

unsigned char temp_buffer[6];



uint8_t I2C_2_Read(uint8_t DeviceAddress, uint8_t RegisterAddress){
    return MPU6050_ReadReg(DeviceAddress, RegisterAddress);
}
void I2C_2_Write(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data){
    MPU6050_WriteReg(DeviceAddress, RegisterAddress, input_Data);
}



void MPU9250_Init(void){
    I2C_2_Write(GYRO_ADDRESS, PWR_MGMT_1,   0x00);
    I2C_2_Write(GYRO_ADDRESS, SMPLRT_DIV,   0x07);
    I2C_2_Write(GYRO_ADDRESS, CONFIG,       0x06);
    I2C_2_Write(GYRO_ADDRESS, GYRO_CONFIG,  0x10);
    I2C_2_Write(GYRO_ADDRESS, ACCEL_CONFIG, 0x01);

    Delay_ms(10);
}

void MPU9250_Update_Data(void){
    MPU9250_Read_Accel();
    MPU9250_Read_Gyro();
    MPU9250_Read_Mag();
}

void MPU9250_Read_Accel(void){
    
    temp_buffer[0] = I2C_2_Read(ACCEL_ADDRESS, ACCEL_XOUT_L); 
    temp_buffer[1] = I2C_2_Read(ACCEL_ADDRESS, ACCEL_XOUT_H);

    temp_buffer[2] = I2C_2_Read(ACCEL_ADDRESS, ACCEL_YOUT_L);
    temp_buffer[3] = I2C_2_Read(ACCEL_ADDRESS, ACCEL_YOUT_H);

    temp_buffer[4] = I2C_2_Read(ACCEL_ADDRESS, ACCEL_ZOUT_L);
    temp_buffer[5] = I2C_2_Read(ACCEL_ADDRESS, ACCEL_ZOUT_H);
    	
    mpu9250_accel[0] = (temp_buffer[1] << 8) | temp_buffer[0];
    mpu9250_accel[1] = (temp_buffer[3] << 8) | temp_buffer[2];
    mpu9250_accel[2] = (temp_buffer[5] << 8) | temp_buffer[4];

}

void MPU9250_Read_Gyro(void){

    temp_buffer[0] = I2C_2_Read(GYRO_ADDRESS, GYRO_XOUT_L); 
    temp_buffer[1] = I2C_2_Read(GYRO_ADDRESS, GYRO_XOUT_H);

    temp_buffer[2] = I2C_2_Read(GYRO_ADDRESS, GYRO_YOUT_L);
    temp_buffer[3] = I2C_2_Read(GYRO_ADDRESS, GYRO_YOUT_H);

    temp_buffer[4] = I2C_2_Read(GYRO_ADDRESS, GYRO_ZOUT_L);
    temp_buffer[5] = I2C_2_Read(GYRO_ADDRESS, GYRO_ZOUT_H);

    mpu9250_gyro[0] = (temp_buffer[1] << 8) | temp_buffer[0];
    mpu9250_gyro[1] = (temp_buffer[3] << 8) | temp_buffer[2];
    mpu9250_gyro[2] = (temp_buffer[5] << 8) | temp_buffer[4];

}

void MPU9250_Read_Mag(void){

    I2C_2_Write(GYRO_ADDRESS, 0x37, 0x02);  //turn on Bypass Mode 
    Delay_ms(10);
    I2C_2_Write(MAG_ADDRESS, 0x0A, 0x01);	
    Delay_ms(10);

    temp_buffer[0] = I2C_2_Read(MAG_ADDRESS, MAG_XOUT_L);
    temp_buffer[1] = I2C_2_Read(MAG_ADDRESS, MAG_XOUT_H);
 
    temp_buffer[2] = I2C_2_Read(MAG_ADDRESS, MAG_YOUT_L);
    temp_buffer[3] = I2C_2_Read(MAG_ADDRESS, MAG_YOUT_H);
 
    temp_buffer[4] = I2C_2_Read(MAG_ADDRESS, MAG_ZOUT_L);
    temp_buffer[5] = I2C_2_Read(MAG_ADDRESS, MAG_ZOUT_H);

    mpu9250_mag[1] = (temp_buffer[1] << 8) | temp_buffer[0];
    mpu9250_mag[0] = (temp_buffer[3] << 8) | temp_buffer[2];
    mpu9250_mag[2] = (temp_buffer[5] << 8) | temp_buffer[4];

    mpu9250_mag[2] = -mpu9250_mag[2];
}

// char imu_sprintf(int16_t *input_accel, int16_t *input_gyro, int16_t *input_mag){
//     char output_string;
//     int16_t it_buffer[9];
//     int i1;

//     for(i1 = 0; i1 < 3; i1++){
//         it_buffer[i1]     = input_accel[i1];
//         it_buffer[i1 + 3] = input_gyro[i1];
//         it_buffer[i1 + 6] = input_mag[i1];
//     }

//     for(i1 = 0; i1 < 9; i1++){
//         sprintf(output_string, "%s%d,", output_string, it_buffer[i1]);
//     }

//     sprintf(output_string, "%s\r\n", output_string);
//     return output_string;
// }

void my_mpu9250_self_test(void){
    static int m_m_9_executed = 0;

    if(0 == m_m_9_executed){
        // MPU9250_Init();
        m_m_9_executed = 1;
    }

    MPU9250_Update_Data();
    printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n", 
    mpu9250_accel[0], mpu9250_accel[1], mpu9250_accel[2],
     mpu9250_gyro[0],  mpu9250_gyro[1],  mpu9250_gyro[2], 
      mpu9250_mag[0],   mpu9250_mag[1],   mpu9250_mag[2]
    );
    HAL_Delay(10);
}





