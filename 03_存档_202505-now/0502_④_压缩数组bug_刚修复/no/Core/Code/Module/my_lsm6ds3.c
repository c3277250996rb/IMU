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



void LSM6DS3_Init(void){
    I2C_Write_LSM6DS3(GYRO_ADDRESS,  0x18,  0x38);
    I2C_Write_LSM6DS3(GYRO_ADDRESS,  0x19,  0x38);
    I2C_Write_LSM6DS3(GYRO_ADDRESS,  ADDRESS_CTRL1_XL_10h,  DATA_CTRL1_XL_10h);
    I2C_Write_LSM6DS3(GYRO_ADDRESS,  ADDRESS_CTRL2_G_11h ,  DATA_CTRL2_G_11h );

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
    lsm6ds3_gyro[2] = -(temp_lsm6ds3[5] << 8) | temp_lsm6ds3[4];

}


void my_lsm6ds3_self_test(void){

    LSM6DS3_Update_Data();
    
    char* temp_char = multi_printf_arrays(2, ",", "%d", 
        lsm6ds3_accel, 3, 
        lsm6ds3_gyro,  3
    );
    printf("%s\r\n", temp_char);
    delay_ms(500);
}





