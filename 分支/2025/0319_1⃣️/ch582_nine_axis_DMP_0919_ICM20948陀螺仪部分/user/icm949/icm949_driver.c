
#include "icm949_driver.h"
#include "i2c_driver.h"
#include "CH58x_common.h"

#define icm20949_debug 1

#define ICM20948_ADDR 0x68 << 1
#define ICM20948_ADDR_W (0x68 << 1) + 1

static uint8_t currentBank = 0xff;
bool switchBank(uint8_t newBank);
static bool icm20948_read_reg(uint8_t reg, uint8_t *value, uint16_t len);
static bool icm20948_write_reg(uint8_t reg, uint8_t *value, uint16_t len);
int writeRegister8(uint8_t bank, uint8_t reg, uint8_t val);
void setGyrDLPF(ICM20948_dlpf dlpf);
static void sleep(bool sleep);
void setGyrRange(ICM20948_gyroRange gyroRange);
void setAccRange(ICM20948_accRange accRange);
void setAccDLPF(ICM20948_dlpf dlpf);

bool AK09916_initMagnetometer(void);
bool Lis2mdl_initMagnetometer(void);

static uint8_t regVal;
static uint8_t accRangeFactor;
static uint8_t gyrRangeFactor;
uint8_t icm20948_all_buff[20] = {0};

// PB12 SDA
// PB13 SCL
// AD0 GND
bool icm20948_init(void)
{
    uint8_t who=0xff;
    i2c_app_init(ICM20948_ADDR >> 1);

    mDelaymS(100);
    reset_ICM20948();
//    who=whoAmI();
//    if (who != ICM20948_WHO_AM_I_CONTENT)
//    {
//#if icm20949_debug
//        printf("icm20948_init ERR,%02x \r\n",who);
//#endif
//        return false;
//    }
//#if icm20949_debug
//    printf("icm20948_init OK\r\n");
//#endif
//
//    sleep(false);
//    writeRegister8(2, ICM20948_ODR_ALIGN_EN, 1); // aligns ODR
//#if user_has_mag_def
//    AK09916_initMagnetometer();
//#endif
////    Lis2mdl_initMagnetometer();
//
//    setGyrDLPF(ICM20948_DLPF_6);          // lowest noise
//    setGyrRange(ICM20948_GYRO_RANGE_500); // highest resolution
//    setAccRange(ICM20948_ACC_RANGE_2G);
//    setAccDLPF(ICM20948_DLPF_6);
//    //ped

    return true;
}

void icm20948_delayms(uint16_t ms)
{
    mDelaymS(ms);
}
/************ Basic Settings ************/
// 0ok
// 1 err
static bool icm20948_read_reg(uint8_t reg, uint8_t *value, uint16_t len)
{
    bool ret = 0;
    int code = 0;
    code = i2c_read_bytes(ICM20948_ADDR, reg, value, len);
    ret = code ? false : true;
    if(ret>0)
    {
//#if icm20949_debug
//     printf("icm20948_write_reg reg %02x ,code %d \r\n",reg,code);
//#endif
    }
    return ret;
}

static bool icm20948_write_reg(uint8_t reg, uint8_t *value, uint16_t len)
{
    bool ret = 0;
    int code = 0;
    code = i2c_write_bytes(ICM20948_ADDR, reg, value, len);
    ret = code ? false : true;
    if(ret>0)
    {
//#if icm20949_debug
//     printf("icm20948_write_reg reg %02x ,code %d \r\n",reg,code);
//#endif
    }
    return ret;
}
int writeRegister8(uint8_t bank, uint8_t reg, uint8_t val)
{
    switchBank(bank);
    return icm20948_write_reg(reg, &val, 1);
}
uint8_t readRegister8(uint8_t bank, uint8_t reg)
{
    switchBank(bank);
    uint8_t regValue = 0;
    icm20948_read_reg(reg, &regValue, 1);

    return regValue;
}
int16_t readRegister16(uint8_t bank, uint8_t reg){
    switchBank(bank);
    uint8_t reg16Val_buf[2];
    int16_t reg16Val = 0; 
    
    icm20948_read_reg(reg, reg16Val_buf, 2);

    reg16Val = (reg16Val_buf[0]<<8) + reg16Val_buf[1];
    return reg16Val;
}
void writeRegister16(uint8_t bank, uint8_t reg, int16_t val)
{

    uint8_t buf[2];
    switchBank(bank);
    buf[0] = val >> 8;
    buf[1] = val & 0xff;
    icm20948_write_reg(reg, buf, 2);
}

