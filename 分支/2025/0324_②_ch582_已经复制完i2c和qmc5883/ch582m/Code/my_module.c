#include "my_module.h"

#ifndef __QMC5883__All__Reference__
#define __QMC5883__All__Reference__

    int16_t qmc5883_mag[3];
    unsigned char temp_qmc5883[6];

    
    uint8_t temp_read[1];
    uint8_t I2C_Read_QMC5883(uint8_t DeviceAddress, uint8_t RegisterAddress){
        i2c_read_from(DeviceAddress, temp_read, 4, true, 1000);
        return temp_read[1];
    }
    void I2C_Write_QMC5883(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data){
        i2c_write_to(DeviceAddress, &input_Data, sizeof(input_Data), true, true);
    }


    void QMC5883_Init(void){
        // 8G  full scale
        // LSBpG=3000
        I2C_Write_QMC5883(MAG_ADDRESS,  0x09,  0x1D);
        I2C_Write_QMC5883(MAG_ADDRESS,  0x0B,  0x01);
        I2C_Write_QMC5883(MAG_ADDRESS,  0x20,  0x40);
        I2C_Write_QMC5883(MAG_ADDRESS,  0x21,  0x01);

        DelayMs(10);
    }

    void QMC5883_Update_Data(void){
        QMC5883_Read_Mag();
    }

    void QMC5883_Read_Mag(void){

        temp_qmc5883[0] = I2C_Read_QMC5883(MAG_ADDRESS, MAG_XOUT_L);
        temp_qmc5883[1] = I2C_Read_QMC5883(MAG_ADDRESS, MAG_XOUT_H);
    
        temp_qmc5883[2] = I2C_Read_QMC5883(MAG_ADDRESS, MAG_YOUT_L);
        temp_qmc5883[3] = I2C_Read_QMC5883(MAG_ADDRESS, MAG_YOUT_H);
    
        temp_qmc5883[4] = I2C_Read_QMC5883(MAG_ADDRESS, MAG_ZOUT_L);
        temp_qmc5883[5] = I2C_Read_QMC5883(MAG_ADDRESS, MAG_ZOUT_H);

        qmc5883_mag[1] = (temp_qmc5883[1] << 8) | temp_qmc5883[0];
        qmc5883_mag[0] = (temp_qmc5883[3] << 8) | temp_qmc5883[2];
        qmc5883_mag[2] = (temp_qmc5883[5] << 8) | temp_qmc5883[4];

        qmc5883_mag[2] = -qmc5883_mag[2];
    }


    void my_qmc5883_self_test(void){

        QMC5883_Update_Data();
        
        char* temp_char = muti_printf_arrays(1, ",", "%d",
            qmc5883_mag, 3
        );
        printf("%s\r\n", temp_char);
        DelayMs(500);
    }

#endif

