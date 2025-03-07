#include "my_lsm6ds3.h"

int16_t lsm6ds3_accel[3];
int16_t lsm6ds3_gyro[3];

unsigned char temp_lsm6ds3[6];



uint8_t I2C_Read_LSM6DS3(uint8_t DeviceAddress, uint8_t RegisterAddress){
    return I2C_2_Read(DeviceAddress, RegisterAddress);
}
void I2C_Write_LSM6DS3(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data){
    I2C_2_Write(DeviceAddress, RegisterAddress, input_Data);
}


    //  CTRL1_XL (10h)    
    //  ODR_XL [7:4]  [0110]  416Hz  output data rate and power mode selection
    //  FS_XL [3:2]  [00]  +-2g  LSBpg=10^6/61  Accelerometer full-scale selection  加速度计全量程
    //  BW_XL [1:0]  [01]  200Hz  anti-aliasing filter bandwidth selection  抗锯齿滤波器
    
    //  CTRL2_G (11h)
    //  ODR_G [7:4]  [0110]  416Hz  gyroscope output data rate selection
    //  FS_G [3:2]  [00]  245dps  LSBpd=10^6/8750  gyroscope full-scale
    //  FS_125 [1]  [0]  disable  gyroscope full-scale at 125 dps
    //  [0]
    

void LSM6DS3_Init(void){
    I2C_Write_LSM6DS3(GYRO_ADDRESS,  0x18,  0x38);
    I2C_Write_LSM6DS3(GYRO_ADDRESS,  0x19,  0x38);
    // +-2g  LSBpg=10^6/61
    I2C_Write_LSM6DS3(GYRO_ADDRESS,  0x10,  0x61);
    // 1000dps  LSBpd=10^6/3500
    I2C_Write_LSM6DS3(GYRO_ADDRESS,  0x11,  0x68);

    delay_ms(10);
}

void LSM6DS3_Update_Data(void){
    LSM6DS3_Read_Accel();
    LSM6DS3_Read_Gyro();
}

void LSM6DS3_Read_Accel(void){
    
    temp_lsm6ds3[0] = I2C_Read_LSM6DS3(ACCEL_ADDRESS, ACCEL_XOUT_L); 
    temp_lsm6ds3[1] = I2C_Read_LSM6DS3(ACCEL_ADDRESS, ACCEL_XOUT_H);

    temp_lsm6ds3[2] = I2C_Read_LSM6DS3(ACCEL_ADDRESS, ACCEL_YOUT_L);
    temp_lsm6ds3[3] = I2C_Read_LSM6DS3(ACCEL_ADDRESS, ACCEL_YOUT_H);

    temp_lsm6ds3[4] = I2C_Read_LSM6DS3(ACCEL_ADDRESS, ACCEL_ZOUT_L);
    temp_lsm6ds3[5] = I2C_Read_LSM6DS3(ACCEL_ADDRESS, ACCEL_ZOUT_H);
    	
    lsm6ds3_accel[0] = (temp_lsm6ds3[1] << 8) | temp_lsm6ds3[0];
    lsm6ds3_accel[1] = (temp_lsm6ds3[3] << 8) | temp_lsm6ds3[2];
    lsm6ds3_accel[2] = (temp_lsm6ds3[5] << 8) | temp_lsm6ds3[4];

}

void LSM6DS3_Read_Gyro(void){

    temp_lsm6ds3[0] = I2C_Read_LSM6DS3(GYRO_ADDRESS, GYRO_XOUT_L); 
    temp_lsm6ds3[1] = I2C_Read_LSM6DS3(GYRO_ADDRESS, GYRO_XOUT_H);

    temp_lsm6ds3[2] = I2C_Read_LSM6DS3(GYRO_ADDRESS, GYRO_YOUT_L);
    temp_lsm6ds3[3] = I2C_Read_LSM6DS3(GYRO_ADDRESS, GYRO_YOUT_H);

    temp_lsm6ds3[4] = I2C_Read_LSM6DS3(GYRO_ADDRESS, GYRO_ZOUT_L);
    temp_lsm6ds3[5] = I2C_Read_LSM6DS3(GYRO_ADDRESS, GYRO_ZOUT_H);

    lsm6ds3_gyro[0] = (temp_lsm6ds3[1] << 8) | temp_lsm6ds3[0];
    lsm6ds3_gyro[1] = (temp_lsm6ds3[3] << 8) | temp_lsm6ds3[2];
    lsm6ds3_gyro[2] = (temp_lsm6ds3[5] << 8) | temp_lsm6ds3[4];

}


void my_lsm6ds3_self_test(void){

    LSM6DS3_Update_Data();
    
    char* temp_char = muti_printf_arrays(2, ",", "%d", 
        lsm6ds3_accel, 3, 
        lsm6ds3_gyro,  3
    );
    printf("%s\r\n", temp_char);
    delay_ms(500);
}