void enableI2CMaster()
{
    writeRegister8(0, ICM20948_USER_CTRL, ICM20948_I2C_MST_EN); // enable I2C master
    writeRegister8(3, ICM20948_I2C_MST_CTRL, 0x07);             // set I2C clock to 345.60 kHz
    icm20948_delayms(10);
}

void i2cMasterReset()
{
    uint8_t regVal = readRegister8(0, ICM20948_USER_CTRL);
    regVal |= ICM20948_I2C_MST_RST;
    writeRegister8(0, ICM20948_USER_CTRL, regVal);
    icm20948_delayms(10);
}

void enableMagDataRead(uint8_t reg, uint8_t bytes)
{
    writeRegister8(3, ICM20948_I2C_SLV0_ADDR, AK09916_ADDRESS | AK09916_READ); // read AK09916
    writeRegister8(3, ICM20948_I2C_SLV0_REG, reg);                             // define AK09916 register to be read
    writeRegister8(3, ICM20948_I2C_SLV0_CTRL, 0x80 | bytes);                   // enable read | number of byte
    icm20948_delayms(1);
}

void writeAK09916Register8(uint8_t reg, uint8_t val)
{
    writeRegister8(3, ICM20948_I2C_SLV0_ADDR, AK09916_ADDRESS); // write AK09916
    writeRegister8(3, ICM20948_I2C_SLV0_REG, reg);              // define AK09916 register to be written to
    writeRegister8(3, ICM20948_I2C_SLV0_DO, val);
}

uint8_t readAK09916Register8(uint8_t reg)
{
    enableMagDataRead(reg, 0x01);
    enableMagDataRead(AK09916_HXL, 0x08);
    regVal = readRegister8(0, ICM20948_EXT_SLV_SENS_DATA_00);
    return regVal;
}


uint8_t readAK09916STL(uint8_t reg)
{
    enableMagDataRead(reg, 0x01);
    regVal = readRegister8(0, ICM20948_EXT_SLV_SENS_DATA_00);
    return regVal;
}

int16_t readAK09916Register16(uint8_t reg)
{
    int16_t regValue = 0;
    enableMagDataRead(reg, 0x02);
    regValue = readRegister16(0, ICM20948_EXT_SLV_SENS_DATA_00);
    enableMagDataRead(AK09916_HXL, 0x08);
    return regValue;
}
void setMagOpMode(AK09916_opMode opMode)
{
    writeAK09916Register8(AK09916_CNTL_2, opMode);
    icm20948_delayms(10);
    if (opMode != AK09916_PWR_DOWN)
    {
        enableMagDataRead(AK09916_HXL, 0x08);
    }
}
void resetMag()
{
    writeAK09916Register8(AK09916_CNTL_3, 0x01);
    icm20948_delayms(100);
}
bool readAllData(uint8_t *data, uint8_t len)
{
    switchBank(0);
    return icm20948_read_reg(ICM20948_ACCEL_OUT, data, len);

}
//6B
bool readMagAllData(uint8_t *data, uint8_t len)
{

    uint8_t drdy=0;
    drdy =readAK09916STL(0x10)&0x01;
    if(drdy==0)
    {
        return false;
    }
    enableMagDataRead(AK09916_HXL, 0x06);
    icm20948_read_reg(ICM20948_EXT_SLV_SENS_DATA_00, data, len);
//    drdy =readAK09916STL(0x18)&0x01;
//    if(drdy==0)
//    {
//        return false;
//    }
    return true;
}


int reset_ICM20948(void)
{
    writeRegister8(0, ICM20948_PWR_MGMT_1, ICM20948_RESET);
    icm20948_delayms(100); // wait for registers to reset
}

bool switchBank(uint8_t newBank)
{
    if (newBank != currentBank)
    {
        currentBank = newBank;
        newBank = newBank << 4;
        return icm20948_write_reg(ICM20948_REG_BANK_SEL, &newBank, 1);
    }
    return true;
}

