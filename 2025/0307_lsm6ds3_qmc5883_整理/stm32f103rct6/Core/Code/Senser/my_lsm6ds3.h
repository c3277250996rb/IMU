#include "my_senser.h"


/* define LSM6DS3 register address */
#if 1
    #define	GYRO_XOUT_L		0x22
    #define	GYRO_XOUT_H		0x23
    #define	GYRO_YOUT_L		0x24
    #define	GYRO_YOUT_H		0x25
    #define	GYRO_ZOUT_L		0x26
    #define	GYRO_ZOUT_H		0x27

    #define	ACCEL_XOUT_L	0x28
    #define	ACCEL_XOUT_H	0x29
    #define	ACCEL_YOUT_L	0x2A
    #define	ACCEL_YOUT_H	0x2B
    #define	ACCEL_ZOUT_L	0x2C
    #define	ACCEL_ZOUT_H	0x2D

    #define	GYRO_ADDRESS    0xD6    //Gyro and Accel device address
    #define ACCEL_ADDRESS   0xD6 
#endif
/* ------------------------------- */


#if 1
    #define three_axis_d "%d,%d,%d\r\n"
    #define three_axis_f "%f,%f,%f\r\n"

    #define six_axis_d "%d,%d,%d,%d,%d,%d\r\n"
    #define six_axis_f "%f,%f,%f,%f,%f,%f\r\n"

    #define nine_axis_d "%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n"
    #define nine_axis_f "%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n"
#endif


extern int16_t lsm6ds3_accel[3];
extern int16_t lsm6ds3_gyro[3];


void LSM6DS3_Init(void);

void LSM6DS3_Update_Data(void);

void LSM6DS3_Read_Accel(void);
void LSM6DS3_Read_Gyro(void);
void LSM6DS3_Read_Mag(void);








