#include "MPU6050.h"
#include "_I2C_.h"
#include "math.h"

I2C_BUS MPU6050_I2C;

typedef enum{
    Band_256Hz = 0x00,
    Band_186Hz,
    Band_96Hz,
    Band_43Hz,
    Band_21Hz,
    Band_10Hz,
    Band_5Hz
}Filter_Typedef;

typedef enum{
    gyro_250 = 0x00,
    gyro_500 = 0x08,
    gyro_1000 = 0x10,
    gyro_2000 = 0x18
}GYRO_CONFIG_Typedef;

typedef enum{
    acc_2g = 0x00,
    acc_4g = 0x08,
    acc_8g = 0x10,
    acc_16g = 0x18
}ACCEL_CONFIG_Typedef;

typedef enum{
    FIFO_Disable,
    Acc_OUT = 0x08,
    Gyro_zOUT = 0x10,
    Gyro_yOUT = 0x20,
    Gyro_xOUT = 0x40,
    Temp_OUT =0x80,
}FIFO_EN_Typedef;

typedef enum{
    interrupt_Disable,
    Data_Ready_EN = 0x01,
    I2C_Master_EN = 0x08,
    FIFO_overFolow_EN = 0x10,
    Motion_EN = 0x40,
}INT_EN_Typedef;
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct MPU6050_InitTypeDef
{
    uint16_t SMPLRT_Rate;
    Filter_Typedef Filter;
    GYRO_CONFIG_Typedef gyro_range;
    ACCEL_CONFIG_Typedef acc_range;
    FIFO_EN_Typedef FIFO_EN;
    INT_EN_Typedef INT;
}MPU6050_InitTypeDef;
///////////////////////////////////////////////////////////////////////////////////////////////////
static void MPU6050_Register_init(MPU6050_InitTypeDef* this){
    
    I2C(MPU6050_I2C)->Write_Reg(MPU6050_PWR_MGMT_1,0x80);//复位
    
    Delay_ms(100);
    I2C(MPU6050_I2C)->Write_Reg(MPU6050_PWR_MGMT_1,0x00);//唤醒
    uint8_t SMPLRT_DIV;
    if(this->SMPLRT_Rate>1000)this->SMPLRT_Rate=1000;
    else if(this->SMPLRT_Rate<4)this->SMPLRT_Rate=4;
    SMPLRT_DIV=1000/this->SMPLRT_Rate-1;//由计算公式得
    I2C(MPU6050_I2C)->Write_Reg(MPU6050_SMPLRT_DIV,SMPLRT_DIV);

    I2C(MPU6050_I2C)->Write_Reg(MPU6050_INT_ENABLE,this->INT);
    I2C(MPU6050_I2C)->Write_Reg(MPU6050_CONFIG,this->Filter);
    I2C(MPU6050_I2C)->Write_Reg(MPU6050_GYRO_CONFIG,this->gyro_range);
    I2C(MPU6050_I2C)->Write_Reg(MPU6050_ACCEL_CONFIG,this->acc_range);
    I2C(MPU6050_I2C)->Write_Reg(MPU6050_FIFO_EN,this->FIFO_EN);
    uint8_t temp = 0x00;
    if(this->FIFO_EN!=0x00)//如果打开了FIFO
        temp = 0x40;
    if((this->INT & 0x08)==1)//如果打开了中断
        temp |= 0x08;
    I2C(MPU6050_I2C)->Write_Reg(MPU6050_USER_CTRL,temp);
    I2C(MPU6050_I2C)->Write_Reg(MPU6050_PWR_MGMT_1,0x01);//X轴为参考
}

// ?????????????
float gyro_zero_bias_z = 0;
// ????????
const int calibration_samples = 1000;

