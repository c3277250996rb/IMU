#include "my_mpu9250.h"

int16_t mpu9250_accel[3];
int16_t mpu9250_gyro[3];
int16_t mpu9250_mag[3];

unsigned char temp_buffer[6];



uint8_t I2C_Read_MPU9250(uint8_t DeviceAddress, uint8_t RegisterAddress){
    return I2C_2_Read(DeviceAddress, RegisterAddress);
}
void I2C_Write_MPU9250(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data){
    I2C_2_Write(DeviceAddress, RegisterAddress, input_Data);
}



void MPU9250_Init(void){
    I2C_Write_MPU9250(GYRO_ADDRESS, PWR_MGMT_1,   0x00);
    I2C_Write_MPU9250(GYRO_ADDRESS, SMPLRT_DIV,   0x07);
    I2C_Write_MPU9250(GYRO_ADDRESS, CONFIG,       0x06);
    //  +-1000dps  FS_SET=2  LSBpd=32.8
    I2C_Write_MPU9250(GYRO_ADDRESS, GYRO_CONFIG,  0x10);
    //  +2g  AFS_SET=0  LSBpg=16384
    I2C_Write_MPU9250(GYRO_ADDRESS, ACCEL_CONFIG, 0x01);
    //  +-48G  LSBpG=1000/6
    delay_ms(10);
}

void MPU9250_Update_Data(void){
    MPU9250_Read_Accel();
    MPU9250_Read_Gyro();
    MPU9250_Read_Mag();
}

void MPU9250_Read_Accel(void){
    
    temp_buffer[0] = I2C_Read_MPU9250(ACCEL_ADDRESS, ACCEL_XOUT_L); 
    temp_buffer[1] = I2C_Read_MPU9250(ACCEL_ADDRESS, ACCEL_XOUT_H);

    temp_buffer[2] = I2C_Read_MPU9250(ACCEL_ADDRESS, ACCEL_YOUT_L);
    temp_buffer[3] = I2C_Read_MPU9250(ACCEL_ADDRESS, ACCEL_YOUT_H);

    temp_buffer[4] = I2C_Read_MPU9250(ACCEL_ADDRESS, ACCEL_ZOUT_L);
    temp_buffer[5] = I2C_Read_MPU9250(ACCEL_ADDRESS, ACCEL_ZOUT_H);
    	
    mpu9250_accel[0] = (temp_buffer[1] << 8) | temp_buffer[0];
    mpu9250_accel[1] = (temp_buffer[3] << 8) | temp_buffer[2];
    mpu9250_accel[2] = (temp_buffer[5] << 8) | temp_buffer[4];

}

void MPU9250_Read_Gyro(void){

    temp_buffer[0] = I2C_Read_MPU9250(GYRO_ADDRESS, GYRO_XOUT_L); 
    temp_buffer[1] = I2C_Read_MPU9250(GYRO_ADDRESS, GYRO_XOUT_H);

    temp_buffer[2] = I2C_Read_MPU9250(GYRO_ADDRESS, GYRO_YOUT_L);
    temp_buffer[3] = I2C_Read_MPU9250(GYRO_ADDRESS, GYRO_YOUT_H);

    temp_buffer[4] = I2C_Read_MPU9250(GYRO_ADDRESS, GYRO_ZOUT_L);
    temp_buffer[5] = I2C_Read_MPU9250(GYRO_ADDRESS, GYRO_ZOUT_H);

    mpu9250_gyro[0] = (temp_buffer[1] << 8) | temp_buffer[0];
    mpu9250_gyro[1] = (temp_buffer[3] << 8) | temp_buffer[2];
    mpu9250_gyro[2] = (temp_buffer[5] << 8) | temp_buffer[4];

}

void MPU9250_Read_Mag(void){

    I2C_Write_MPU9250(GYRO_ADDRESS, 0x37, 0x02);  //turn on Bypass Mode 
    delay_ms(10);
    I2C_Write_MPU9250(MAG_ADDRESS, 0x0A, 0x01);	
    delay_ms(10);

    temp_buffer[0] = I2C_Read_MPU9250(MAG_ADDRESS, MAG_XOUT_L);
    temp_buffer[1] = I2C_Read_MPU9250(MAG_ADDRESS, MAG_XOUT_H);
 
    temp_buffer[2] = I2C_Read_MPU9250(MAG_ADDRESS, MAG_YOUT_L);
    temp_buffer[3] = I2C_Read_MPU9250(MAG_ADDRESS, MAG_YOUT_H);
 
    temp_buffer[4] = I2C_Read_MPU9250(MAG_ADDRESS, MAG_ZOUT_L);
    temp_buffer[5] = I2C_Read_MPU9250(MAG_ADDRESS, MAG_ZOUT_H);

    mpu9250_mag[1] = (temp_buffer[1] << 8) | temp_buffer[0];
    mpu9250_mag[0] = (temp_buffer[3] << 8) | temp_buffer[2];
    mpu9250_mag[2] = (temp_buffer[5] << 8) | temp_buffer[4];

    mpu9250_mag[2] = -mpu9250_mag[2];
}


void my_mpu9250_self_test(void){

    MPU9250_Update_Data();
    
    char* temp_char = muti_printf_arrays(3, ",", "%d", 
        mpu9250_accel, 3, 
        mpu9250_gyro,  3,
        mpu9250_mag,   3
    );
    printf("%s\r\n", temp_char);
    delay_ms(500);
}





