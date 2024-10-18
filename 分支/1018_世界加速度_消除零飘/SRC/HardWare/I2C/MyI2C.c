#include "MyI2C.h"
#define LSM6DS3_ADDRESS 0xD6

void MyI2C_W_SCL(uint8_t BitValue)
{
	// HAL_GPIO_WritePin(GPIOA, GPIO_Pin_8, (GPIO_PinState)BitValue);
	//HAL_Delay(1);

	if(BitValue == 1){
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
	}else{
		GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
	}
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	// HAL_GPIO_WritePin(GPIOA, GPIO_Pin_11, (GPIO_PinState)BitValue);
	//HAL_Delay(1);

	if(BitValue == 1){
		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
	}else{
		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);
	}
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	//HAL_Delay(1);
	return BitValue;
}


void LSM6DS3_WriteReg(uint8_t RegAddress, uint8_t Data){
    MyI2C_Start();
	MyI2C_SendByte(LSM6DS3_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}

uint8_t LSM6DS3_ReadReg(uint8_t RegAddress){
    uint8_t Data;

	MyI2C_Start();
	MyI2C_SendByte(LSM6DS3_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	MyI2C_Stop();

	MyI2C_Start();
	MyI2C_SendByte(LSM6DS3_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();

	return Data;
}

void LSM6DS3_Init(void){

	LSM6DS3_WriteReg(0x18, 0x38);
	LSM6DS3_WriteReg(0x19, 0x38);
	LSM6DS3_WriteReg(0x10, 0x60);
	LSM6DS3_WriteReg(0x11, 0x60);

}


void MyI2C_Init(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //     GPIO_Mode_Out_PP
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	// GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);

}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
	Delay_us(4);
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
	Delay_us(3);
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
		Delay_us(3);
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
		Delay_us(2);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{

	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
	Delay_us(2);
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}