void MPU6050_init(GPIO_TypeDef* GPIOx,uint16_t SCl,uint16_t SDA){
    MPU6050_I2C = Create_SI2C(GPIOx,SCl,SDA,MPU6050_ADDRESS<<1);
    
    MPU6050_InitTypeDef MPU6050_init_Struct;
    MPU6050_init_Struct.SMPLRT_Rate = 500;            //采样率Hz
    MPU6050_init_Struct.Filter = Band_5Hz;            //滤波器带宽
    MPU6050_init_Struct.gyro_range = gyro_2000;       //陀螺仪测量范围
    MPU6050_init_Struct.acc_range = acc_16g;          //加速度计测量范围
    MPU6050_init_Struct.FIFO_EN = FIFO_Disable;       //FIFO
    MPU6050_init_Struct.INT = interrupt_Disable;      //中断配置
    
    MPU6050_Register_init(&MPU6050_init_Struct);      //初始化寄存器

    // ???????
    for (int i = 0; i < calibration_samples; i++) {
        int16_t temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_L);
        float current_gz = temp * 0.005 * 0.0174533;
        gyro_zero_bias_z += current_gz;
        // ????,???????????????
        Delay_ms(5); 
    }
    gyro_zero_bias_z /= calibration_samples;
}

void MPU6050_Get_Raw(MPU6050_raw* this){
    this->AccX = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_XOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_XOUT_L);
    this->AccY = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_YOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_YOUT_L);
    this->AccZ = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_ZOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_ZOUT_L);
    this->GyroX = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_XOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_XOUT_L);
    // ????????
    this->GyroY = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_YOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_YOUT_L);
    this->GyroZ = ((int16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_L);
    this->Temp = ((uint16_t)(I2C(MPU6050_I2C)->Read_Reg(MPU6050_TEMP_OUT_H))<<8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_TEMP_OUT_L);
}

float MPU6050_GetTemp(void){
    uint8_t temp = (((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_TEMP_OUT_H)) << 8) | I2C(MPU6050_I2C)->Read_Reg(MPU6050_TEMP_OUT_L);
    float temperature = (float)temp/340 + 36.53;
    return temperature;
}

// ???????
float normalize_angle(float angle) {
    while (angle > 180.0) angle -= 360.0;
    while (angle < -180.0) angle += 360.0;
    return angle;
}

void MPU6050_Get_Angle(MPU6050* this){
    int16_t temp=0;
    float Ax,Ay,Az=0;
    float Gx,Gy,Gz=0;
    static float Gyroscope_roll=0;
    static float Gyroscope_pitch=0;
    const static float dt=0.005;
    const static float weight = 0.95;//权重

    // ?????????????
    temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_XOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_XOUT_L);
    Ax = temp * 16.0/32768;

    temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_YOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_YOUT_L);
    Ay = temp * 16.0/32768;

    temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_ZOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_ACCEL_ZOUT_L);
    Az = temp * 16.0/32768;

    // ????????????
    temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_XOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_XOUT_L);
    Gx = temp *dt*0.0174533;

    temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_YOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_YOUT_L);
    Gy = temp *dt*0.0174533;

    temp = ((uint16_t)I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_H) << 8)+I2C(MPU6050_I2C)->Read_Reg(MPU6050_GYRO_ZOUT_L);
    Gz = temp *dt*0.0174533;

    Gz -= gyro_zero_bias_z;

    // ???????
    float acc_roll = atan2(Ay, Az) * 180 / 3.1415926;
    float acc_pitch = atan2(-Ax, Az) * 180 / 3.1415926;

    float gyro_roll_rate = Gy;
    float gyro_pitch_rate = Gx;

    float gyro_roll = Gyroscope_roll + gyro_roll_rate * dt;
    float gyro_pitch = Gyroscope_pitch + gyro_pitch_rate * dt;

    float new_weight_roll = 0.95; // ????
    float new_weight_pitch = 0.95; // ????

    // ?????????????????
    if (fabs(acc_roll - gyro_roll) > 10) {
        new_weight_roll = 0.8;
    }
    if (fabs(acc_pitch - gyro_pitch) > 10) {
        new_weight_pitch = 0.8;
    }

    // ?????roll?pitch
    this->roll = new_weight_roll * acc_roll + (1 - new_weight_roll) * gyro_roll;
    this->pitch = new_weight_pitch * acc_pitch + (1 - new_weight_pitch) * gyro_pitch;

    // ?????????
    Gyroscope_roll = gyro_roll;
    Gyroscope_pitch = gyro_pitch;

    // ??yaw
    const float scale_factor = 10;
    const float compensation = 0;

    this->yaw += Gz * scale_factor + compensation;

    this->yaw = normalize_angle(this->yaw);
}

uint8_t MPU6050_ID(void)
{
    return I2C(MPU6050_I2C)->Read_Reg(MPU6050_WHO_AM_I);
}