uint8_t whoAmI(void)
{
    return readRegister8(0, ICM20948_WHO_AM_I);
}
uint16_t whoAmIMag(void)
{
    return (readAK09916Register16(AK09916_WIA_1));
}
static void sleep(bool sleep)
{

    regVal = readRegister8(0, ICM20948_PWR_MGMT_1);
    if (sleep)
    {
        regVal |= ICM20948_SLEEP;
    }
    else
    {
        regVal &= ~ICM20948_SLEEP;
    }
    writeRegister8(0, ICM20948_PWR_MGMT_1, regVal);
}
void enableAcc(bool enAcc)
{
    regVal = readRegister8(0, ICM20948_PWR_MGMT_2);
    if (enAcc)
    {
        regVal &= ~ICM20948_ACC_EN;
    }
    else
    {
        regVal |= ICM20948_ACC_EN;
    }
    writeRegister8(0, ICM20948_PWR_MGMT_2, regVal);
}

void setAccRange(ICM20948_accRange accRange)
{
    regVal = readRegister8(2, ICM20948_ACCEL_CONFIG);
    regVal &= ~(0x06);
    regVal |= (accRange << 1);
    writeRegister8(2, ICM20948_ACCEL_CONFIG, regVal);
    accRangeFactor = 1 << accRange;
}

void setAccDLPF(ICM20948_dlpf dlpf)
{
    regVal = readRegister8(2, ICM20948_ACCEL_CONFIG);
    if (dlpf == ICM20948_DLPF_OFF)
    {
        regVal &= 0xFE;
        writeRegister8(2, ICM20948_ACCEL_CONFIG, regVal);
        return;
    }
    else
    {
        regVal |= 0x01;
        regVal &= 0xC7;
        regVal |= (dlpf << 3);
    }
    writeRegister8(2, ICM20948_ACCEL_CONFIG, regVal);
}

void setAccSampleRateDivider(uint16_t accSplRateDiv)
{
    writeRegister16(2, ICM20948_ACCEL_SMPLRT_DIV_1, accSplRateDiv);
}
void enableGyr(bool enGyr)
{
    regVal = readRegister8(0, ICM20948_PWR_MGMT_2);
    if (enGyr)
    {
        regVal &= ~ICM20948_GYR_EN;
    }
    else
    {
        regVal |= ICM20948_GYR_EN;
    }
    writeRegister8(0, ICM20948_PWR_MGMT_2, regVal);
}
void setGyrRange(ICM20948_gyroRange gyroRange)
{
    regVal = readRegister8(2, ICM20948_GYRO_CONFIG_1);
    regVal &= ~(0x06);
    regVal |= (gyroRange << 1);
    writeRegister8(2, ICM20948_GYRO_CONFIG_1, regVal);
    gyrRangeFactor = (1 << gyroRange);
}
void setGyrDLPF(ICM20948_dlpf dlpf)
{
    regVal = readRegister8(2, ICM20948_GYRO_CONFIG_1);
    if (dlpf == ICM20948_DLPF_OFF)
    {
        regVal &= 0xFE;
        writeRegister8(2, ICM20948_GYRO_CONFIG_1, regVal);
        return;
    }
    else
    {
        regVal |= 0x01;
        regVal &= 0xC7;
        regVal |= (dlpf << 3);
    }
    writeRegister8(2, ICM20948_GYRO_CONFIG_1, regVal);
}
void setGyrSampleRateDivider(uint8_t gyrSplRateDiv)
{
    writeRegister8(2, ICM20948_GYRO_SMPLRT_DIV, gyrSplRateDiv);
}

void setTempDLPF(ICM20948_dlpf dlpf)
{
    writeRegister8(2, ICM20948_TEMP_CONFIG, dlpf);
}

// void setI2CMstSampleRate(uint8_t rateExp){
//     if(rateExp < 16){
//         writeRegister8(3, ICM20948_I2C_MST_ODR_CFG, rateExp);
//     }
// }


/************* x,y,z results *************/

