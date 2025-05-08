#ifndef __SENSOR_
#define __SENSOR_


#define L2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define L1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define M0 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define R1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
#define R2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define L3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define R3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)



void Sensor_Init(void);
void sensor_read(void);
void Sensor_pid(void);
void car_ggg(void);
#endif

