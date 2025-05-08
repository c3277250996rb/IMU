#include "QMC5883L.h"
#include "my_i2c.h"
#include "generic.h"


int16_t qmc5883_mag[3];

unsigned char temp_qmc5883[6];

uint8_t qmc5883_i2c_read(I2C_DEPLOY *bus, uint8_t RegisterAddress)
{
    uint8_t qmc5883_data;

    qmc5883_data = bus->ReadReg(bus, RegisterAddress);

    return qmc5883_data;
}



uint8_t QMC5883_Init(I2C_DEPLOY *bus)
{
    // uint8_t ctrl_reg_val = QMC5883_MODE_CONT | QMC5883_ODR_200HZ | QMC5883_RNG_8G | QMC5883_OSR_512;
    bus->WriteReg(bus, 0x0B, 0x01);
    bus->WriteReg(bus, 0x09, 0x1D);
    return bus->CheckDevice(bus); // 返回ACK（0 表示成功）
}

void QMC5883_Update_Data(I2C_DEPLOY *bus){
    QMC5883_Read_Mag(bus);
}

void QMC5883_Read_Mag(I2C_DEPLOY *bus){

    temp_qmc5883[0] = qmc5883_i2c_read(bus, MAG_XOUT_L);
    temp_qmc5883[1] = qmc5883_i2c_read(bus, MAG_XOUT_H);
 
    temp_qmc5883[2] = qmc5883_i2c_read(bus, MAG_YOUT_L);
    temp_qmc5883[3] = qmc5883_i2c_read(bus, MAG_YOUT_H);
 
    temp_qmc5883[4] = qmc5883_i2c_read(bus, MAG_ZOUT_L);
    temp_qmc5883[5] = qmc5883_i2c_read(bus, MAG_ZOUT_H);

    qmc5883_mag[1] = (temp_qmc5883[1] << 8) | temp_qmc5883[0];
    qmc5883_mag[0] = (temp_qmc5883[3] << 8) | temp_qmc5883[2];
    qmc5883_mag[2] = (temp_qmc5883[5] << 8) | temp_qmc5883[4];

    qmc5883_mag[2] = -qmc5883_mag[2];
}


// void my_qmc5883_self_test(void){

//     QMC5883_Update_Data();
    
//     char* temp_char = multi_printf_arrays(1, ",", "%d",
//         qmc5883_mag, 3
//     );
//     printf("%s\r\n", temp_char);
//     delay_ms(500);
// }