bool readSensor(void)
{
   return readAllData(icm20948_all_buff, sizeof(icm20948_all_buff));
}
bool readMagSensor(void)
{
   return readAllData(icm20948_all_buff, sizeof(icm20948_all_buff));
}
void getAccRawValues(xyzFloat *dat)
{
    xyzFloat accRawVal;
    int16_t x, y, z;
    x = (((icm20948_all_buff[0]) << 8) | icm20948_all_buff[1]);
    y = (((icm20948_all_buff[2]) << 8) | icm20948_all_buff[3]) * 1.0f;
    z = (((icm20948_all_buff[4]) << 8) | icm20948_all_buff[5]) * 1.0f;
    dat->x = x * 1.0f;
    dat->y = y * 1.0f;
    dat->z = z * 1.0f;
    //    accRawVal.x =  x* 1.0f;
}
void getAccRawValuesInt16(int16_t *acc)
{
    acc[0] = (((icm20948_all_buff[0]) << 8) | icm20948_all_buff[1]);
    acc[1] = (((icm20948_all_buff[2]) << 8) | icm20948_all_buff[3]);
    acc[2] = (((icm20948_all_buff[4]) << 8) | icm20948_all_buff[5]);
}

xyzFloat getCorrectedAccRawValues(void)
{
    xyzFloat accRawVal;
    //            accRawVal=    getAccRawValues();
    // accRawVal = correctAccRawValues(accRawVal);

    return accRawVal;
}

xyzFloat getGValues(void)
{
    xyzFloat gVal, accRawVal;
    accRawVal = getCorrectedAccRawValues();

    gVal.x = accRawVal.x * accRangeFactor / 16384.0;
    gVal.y = accRawVal.y * accRangeFactor / 16384.0;
    gVal.z = accRawVal.z * accRangeFactor / 16384.0;
    return gVal;
}

// xyzFloat getAccRawValuesFromFifo(){
//     xyzFloat accRawVal = readICM20948xyzValFromFifo();
//     return accRawVal;
// }

// xyzFloat getCorrectedAccRawValuesFromFifo(){
//     xyzFloat accRawVal = getAccRawValuesFromFifo();
//     // accRawVal = correctAccRawValues(accRawVal);

//     return accRawVal;
// }

// xyzFloat getGValuesFromFifo(){
//     xyzFloat gVal, accRawVal;
//     accRawVal = getCorrectedAccRawValuesFromFifo();

//     gVal.x = accRawVal.x * accRangeFactor / 16384.0;
//     gVal.y = accRawVal.y * accRangeFactor / 16384.0;
//     gVal.z = accRawVal.z * accRangeFactor / 16384.0;
//     return gVal;
// }

float getResultantG(xyzFloat gVal)
{
    float resultant = 0.0;
    resultant = sqrt(sq(gVal.x) + sq(gVal.y) + sq(gVal.z));

    return resultant;
}

float getTemperature()
{
    int16_t rawTemp = (int16_t)(((icm20948_all_buff[12]) << 8) | icm20948_all_buff[13]);
    float tmp = (rawTemp * 1.0 - ICM20948_ROOM_TEMP_OFFSET) / ICM20948_T_SENSITIVITY + 21.0;
    return tmp;
}

void getGyrRawValues(xyzFloat *dat)
{
    xyzFloat gyrRawVal;
    int16_t x, y, z;

    x = (int16_t)(((icm20948_all_buff[6]) << 8) | icm20948_all_buff[7]);
    y = (int16_t)(((icm20948_all_buff[8]) << 8) | icm20948_all_buff[9]);
    z = (int16_t)(((icm20948_all_buff[10]) << 8) | icm20948_all_buff[11]);

    dat->x = x * 1.0f;
    dat->y = y * 1.0f;
    dat->z = z * 1.0f;
}
void getGyrRawValuesInt16(int16_t *dat)
{
    int16_t x, y, z;
    dat[0] = (int16_t)(((icm20948_all_buff[6]) << 8) | icm20948_all_buff[7]);
    dat[1] = (int16_t)(((icm20948_all_buff[8]) << 8) | icm20948_all_buff[9]);
    dat[2] = (int16_t)(((icm20948_all_buff[10]) << 8) | icm20948_all_buff[11]);
}

xyzFloat getCorrectedGyrRawValues()
{
    xyzFloat gyrRawVal;
    //    getGyrRawValues();
    // gyrRawVal = correctGyrRawValues(gyrRawVal);
    return gyrRawVal;
}

