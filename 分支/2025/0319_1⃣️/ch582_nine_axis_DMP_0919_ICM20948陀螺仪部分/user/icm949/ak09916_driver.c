/*
 * ak09916_driver.c
 *
 *  Created on: Jul 30, 2024
 *      Author: liu
 */
#if 0
#include "icm949_driver.h"
#include "imu_app.h"

extern imu_t imu_9;

#define ICMAK09916_ADDR AK09916_ADDRESS<<1
#define ICMAK09916_ADDR_W (AK09916_ADDRESS << 1) + 1

/************ Basic Settings ************/
// 0ok
// 1 err
static bool ak09916_read_reg(uint8_t reg, uint8_t *value, uint16_t len)
{
    bool ret = 0;
    int code = 0;
    code = i2c_read_bytes(ICMAK09916_ADDR, reg, value, len);
    ret = code ? false : true;
    if(ret>0)
    {
//#if icm20949_debug
//     printf("icm20948_write_reg reg %02x ,code %d \r\n",reg,code);
//#endif
    }
    return ret;
}

static bool ak09916_write_reg(uint8_t reg, uint8_t *value, uint16_t len)
{
    bool ret = 0;
    int code = 0;
    code = i2c_write_bytes(ICMAK09916_ADDR, reg, value, len);
    ret = code ? false : true;
    if(ret>0)
    {
//#if icm20949_debug
//     printf("icm20948_write_reg reg %02x ,code %d \r\n",reg,code);
//#endif
    }
    return ret;
}

uint8_t ak09916_debug_buf[10];
int ak09916_driver_init()
{
    uint16_t id;
    i2c_app_init(ICMAK09916_ADDR);
    ak09916_read_reg(AK09916_WIA_1,ak09916_debug_buf,2);
    id=(ak09916_debug_buf[0]<<8) + ak09916_debug_buf[1];
    printf("AKL0916 id= %04x\r\n",id);
    ak09916_debug_buf[0]=AK09916_CONT_MODE_100HZ;
    ak09916_write_reg(AK09916_CNTL_2,ak09916_debug_buf,1);
    ak09916_debug_buf[0]=0;
    ak09916_read_reg(AK09916_CNTL_2,ak09916_debug_buf,2);
    printf("AKL0916 opmode= %02x\r\n",ak09916_debug_buf[0]);


}
uint8_t akstats=0;
void ak09916_read_test()
{
//

    ak09916_read_reg(AK09916_HXL,ak09916_debug_buf,6);

    ak09916_read_reg(AK09916_STATUS_2,&akstats,1);
    int16_t x, y, z;

      x = (int16_t)((ak09916_debug_buf[1]) << 8) | ak09916_debug_buf[0];
      y = (int16_t)((ak09916_debug_buf[3]) << 8) | ak09916_debug_buf[2];
      z = (int16_t)((ak09916_debug_buf[5]) << 8) | ak09916_debug_buf[4];

      imu_9.i_mag[0] = x * AK09916_MAG_LSB;
      imu_9.i_mag[1] = y * AK09916_MAG_LSB;
      imu_9.i_mag[2] = z * AK09916_MAG_LSB;

}

#endif
