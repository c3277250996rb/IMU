#include "stm32f10x.h"
#include "my_i2c.h"
#include "OLED.h"
#include "motor.h"
#include "mpu6050.h"
#include "QMC5883L.h"
#include "test.h"
#include "serial.h"

void test()
{
    I2C_DEPLOY oled_i2c = soft_i2c_create(GPIOB, GPIO_Pin_12, GPIO_Pin_15, 0x78);
    OLED_Init(&oled_i2c);
    I2C_DEPLOY mpu6050_i2c = soft_i2c_create(GPIOB, GPIO_Pin_0, GPIO_Pin_1, MPU6050_ADDRESS << 1); // 写地址左移1位
    MPU6050_Init(&mpu6050_i2c);
    I2C_DEPLOY qmc5883_i2c = soft_i2c_create(GPIOB, GPIO_Pin_8, GPIO_Pin_9, 0x0D << 1);
    QMC5883_Init(&qmc5883_i2c);

    while (1)
    {
        motor_polirity_set();
        // car_set_state(CAR_STATE_FORWARD);
        mpu6050_read_all(&mpu6050_i2c);
        QMC5883_Update_Data(&qmc5883_i2c);

        imu_get_euler();
        OLED_Clear();
        my_oled_showslo_debug(0, 0, "angle[0]:", euler_angle[0], 3);
        my_oled_showslo_debug(0, 20, "angle[1]:", euler_angle[1], 3);
        my_oled_showslo_debug(0, 40, "angle[2]:", euler_angle[2], 3);

        // my_oled_showslo_debug(0, 0, "angle[0]:", qmc5883_mag[0], 3);
        // my_oled_showslo_debug(0, 20, "angle[1]:", qmc5883_mag[1], 3);
        // my_oled_showslo_debug(0, 40, "angle[2]:", qmc5883_mag[2], 3);
        // OLED_ShowFloatNum(0, 0 , euler_angle[0], 2, 2, OLED_8X16);
        Serial_Printf("%f, %f, %f\r\n", euler_angle[0], euler_angle[1], euler_angle[2]);
        // Serial_Printf("%d, %d, %d\r\n", 123, 23, 43);
        // Serial_Printf("123");
        OLED_Update(&oled_i2c);
    }
}


/*
char error_name[] = "Serial";
char error[] = "Serial_RxData";
uint16_t error_num = Serial_RxData;
erro_show(error_name, error, error_num);
*/
void erro_show(char error_name[], char erro[], uint16_t error_num)
{
    // OLED_Clear();
    // OLED_ShowString(0, 0, "系统异常", OLED_8X16);
    // OLED_ShowString(60, 0, error_name, OLED_8X16);
    // OLED_ShowString(0, 20, "error", OLED_8X16);
    // OLED_ShowNum(0, 40, error_num, 5,  OLED_8X16);
    // OLED_ShowString(50, 40, "即将复位", OLED_8X16);
    // OLED_Update();
    // Delay_ms(1000);
    // NVIC_SystemReset();
}