xyzFloat getGyrValues()
{
    xyzFloat gyrVal = getCorrectedGyrRawValues();

    gyrVal.x = gyrVal.x * gyrRangeFactor * 250.0 / 32768.0;
    gyrVal.y = gyrVal.y * gyrRangeFactor * 250.0 / 32768.0;
    gyrVal.z = gyrVal.z * gyrRangeFactor * 250.0 / 32768.0;

    return gyrVal;
}

// xyzFloat getGyrValuesFromFifo(){
//     xyzFloat gyrVal;
//     xyzFloat gyrRawVal = readICM20948xyzValFromFifo();

//     gyrRawVal = correctGyrRawValues(gyrRawVal);
//     gyrVal.x = gyrRawVal.x * gyrRangeFactor * 250.0 / 32768.0;
//     gyrVal.y = gyrRawVal.y * gyrRangeFactor * 250.0 / 32768.0;
//     gyrVal.z = gyrRawVal.z * gyrRangeFactor * 250.0 / 32768.0;

//     return gyrVal;
// }

void getMagValues(xyzFloat *dat)
{
    int16_t x, y, z;

    x = (int16_t)((icm20948_all_buff[15]) << 8) | icm20948_all_buff[14];
    y = (int16_t)((icm20948_all_buff[17]) << 8) | icm20948_all_buff[16];
    z = (int16_t)((icm20948_all_buff[19]) << 8) | icm20948_all_buff[18];

    dat->x = x * AK09916_MAG_LSB;
    dat->y = y * AK09916_MAG_LSB;
    dat->z = z * AK09916_MAG_LSB;
}
void getMagValuesInt16(int16_t *dat)
{
    int16_t x, y, z;

    dat[0] = (int16_t)((icm20948_all_buff[15]) << 8) | icm20948_all_buff[14];
    dat[1] = (int16_t)((icm20948_all_buff[17]) << 8) | icm20948_all_buff[16];
    dat[2] = (int16_t)((icm20948_all_buff[19]) << 8) | icm20948_all_buff[18];

}

/************** Magnetometer **************/

bool AK09916_initMagnetometer(void)
{
    enableI2CMaster();
    reset_ICM20948();
    sleep(false);
    resetMag();
    writeRegister8(2, ICM20948_ODR_ALIGN_EN, 1); // aligns ODR
    enableI2CMaster();

    bool initSuccess = false;
    uint8_t tries = 0;
    while (!initSuccess && (tries < 10))
    { // max. 10 tries to init the magnetometer
        icm20948_delayms(10);
        enableI2CMaster();
        icm20948_delayms(10);

        int16_t whoAmI = whoAmIMag();
        if (!((whoAmI == AK09916_WHO_AM_I_1) || (whoAmI == AK09916_WHO_AM_I_2)))
        {
            initSuccess = false;
            i2cMasterReset();
        }
        else
        {
            initSuccess = true;
        }
    }
    if (initSuccess)
    {
        setMagOpMode(AK09916_CONT_MODE_100HZ);
#if icm20949_debug
        printf("icm20948_init initMagnetometer OK\r\n");
#endif
    }
    else
    {
       #if icm20949_debug
        printf("icm20948_init initMagnetometer ERR..........\r\n");
#endif 
    }

    return initSuccess;
}
uint8_t mag_data_buf[6];
int16_t mag_debug_buf[4];
void mag_data_test()
{
    bool ret;
    ret= readMagAllData(mag_data_buf,6);
    if(ret)
    {
        int16_t x, y, z;

           x = (int16_t)((mag_data_buf[1]) << 8) | mag_data_buf[0];
           y = (int16_t)((mag_data_buf[3]) << 8) | mag_data_buf[2];
           z = (int16_t)((mag_data_buf[5]) << 8) | mag_data_buf[4];

           mag_debug_buf[0] = x * AK09916_MAG_LSB;
           mag_debug_buf[1] = y * AK09916_MAG_LSB;
           mag_debug_buf[2] = z * AK09916_MAG_LSB;
           mag_debug_buf[3] = atan2(mag_debug_buf[1],mag_debug_buf[0])*57.295f;

           printf("samples:%d \r\n",mag_debug_buf[3]);

    }

}
void enable_Lis2mdl_DataRead(uint8_t reg, uint8_t bytes)
{
    writeRegister8(3, ICM20948_I2C_SLV0_ADDR, LIS2MDL_I2C_ADD | 0x80); // read AK09916
    writeRegister8(3, ICM20948_I2C_SLV0_REG, reg);                             // define AK09916 register to be read
    writeRegister8(3, ICM20948_I2C_SLV0_CTRL, 0x80 | bytes);                   // enable read | number of byte
    icm20948_delayms(1);
}
uint8_t readLis2mdlRegister8(uint8_t reg)
{
    enable_Lis2mdl_DataRead(reg, 0x01);

    regVal = readRegister8(0, ICM20948_EXT_SLV_SENS_DATA_00);
    return regVal;
}
void writeLis2mdlRegister8(uint8_t reg, uint8_t val)
{
    writeRegister8(3, ICM20948_I2C_SLV0_ADDR, LIS2MDL_I2C_ADD); // write AK09916
    writeRegister8(3, ICM20948_I2C_SLV0_REG, reg);              // define AK09916 register to be written to
    writeRegister8(3, ICM20948_I2C_SLV0_DO, val);
}

uint8_t Lis2mdl_whoAmIMag(void)
{
    return (readLis2mdlRegister8(LIS2MDL_WHO_AM_I));
}
void Lis2mdlsetMagOpMode(void)
{
    uint8_t dat=0;
    writeLis2mdlRegister8(LIS2MDL_CFG_REG_A, LIS2MDL_CFGA_TEMP_EN | LIS2MDL_CFGA_ODR_100HZ | LIS2MDL_CFGA_MODE_CONTINUE);
    icm20948_delayms(10);
    writeLis2mdlRegister8(LIS2MDL_CFG_REG_B, LIS2MDL_CFGB_OFF_CANC_EN | LIS2MDL_CFGB_LPF_EN);
    icm20948_delayms(10);
    writeLis2mdlRegister8(LIS2MDL_CFG_REG_C, LIS2MDL_CFGC_BDU );
    icm20948_delayms(10);
    dat= readLis2mdlRegister8(LIS2MDL_CFG_REG_A);
    enable_Lis2mdl_DataRead(LIS2MDL_OFFSET_X_REG_L, 0x06);
    printf("Lis2mdlsetMagOpMode %2x \r\n",dat);

}

bool Lis2mdl_initMagnetometer(void)
{
    enableI2CMaster();
    sleep(false);
    writeRegister8(2, ICM20948_ODR_ALIGN_EN, 1); // aligns ODR
    enableI2CMaster();

    bool initSuccess = false;
    uint8_t tries = 0;
    uint8_t whoAmI;
    while (!initSuccess && (tries < 10))
    { // max. 10 tries to init the magnetometer
        icm20948_delayms(10);
        enableI2CMaster();
        icm20948_delayms(10);

        whoAmI = Lis2mdl_whoAmIMag();
        if (!(whoAmI == LIS2MDL_ID))
        {
            initSuccess = false;
            i2cMasterReset();
        }
        else
        {
            initSuccess = true;
        }
    }
    if (initSuccess)
    {
        Lis2mdlsetMagOpMode();
#if icm20949_debug
        printf("Lis2mdl_init initMagnetometer OK\r\n");
#endif
    }
    else
    {
       #if icm20949_debug
        printf("Lis2mdl_init initMagnetometer ERR..........%d\r\n",whoAmI);
#endif
    }

    return initSuccess;
}


void icm_test(void)
{
    uint8_t regVal;

    printf("icm test \r\n");

    uint8_t tx_buf[5] = {0};
    int ret = 0;

    tx_buf[0] = 0;

    ret = icm20948_read_reg(0x05, tx_buf, 3);
    printf("read2 len:%d ,id= %x,%x %x,%x\r\n", ret, tx_buf[0], tx_buf[1], tx_buf[2], tx_buf[3]);

    tx_buf[0] = 0x60;
    tx_buf[1] = 0x43;
    tx_buf[2] = 0x07;
    ret = icm20948_write_reg(0x05, tx_buf, 3);
    printf("read2 len:%d ,id= %x,%x %x,%x\r\n", ret, tx_buf[0], tx_buf[1], tx_buf[2], tx_buf[3]);

    ret = icm20948_read_reg(0x05, tx_buf, 3);
    printf("read3 len:%d ,id= %x,%x %x,%02x\r\n", ret, tx_buf[0], tx_buf[1], tx_buf[2], tx_buf[3]);
}
